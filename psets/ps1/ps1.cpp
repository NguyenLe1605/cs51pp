#include "ps1.hpp"
// #include <stdexcept>
#include <algorithm>
#include <deque>
#include <iterator>
#include "matchit.h"

using namespace matchit;

template <class Iterator, typename T = typename Iterator::value_type>
auto nonincreasing_helper(const Subrange<Iterator> &range) -> bool {
  Id<T> head;
  Id<Subrange<Iterator>> tail;
  return match(range)(
      pattern | ds() = []() { return true; },
      pattern | ds(head) = []() { return true; },
      pattern | ds(head, tail.at(ooo)) =
          [&]() {
            const auto &next = (*tail).begin();
            return head.value() >= *next && nonincreasing_helper(*tail);
          });
}

auto nonincreasing(const std::vector<int> &vec) -> bool {
  auto range = Subrange(vec.begin(), vec.end());
  return nonincreasing_helper(range);
}

template <typename Iterator>
auto merge_helper(Iterator lfirst, Iterator llast, Iterator rfirst,
                  Iterator rlast) -> std::deque<int> {
  if (lfirst == llast && rfirst == rlast) {
    // both vectors are empty
    return {};
  } else if (lfirst == llast) {
    // the left are empty, take the rest of the right
    return std::deque<int>(rfirst, rlast);
  } else if (rfirst == rlast) {
    // the right are empty, take the rest of the left
    return std::deque<int>(lfirst, llast);
  } else {
    if (*lfirst <= *rfirst) {
      auto deq = merge_helper(lfirst + 1, llast, rfirst, rlast);
      deq.push_front(*lfirst);
      return deq;
    } else {
      auto deq = merge_helper(lfirst, llast, rfirst + 1, rlast);
      deq.push_front(*rfirst);
      return deq;
    }
  }
}

auto merge(const std::vector<int> &lhs, const std::vector<int> &rhs)
    -> std::vector<int> {
  auto deq = merge_helper(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  std::vector<int> result;
  std::ranges::copy(deq, std::back_inserter(result));
  return result;
}

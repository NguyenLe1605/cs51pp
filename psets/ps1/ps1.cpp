#include "ps1.hpp"
#include <algorithm>
#include <deque>
#include <iostream>
#include <tuple>
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

template <typename Iterator>
auto merge_from_reverse(Iterator lfirst, Iterator llast, Iterator rfirst,
                        Iterator rlast) -> std::vector<int> {
  if (lfirst == llast && rfirst == rlast) {
    // both vectors are empty
    return {};
  } else if (lfirst == llast) {
    // the left are empty, take the rest of the right
    return std::vector<int>((rlast).base(), rfirst.base());
  } else if (rfirst == rlast) {
    // the right are empty, take the rest of the left
    return std::vector<int>(llast.base(), lfirst.base());
  } else {
    if (*lfirst > *rfirst) {
      auto res = merge_from_reverse(lfirst + 1, llast, rfirst, rlast);
      res.push_back(*lfirst);
      return res;
    } else {
      auto deq = merge_from_reverse(lfirst, llast, rfirst + 1, rlast);
      deq.push_back(*rfirst);
      return deq;
    }
  }
}

auto merge(const std::vector<int> &lhs, const std::vector<int> &rhs)
    -> std::vector<int> {
  return merge_from_reverse(lhs.rbegin(), lhs.rend(), rhs.rbegin(), rhs.rend());
}

template <class Iterator, typename T = Iterator::value_type, typename B = bool>
auto unzip_reverse_range(Subrange<Iterator> const &rev_range)
    -> std::tuple<std::vector<B>, std::vector<B>> {
  Id<T> head;
  Id<Subrange<Iterator>> tail;
  return match(rev_range)(
      pattern | ds() =
          [&] { return std::make_tuple(std::vector<B>(), std::vector<B>()); },
      pattern | ds(head, tail.at(ooo)) =
          [&] {
            B first, second;
            std::vector<B> fvec, svec;
            std::tie(first, second) = head.value();
            std::tie(fvec, svec) = unzip_reverse_range(*tail);
            fvec.push_back(first);
            svec.push_back(second);
            return std::make_tuple(std::move(fvec), std::move(svec));
          });
}

auto unzip(const std::vector<std::tuple<bool, bool>> &list)
    -> std::tuple<std::vector<bool>, std::vector<bool>> {
  return unzip_reverse_range(Subrange(list.rbegin(), list.rend()));
}

template <class Iterator, typename T = Iterator::value_type>
auto to_run_length_rev(Subrange<Iterator> const &range)
    -> std::pair<std::vector<std::tuple<int, char>>, char> {
  Id<T> head;
  Id<Subrange<Iterator>> tail;
  return match(range)(
      pattern | ds() =
          [&] {
            return std::make_pair(std::vector<std::tuple<int, char>>(), '\0');
          },
      pattern | ds(head, tail.at(ooo)) =
          [&] {
            std::vector<std::tuple<int, char>> result;
            char previous;
            auto current = head.value();
            std::tie(result, previous) = to_run_length_rev(*tail);
            if (result.empty() || current != previous) {
              result.push_back(std::make_tuple(1, current));
            } else {
              auto &count = std::get<0>(result.back());
              count += 1;
            }
            return std::make_pair(result, current);
          });
}

auto to_run_length(const std::vector<char> &list)
    -> std::vector<std::tuple<int, char>> {
  std::vector<std::tuple<int, char>> result;
  std::tie(result, std::ignore) =
      to_run_length_rev(Subrange(list.rbegin(), list.rend()));
  return result;
}

auto repeat_char(const std::vector<char> &v, int count, char ch)
    -> std::vector<char> {
  if (count <= 0) {
    return v;
  } else {
    auto result = repeat_char(v, count - 1, ch);
    result.push_back(ch);
    return result;
  }
}

template <class Iterator, typename T = Iterator::value_type>
auto from_run_length_rev(Subrange<Iterator> const &range) -> std::vector<char> {
  Id<T> head;
  Id<Subrange<Iterator>> tail;
  return match(range)(
      pattern | ds() = [&] { return std::vector<char>(); },
      pattern | ds(head, tail.at(ooo)) =
          [&] {
            int count;
            char ch;
            std::tie(count, ch) = head.value();
            auto result = from_run_length_rev(*tail);
            result = repeat_char(result, count, ch);
            return result;
          });
}

auto from_run_length(const std::vector<std::tuple<int, char>> &list)
    -> std::vector<char> {
  return from_run_length_rev(Subrange(list.rbegin(), list.rend()));
}

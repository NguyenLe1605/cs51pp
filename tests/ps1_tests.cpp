#include <gtest/gtest.h>
#include <tuple>
#include <vector>
#include "ps1.hpp"

TEST(Ps1Tests, NonIncreasingTest) {
  ASSERT_TRUE(nonincreasing({})) << "nonincreasing empty";
  ASSERT_TRUE(nonincreasing({7})) << "nonincreasing single";
  ASSERT_TRUE(nonincreasing({4, 4, 4})) << "nonincreasing repeat";
  ASSERT_TRUE(!nonincreasing({2, 1, 2})) << "nonincreasing inc at start";
  ASSERT_TRUE(nonincreasing({2, 2, 1})) << "nonincreasing inc at dup";
  ASSERT_TRUE(nonincreasing({9, 8, 7, 6, 5, 5, 5, 4, 4, -2}))
      << "nonincreasing long with negs";
  ASSERT_TRUE(!nonincreasing({9, 8, 7, 6, 7, 5, 5, 5, 4, 3}))
      << "nonincreasing long inc at mid";
}

TEST(Ps1Tests, MergeTest) {
  ASSERT_EQ(merge({1, 3, 5}, {2, 4, 6}), std::vector<int>({1, 2, 3, 4, 5, 6}))
      << "merge lists with same size without repeat";
  ASSERT_EQ(merge({1, 2, 2}, {2, 4, 6}), std::vector<int>({1, 2, 2, 2, 4, 6}))
      << "merge lists with same size with repeat";
  ASSERT_EQ(merge({1, 3, 5}, {2, 4, 6, 12}),
            std::vector<int>({1, 2, 3, 4, 5, 6, 12}))
      << "merge lists with larger size on the right";
  ASSERT_EQ(merge({1, 3, 5, 700, 702}, {2, 4, 6, 12}),
            std::vector<int>({1, 2, 3, 4, 5, 6, 12, 700, 702}))
      << "merge lists with larger size on the left";
  ASSERT_EQ(merge({}, {}), std::vector<int>{}) << "merge 2 empty lists";
  ASSERT_EQ(merge({1}, {}), std::vector<int>{1})
      << "merge a single list with an empty list";
}

TEST(Ps1Tests, UnzipTest) {
  ASSERT_EQ(unzip(std::vector<std::tuple<bool, bool>>(
                {{true, false}, {false, false}, {false, true}})),
            std::make_tuple(std::vector<bool>{true, false, false},
                            std::vector<bool>{false, false, true}))
      << "unzip a simple list";
  ASSERT_EQ(unzip(std::vector<std::tuple<bool, bool>>()),
            std::make_tuple(std::vector<bool>(), std::vector<bool>()))
      << "unzip an empty list";
}

TEST(Ps1Tests, RunLengthTest) {
  std::vector<char> from{'a', 'a', 'a', 'a', 'a', 'b', 'b',
                         'b', 'a', 'd', 'd', 'd', 'd'};
  std::vector<std::tuple<int, char>> to{{5, 'a'}, {3, 'b'}, {1, 'a'}, {4, 'd'}};

  ASSERT_EQ(to_run_length(from), to) << "to_run_length";
  ASSERT_EQ(from_run_length(to), from) << "from_run_length";
  ASSERT_EQ(from_run_length(to_run_length(from)), from)
      << "to_run_length -> from_run_length";
}

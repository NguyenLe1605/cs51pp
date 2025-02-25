#include <gtest/gtest.h>
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

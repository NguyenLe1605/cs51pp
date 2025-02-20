#include <gtest/gtest.h>

#include "lab1.hpp"

TEST(Lab1Test, TestSquare) {
  ASSERT_EQ(square(0), 0) << "square zero";
  ASSERT_EQ(square(1), 1) << "square one";
  ASSERT_EQ(square(-1), 1) << "square neg_one";
  ASSERT_EQ(square(4), 16) << "square four";
  ASSERT_EQ(square(-10), 100) << "square neg_ten";
}

TEST(Lab1Test, TestExclaim) {
  ASSERT_EQ(exclaim("hello"), "Hello!") << "exclaim hello";
  ASSERT_EQ(exclaim(""), "!") << "exclaim empty string";
  ASSERT_EQ(exclaim("what's up"), "What's up!") << "exclaim what's up";
  ASSERT_EQ(exclaim("UPPERCASE!"), "UPPERCASE!!") << "exclaim uppercase with eclaimation at the end";
}

TEST(Lab1Test, TestNeedsSmallBills) {
  ASSERT_TRUE(needs_small_bills(105)) << "needs_small_bills 105";
  ASSERT_FALSE(needs_small_bills(1000)) << "needs_small_bills 1000";
  ASSERT_TRUE(needs_small_bills(150)) << "needs_small_bills 150";
  ASSERT_TRUE(needs_small_bills(5)) << "needs_small_bills 5";
}

TEST(Lab1Test, Computus) {
  ASSERT_EQ(computus_month(2018), 4) << "computus_month 2018";
  ASSERT_EQ(computus_day(2018), 1) << "computus_day 2018";
}

TEST(Lab1Test, FrustrumVolume) { ASSERT_FLOAT_EQ(frustrum_volume(3.0, 4.0, 4.0), 154.985599999999977); }

TEST(Lab1Test, Factorial) {
  ASSERT_EQ(factorial(4), 24);
  ASSERT_EQ(factorial(0), 1);
  ASSERT_EQ(factorial(1), 1);
}

TEST(Lab1Test, SumFromZero) {
  ASSERT_EQ(sum_from_zero(5), 15);
  ASSERT_EQ(sum_from_zero(100), 5050);
  ASSERT_EQ(sum_from_zero(-3), -6);
  ASSERT_EQ(sum_from_zero(0), 0);
}

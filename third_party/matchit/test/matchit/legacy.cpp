#include "matchit.h"
#include <any>
#include <array>
#include <gtest/gtest.h>
#include <iostream>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>

using namespace matchit;

static_assert(impl::StorePointer<std::unique_ptr<int32_t> const,
                                 std::unique_ptr<int32_t> const &>::value);
static_assert(!impl::StorePointer<std::unique_ptr<int32_t>,
                                  std::unique_ptr<int32_t> &&>::value);

std::tuple<> xxx();

bool func1() { return true; }

int64_t func2() { return 12; }

TEST(Match, test1)
{
  auto const matchFunc = [](int32_t input)
  {
    Id<int32_t> ii;
    return match(input)(
        pattern | 1 = func1, pattern | 2 = func2, pattern | or_(56, 59) = func2,
        pattern | (_ < 0) = expr(-1), pattern | (_ < 10) = expr(-10),
        pattern | and_(_<17, _> 15) = expr(16),
        pattern | app(_ * _, _ > 1000) = expr(1000),
        pattern | app(_ * _, ii) = expr(ii), pattern | ii = -ii,
        pattern | _ = expr(111));
  };
  EXPECT_EQ(matchFunc(1), 1);
  EXPECT_EQ(matchFunc(2), 12);
  EXPECT_EQ(matchFunc(11), 121);   // Id matched.
  EXPECT_EQ(matchFunc(59), 12);    // or_ matched.
  EXPECT_EQ(matchFunc(-5), -1);    // meet matched.
  EXPECT_EQ(matchFunc(10), 100);   // app matched.
  EXPECT_EQ(matchFunc(100), 1000); // app > meet matched.
  EXPECT_EQ(matchFunc(5), -10);    // _ < 10 matched.
  EXPECT_EQ(matchFunc(16), 16);    // and_ matched.
}

TEST(Match, test2)
{
  auto const matchFunc = [](auto &&input)
  {
    Id<int32_t> i;
    Id<int32_t> j;
    return match(input)(
        pattern | ds('/', 1, 1) = expr(1), pattern | ds('/', 0, _) = expr(0),
        pattern | ds('*', i, j) = i * j, pattern | ds('+', i, j) = i + j,
        pattern | _ = expr(-1));
  };
  EXPECT_EQ(matchFunc(std::make_tuple('/', 1, 1)), 1);
  EXPECT_EQ(matchFunc(std::make_tuple('+', 2, 1)), 3);
  EXPECT_EQ(matchFunc(std::make_tuple('/', 0, 1)), 0);
  EXPECT_EQ(matchFunc(std::make_tuple('*', 2, 1)), 2);
  EXPECT_EQ(matchFunc(std::make_tuple('/', 2, 1)), -1);
  EXPECT_EQ(matchFunc(std::make_tuple('/', 2, 3)), -1);
}

struct A
{
  int32_t a;
  int32_t b;
};
bool operator==(A const lhs, A const rhs)
{
  return lhs.a == rhs.a && lhs.b == rhs.b;
}

TEST(Match, test3)
{
  auto const matchFunc = [](A const &input)
  {
    Id<int32_t> i;
    // compose patterns for destructuring struct A.
    auto const dsA = dsVia(&A::a, &A::b);
    return match(input)(pattern | dsA(i, 1) = expr(i), pattern | _ = expr(-1));
  };
  EXPECT_EQ(matchFunc(A{3, 1}), 3);
  EXPECT_EQ(matchFunc(A{2, 2}), -1);
}

enum class Kind
{
  kONE,
  kTWO
};

class Num
{
public:
  virtual ~Num() = default;
  virtual Kind kind() const = 0;
};

class One : public Num
{
public:
  constexpr static auto k = Kind::kONE;
  Kind kind() const override { return k; }
};

class Two : public Num
{
public:
  constexpr static auto k = Kind::kTWO;
  Kind kind() const override { return k; }
};

template <Kind k>
constexpr auto kind = app(&Num::kind, k);

template <typename T>
auto get_if(Num const *num)
{
  return static_cast<T const *>(num->kind() == T::k ? num : nullptr);
}

TEST(Match, test4)
{
  auto const matchFunc = [](Num const &input)
  {
    return match(input)(pattern | as<One>(_) = expr(1),
                        pattern | kind<Kind::kTWO> = expr(2),
                        pattern | _ = expr(3));
  };
  matchit::impl::AsPointer<Two>()(std::variant<One, Two>{});
  EXPECT_EQ(matchFunc(One{}), 1);
  EXPECT_EQ(matchFunc(Two{}), 2);
}

TEST(Match, test5)
{
  auto const matchFunc = [](std::pair<int32_t, int32_t> ij)
  {
    return match(ij.first % 3, ij.second % 5)(
        pattern | ds(0, 0) = expr(1), pattern | ds(0, _ > 2) = expr(2),
        pattern | ds(_, _ > 2) = expr(3), pattern | _ = expr(4));
  };
  EXPECT_EQ(matchFunc(std::make_pair(3, 5)), 1);
  EXPECT_EQ(matchFunc(std::make_pair(3, 4)), 2);
  EXPECT_EQ(matchFunc(std::make_pair(4, 4)), 3);
  EXPECT_EQ(matchFunc(std::make_pair(4, 1)), 4);
}

int32_t fib(int32_t n)
{
  EXPECT_TRUE(n > 0);
  return match(n)(
      pattern | 1 = expr(1), pattern | 2 = expr(1),
      pattern | _ = [n]
      { return fib(n - 1) + fib(n - 2); });
}

TEST(Match, test6)
{
  EXPECT_EQ(fib(1), 1);
  EXPECT_EQ(fib(2), 1);
  EXPECT_EQ(fib(3), 2);
  EXPECT_EQ(fib(4), 3);
  EXPECT_EQ(fib(5), 5);
}

TEST(Match, test7)
{
  auto const matchFunc = [](std::pair<int32_t, int32_t> ij)
  {
    Id<std::tuple<int32_t, int32_t>> id;
    // delegate at to and_
    auto const at = [](auto &&id, auto &&pattern)
    { return and_(pattern, id); };
    return match(ij.first % 3, ij.second % 5)(
        pattern | ds(0, _ > 2) = expr(2), pattern | ds(1, _ > 2) = expr(3),
        pattern | at(id, ds(_, 2)) =
            [&id]
        {
          EXPECT_TRUE(std::get<1>(*id) == 2);
          static_cast<void>(id);
          return 4;
        },
        pattern | _ = expr(5));
  };
  EXPECT_EQ(matchFunc(std::make_pair(4, 2)), 4);
}

TEST(Match, test8)
{
  auto const equal = [](std::pair<int32_t, std::pair<int32_t, int32_t>> ijk)
  {
    Id<int32_t> x;
    return match(ijk)(pattern | ds(x, ds(_, x)) = expr(true),
                      pattern | _ = expr(false));
  };
  EXPECT_TRUE(equal(std::make_pair(2, std::make_pair(1, 2))));
  EXPECT_FALSE(equal(std::make_pair(2, std::make_pair(1, 3))));
}

// optional
TEST(Match, test9)
{
  auto const optional = [](auto const &i)
  {
    Id<int32_t> x;
    return match(i)(pattern | some(x) = expr(true),
                    pattern | none = expr(false));
  };
  EXPECT_EQ(optional(std::make_unique<int32_t>(2)), true);
  EXPECT_EQ(optional(std::make_shared<int32_t>(2)), true);
  EXPECT_EQ(optional(std::unique_ptr<int32_t>{}), false);
  EXPECT_EQ(optional(std::make_optional<int32_t>(2)), true);
  EXPECT_EQ(optional(std::optional<int32_t>{}), false);
  int32_t *p = nullptr;
  EXPECT_EQ(optional(p), false);
  int32_t a = 3;
  EXPECT_EQ(optional(&a), true);
}

struct Shape
{
  virtual bool is() const = 0;
  virtual ~Shape() = default;
};
struct Circle : Shape
{
  bool is() const final { return false; }
};
struct Square : Shape
{
  bool is() const final { return false; }
};

bool operator==(Shape const &, Shape const &) { return true; }

TEST(Match, test10)
{
  static_assert(matchit::impl::StorePointer<Shape, Shape &>::value);
  static_assert(matchit::impl::StorePointer<Shape const, Shape const &>::value);

  auto const dynCast = [](auto const &i)
  {
    return match(i)(pattern | some(as<Circle>(_)) = expr("Circle"),
                    pattern | some(as<Square>(_)) = expr("Square"),
                    pattern | none = expr("None"));
  };

  EXPECT_EQ(dynCast(std::unique_ptr<Shape>(new Square{})), "Square");
  EXPECT_EQ(dynCast(std::unique_ptr<Shape>(new Circle{})), "Circle");
  EXPECT_EQ(dynCast(std::unique_ptr<Shape>()), "None");
}

TEST(Match, test10_)
{
  auto const dToBCast = [](auto const &i)
  {
    return match(i)(pattern | some(as<Shape>(_)) = expr("Shape"),
                    pattern | none = expr("None"));
  };

  EXPECT_EQ(dToBCast(std::make_unique<Circle>()), "Shape");
}

TEST(Match, test11)
{
  auto const getIf = [](auto const &i)
  {
    return match(i)(pattern | as<Square>(_) = expr("Square"),
                    pattern | as<Circle>(_) = expr("Circle"));
  };

  std::variant<Square, Circle> sc = Square{};
  EXPECT_EQ(getIf(sc), "Square");
  sc = Circle{};
  EXPECT_EQ(getIf(sc), "Circle");
}

TEST(Match, test12)
{
  EXPECT_TRUE(matched(std::array<int32_t, 2>{1, 2}, ds(ooo, _)));
  EXPECT_TRUE(matched(std::array<int32_t, 3>{1, 2, 3}, ds(ooo, _)));
  EXPECT_TRUE(matched(std::array<int32_t, 2>{1, 2}, ds(ooo, _)));
}

template <size_t I>
constexpr auto &get(A const &a)
{
  if constexpr (I == 0)
  {
    return a.a;
  }
  else if constexpr (I == 1)
  {
    return a.b;
  }
}

namespace std
{
  template <>
  class tuple_size<A> : public std::integral_constant<size_t, 2>
  {
  };
} // namespace std

TEST(Match, test13)
{
  auto const dsAgg = [](auto const &v)
  {
    Id<int32_t> i;
    return match(v)(pattern | ds(1, i) = expr(i), pattern | ds(_, i) = expr(i));
  };

  EXPECT_EQ(dsAgg(A{1, 2}), 2);
  EXPECT_EQ(dsAgg(A{3, 2}), 2);
  EXPECT_EQ(dsAgg(A{5, 2}), 2);
  EXPECT_EQ(dsAgg(A{2, 5}), 5);
}

TEST(Match, test14)
{
  auto const anyCast = [](auto const &i)
  {
    return match(i)(pattern | as<Square>(_) = expr("Square"),
                    pattern | as<Circle>(_) = expr("Circle"));
  };

  std::any sc;
  sc = Square{};
  EXPECT_EQ(anyCast(sc), "Square");
  sc = Circle{};
  EXPECT_EQ(anyCast(sc), "Circle");

  EXPECT_TRUE(matched(sc, as<Circle>(_)));
  EXPECT_FALSE(matched(sc, as<Square>(_)));
  //     // one would write if let like
  //     // if (matched(value, pattern))
  //     // {
  //     //     ...
  //     // }
}

TEST(Match, test15)
{
  auto const optional = [](auto const &i)
  {
    Id<char> c;
    return match(i)(pattern | none = expr(1), pattern | some(none) = expr(2),
                    pattern | some(some(c)) = expr(c));
  };
  char const **x = nullptr;
  char const *y_ = nullptr;
  char const **y = &y_;
  char const *z_ = "x";
  char const **z = &z_;

  EXPECT_EQ(optional(x), 1);
  EXPECT_EQ(optional(y), 2);
  EXPECT_EQ(optional(z), 'x');
}

TEST(Match, test16)
{
  auto const notX = [](auto const &i)
  {
    return match(i)(pattern | not_(or_(1, 2)) = expr(3), pattern | 2 = expr(2),
                    pattern | _ = expr(1));
  };
  EXPECT_EQ(notX(1), 1);
  EXPECT_EQ(notX(2), 2);
  EXPECT_EQ(notX(3), 3);
}

// when
TEST(Match, test17)
{
  auto const whenX = [](auto const &x)
  {
    Id<int32_t> i, j;
    return match(x)(pattern | ds(i, j) | when(i + j == 10) = expr(3),
                    pattern | ds(_ < 5, _) = expr(5), pattern | _ = expr(1));
  };
  EXPECT_EQ(whenX(std::make_pair(1, 9)), 3);
  EXPECT_EQ(whenX(std::make_pair(1, 7)), 5);
  EXPECT_EQ(whenX(std::make_pair(7, 7)), 1);
}

TEST(Match, test18)
{
  auto const idNotOwn = [](auto const &x)
  {
    Id<int32_t> i;
    return match(x)(pattern | i | when(i == 5) = expr(1),
                    pattern | _ = expr(2));
  };
  EXPECT_EQ(idNotOwn(1), 2);
  EXPECT_EQ(idNotOwn(5), 1);
}

TEST(Match, test19)
{
  auto const matchFunc = [](auto &&input)
  {
    Id<int32_t> j;
    return match(input)(
        // `... / 2 3`
        pattern | ds(ooo, '/', 2, 3) = expr(1),
        // `... 3`
        pattern | ds(ooo, 3) = expr(3),
        // `/ ...`
        pattern | ds('/', ooo) = expr(4),

        pattern | ooo = expr(222),
        // `3 3 3 3 ..` all 3
        pattern | ooo = expr(333),

        // `... int32_t 3`
        pattern | ds(ooo, j, 3) = expr(7),
        // `... int32_t 3`
        pattern | ds(ooo, or_(j), 3) = expr(8),

        // `...`
        pattern | ooo = expr(12),

        pattern | _ = expr(-1));
  };
  EXPECT_EQ(matchFunc(std::make_tuple('/', 2, 3)), 1);
  EXPECT_EQ(matchFunc(std::make_tuple(3, 3, 3, 3, 3)), 3);
  EXPECT_TRUE(matched(std::make_tuple(3, 3, 3, 3, 3), ds(ooo)));
  EXPECT_TRUE(matched(std::make_tuple("123", 3, 3, 3, 2), ds("123", ooo, 2)));
  EXPECT_TRUE(matched(std::make_tuple("string", 3, 3, 3, 3), ds(ooo, 3)));
  EXPECT_TRUE(matched(std::make_tuple("string", 3, 3, 3, 3), ds(ooo)));
  EXPECT_TRUE(matched(std::make_tuple("string"), ds(ooo)));
}

TEST(Match, test20)
{
  Id<std::string> strA;
  Id<const char *> strB;
  EXPECT_TRUE(matched("abc", strA));
  EXPECT_TRUE(matched("abc", strB));
}
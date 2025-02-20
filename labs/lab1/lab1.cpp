#include "lab1.hpp"
#include <cctype>
#include <iostream>

constexpr float PI = 3.1416;

auto hello_lab1() -> void { std::cout << "hello world" << std::endl; }

auto square(int x) -> int { return x * x; }

auto exclaim(string text) -> string {
  auto first = text.empty() ? "" : static_cast<char>(std::toupper(text.front())) + text.substr(1);
  return first + "!";
}

auto needs_small_bills(int price) -> bool { return price % 20 != 0; }

auto computus_base(int year) -> int {
  auto a = year % 19;
  auto b = year / 100;
  auto c = year % 100;
  auto d = b / 4;
  auto e = b % 4;
  auto f = (b + 8) / 25;
  auto g = (b - f + 1) / 3;
  auto h = (19 * a + b - d - g + 15) % 30;
  auto i = c / 4;
  auto k = c % 4;
  auto l = (32 + 2 * e + 2 * i - h - k) % 7;
  auto m = (a + 11 * h + 22 * l) / 451;
  return (h + l - 7 * m + 114);
}

auto computus_month(int year) -> int { return computus_base(year) / 31; }

auto computus_day(int year) -> int { return (computus_base(year) % 31) + 1; }

auto frustrum_volume(float radius1, float radius2, float height) -> float {
  return (PI * height * (square(radius1) + radius1 * radius2 + square(radius2))) / 3;
}

auto factorial(int x) -> int {
  if (x <= 1) {
    return 1;
  } else {
    return x * factorial(x - 1);
  }
}

auto sum_from_zero(int x) -> int {
  if (x == 0) {
    return 0;
  } else if (x > 0) {
    return x + sum_from_zero(x - 1);
  } else {
    return x + sum_from_zero(x + 1);
  }
}

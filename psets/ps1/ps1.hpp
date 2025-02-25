#pragma once

#include <vector>

// TODO: write thorough documentation
auto nonincreasing(const std::vector<int> &vec) -> bool;

auto merge(const std::vector<int> &lhs, const std::vector<int> &rhs)
    -> std::vector<int>;

auto unzip(const std::vector<std::tuple<bool, bool>> &list)
    -> std::tuple<std::vector<bool>, std::vector<bool>>;

auto to_run_length(const std::vector<char> &list)
    -> std::vector<std::tuple<int, char>>;

auto from_run_length(const std::vector<std::tuple<int, char>> &list)
    -> std::vector<char>;

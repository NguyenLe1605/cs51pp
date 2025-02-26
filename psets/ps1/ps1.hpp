#pragma once

#include <functional>
#include <vector>

// Part 1: Utility functions

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

// Part 2: Prison Dilenma
enum class action_t { COOP, DEFECT };
using play_t = std::pair<action_t, action_t>;
using payoff_matrix_t = std::vector<std::pair<play_t, std::pair<int, int>>>;

auto extract_entry(play_t play, const payoff_matrix_t &matrix)
    -> std::pair<int, int>;

// head of the list is the most recent action
using history_t = std::vector<play_t>;
using strategy_t = std::function<action_t(const history_t &)>;

inline auto nasty([[maybe_unused]] const history_t &history) -> action_t {
  return action_t::DEFECT;
}

inline auto pasty([[maybe_unused]] const history_t &history) -> action_t {
  return action_t::COOP;
}

auto count_defections(const history_t &history) -> std::pair<int, int>;
auto count_cooperations(const history_t &history) -> std::pair<int, int>;

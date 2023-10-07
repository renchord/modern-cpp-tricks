//
// Created by chordr on 10/4/2023.
//
#include <iostream>
#include <array>
#include <functional>
#include <type_traits>

static const int MAX_N = 9;

template <int N, int K>
struct comb_number {
    static constexpr uint64_t value =
            comb_number<N - 1, K>::value + comb_number<N - 1, K - 1>::value;
};

template <int N>
struct comb_number<N, 0> {
    static constexpr uint64_t value = 1;
};

template <int N>
struct comb_number<N, N> {
    static constexpr uint64_t value = 1;
};

using ret_t = std::array<std::array<uint64_t, MAX_N + 1>, MAX_N + 1>;

constexpr ret_t compiler_time {
    []() {
        ret_t temp {};
        temp[1][1] = 1;
        temp[1][0] = 1;
        for (int n = 2; n <= MAX_N; ++n) {
            for (int k = 0; k <= n; ++k) {
                temp[n][k] = temp[n - 1][k] + (k == 0 ? 0 : temp[n - 1][k - 1]);
            }
        }
        return temp;
    }()
};

static_assert(compiler_time[8][2] == 28);
static_assert(compiler_time[5][2] == 10);
static_assert(compiler_time[1][1] == 1);

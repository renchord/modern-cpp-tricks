//
// Created by chordr on 9/27/2023.
//
#include <vector>
#include <iostream>

static constexpr int N = 10;
static int comb[N + 1][N + 1] = {};

static auto solve = []() {
    comb[1][1] = 1;
    comb[1][0] = 1;
    for (int n = 2; n <= N; ++n) {
        for (int k = 0; k <= n; ++k) {
            comb[n][k] = comb[n - 1][k] + (k == 0 ? 0 : comb[n - 1][k - 1]);
        }
    }
    return 0;
}();



#include <iostream>
#include <array>
#include <functional>

static const int MAX_N = 20;

template <int N, int K>
//7. The basic template class definition
// the keyword <constexpr> of C++14 makes it easier to define a compiler-time variable.
// static keyword qualifies the fact that for a given N and given K, class CombinationCount<N, K> has an only one value.
struct CombinationCount {
    static constexpr uint64_t value = CombinationCount<N - 1, K>::value + CombinationCount<N-1, K-1>::value;
};

template <int N>
//8. The partial specialization of class  CombinationCount by setting K == 0
// or else the recursion delegation will never be ended.
struct CombinationCount<N, 0> {
    static constexpr uint64_t value = 1;
};

template <int N>
struct CombinationCount<N, N> {
    static constexpr uint64_t value = 1;
};

static uint64_t ret[MAX_N][MAX_N] = {};

template<int N, int K>
constexpr void CombinationPrint()
{
    //6. Here is the instantiation of template function
    //When getting the compilation-time parameter N and K, we can use
    //the mechanism of partial-specialization inner template class
    //to get the ::value.
    ret[N - 1][K] = CombinationCount<N, K + 1>::value;
}

template<int I, int... Js>
constexpr void CombinationPrint(
        std::integer_sequence<int, Js...>)
    //4. Again, we only use the template parameter Js to get type info, not value itself.
{
    //5. AMAZING operation
    // Yeah, it just declares a series of template functions using  <I> and <Js>, the following unpacking
    // procedure would declare template functions by RECURSIVELY!!!
    (CombinationPrint<I, Js>(), ...);
}

template<int... Is>
constexpr void test(
        std::integer_sequence<int, Is...>)
        // 2. You will find this parameter has no-name, because we just need the template parameter "Is", which can be unpacked to [0, 1, ...n -1].
{
    // 3.Unpack variadic template parameter Is, noting that
    // Here we use std::make_integer_sequence again, just for generate different K for each N
    // inner Combination_N_K
    (CombinationPrint<Is>(std::make_integer_sequence<int, Is>{}), ...);
}

#if 0
int main() {
    for (int n = 1; n <= N; ++n) {
        for (int k = 0; k <= n; ++k) {
            std::cout << comb[n][k] << " \n"[k == n];
        }
    }
    return 0;
}
#else
int main()
{
    // 1. auto index_sequence = std::make_integer_sequence<int, MAX_N + 1>{};
    // helps to generate an int sequence [0, 1, ... N]
    auto index_sequence = std::make_integer_sequence<int, MAX_N + 1>{};
    test(index_sequence);

    for (int i = 0; i < MAX_N; ++i) {
        for (int j = 0; j <= i; ++j) {
            std::cout << "C(" << (i+1) << "," << (j + 1) << ")=" << ret[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
#endif


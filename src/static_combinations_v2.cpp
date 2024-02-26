//
// Created by chordr on 10/4/2023.
//
#include <iostream>
#include <array>
#include <functional>
#include <type_traits>

static const int MAX_N = 9;

template <int N, int K>
struct CombinationCount {
    static constexpr uint64_t value = CombinationCount<N - 1, K - 1>::value +
                                      CombinationCount<N - 1, K>::value;
};

template <int N>
struct CombinationCount<N, 0> {
    static constexpr uint64_t value = 1;
};

template <int N>
struct CombinationCount<N, N> {
    static constexpr uint64_t value = 1;
};

static uint64_t ret[MAX_N][MAX_N] = {};

template <int N, int K>
constexpr void CombinationPrint() {
    ret[N - 1][K] = CombinationCount<N, K + 1>::value;
}

template <int I, int... Js>
constexpr typename std::enable_if<I % 2 == 0>::type CombinationPrint(std::integer_sequence<int, Js...>) {
}

template <int I, int... Js>
constexpr typename std::enable_if<I % 2 != 0>::type CombinationPrint(std::integer_sequence<int, Js...>) {
    (CombinationPrint<I, Js>(), ...);
}

template <int... Is>
constexpr void test(std::integer_sequence<int, Is...>)
{
    (CombinationPrint<Is>(std::make_integer_sequence<int, Is> {}), ...);
}

template <typename T, T... Ints>
void print_sequence(std::integer_sequence<T, Ints...> int_seq) {
    std::cout << "The sequence of size " << int_seq.size() << " : ";
    ((std::cout << Ints << ' '), ...);
    std::cout << '\n';
}


int main()
{
    auto index_sequence = std::make_integer_sequence<int, MAX_N + 1>{};

    print_sequence(index_sequence);
    std::cout << std::endl;

    test(index_sequence);

    for (int i = 0; i < MAX_N; ++i) {
        for (int j = 0; j <= i; ++j) {
            std::cout << "C(" << (i+1) << "," << (j + 1) << ")=" << ret[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}

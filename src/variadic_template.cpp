#include <bits/stdc++.h>

//Variadic template, C++11 introduces

//可变参数函数模板是具有可以变数量参数的模板函数

template <typename T>
T min(T a, T b) {
    return a < b ? a : b;
}

template <typename T, typename... Args>
T min(T a, Args... args) {
    return min(a, min(args...));
}

// Core concepts
// Template parameter list: Template args package : typename... Args, which could be type-tm, non-type-tm, double-tm
// Function parameter list: Function args package : Args... args
// Un-package inner function body: args...,
// the result is a comma spilt list consists of zero or multiple value or expressions.

template <typename T, typename... Args>
T sum(T a, Args... args) {
    if constexpr (sizeof...(args) == 0) {
        return a;
    } else {
        return a + sum(args...);
    }
}

template <typename T>
T sum2(T a) {
    return a;
}
template <typename T, typename... Args>
T sum2(T a, Args... args) {
    return a + sum(args...);
}

// sizeof...(args) is not same to sizeof(args...)


// 多参数包不一定只用于可变参数函数模板，也可以用于偏特化中的可变类模板
template <typename, typename >
struct func_pair;

template <typename R1, typename ... A1, typename R2, typename ... A2>
struct func_pair<R1(A1...), R2(A2...)> {
    std::function<R1(A1...)> f;
    std::function<R2(A2...)> g;
};


template <typename... T>
struct sum_wrapper {
    explicit sum_wrapper(T... args) {
        value = (... + args);
    }
    std::common_type_t<T...> value;
};

template <typename... T>
void paraenthesized(T... args) {
    std::array<std::common_type_t<T...>, sizeof...(T)>
        arr{args...};
    sum_wrapper sw1(args...);

    sum_wrapper sw2(++args...);
}

void test_unpack() {
    paraenthesized(1, 2, 3, 4);
}


//How to impl a tuple class
template <typename T, typename... Ts>
struct tuple {
  explicit tuple(const T& t, Ts const& ... ts) : value(t), rest(ts...) {}
  constexpr int size() const {
      return 1 + rest.size();
  }
  T value;
  tuple<Ts...> rest;
};

template <typename T>
struct tuple<T> {
    explicit tuple (const T& t) : value(t) {}
    constexpr int size() const {return 1;}
    T value;
};


template <size_t N, typename T, typename... Ts>
struct nth_type : nth_type<N - 1, Ts...> {
    static_assert(N < sizeof...(Ts) + 1);
};
template <typename T, typename... Ts>
struct nth_type<0, T, Ts...> {
    using value_type = T;
};

template <size_t N, typename... Ts>
typename nth_type<N, Ts...>::value_type & get(tuple<Ts...>& t);
//TODO::


// folding expression
// To folding the element of params package into binary operator

// Classical usage
template <typename... Ts>
auto sum(Ts... args) {
    return (... + args);
    // (... + args) represents folding expression
    // ->  ((arg0 + arg1) + arg2) + .... argN

    // (pack op ...)
    // (... op pack)
    // (pack op ... op init)
    // (init op ... op pack)
}

template <typename ... Ts>
auto sum_from_zero(Ts... args) {
    return (0 + ... + args);
}

void test_sum() {
//    auto v1 = sum(); //Error
    auto v2 = sum_from_zero();
}


//Alias template
template <typename T, typename... Args>
struct foo {
};

template <typename... Args>
using int_foo = foo<int, Args...>;

foo<double, char, int> f1;
foo<int, char, double> f2;
int_foo<char, double> f3;

static_assert(std::is_same_v<decltype(f2), decltype(f3)>);


// integer_sequence
template <typename T, T... Ints>
struct integer_sequence {};

template <std::size_t... Ints>
using index_sequence = integer_sequence<std::size_t, Ints...>;

template <typename T, std::size_t N, T... Ts>
struct make_integer_sequence : make_integer_sequence<T, N - 1, N - 1, Ts...> {};

template <typename T, T... Is>
struct make_integer_sequence<T, 0, Is...> : integer_sequence<T, Is...> {};




template <std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

template <typename... T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;



void integer_test() {
    index_sequence<1, 2, 3, 0, 1, 2, 3> a;
    index_sequence_for<int, double> ss;
}


// Variadic variable template
template <int... R>
constexpr auto Sum = (R + ...);

void test_variadic_variable_template() {
    std::cout << Sum<1, 2, 3> << std::endl;
}


int main(int argc, char** argv) {
    return 0;

    std::vector<std::vector<float>> A, B, D;
    const int M = 64, N = 32, K = 128;
    A.resize(M, std::vector<float>(K));
    B.resize(K, std::vector<float>(N));
    D.resize(M, std::vector<float>(N));

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            float temp = 0.f;
            for (int k = 0; k < K; ++k) {
                temp += A[i][k] * B[k][j];
            }
            D[i][j] = temp;
        }
    }
}


// compilation cal combinations number

constexpr static int MAX_N = 9;
using ret_t = std::array<std::array<uint64_t, MAX_N + 1>, MAX_N + 1>;
constexpr ret_t compiler_time_comb {
    []() {
        ret_t temp {};
        temp[0][0] = 1;
        for (int n = 1; n <= MAX_N; ++n) {
            for (int k = 0; k <= n; ++k) {
                temp[n][k] = temp[n - 1][k] + (k == 0 ? 0 : temp[n - 1][k - 1]);
            }
        }
        return temp;
    }()
};
static_assert(compiler_time_comb[8][2] == 28);



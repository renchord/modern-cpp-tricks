//
// Created by chordr on 2/22/2024.
//
#include <bits/stdc++.h>
namespace ns {
    template <typename T>
    T add(const T a, const T b) {
        return a + b;
    }
    template int add(int, int);
}
using namespace ns;
template double ns::add(double, double);

// When class member is used in return-type or param-type, its member access
// would be ignored.
template <typename T>
class foo {
    struct bar {};
    T f(const bar arg) {
        return {};
    }
};

template int foo<int>::f(foo<int>::bar); //OK

// Explicit template declare
#include "template_basic_features.h"
template struct wrapper_new<int>;

//Template specialization
//Fully specialization
template <typename T>
struct is_floating_point {
    constexpr static bool value = false;
};

template <>
struct is_floating_point<float> {
    constexpr static bool value = true;
};

template <>
struct is_floating_point<double> {
    constexpr static bool value = true;
};

template <>
struct is_floating_point<long double> {
    constexpr static bool value = true;
};

static_assert(is_floating_point<double>::value);
static_assert(!is_floating_point<int>::value);

// The static member could be fully specialized
template <typename T>
struct foo2 {
    static T value;
};
template <typename T> T foo2<T>::value = 0;
template <> int foo2<int>::value = 42;

//TM specialization could be declared without definition
template <typename T>
struct foo3 {};

template <>
struct foo3<int>;

foo3<double> a;
// foo3<int> b; //(incomplete)
foo3<int>* b; //it is OK

// When TM function specialization
// The compiler may could deduct the template arguments
// from function arguments type
template <typename T>
struct foo4 {};

template <typename T>
void func(foo4<T>) {
    std::cout << "primary" << std::endl;
}
template <>
void func(foo4<int>) {
    std::cout << "int" << std::endl;
}

//Important, the function  and member function TM not allow to use default parameters
template <typename T>
void fff(T) {}

//Default argument not permitted on an explicit specialization of function 'fff'
//template <>
//void fff(int a = 0) {
//
//}


//Partial specialization
// Specializing one primary template but only specifying not all template parameters
// Thus, it would have template parameters and template arguments at the meantime.

template <typename T, int S>
struct collection {
    void operator()() {
        std::cout << "Primary template" << std::endl;
    }
};

template <typename T>
struct collection<T, 10> {
    void operator()() {
        std::cout << "Partial specialization<T, 10>" << std::endl;
    }
};

template <int S>
struct collection<int, S> {
    void operator()() {
        std::cout << "Partial specialization<int, S>" << std::endl;
    }
};

template <typename T, int S>
struct collection<T*, S> {
    void operator()() {
        std::cout << "Partial specialization<T*, S>" << std::endl;
    }
};

template <typename T, typename U>
struct is_same_type : public std::false_type {

};

template <typename T>
struct  is_same_type<T, T> : public std::true_type {

};

// Variable Template C++14 introduces
template <typename T>
constexpr T PI = T{3.14};

template <typename T>
T vol(const T r) {
    return 4 * PI<T> * r * r * r / 3;
}

// Template Alias
template <typename T>
using customer_addresses_t = std::map<int, std::vector<T>>;

struct delivery_address_t {};
struct invoice_address_t {};

using cu_d_a_t = customer_addresses_t<delivery_address_t>;
using cu_i_a_t = customer_addresses_t<invoice_address_t>;


// Generic lambda and lambda template
// Generic lambda, C++14 introduces, allow to use auto instead of specifying parameter types.
    // which to convert to generated functors -> it would have template calling functions
// Lambda template, C++20 introduces
// The difference is that generic lambda use auto to deduct parameter type but could not
// apply any restriction on the type
void test_lambda() {
    auto l1 = [](int a) {
        return a + a; // C++ 11
    };

    auto l2 = [](auto a) {
        return a + a; // C++ 14 generic lambda
    };

    auto l3 = []<typename T>(T a) {
        return a + a;
    };

    auto v1 = l1(42);
    auto v2 = l1(42.0); //warning
//    auto v3 = l1(std::string{"42"}); //Error
    auto v5 = l2(42);
    auto v6 = l2(42);
    auto v7 = l2(std::string{"42"});

    auto v8 = l3(42);
    auto v9 = l3(42);
    auto v10 = l3(std::string{"42"});
}

// Any other case is
void another_case() {
    auto l = []<typename T, size_t N>(const std::array<T, N>& arr) {
        return std::accumulate(arr.begin(), arr.end(), static_cast<T>(0));
    };

    auto v1 = l(1); //Error
    auto v2 = l(std::array<int, 3> {1, 2, 3});
}

// Generic lambda could be recursively called easier
void recur_lambda() {
    auto fra = [](auto f, const int n) {
        if (n < 2) return 1;
        else return n * f(f, n - 1);
    };

    auto c = fra(fra, 5);

    struct lambda {
        template <class T1>
        inline auto operator() (T1 f, const int n) const {
            if (n < 2) return 1;
            else return n * f(f, n - 1);
        }

        template <>
        inline int operator()(lambda f, const int n) const {
            if (n < 2) return 1;
            else return n * f.operator()(lambda(f), n - 1);
        }
    };
    lambda fra_inline = lambda {};
    auto cc = fra_inline(fra_inline, 5);
}


int main() {
    func(foo4<int>{});
    func(foo4<double>{});

    collection<char, 42> a;
    collection<int, 42> b;
    collection<double, 10> c;
    collection<int*, 20> d;
    a.operator()();
    b.operator()();
    c.operator()();
    d.operator()();

//    collection<int, 10> f; // 二义性
    test_lambda();

    return 0;
}
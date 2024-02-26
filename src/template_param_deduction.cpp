#include <bits/stdc++.h>
// Forward reference
template <typename T>
void f(T&& arg) // forwarding reference
{
    std::cout << "f(T&&)\n";
}

template <typename T>
void f(const T&& arg) //rvalue reference
{
    std::cout << "f(const T&&)\n";
}

template <typename T>
void f(std::vector<T>&& arg) //rvalue reference
{
    std::cout << "f(vector<T>&&)\n";
}

template <typename T>
struct S {
    void f(T&& arg) { //rvalue reference
        std::cout << "S.f(T&&)\n";
    }
};

void test1() {
    int x = 42;
    f(x);
    f(42);

    const int cx = 100;
    f(cx);
    f(std::move(cx));

    std::vector<int> v{42};
    f(v);
    f(std::vector<int>{42});

    S<int> s;
//    s.f(x);
    s.f(42);
}



// Reference folding
using lrefint = int&;
using rrefint = int&&;
int x = 42;
lrefint & r1 = x;
lrefint && r2 = x;
rrefint  & r3 = x;
rrefint && r4 = 1;

// Must remember that
// Forwarding reference only apply for function template
// And only T&&
// Must not add any cv like const T&& volatile T&&

int b = 42;
auto&& rx = b; // int&
auto&& rc = 42; //int&&
//const auto&& rcx = b; //Error



int main () {

    test1();
    return 0;
}


//decltype
// return the type of expression

int ff() {
    return 42;
}
int g() {
    return 0;
}
int g(int a) {
    return a;
}

struct wrapper {
    int val;
    int get() const { return val; }
};

int a = 42;
int& ra = a;
const double d = 42.99;
long arr[10];
long l = 0;
char* p = nullptr;
char c = 'x';
wrapper w1 {1};
wrapper* w2 = new wrapper {2};

decltype(a) e1;
static_assert(std::is_same_v<decltype(e1), decltype(a)>);

decltype(ra) e2 = a; //e2 is int&

decltype(ff) e3; // int();
decltype(g) e4;
static_assert(std::is_same_v<decltype(++a), int&>);
static_assert(std::is_same_v<decltype(a + 1), int>);

int chord = 3;
auto d_c = chord;
const auto c_d = chord;
static_assert(std::is_same_v<decltype(c_d), const int>);
const int c_chord = 4;
auto ccc_d = c_chord;
static_assert(std::is_same_v<decltype(ccc_d), int>);

volatile int v_chord = 5;
auto d_v_chord = v_chord;
static_assert(std::is_same_v<decltype(d_v_chord), int>);

namespace {
    int num = 4;
    int &num_r = num;
    auto a = num_r;
    static_assert(std::is_same_v<decltype(a), int>);
    auto& a_r = num_r;
    static_assert(std::is_same_v<decltype(a_r), int&>);
    auto& a_r_2 = num;
    static_assert(std::is_same_v<decltype(a_r_2), int&>);
//    auto& a_4 = 5; //Error
    auto&& a_5 = 5;
}

namespace scu {
    template <typename T>
    auto begin(T& c) {
        return c.begin();
    }
    template <typename T, size_t N>
    T* begin(T(&arr)[N]) {
        return arr;
    }
}

namespace scu {
    template <bool B, typename T = void>
    struct enable_if {};

    template <typename T>
    struct enable_if<true, T> {
        using type = T;
    };
    // Enable if is used in bool expression,
    // when it can be evaluated as true, it would have a type-member named type

//    template <typename T, typename enable_if<uses_write<T>>::type* = nullptr> ;
//    template <typename T, typename enable_if<!uses_write<T>>::type* nullptr>;
    const std::vector<int> fff() {
        return {};
    };

    struct miles {

    };
    const miles f2() {
        return {};
    }
    decltype(fff()) b {};
    decltype(f2()) c {};
    static_assert(std::is_same_v<decltype(b), const std::vector<int>>);
    static_assert(std::is_same_v<decltype(c), const miles>);


    template <typename T, typename = typename std::enable_if<std::is_integral_v<T>>::type>
    struct integral_wrapper {
        T value;
    };
    template <typename T, typename = typename std::enable_if<std::is_floating_point_v<T>>::type>
    struct floating_wrapper {
        T value;
    };

    integral_wrapper<int> int_a;
    floating_wrapper<float> float_a;

    template <bool B, typename T = void>
    using enable_if_t = typename enable_if<B, T>::type;

}


struct bar {
    int a = 0;
    bar(const bar& rhs) = delete;
//    bar& operator=(const bar& rhs) {}
//    bar(bar&& rhs) {};
};
static_assert(std::is_trivially_copyable_v<bar>);



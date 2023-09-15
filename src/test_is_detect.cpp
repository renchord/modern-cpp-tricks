#include <iostream>
#include <type_traits>

// void_t in C++17, which maps a sequence of any types into the type void. Which helps in SFINAE.
namespace detail_cpp17 {
template <typename...>
using void_t = void;
}

// From cpp reference:
// A C++11 defect: unused parameters in alias template were not guaranteed to ensure SFINAE and could be ignored.
// void_t in C++11
namespace detail_cpp11 {
template <typename... T>
struct make_void {
    typedef void type;
};
template <typename... T>
using void_t = typename make_void<T...>::type;
}

// The first basic usage inner cpp reference

// Detect if existing a nested "type_match" type definition inner T
namespace {
template<typename T, typename = void>
struct has_type_match : std::false_type {
};
template<typename T>
struct has_type_match<T, detail_cpp17::void_t<typename T::type_match>> : std::true_type {
};

struct A {
    typedef int type_match;
};
struct B {};

static_assert(has_type_match<A>::value);
static_assert(!has_type_match<B>::value);
}


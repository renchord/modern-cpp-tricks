#include <iostream>
#include <type_traits>

#include <vector>

// Impl of enable if by SFINAE
namespace detail {
// The default type void would help to generate this version with T = void, bool = false.
template <bool, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
    typedef T type;
};
}

// SFINAE logic
// If the first template parameter inner template class enable_if is true in compiler time
// The compiler thinks the later one is a better candidate for specialization for the template class.
// which results in it has type definition
// Thus the former one template function would not be generated due to SFINAE logic too.
template <typename T>
typename detail::enable_if<std::is_trivial<T>::value>::type test_enable_if_match_1(T value) {
    std::cout << "Matched the trivial calling function." << std::endl;
}

template <typename T>
typename detail::enable_if<!std::is_trivial<T>::value>::type test_enable_if_match_1(T value) {
    std::cout << "Matched the non-trivial calling function." << std::endl;
}

namespace detail {
    void* voidify(const volatile void* ptr) noexcept {
        return const_cast<void*>(ptr);
    }
}

template <typename T>
typename detail::enable_if<std::is_trivially_default_constructible<T>::value>::type construct(T*) {
    std::cout << "default constructing trivially default constructible T\n";
}

template <typename T>
typename detail::enable_if<!std::is_trivially_default_constructible<T>::value>::type construct(T* p) {
    std::cout << "default constructing non-trivially default constructible T\n";
    ::new(detail::voidify(p)) T; //placement new
}

template <class T>
void destroy(T*, typename detail::enable_if<std::is_trivially_destructible<T>::value>::type* = 0) {
    std::cout << "destorying trivially destructible T\n";
}

template <class T,
        typename detail::enable_if<
                !std::is_trivially_destructible<T>::value &&
                (std::is_class<T>{} || std::is_union<T>{}),
                bool>::type = true>
void destroy(T* t) {
    std::cout << "destorying non-trivially destrtuctible T\n";
    t->~T(); // placement destruct calling
}

template <class T, typename = std::enable_if_t<std::is_array<T>::value>>
void destroy(T* t) {
    for (size_t i = 0; i < std::extent<T>::value; ++i) {
        destroy((*t)[i]);
    }
}

int main() {

    // test1
    {
        std::vector<int> nums;
        test_enable_if_match_1(nums);
        int a;
        test_enable_if_match_1(a);
    }

    // test2
    {
        typename detail::enable_if<true, int>::type t; // OK
        typename detail::enable_if<true>::type; // OK
        typename detail::enable_if<false> {}; // OK
        typename detail::enable_if<false>; // OK
        // typename detail::enable_if<false, int>::type; // Error
    }

    return 0;
}
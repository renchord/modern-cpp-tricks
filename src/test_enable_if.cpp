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
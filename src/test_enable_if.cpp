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

template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;
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

template <typename T, typename... Args>
typename detail::enable_if<std::is_constructible<T, Args&& ...>::value>::type construct(T* p, Args&& ... args) {
    std::cout << "constructing T with operation\n";
    ::new(detail::voidify((p))) T(static_cast<Args&&>(args)...);
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

template <class T, typename = detail::enable_if_t<std::is_array<T>::value>>
void destroy(T* t) {
    for (size_t i = 0; i < std::extent<T>::value; ++i) {
        destroy((*t)[i]);
    }
}

// Now illustrate a common mistake when using std::enable_if
#if 0
struct T{
    enum {int_t, float_t} type;
    template <typename Integer, typename = detail::enable_if_t<std::is_integral<Integer>::value>>
            T(Integer) : type(int_t) {}
    template <typename Floating, typename = detail::enable_if_t<std::is_floating_point<Floating>::value>>
            T(Floating): type(float_t) {} // Error would be treated as redefinition from CPP-reference said
};
#else
struct T{
    enum {int_t, float_t} type;
    template <typename Integer, typename detail::enable_if<std::is_integral<Integer>::value, bool>::type = true>
            T(Integer) : type(int_t) {}
    template <typename Floating, typename detail::enable_if<std::is_floating_point<Floating>::value, bool>::type = true>
            T(Floating) : type(float_t) {}
};
#endif

#if 0
struct AA {
    int m_int;
    template <typename T, typename = detail::enable_if_t<std::is_same<T, int>::value>>
        AA(T) : m_int(0) {}
    template <typename T, typename = detail::enable_if_t<!std::is_same<T, int>::value>>
        AA(T) : m_int(1) {}
};
#else
struct AA {
    int m_int;
    template <typename T, detail::enable_if_t<std::is_same<T, int>::value, int> = 1>
    AA(T) : m_int(0) {}
    template <typename T, detail::enable_if_t<!std::is_same<T, int>::value, int> = 1>
    AA(T) : m_int(1) {}
};
#endif

template <class T, class Enable = void>
class JJ {};

template <class T>
class JJ<T, typename detail::enable_if<std::is_floating_point<T>::value>::type> {};





template <typename T, int Size>
void f(T (&array)[Size]) {
    std::cout << "1" << std::endl;
}

template <typename T, int Size>
void f(T* array) {
    std::cout << "2" << std::endl;
}

template <typename T>
std::enable_if_t<std::is_pointer_v<T>>
f (T ptr) {
    std::cout << "3" << std::endl;
}

void sfinae_test() {
    int array[3];
    f(array);
//    auto ptr = &array;
//    f(ptr);
}

template<typename T>
class A {
    public:
// this does not work because T depends on A, not on h
    template<typename U>
     void h(T,
     std::enable_if_t<std::is_signed_v<T>, int> = 0) {
        std::cout << "signed1";
     }
    template<typename R = T> // now R dependes on h
    void h(R,
           std::enable_if_t<std::is_signed_v<R>, int> = 0) {
        std:: cout << "signed2";
    }
};


int main() {
    A<int> a;
    a.template h(3);
    // test1
    {
        std::vector<int> nums;
        test_enable_if_match_1(nums);
        int a;
        test_enable_if_match_1(a);
    }

    // test2
    {
        // typename detail::enable_if<true, int>::type t; // OK
        // typename detail::enable_if<true>::type; // OK
        // typename detail::enable_if<false>{}; // OK
        // typename detail::enable_if<false>; // OK
        // typename detail::enable_if<false, int>::type; // Error
    }

    // test3
    {
        JJ<int>{};
        JJ<float>{};
    }

    // test4
    {
        union {
            int i;
            char s[sizeof(std::string)];
        } u;
        construct(reinterpret_cast<int*>(&u));
        destroy(reinterpret_cast<int*>(&u));

        construct(reinterpret_cast<std::string*>(&u), "Hello");
        destroy(reinterpret_cast<std::string*>(&u));

    }

    sfinae_test();

    return 0;
}


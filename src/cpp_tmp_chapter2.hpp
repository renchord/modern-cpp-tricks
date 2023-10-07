//
// Created by chordr on 10/4/2023.
//
#include "type_traits"


//2-0. Write a unary metafunction add_const_ref<T> that returns T if it is a
//        reference type, and otherwise returns T const&.  Write a program to test
//        your metafunction.  Hint: you can use boost::is_same to test the results.
template <class T>
struct add_const_ref {
    using type = T const &;
};
template <class T>
struct add_const_ref<T&> {
    using type = T;
};
template <class T>
struct add_const_ref<T const> {
    using type = T;
};

// test
static_assert(std::is_same_v<const int&, add_const_ref<int>::type>);
static_assert(std::is_same_v<int, add_const_ref<int&>::type>);
class dummyA {};
static_assert(std::is_same_v<const dummyA&, add_const_ref<dummyA>::type>);
static_assert(std::is_same_v<const int, add_const_ref<const int &>::type>);




//2-1. Write a ternary metafunction replace_type<c,x,y> that takes an arbitrary
//        compound type c as its first parameter, and replaces all occurrences of a
//        type x within c with y:

template <class C, class X, class Y>
struct replace_type {};

template <class C, class X, class Y, bool Same>
struct replace_type_impl {};

template <class C, class X, class Y> // If the type of C is same to X.
struct replace_type_impl <C, X, Y, true>{
    using type = Y;
};

template <class C, class X, class Y> // If not, solve again.
struct replace_type_impl <C, X, Y, false> {
    using type = typename replace_type<C, X, Y>::type;
};

template <class C, class X, class Y>
struct replace_type<C*, X, Y> {
    using type = typename std::add_pointer<
            typename replace_type_impl<
                    C,
                    X,
                    Y,
                    std::is_same<C, X>::value
                    >::type
            >::type;
};

template <class C, class X, class Y>
struct replace_type<C&, X, Y> {
    using type = typename std::add_lvalue_reference<
            typename replace_type_impl<
                    C,
                    X,
                    Y,
                    std::is_same<C, X>::value
                    >::type
            >::type;
};

template <class C, class X, class Y>
struct replace_type<C&&, X, Y> {
    using type = typename std::add_rvalue_reference<
            typename replace_type_impl<
                    C,
                    X,
                    Y,
                    std::is_same<C, X>::value
            >::type
    >::type;
};

template <class C, class X, class Y, std::size_t N>
struct replace_type<C[N], X, Y> {
private:
    using ret = typename replace_type_impl<
            C,
            X,
            Y,
            std::is_same<C,X>::value>::type;
public:
    using type = ret[N];
};

template <class C, class X, class Y>
struct replace_type<C(*)(), X, Y> {
private:
    using ret = typename replace_type_impl<
            C,
            X,
            Y,
            std::is_same<C, X>::value>::type;
public:
    using type = ret(*)();
};

template <class Cr, class C0, class X, class Y>
struct replace_type<Cr(*)(C0), X, Y> {
private:
    using ret = typename replace_type_impl<
            Cr, X, Y, std::is_same<Cr, X>::value>::type;
    using par0 = typename replace_type_impl<
            C0, X, Y, std::is_same<C0, X>::value>::type;
public:
    using type = ret(*)(par0);
};

template <class Cr, class C0, class C1, class X, class Y>
struct replace_type<Cr(*)(C0, C1), X, Y> {
private:
    using ret = typename replace_type_impl<
            Cr, X, Y, std::is_same<Cr, X>::value>::type ;
    using par0 = typename replace_type_impl<
            C0, X, Y, std::is_same<C0, X>::value>::type;
    using par1 = typename replace_type_impl<
            C1, X, Y, std::is_same<C1, X>::value>::type;
public:
    using type = ret(*)(par0, par1);
};

// test
static_assert(std::is_same_v<replace_type<void*, void, int>::type, int*>);
static_assert(std::is_same_v<replace_type<int const * [10], int const, long>::type, long * [10]>);
static_assert(std::is_same_v<replace_type<char& (*) (char&), char&, long long &>::type, long long &(*) (long long&)>);
static_assert(std::is_same_v<replace_type<int&, int, char>::type, char&>);
static_assert(std::is_same_v<replace_type<int&&, int, char>::type, char&&>);

// 2-2. The boost::polymorphic_downcast function template implements a checked
//     version of static_cast intended for down-casting pointers to polymorphic
//     objects:
//     In released software, the assertion disappears and polymorphic_downcast
//     can be as efficient as a simple static_cast.  Use the type traits
//     facilities to write an implementation of the template that allows both
//     pointer and reference arguments.
template <class T, class S>
inline T polymorphic_downcast(S* x)
{
    assert(dynamic_cast<T>(x) == x);
    return static_cast<T>(x);
}

template <class T, class S>
inline T polymorphic_downcast(S& x) {
    assert(dynamic_cast<typename std::add_pointer<
            typename std::remove_reference<T>::type
            >::type>(&x) == &x);
    return static_cast<T>(x);
}


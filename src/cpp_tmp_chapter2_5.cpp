//
// Created by chordr on 10/7/2023.
//
#include <iostream>

template <class T>
struct type_descriptor {};

template <class T>
struct type_descriptor_impl {};

template <class T>
std::ostream& operator<<(std::ostream& os, const type_descriptor<T>& m) {
    return os << type_descriptor_impl<T>() << std::endl;
}

template <class T>
struct dirty {};

template <class T>
std::ostream& operator<<(std::ostream& os, const dirty<T>& m) {
    return os;
}

template <class T>
std::ostream & operator<<(std::ostream& os, const dirty<T()>& m) {
    return os << "function returning " << type_descriptor_impl<T>();
}

template <class T>
std::ostream & operator<<(std::ostream& os, const dirty<T(*)()>& m) {
    return os << "pointer to function returning " << type_descriptor<T>();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<T>& m) {
    if (std::is_lvalue_reference_v<T>) {
        os << "lv-reference to " << type_descriptor_impl<std::remove_reference_t<T>>();
    } else if (std::is_rvalue_reference_v<T>) {
        os << "rv-reference to " << type_descriptor_impl<std::remove_reference_t<T>>();
    } else if (std::is_volatile_v<T>) {
        os << "volatile " << type_descriptor_impl<std::remove_volatile_t<T>>();
    } else if (std::is_pointer_v<T>) {
        os << "pointer to " << type_descriptor_impl<std::remove_pointer_t<T>>();
    } else if (std::is_const_v<T>) {
        os << "const " << type_descriptor_impl<std::remove_const_t<T>>();
    } else if (std::is_same_v<int, T>){
        os << "int";
    } else if (std::is_same_v<char, T>) {
        os << "char";
    } else if (std::is_same_v<long, T>) {
        os << "long";
    } else if (std::is_same_v<short, T>) {
        os << "short";
    } else if (std::is_array_v<T>) {
        using nextT = std::remove_extent_t<T>;
        auto n = std::extent_v<T>;
        if (n == 0) {
            os << "array of " << type_descriptor_impl<nextT>();
        } else {
            os << std::extent_v<T> << " element array of " << type_descriptor_impl<nextT>();
        }

    } else if (std::is_function_v<T>) {
        os << dirty<T>();
    }
    return os;
}

int main () {
    std::cout << type_descriptor<const int *>();
    std::cout << type_descriptor<char*>();
    std::cout << type_descriptor<const short&&>();
    std::cout << type_descriptor<long const* &>();
    std::cout << type_descriptor<volatile int&&>();
    std::cout << type_descriptor<int[3]>();
    std::cout << type_descriptor<int[3][2]>();
    std::cout << type_descriptor<int[][2][2][5]>();
    std::cout << type_descriptor<int()>();
    std::cout << type_descriptor<char *(*[])()>();
    std::cout << type_descriptor<long[]>();
    return 0;
}
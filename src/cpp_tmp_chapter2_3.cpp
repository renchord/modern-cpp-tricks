//
// Created by chordr on 10/7/2023.
//

//2-3. Use the type traits facilities to implement a type_descriptor class
//     template, whose instances, when streamed, print the type of their
//     template parameters:
//// prints "int"
//std::cout << type_descriptor<int>();
//// prints "char*"
//std::cout << type_descriptor<char*>();
//// prints "long const*&"
//std::cout << type_descriptor<long const*&>();
//     You can assume that type_descriptor’s template parameter is limited to
//     compound types built from the following four integral types: char,
//     short int, int, and long int.

#include <iostream>

template <class T>
struct type_descriptor {
};

template <class T>
struct type_descriptor_impl {
};

template <class T>
std::ostream& operator<<(std::ostream& os, const type_descriptor<T>& m) {
    return os << type_descriptor_impl<T>() << std::endl;
}

template <class T>
void getCorrectExtent(std::ostream& os) {
    os << "[" << std::extent_v<T> << "]";
    using nextT = std::remove_extent_t<T>;
    if (std::is_array_v<nextT>) {
        getCorrectExtent<nextT>(os);
    }
}

template <class T>
std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<T>& m) {
    if (std::is_lvalue_reference_v<T>) {
        os << type_descriptor_impl<std::remove_reference_t<T>>() << "&";
    } else if (std::is_rvalue_reference_v<T>) {
        os << type_descriptor_impl<std::remove_reference_t<T>>() << "&&";
    } else if (std::is_volatile_v<T>) {
        os << type_descriptor_impl<std::remove_volatile_t<T>>() << " volatile";
    } else if (std::is_pointer_v<T>) {
        os << type_descriptor_impl<std::remove_pointer_t<T>>() << "*";
    } else if (std::is_const_v<T>) {
        os << type_descriptor_impl<std::remove_const_t<T>>() << " const";
    } else if (std::is_same_v<int, T>){
        os << "int";
    } else if (std::is_same_v<char, T>) {
        os << "char";
    } else if (std::is_same_v<long, T>) {
        os << "long";
    } else if (std::is_same_v<short, T>) {
        os << "short";
    } else if (std::is_array_v<T>) {
        using nextT = std::remove_all_extents_t<T>;
        os << type_descriptor_impl<nextT>();
        getCorrectExtent<T>(os);
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
    std::cout << type_descriptor<int[3][2][2][5]>();
    return 0;
}
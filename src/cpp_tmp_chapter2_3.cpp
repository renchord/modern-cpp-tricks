//
// Created by chordr on 10/7/2023.
//

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
std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<T>& m) {
    if (std::is_lvalue_reference_v<T>) {
        os << type_descriptor_impl<std::remove_reference_t<T>>() << "&";
    } else if (std::is_rvalue_reference_v<T>) {
        os << type_descriptor_impl<std::remove_reference_t<T>>() << "&&";
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
    }
    return os;
}


int main () {
    std::cout << type_descriptor<const int *>();
    std::cout << type_descriptor<char*>();
    std::cout << type_descriptor<const short&&>();
    std::cout << type_descriptor<long const* &>();
    return 0;
}
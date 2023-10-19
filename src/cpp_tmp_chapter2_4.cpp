
//2-4. Write an alternative solution for exercise 2-3 that does not use the
//     Type Traits library.  Compare the solutions.
#include <iostream>

template <class T>
struct type_descriptor {};


template <class T>
struct type_descriptor_impl {};

template <class T>
std::ostream& operator<<(std::ostream& os, const type_descriptor<T>& m) {
    return os << type_descriptor_impl<T>() << std::endl;
}

std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<int>& m) {
    return os << "int";
}

std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<char>& m) {
    return os << "char";
}

std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<short>& m) {
    return os << "short";
}

std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<long>& m) {
    return os << "long";
}

template <class T>
std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<T&>& m) {
    return os << type_descriptor_impl<T>() << "&";
}


template <class T>
std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<T const >& m) {
    return os << type_descriptor_impl<T>() << " const";
}
template <class T>
std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<T&&>& m) {
    return os << type_descriptor_impl<T>() << "&&";
}

template <class T>
std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<T*>& m) {
    return os << type_descriptor_impl<T>() << "*";
}

template <class T>
std::ostream& operator<<(std::ostream& os, const type_descriptor_impl<volatile T>& m) {
    return os << type_descriptor_impl<T>() << " volatile";
}

int main () {
    std::cout << type_descriptor<int>();
    std::cout << type_descriptor<int*>();
    std::cout << type_descriptor<int&>();
    std::cout << type_descriptor<char*>();
    std::cout << type_descriptor<volatile int&>();
    std::cout << type_descriptor<const short&>();
    std::cout << type_descriptor<long const * const & >();
    std::cout << type_descriptor<char&&>();

    // i wanner not to impl array version.
    return 0;
}
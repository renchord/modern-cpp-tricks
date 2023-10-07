//
// Created by chordr on 10/7/2023.
//

#ifndef MODERN_CPP_TRICKS_CPP_TMP_CHAPTER2_3_HPP
#define MODERN_CPP_TRICKS_CPP_TMP_CHAPTER2_3_HPP

#include <iostream>
using std::ostream;


// Basic type
template <class T>
struct type_descriptor_impl {
    static constexpr char* name = ERROR; // TODO:
};

#define REG_TYPE_DESCRIPTOR(_type) \
    template <> \
    struct type_descriptor_impl <_type> \
    {                              \
        constexpr static char* const name = #_type;       \
    };

REG_TYPE_DESCRIPTOR(char);
REG_TYPE_DESCRIPTOR(short int);
REG_TYPE_DESCRIPTOR(int);
REG_TYPE_DESCRIPTOR(long int);

template <class T>
struct type_descriptor {
    constexpr static char* const name = type_descriptor_impl<T>::name;
};

template <class T>
struct type_descriptor_dispatch {

};




#endif //MODERN_CPP_TRICKS_CPP_TMP_CHAPTER2_3_HPP

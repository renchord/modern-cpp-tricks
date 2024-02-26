#ifndef MODERN_CPP_TRICKS_TEMPLATE_BASIC_FEATURES_H
#define MODERN_CPP_TRICKS_TEMPLATE_BASIC_FEATURES_H

template <typename T>
struct wrapper_new{
    T data;
};

extern template struct wrapper_new<int>; //


#endif //MODERN_CPP_TRICKS_TEMPLATE_BASIC_FEATURES_H

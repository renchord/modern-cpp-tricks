#include <iostream>
#include <type_traits>

template <typename T, typename = void>
struct has_doSomething : std::false_type {};

template <typename T>
struct has_doSomething<T, std::void_t<decltype(std::declval<T&>().doSomethingImpl())>> : std::true_type {};

template <class T>
class Base {
public:
    void doSomething() {
        if constexpr (has_doSomething<T>::value) {
            GetDerivedPtr()->doSomethingImpl();
        } else {
            std::cout << "Can not detect." << std::endl;
        }
    }
private:
    T* GetDerivedPtr() {
        return static_cast<T*>(this);
    }
};

class Derived1 : public Base<Derived1> {
public:
    void doSomethingImpl() {
        std::cout << "Call Derived1 impl" << std::endl;
    }
};

class Derived2 : public Base<Derived2> {
public:
    void doSomethingImpl() {
        std::cout << "Call Derived2 impl" << std::endl;
    }
};

class Derived3 : public Base<Derived3> {
public:
    void doSomethingElse() {
        std::cout << "Call Derived3 impl" << std::endl;
    }
};

template <class T>
void dispatch(Base<T>& ref) {
    ref.doSomething();
}

int main() {
    Derived1 d1;
    Derived2 d2;
    Derived3 d3;
    dispatch(d1);
    dispatch(d2);
    dispatch(d3);
    return 0;
}
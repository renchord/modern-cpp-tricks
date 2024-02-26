#include "bits/stdc++.h"

// Thread-safe Singleton
class Singleton {
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
private:
    Singleton() = default;
};

class A{
public:
    A();
    ~A();
    void f();
private:
    class Impl;
    Impl* m_ptr;
};
class A::Impl {
public:
    void internal_f() {

    }
private:
    int data_1;
    int data_2;
};

A::A() : m_ptr(new A::Impl()) {
};
A::~A() {
    delete m_ptr;
}
void A::f() {
    m_ptr->internal_f();
}

struct MyINT {
public:
    const MyINT operator++(int) {
        auto temp = *this;
        ++(*this);
        return temp;
    }
    MyINT& operator++() {
        this->a++;
        return *this;
    }
private:
    int a = 0;
};

int main () {
    std::atomic<int> counter;
    counter.store(2);
    int old = 1;
    bool equ = counter.compare_exchange_strong(old, 3);
    std::cout << "Equal= " << equ << std::endl;
    std::cout << "old= " << old << std::endl;

    int now = counter.load();
    std::cout << "cnt= " << now << std::endl;

    old = 2;
    equ = counter.compare_exchange_strong(old, 3);
    std::cout << "Equal= " << equ << std::endl;
    std::cout << "old= " << old << std::endl;

    now = counter.load();
    std::cout << "cnt= " << now << std::endl;

    return 0;

}
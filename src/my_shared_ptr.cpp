#pragma once
#include <atomic>
namespace details {
    template <typename T>
    class shared_ptr {
        using Type = shared_ptr<T>;
    public:
        shared_ptr(T* ptr) : m_ptr(ptr), p_cnt(new size_t(1)) {}
        ~shared_ptr() {
            --(*p_cnt);
            if (*p_cnt == 0) {
                delete m_ptr;
                delete p_cnt;
            }
        }
        shared_ptr(const Type& rhs) {
            m_ptr = rhs.m_ptr;
            p_cnt = rhs.p_cnt;
            ++(*p_cnt);
        }
        shared_ptr(Type&& rhs) {
            m_ptr = rhs.m_ptr;
            p_cnt = rhs.p_cnt;
        }
        Type& operator=(const Type& rhs) {
            if (this == &rhs) return *this;
            if (--*p_cnt == 0) {
                delete p_cnt;
                delete m_ptr;
            }
            m_ptr = rhs.m_ptr;
            p_cnt = rhs.p_cnt;
            ++(*p_cnt);
        }
        T* operator->() {
            return m_ptr;
        }
        T& operator*() {
            return *m_ptr;
        }
        size_t count() const {
            if (p_cnt) return *p_cnt;
            return 0;
        }
        T* get() const {
            return m_ptr;
        }
    private:
        T* m_ptr;
        size_t* p_cnt;
    };
}

using details::shared_ptr;
#include <iostream>
int main() {
    shared_ptr<int> pa(new int(3));
    std::cout << pa.count() << std::endl;
    auto pb = pa;
    std::cout << pb.count() << std::endl;
    {
        auto pc = std::move(pb);
        std::cout << pc.count() << std::endl;
    }
    return 0;
}

namespace details {
    template <typename T>
    constexpr typename std::remove_reference<T>::type&& move(T&& t) {
        return static_cast<typename std::remove_reference<T>::type&&>(t);
    }

    template <typename T>
    constexpr T&& forward(typename std::remove_reference<T>::type& t) {
        return static_cast<T&&>(t);
    }

    template <typename T>
    constexpr T&& forward(typename std::remove_reference<T>::type&& t) {
        return static_cast<T&&>(t);
    }
}

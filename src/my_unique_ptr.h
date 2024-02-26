#ifndef MODERN_CPP_TRICKS_MY_UNIQUE_PTR_H
#define MODERN_CPP_TRICKS_MY_UNIQUE_PTR_H

#include <utility>

template <class T, class U>
T exchange(T& dst, U&& val) { //dst换成val 并且将dst原来的值返回
    T tmp = std::move(dst); //构造一个临时变量
    dst = std::forward<U>(val); //如果U是lvalue-ref 调用operator, rvalue调用move operator
    return tmp;
}

template <class T>
struct DefaultDeleter {
    void operator()(T* p) const {
        delete p;
    }
};

template <class T>
struct DefaultDeleter<T[]> {
    void operator()(T *p) const {
        delete[] p;
    }
};


template <typename T, class Deleter = DefaultDeleter<T>>
struct unique_ptr {
private:
    T* m_p; //Hold the original pointer
    template <class U, class UDeleter>
    friend class unique_ptr;
public:
    unique_ptr(std::nullptr_t dummy = nullptr) {
        m_p = nullptr;
    }
    explicit unique_ptr(T * p) {
        m_p = p;
    }
    template <class U, class UDeleter, class = std::enable_if_t<std::is_convertible_v<U *, T*>>>
    unique_ptr(unique_ptr<U, UDeleter>&& rhs) {
        m_p = exchange(rhs.m_p, nullptr); //
    }
    ~unique_ptr() {
        if (m_p) Deleter{}(m_p); //用一个临时对象调用operator
    }

    unique_ptr(unique_ptr const& rhs) = delete;
    unique_ptr& operator=(const unique_ptr& rhs) = delete;

    unique_ptr(unique_ptr&& rhs) {
        m_p = exchange(rhs.m_p, nullptr);
    }
    unique_ptr& operator=(unique_ptr&& rhs) {
        if (this != &rhs) {
            if (m_p) Deleter{}(m_p);
            m_p = exchange(rhs.m_p, nullptr);
        }
        return *this;
    }

    T* get() const {
        return m_p;
    }
    T* release() {
        return exchange(this->m_p, nullptr);
    }
    void reset(T* p = nullptr) {
        if (m_p) Deleter{}(m_p);
        m_p = p;
    }
    T& operator*() const {
        return *m_p;
    }
    T* operator->() const {
        return m_p;
    }
};

template <class T, class Deleter>
struct unique_ptr<T[], Deleter> : unique_ptr<T, Deleter> {}; //继承 因此可转换

template <class T, class... Args>
unique_ptr<T> make_unique(Args... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...)); //完美转发
}



#endif //MODERN_CPP_TRICKS_MY_UNIQUE_PTR_H

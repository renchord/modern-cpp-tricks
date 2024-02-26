//
// Created by chordr on 2/23/2024.
//

#ifndef MODERN_CPP_TRICKS_MY_FUNCTION_H
#define MODERN_CPP_TRICKS_MY_FUNCTION_H


#include <iostream>
#include <utility>
#include <memory>
#include <type_traits>
#include <functional>

template <class FnSig>
struct Function {
    static_assert(!std::is_same_v<FnSig, FnSig>, "not a valid function signature");
};

template <class Ret, class ...Args>
struct Function<Ret(Args...)> {
private:
    struct FunctionBase {
        virtual Ret call(Args... args) = 0;
        virtual ~FunctionBase() = default;
    };

    template <class F>
    struct FuncImpl : FunctionBase {
        F m_f;
        FuncImpl(F f) : m_f(std::move(f)) {}
        virtual Ret call(Args... args) override {
            return m_f(std::forward<Args>(args)...);
        }
    };

    std::shared_ptr<FunctionBase> m_base;
public:
    Function() = default;
    template <class F, class = std::enable_if_t<std::is_invocable_r_v<Ret, F&, Args...>>>
            Function(F f) : m_base(std::make_shared<FuncImpl<F>>(std::move(f))) {

            }
    Ret operator()(Args ...args) const {
        if (!m_base) throw std::runtime_error("Error");
        return m_base->call(std::forward<Args>(args)...);
    }
};


#endif //MODERN_CPP_TRICKS_MY_FUNCTION_H

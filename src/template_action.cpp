#include <bits/stdc++.h>
namespace details {
    template <bool B, typename = void>
    struct enable_if {

    };
    template <typename T>
    struct enable_if<true, T> {
        using type = T;
    };
}
// CRTP
// 派生类本身就是基类模板的模板参数
// 基类模板的接口是静态的
// 基类的成员函数调用其（派生类）的成员函数

// Basic usage
template <typename T>
struct game_unit {
    void attack() {
        static_cast<T*>(this)->do_attack();
    }
};
struct knight : public game_unit<knight> {
    void do_attack() {
        // do sth
    }
};

// 限制类型可以实例化的次数
// 增加公共多功能并避免代码重复
// 实现符合设计模式

// Limit the instance tims
template <typename T, size_t N>
struct limited_instances {
    static std::atomic<size_t> count;
    limited_instances() {
        if (count >= N)
            throw std::logic_error {"Too many instances"};
        ++count;
    }
    ~limited_instances() {--count;}
};
template <typename T, size_t N>
std::atomic<size_t> limited_instances<T, N>::count = 0;

struct excalibur : limited_instances<excalibur, 1> {};
struct book : limited_instances<book, 4> {};

// Add functions to type
template <typename T>
struct movable_unit {
    void advace(size_t steps) {
        while(steps--) {
            static_cast<T*>(this)->step_forth();
        }
    }
    void retreat(size_t steps) {
        while(steps--) {
            static_cast<T*>(this)->step_back();
        }
    }
};

struct kk : movable_unit<kk> {
    void step_forth() {}
    void step_back() {}
};
void ttt() {
    kk a;
    a.advace(3);
    a.retreat(2);
    a.step_back();
}


// Mixins
struct nkngiht {
    void step_forth() {

    }
    void step_back() {}
};

namespace type_erase {
    struct knight {
        void attack() {
            std::cout << "Draw sword\n";
        }
    };
    struct mage {
        void attack() {
            std::cout << "Spell magic curse\n";
        }
    };
    struct game_unit {
        virtual void attack() = 0;
        virtual ~game_unit() = default;
    };
    struct knight_unit : game_unit {
        knight_unit(knight& u) : k(u) {}
        void attack() override {k.attack();}
    private:
        knight& k;
    };

    struct mage_unit : game_unit {
        mage_unit(mage& m) : m(m) {}
        void attack() override {m.attack();}
    private:
        mage& m;
    };
    void fight(std::vector<game_unit*> const & units) {
        for (auto u : units) {
            u->attack();
        }
    }

    void test() {
        knight k;
        mage m;
        knight_unit ku = k;
        mage_unit mu = m;
        std::vector<game_unit*> v {&ku, &mu};
        fight(v);
    }

    template <typename T>
    struct game_unit_wrapper : public game_unit {
        game_unit_wrapper(T& unit) : t(unit) {}
        void attack() override {t.attack();};
    private:
        T& t;
    };

    void test2() {
        knight k;
        mage m;
        game_unit_wrapper ku {k};
        game_unit_wrapper mu {m};
        std::vector<game_unit*> v {&ku, &mu};
        fight(v);
    }
}
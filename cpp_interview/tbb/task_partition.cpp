#include <iostream>
#include <string>
#include <tbb/task_group.h>
#include <thread>
#include <tbb/parallel_invoke.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/blocked_range2d.h>
#include <tbb/parallel_reduce.h>
#include <tbb/task_arena.h>
#include <vector>
#include <cmath>

#include <tbb/tick_count.h>
#include <mutex>
#define TICK(x) auto bench_##x = tbb::tick_count::now();
#define TOCK(x) std::cout << #x ": " << (tbb::tick_count::now() - bench_##x).seconds() << "s" << std::endl;

void test_static() {
    size_t n = 32;
    TICK(for);
    tbb::task_arena ta(4);
    ta.execute([&]{
        tbb::parallel_for(tbb::blocked_range<size_t>(0, n), [&](tbb::blocked_range<size_t> r){
            for (auto i = r.begin(); i != r.end(); ++i) {
                std::cout << "Thread: " <<  tbb::this_task_arena::current_thread_index() << "\t" << ", size : " << r.size() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
            }
        },tbb::static_partitioner());
    });
    TOCK(for);
}

void test_simple() {
        size_t n = 32;
    TICK(for);
    tbb::task_arena ta(4);
    ta.execute([&]{
        tbb::parallel_for(tbb::blocked_range<size_t>(0, n), [&](tbb::blocked_range<size_t> r){
            for (auto i = r.begin(); i != r.end(); ++i) {
                std::cout << "Thread: " <<  tbb::this_task_arena::current_thread_index() << "\t" << ", size : " << r.size() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
            }
        },tbb::simple_partitioner());
    });
    TOCK(for);
}

#include <tbb/concurrent_vector.h>
void test_vector() {
    size_t n = 1 << 10;
    tbb::concurrent_vector<float> a;
    std::vector<float*> pa(n);
    for (size_t i = 0; i < n; ++i) {
        auto it = a.push_back(std::sin(i));
        pa[i] = &*it;
    }
    // for (size_t i = 0; i < n; ++i) {
        // std::cout << (&a[i] == pa[i]) << ' ';
    // }
    // std::cout << std::endl;

    for (size_t i = 0; i < n; ++i) {
        auto it = a.grow_by(2);
        *it++ = std::sin(i);
        *it++ = std::cos(i);
    }
}

void test_parallel_with_vector() {
    size_t n = 1 << 10;
    tbb::concurrent_vector<float> a;
    tbb::parallel_for(tbb::blocked_range(a.begin(), a.end()), [&](tbb::blocked_range<decltype(a.begin())> r) {
        for (auto it = r.begin(); it != r.end(); ++it) {
            *it += 1.0f;
        }
    });
}


void serial_select(size_t n) {
    std::vector<float> a;
    TICK(serial);
    for (size_t i = 0; i < n; ++i) {
        auto val = std::sin(i);
        if (val > 0) a.push_back(val);
    }
    TOCK(serial);
}

void multi_just_vec(size_t n) {
    tbb::concurrent_vector<float> a;
    TICK(vec);
    tbb::parallel_for(tbb::blocked_range<size_t>(0, n), [&](auto r) {
        for (auto i = r.begin(); i != r.end(); ++i) {
            auto val = std::sin(i);
            if (val > 0) a.push_back(val);
        }
    });
    TOCK(vec);
}

void multi_refine(size_t n) {
    tbb::concurrent_vector<float> a;
    TICK(refine);
    tbb::parallel_for(tbb::blocked_range<size_t>(0, n), [&](auto r) {
        std::vector<float> temp;
        temp.reserve(r.size());
        for (auto i = r.begin(); i != r.end(); ++i) {
            auto val = std::sin(i);
            if (val > 0) temp.push_back(val);
        }
        auto it = a.grow_by(temp.size());
        // for (size_t i = 0; i < temp.size(); ++i) {
        //     *it++ = temp[i];
        // } also work by above
        std::copy(temp.begin(), temp.end(), it);
    });
    TOCK(refine);
}

void multi_refine_lock(size_t n) {
    std::vector<float> a;
    std::mutex mtx;
    TICK(lock);
    // a.reserve(n * 2 / 3);
    tbb::parallel_for(tbb::blocked_range<size_t>(0, n), [&](auto r) {
        std::vector<float> temp;
        temp.reserve(r.size());
        for (auto i = r.begin(); i != r.end(); ++i) {
            auto val = std::sin(i);
            if (val > 0) temp.push_back(val);
        }
        std::lock_guard lck(mtx);
        std::copy(temp.begin(), temp.end(), std::back_inserter(a));
    });
    TOCK(lock);
}

void multi_refine_lock_2(size_t n) {
    std::vector<float> a;
    std::mutex mtx;
    TICK(lock);
    a.reserve(n * 2 / 3);
    tbb::parallel_for(tbb::blocked_range<size_t>(0, n), [&](auto r) {
        std::vector<float> temp;
        temp.reserve(r.size());
        for (auto i = r.begin(); i != r.end(); ++i) {
            auto val = std::sin(i);
            if (val > 0) temp.push_back(val);
        }
        std::lock_guard lck(mtx);
        std::copy(temp.begin(), temp.end(), std::back_inserter(a));
    });
    TOCK(lock);
}

void test_select() {
    size_t n = 1 << 20;
    serial_select(n);
    multi_just_vec(n);
    multi_refine(n);
    multi_refine_lock(n);
    multi_refine_lock_2(n);
}

int main() {
    // test_static();
    // test_simple();
    // test_vector();
    // test_parallel_with_vector();
    
    test_select();

    return 0;
}
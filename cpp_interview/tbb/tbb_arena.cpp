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

void test_basic_arena() {
    tbb::task_arena ta(8);
    size_t n = 1 << 20;
    std::vector<float> a(n);
    TICK(test)
    ta.execute([&]() {
        tbb::parallel_for((size_t)0, (size_t)n, [&](size_t i) {
            a[i] = std::sin(i);
        });
    });
    TOCK(test)
}

void test_foor_loop() {
    size_t n = 1 << 10;
    std::vector<float> a(n * n);
    std::recursive_mutex mtx;
    TICK(test)
    tbb::parallel_for((size_t)0, (size_t)n, [&](size_t i) {
        std::lock_guard<std::recursive_mutex> lck(mtx);
        tbb::parallel_for((size_t)0, (size_t)n, [&](size_t j) {
            a[n * i + j] = std::sin(i) * std::sin(j);
        });
    });
    TOCK(test)
}

void test_foor_loop_arena() {
    size_t n = 1 << 10;
    std::vector<float> a(n * n);
    TICK(test)
    tbb::parallel_for((size_t)0, (size_t)n, [&](size_t i) {
        tbb::task_arena ta;
        ta.execute([&]{
            tbb::parallel_for((size_t)0, (size_t)n, [&](size_t j) {
                a[n * i + j] = std::sin(i) * std::sin(j);
            });
        });
    });
    TOCK(test)
}
void test_foor_loop_arena_iso() {
    size_t n = 1 << 10;
    std::vector<float> a(n * n);
    TICK(test)
    tbb::parallel_for((size_t)0, (size_t)n, [&](size_t i) {
        tbb::this_task_arena::isolate([&]{
            tbb::parallel_for((size_t)0, (size_t)n, [&](size_t j) {
                a[n * i + j] = std::sin(i) * std::sin(j);
            });
        });
    });
    TOCK(test)
}


int main() {
    // test_basic_arena();
    test_foor_loop();
    test_foor_loop_arena();
    test_foor_loop_arena_iso();
    return 0;
}
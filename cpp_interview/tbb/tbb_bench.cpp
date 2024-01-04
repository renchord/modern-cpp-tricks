#include <iostream>
#include <string>
#include <tbb/task_group.h>
#include <thread>
#include <tbb/parallel_invoke.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/blocked_range2d.h>
#include <tbb/parallel_reduce.h>
#include <vector>
#include <cmath>

#include <tbb/tick_count.h>
#define TICK(x) auto bench_##x = tbb::tick_count::now();
#define TOCK(x) std::cout << #x ": " << (tbb::tick_count::now() - bench_##x).seconds() << "s" << std::endl;


void test_serial(size_t n) {
    std::vector<float> a(n);
    TICK(loop);
    for (size_t i = 0; i < n; ++i) {
        a[i] = std::sin(i);
    }
    TOCK(loop);
    // std::cout << a[n] << std::endl;
    float res = 0;
    TICK(reduce);
    for (size_t i = 0; i < n; ++i) {
        res += a[i];
    }
    TOCK(reduce);
    std::cout << res << std::endl;
}

void test_parallel_for(size_t n) {
    std::vector<float> a(n);
    TICK(loop);
    tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
    [&](tbb::blocked_range<size_t>r) {
        for (auto i = r.begin(); i != r.end(); ++i) {
            a[i] = std::sin(i);
        }
    });
    TOCK(loop);
    //  std::cout << a[n] << std::endl;
    TICK(reduce)
    auto res = tbb::parallel_reduce(tbb::blocked_range<size_t>(0, n), (float)0, [&](tbb::blocked_range<size_t> r, float local_res) {
        for (int i = r.begin(); i != r.end(); ++i) {
            local_res += a[i];
        }
        return local_res;
    }, [&](auto x, auto y) {
        return x + y;
    });
    TOCK(reduce);
    std::cout << res << std::endl;
}

int main () {
    size_t n = 1 << 27;
    test_serial(n);
    test_parallel_for(n);
    return 0;
}
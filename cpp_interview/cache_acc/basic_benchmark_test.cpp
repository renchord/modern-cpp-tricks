#include <vector>
#include <iostream>
#include <benchmark/benchmark.h>

constexpr size_t n = 1 << 28;
std::vector<float> a(n); // 1GB


static float func(float x) {
    return x * (x * x + x * 3.14f - 1 / (x + 1)) + 42 / (2.718f - x);
}
void BM_serial_func(benchmark::State& bm) {
    for (auto _ : bm) {
        for (size_t i = 0; i < n; ++i) {
            a[i] = func(a[i]);
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_serial_func);

void BM_parallel_func(benchmark::State& bm) {
    for (auto _: bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            a[i] = func(a[i]);
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_parallel_func);

// struct CacheEntry { //chache line
//     bool valid, dirty;
//     uint64_t address;
//     char data[64];
// };

// CacheEntry cache[512];

void BM_skip1(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n; i += 1) {
            a[i] = 1;
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_skip1);

void BM_skip2(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n; i += 2) {
            a[i] = 1;
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_skip2);

void BM_skip4(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n; i += 4) {
            a[i] = 1;
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_skip4);

void BM_skip32(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n; i += 32) {
            a[i] = 1;
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_skip32);

void BM_skip64(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n; i += 64) {
            a[i] = 1;
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_skip64);

void BM_skip128(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n; i += 128) {
            a[i] = 1;
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_skip128);

BENCHMARK_MAIN();


//AOSOA
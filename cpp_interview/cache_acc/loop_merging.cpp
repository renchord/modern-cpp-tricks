#include <vector>
#include <iostream>
#include <benchmark/benchmark.h>
#include <mm_malloc.h>
#include <cstring>
#include <tbb/tick_count.h>
#include <cmath>
#define TICK(x) auto bench_##x = tbb::tick_count::now();
#define TOCK(x) std::cout << #x ": " << (tbb::tick_count::now() - bench_##x).seconds() << "s" << std::endl;
// If two loop could be merged, should be merged
// cache friendly

//jacobi interation

constexpr size_t n = 1 << 26;
constexpr size_t steps = 32;
std::vector<float> a(n);
std::vector<float> b(n);

void basic_jacobi() {
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        a[i] = std::sin(i * 0.1f);
    }
    TICK(iter);
    for (int step = 0; step < steps; ++step) {
#pragma omp parallel for
        for (size_t i = 1; i < n - 1; ++i) {
            b[i] = (a[i - 1] + a[i + 1]) * 0.5f;
        }
        std::swap(a, b);
    }
    TOCK(iter);
    float loss = 0;
#pragma omp parallel for reduction(+:loss)
    for (size_t i = 1; i < n - 1; ++i) {
        loss += std::pow(a[i - 1] + a[i + 1] - a[i] * 2, 2);
    }
    loss = std::sqrt(loss);
    std::cout << "loss: " << loss << std::endl;
}

void second_iteration_jacobi() {
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        a[i] = std::sin(i * 0.1f);
    }
    TICK(iter);
    for (int step = 0; step < steps / 2; ++step) {
#pragma omp parallel for
        for (size_t i = 2; i < n - 2; ++i) {
            b[i] = (a[i - 2] + a[i + 2]) * 0.25f + a[i] * 0.5f;
        }
        std::swap(a, b);
    }
    TOCK(iter);
    float loss = 0.f;
#pragma omp parallel for reduction(+:loss)
    for (size_t i = 0; i < n - 1; ++i) {
        loss += std::pow(a[i - 1] + a[i + 1] - a[i] * 2, 2);
    }
    loss = std::sqrt(loss);
    std::cout << "loss: " << loss << std::endl;
}

void refine_jacobi() {
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        a[i] = std::sin(i * 0.1f);
    }
    TICK(iter);
    for (int step = 0; step < 2; ++step) {  
#pragma omp parallel for
        for (size_t ibase = 16; ibase < n - 16; ibase += 32) {
            float ta[32 + 16 * 2], tb[32 + 16 * 2];
            for (intptr_t i = -16; i < 32 + 16; ++i) {
                ta[16 + i] = a[ibase + i];
            }
            for (intptr_t s = 1; s < 16; s += 2) {
                for (intptr_t i = -16 + s; i < 32 + 16 - s; ++i) {
                    tb[16 + i] = (ta[16 + i - 1] + ta[16 + i + 1]) * 0.5f;
                }
                for (intptr_t i = -16 + s + 1; i < 32 + 16 - s - 1; ++i) {
                    ta[16 + i] = (tb[16 + i - 1] + tb[16 + i + 1]) * 0.5f;
                }
            }
            for (intptr_t i = 0; i < 32; ++i) {
                b[ibase + i] = tb[16 + i];
            }
        }
        std::swap(a, b);
    }
    TOCK(iter);
    float loss = 0.f;
#pragma omp parallel for reduction(+:loss)
    for (size_t i = 0; i < n - 1; ++i) {
        loss += std::pow(a[i - 1] + a[i + 1] - a[i] * 2, 2);
    }
    loss = std::sqrt(loss);
    std::cout << "loss: " << loss << std::endl;
}

int main() {
    basic_jacobi();
    second_iteration_jacobi();
    refine_jacobi();
    return 0;
}
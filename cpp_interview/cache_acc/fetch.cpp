#include <vector>
#include <iostream>
#include <benchmark/benchmark.h>
#include <mm_malloc.h>
#include <cstring>

constexpr size_t n = 1 << 27; // 512MB

std::vector<float> a(n);

void BM_ordered(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            benchmark::DoNotOptimize(a[i]);
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_ordered);
// A hash function to randomize i
static uint32_t randomize(uint32_t i) {
    i = (i ^ 61) ^ (i >> 16);
    i *= 9;
    i ^= i << 4;
    i *= 0x27d4eb2d;
    i ^= i >> 15;
    return i;
} 

void BM_random_4B(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            size_t r = randomize(i) % n;
            benchmark::DoNotOptimize(a[r]);
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_random_4B);

void BM_random_64B(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n / 16; ++i) {
            size_t r = randomize(i) % (n / 16);
            for (size_t j = 0; j < 16; ++j) {
                 benchmark::DoNotOptimize(a[r * 16 + j]);
            }
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_random_64B);

#include <xmmintrin.h>
void BM_random_64B_prefetch(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n / 16; ++i) {
            size_t next_r = randomize(i + 16) % (n / 16);
            _mm_prefetch(reinterpret_cast<const char*>(&a[next_r * 16]), _MM_HINT_T1);
            size_t r = randomize(i) % (n / 16);
            for (size_t j = 0; j < 16; ++j) {
                 benchmark::DoNotOptimize(a[r * 16 + j]);
            }
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_random_64B_prefetch);

// One time random with 64 times sequential
void BM_random_4KB(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n / 1024; ++i) {
            size_t r = randomize(i) % (n / 1024);
            for (size_t j = 0; j < 1024; ++j) {
                 benchmark::DoNotOptimize(a[r * 1024 + j]);
            }
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_random_4KB);

void BM_random_4KB_aligned(benchmark::State& bm) {
    float* a = (float*)_mm_malloc(n* sizeof(float), 4096);
    memset(a, 0, n * sizeof(float));
    for (auto _ : bm) {
#pragma omp parallel for
        for (size_t i = 0; i < n / 1024; ++i) {
            size_t r = randomize(i) % (n / 1024);
            for (size_t j = 0; j < 1024; ++j) {
                 benchmark::DoNotOptimize(a[r * 1024 + j]);
            }
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_random_4KB_aligned);

void BM_read(benchmark::State& bm) {
    for (auto _ : bm) {
        float res = 0;
#pragma omp parallel for 
        for (size_t i = 0; i < n; ++i) {
            res += a[i];
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_read);

void BM_write(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for 
        for (size_t i = 0; i < n; ++i) {
            a[i] = 1;
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_write);



void BM_read_and_write(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for 
        for (size_t i = 0; i < n; ++i) {
            a[i] = a[i] + 1;
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_read_and_write);

#include <emmintrin.h>
// _mm_stream_si32 can omit cache by using a queue
void BM_write_streamd(benchmark::State& bm) {
    for (auto _ : bm) {
#pragma omp parallel for 
        for (size_t i = 0; i < n; ++i) {
            float value = 1;
            _mm_stream_si32((int*)&a[i], *(int*)&value);
            // If read again, would loss effienicy a lot
            // If a stride, merging would be failed
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(BM_write_streamd);

BENCHMARK_MAIN();
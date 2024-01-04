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


int serial_fib(int n) {
    if (n < 2) return n;
    auto fib1 = serial_fib(n - 1);
    auto fib2 = serial_fib(n - 2);
    return fib1 + fib2;
}

int parall_fib(int n) {
    if (n < 29) return serial_fib(n);
    int fib1 = 0, fib2 = 0;
    tbb::parallel_invoke([&]{
        fib1 = parall_fib(n - 1);
    }, [&]{
        fib2 = parall_fib(n - 2);
    });
    return fib1 + fib2;
}

void t1() {
    TICK(serial);
    auto res = serial_fib(39);
    TOCK(serial);
}

void t2() {
    TICK(parallel);
    auto res = parall_fib(39);
    TOCK(parallel);
}

#include <algorithm>

void standard_sort() {
    size_t n = 1 << 22;
    std::vector<int> arr(n);
    std::generate(arr.begin(), arr.end(), std::rand);
    TICK(sort);
    std::sort(arr.begin(), arr.end());
    TOCK(sort);
}


template <class T>
void quick_sort(T* data, size_t size) {
    if (size < 1) return;
    size_t mid = std::hash<size_t> {}(size);
    mid ^= std::hash<void*>{}(static_cast<void*>(data));
    mid %= size;
    // mid = 0;
    std::swap(data[0], data[mid]);
    T pivot = data[0];
    size_t left = 0, right = size - 1;
    while (left < right) {
        while (left < right && !(data[right] < pivot))
            --right;
        if (left < right) 
            data[left++] = data[right];
        while (left < right && data[left] < pivot)
            ++left;
        if (left < right)
            data[right--] = data[left];
    }
    data[left] = pivot;
    quick_sort(data, left);
    quick_sort(data + left + 1, size - left - 1);
}

template <class T>
void quick_sort_2(T* data, size_t size) {
    if (size < 1) return;
    size_t mid = std::hash<size_t> {}(size);
    mid ^= std::hash<void*>{}(static_cast<void*>(data));
    mid %= size;

    std::swap(data[0], data[mid]);
    T pivot = data[0];
    size_t left = 0, right = size - 1;
    while (left < right) {
        while (left < right && !(data[right] < pivot))
            --right;
        if (left < right) 
            data[left++] = data[right];
        while (left < right && data[left] < pivot)
            ++left;
        if (left < right)
            data[right--] = data[left];
    }
    data[left] = pivot;
    tbb::parallel_invoke([&]{
        quick_sort_2(data, left);
    }, [&]{
        quick_sort_2(data + left + 1, size - left - 1);
    });
}

template <class T>
void quick_sort_3(T* data, size_t size) {
    if (size < 1) return;
    if (size < (1 << 15)) {
        std::sort(data, data + size);
        return;
    }
    size_t mid = std::hash<size_t> {}(size);
    mid ^= std::hash<void*>{}(static_cast<void*>(data));
    mid %= size;

    std::swap(data[0], data[mid]);
    T pivot = data[0];
    size_t left = 0, right = size - 1;
    while (left < right) {
        while (left < right && !(data[right] < pivot))
            --right;
        if (left < right) 
            data[left++] = data[right];
        while (left < right && data[left] < pivot)
            ++left;
        if (left < right)
            data[right--] = data[left];
    }
    data[left] = pivot;
    tbb::parallel_invoke([&]{
        quick_sort_3(data, left);
    }, [&]{
        quick_sort_3(data + left + 1, size - left - 1);
    });
}

void quick_sort() {
    size_t n = 1 << 22;
    std::vector<int> arr(n);
    std::generate(arr.begin(), arr.end(), std::rand);
    TICK(quick_sort);
    quick_sort(arr.data(), arr.size());
    TOCK(quick_sort);
}
void quick_sort_2() {
    size_t n = 1 << 22;
    std::vector<int> arr(n);
    std::generate(arr.begin(), arr.end(), std::rand);
    TICK(quick_sort_2);
    quick_sort_2(arr.data(), arr.size());
    TOCK(quick_sort_2);
}

void quick_sort_3() {
    size_t n = 1 << 22;
    std::vector<int> arr(n);
    std::generate(arr.begin(), arr.end(), std::rand);
    TICK(quick_sort_2);
    quick_sort_3(arr.data(), arr.size());
    TOCK(quick_sort_2);
}

#include <tbb/parallel_sort.h>

void tbb_sort() {
    size_t n = 1 << 22;
    std::vector<int> arr(n);
    std::generate(arr.begin(), arr.end(), std::rand);
    TICK(tbb_sort);
    tbb::parallel_sort(arr.begin(), arr.end(), std::less<int>{});
    TOCK(tbb_sort);
}

int main() {
    // t1();
    // t2();
    standard_sort();
    quick_sort();
    quick_sort_2();
    quick_sort_3();
    tbb_sort();
    return 0;
}
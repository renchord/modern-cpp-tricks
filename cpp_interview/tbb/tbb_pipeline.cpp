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

struct Data {
    std::vector<float> arr;
    Data() {
        arr.resize(10000);
        for (int i = 0; i < arr.size(); ++i) {
            arr[i]  = std::rand() * (1.f / (float)RAND_MAX);
        }
    }

    void step1() {
        for (auto& a : arr) {
            a += 3.14f;
        }
    }
    void step2() {
        std::vector<float> temp(arr.size());
        for (int i = 1; i < arr.size() - 1; ++i) {
            temp[i] = arr[i - 1] + arr[i] + arr[i + 1];
        }
        std::swap(temp, arr);
    }
    void step3() {
        for (int i = 0; i < arr.size(); ++i) {
            arr[i] = std::sqrt(std::abs(arr[i]));
        }
    }
    void step4() {
        std::vector<float> temp(arr.size());
        for (int i = 0; i < arr.size() - 1; ++i) {
            temp[i] = arr[i - 1] - 2 * arr[i] + arr[i + 1];
        }
        std::swap(temp, arr);
    }
};

void t1(size_t n) {
    std::vector<Data> dats(n);
    TICK(process);
    for (auto & dat : dats) {
        dat.step1();
        dat.step2();
        dat.step3();
        dat.step4();
    }
    TOCK(process);
}

void t2(size_t n) {
    std::vector<Data> dats(n);
    TICK(process);
    tbb::parallel_for_each(dats.begin(), dats.end(), [&](auto& dat){
        dat.step1();
        dat.step2();
        dat.step3();
        dat.step4();
    });
    TOCK(process);
}

void t3(size_t n) {
    std::vector<Data> dats(n);
    TICK(process);
    tbb::parallel_for_each(dats.begin(), dats.end(), [&](auto& dat){
        dat.step1();
    });
    tbb::parallel_for_each(dats.begin(), dats.end(), [&](auto& dat){
        dat.step2();
    });
    tbb::parallel_for_each(dats.begin(), dats.end(), [&](auto& dat){
        dat.step3();
    });
    tbb::parallel_for_each(dats.begin(), dats.end(), [&](auto& dat){
        dat.step4();
    });
    TOCK(process);
}

#include <tbb/pipeline.h>

void pipeline(size_t n) {

    std::vector<Data> dats(n);
    TICK(process);
    auto it = dats.begin();
    tbb::parallel_pipeline(128, 
    tbb::make_filter<void, Data*> (tbb::filter::mode::serial_in_order, [&](tbb::flow_control& fc)->Data* {
        if (it == dats.end()) {
            fc.stop();
            return nullptr;
        }
        return &*it++;
    }) & 
    tbb::make_filter<Data*, Data*>(tbb::filter::mode::parallel, [&](Data* dat)->Data* {
        dat->step1();
        return dat;
    }) &
        tbb::make_filter<Data*, Data*>(tbb::filter::mode::parallel, [&](Data* dat)->Data* {
        dat->step2();
        return dat;
    }) &
        tbb::make_filter<Data*, Data*>(tbb::filter::mode::parallel, [&](Data* dat)->Data* {
        dat->step3();
        return dat;
    }) &
        tbb::make_filter<Data*, void>(tbb::filter::mode::parallel, [&](Data* dat)->void {
        dat->step4();
    })
    );

    TOCK(process);
}

int main() {
    size_t n = 1 << 12;
    t1(n);
    t2(n);
    t3(n);
    pipeline(n);
    return 0;
}
#include <iostream>
#include <string>
#include <tbb/task_group.h>
#include <thread>
#include <tbb/parallel_invoke.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/blocked_range2d.h>
#include <vector>
#include <cmath>


void download(const std::string& file_name) {
    for (int i = 0; i < 10; ++i) {
        std::cout << "Downloading " << file_name << " (" << i * 10 
        << "%)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    std::cout << "Download complete" << std::endl;
}
void interact() {
    std::string name;
    std::cin >> name;
    std::cout << "Hi, " << name << std::endl;
}

void test_task_group() {
    tbb::task_group tg;
    tg.run([&]() {
        download("tg.exe");
    });
    tg.run([&]{
        interact();
    });
    tg.wait();
}

void test_parallel_invoke() {
    tbb::parallel_invoke([&]{
        download("pi.exe");
    }, [&]{
        interact();
    });
}

void test_parallel_for() {
    const size_t n = 1 << 26;
    std::vector<float> a(n);
    tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
    [&](tbb::blocked_range<size_t> r) {
        for (size_t i = r.begin(); i != r.end(); ++i) {
            a[i] = std::sin(i);
        }
    });

    //easier version but lower efficiency 
    tbb::parallel_for((size_t)0, (size_t)n, [&](size_t i) {
        a[i] = std::sin(i);
    });

    // parallel for each
    tbb::parallel_for_each(a.begin(), a.end(), [&](auto& f) {
        f = 32.f;
    });

    // parallel for 2d
    size_t n2 = 1 << 13;
    std::vector<float> b(n2 * n2);
    tbb::parallel_for(tbb::blocked_range2d<size_t>((size_t)0, n2, 0, n2),
    [&](auto r) {
        for (auto i = r.cols().begin(); i != r.cols().end(); ++i) {
            for (auto j = r.rows().begin(); j != r.rows().end(); ++j) {
                b[i * n2 + j] = std::sin(i * n + j);
            }
        }
    });

    // parallel for 3d
    // omited
}

#include <tbb/parallel_reduce.h>
void test_parallel_reduce() {
    size_t n = 1 << 26;

    // range ... normal
    // identity val -> init value
    // real body, the minium unit cal impl
    // reduction body, the reduction procedure impl

    // auto res = tbb::parallel_reduce
    auto res = tbb::parallel_deterministic_reduce
    (tbb::blocked_range<size_t>(0, n), 
    (float)(0), 
    [&](tbb::blocked_range<size_t> r, float local) { // local must not be reference
        for (size_t i = r.begin(); i != r.end(); ++i) {
            local += std::sin(i);
        }
        return local;
    }, [](auto& x, auto& y) {
        return x + y;
    });
    std::cout << res << std::endl;
}

#include <tbb/parallel_scan.h>
void test_parallel_scan() {
    size_t n = 1 << 26;
    std::vector<float> ans(n);
    float res = tbb::parallel_scan(tbb::blocked_range<size_t>(0, n), (float)0, 
    [&](tbb::blocked_range<size_t> r, float local_res, auto is_final) {
        for (size_t i = r.begin(); i != r.end(); ++i) {
            local_res += std::sin(i);
            if (is_final) {
                ans[i] = local_res;
            }
        }
        return local_res;
    }, [&](auto x, auto y) {
        return x + y;
    });
    std::cout << ans[n / 2] << std::endl;
    std::cout << ans.back() << std::endl;
}


int main () {
    std::cout << "basic test" << std::endl;
    // test_task_group();
    // test_parallel_invoke();
    // test_parallel_for();
    // test_parallel_reduce();
    test_parallel_scan();
    return 0;
}
#include <ctime>
#include <random>
#include <algorithm>
#include <iostream>
#include <vector>

#define cal_KB(x) ((size_t)(x ) << 10)

int main () {
    std::vector<size_t> lens;
    for (int i = 1; i < 18; ++i) {
        lens.emplace_back(1 << i);
    }

    std::random_device rd_root;
    std::mt19937 gen(rd_root());

    for (const auto& size : lens) {
        auto cur_len = cal_KB(size);
        std::uniform_int_distribution<> dis(0, cur_len - 1);
        std::vector<char> memory(cur_len);
        std::fill(memory.begin(), memory.end(), 1);

        int res = 0;
        clock_t start = clock();
        for (int i = 0; i < (1 << 25); ++i) {
            res += memory[dis(gen)];
        }
        clock_t finish = clock();
        double time = double(finish - start) / CLOCKS_PER_SEC;
        std::cout << size << " KB, " << time << " secs, res : " << res << std::endl;
    }
    return 0;
}
//
// Created by chordr on 2/24/2024.
//
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
int number = 0;
bool isOdd = false;

void printOdd() {
    while (number < 9) {
        std::unique_lock lock(mtx);
        cv.wait(lock, [&]{
            return isOdd;
        });
        std::cout << "Odd: " << number << std::endl;
        ++number;
        isOdd = false;
        cv.notify_one();
    }
}

void printEven() {
    while (number < 9) {
        std::unique_lock lock(mtx);
        cv.wait(lock, [&] {
            return !isOdd;
        });
        std::cout << "Even: " << number << std::endl;
        ++number;
        isOdd = true;
        cv.notify_one();
    }
}

int main() {
    std::thread t1(printEven);
    std::thread t2(printOdd);

    t1.join();
    t2.join();
    return 0;
}

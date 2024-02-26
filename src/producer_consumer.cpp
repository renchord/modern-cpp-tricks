#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <semaphore.h>
sem_t sem;
sem_init(&sem, 0, 1);

std::queue<int> buffer;
constexpr int BUFFER_SIZE = 10;
std::mutex mtx;
std::condition_variable cv;

void producer() {
    for (int i = 1; i <= 20; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []() {
            return buffer.size() < BUFFER_SIZE;
        });
        buffer.push(i);
        std::cout << "Produced: " << i << std::endl;
        lock.unlock();
        cv.notify_all();
    }
}
void consumer() {
    for (int i = 1; i <= 20; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []()  {
            return buffer.size() > 0;
        });
        int data = buffer.front();
        buffer.pop();
        std::cout << "Consumer: " << data << std::endl;
        lock.unlock();
        cv.notify_all();
    }
}

int main() {
    std::thread a(producer);
    std::thread b(consumer);
    a.join();
    b.join();
    return 0;
}
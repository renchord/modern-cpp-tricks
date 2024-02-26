#include <iostream>


class CircularQueue {
private:
    int *arr;
    int front, rear, size, capacity;

public:
    CircularQueue(int cap) {
        capacity = cap;
        arr = new int[capacity];
        front = -1;
        rear = -1;
        size = 0;
    }

    // 入队 (Enqueue)
    void enqueue(int data) {
        if ((rear + 1) % capacity == front) {
            // 队列已满
            std::cout << "Full\n";
            return;
        }
        if (size == 0) {
            front = rear = 0;
        } else {
            rear = (rear + 1) % capacity;
        }
        arr[rear] = data;
        size++;
    }

    // 出队 (Dequeue)
    int dequeue() {
        if (size == 0) {
            // 队列为空
            std::cout << "Empty\n";
            return -1;
        }
        int temp = arr[front];
        front = (front + 1) % capacity;
        size--;
        return temp;
    }
    void display() {
        int i = front;
        while (1) {
            std::cout << arr[i] << " ";
            if (i == rear) break;
            i = (i + 1) % capacity;

        }
        std::cout << std::endl;
    }
};

int main () {
    CircularQueue q(5);
    auto x = q.dequeue();
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    q.dequeue();
    q.display();
    q.enqueue(4);
    q.enqueue(8);
    q.display();
    q.enqueue(9);
    q.display();
    q.enqueue(10);
    q.display();
    return 0;
}
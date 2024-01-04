#include <memory>
#include <iostream>

void usage_of_std_ref() {
    int num = 10;
    int& a = num;
    auto b = std::ref(a);
    ++b;
    std::cout << num << std::endl;
    auto c = std::ref(b);
    std::cout << c << std::endl;
    // auto d = std::ref(std::move(num)); // not ok, must be lvalue-ref
    auto d = c;
}

int main () {
    usage_of_std_ref();
    return 0;
}
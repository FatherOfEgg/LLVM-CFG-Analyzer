#include <iostream>

void a() {
    std::cout << "a" << std::endl;
}

void b() {
    a();
    std::cout << "b" << std::endl;
}

void c() {
    b();
    std::cout << "c" << std::endl;
}

int main() {
    c();
    return 0;
}

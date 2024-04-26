#include <iostream>

void b() {
    std::cout << "foo" << std::endl;
}

void c() {
    std::cout << "bar" << std::endl;
}

int main() {
    b();
    c();
    return 0;
}

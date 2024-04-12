#include <iostream>

void foo() {
    std::cout << "foo" << std::endl;
}

void bar() {
    std::cout << "bar" << std::endl;
}

int main() {
    foo();
    bar();
    return 0;
}

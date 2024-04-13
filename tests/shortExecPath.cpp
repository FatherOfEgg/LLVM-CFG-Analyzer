#include <iostream>

void foo() {
    std::cout << "foo" << std::endl;
}

void bar() {
    foo();
    std::cout << "bar" << std::endl;
}

int main() {
    bar();
    return 0;
}

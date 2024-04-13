#include <iostream>

void printOdd() {
    std::cout << "Odd" << std::endl;
}

int main() {
    int n;
    std::cout << "Enter number: ";
    std::cin >> n;

    if (n % 2 == 0) {
        std::cout << "Even" << std::endl;
    } else {
        printOdd();
    }
    
    return 0;
}

#include <iostream>

// assume int is 4 bytes
void doSomething(unsigned int x)
{
    std::cout << "x is " << x << '\n';
}

// g++ -std=c++17 unsignedArgs.cpp -o unsignedArgs && ./unsignedArgs
int main()
{
    doSomething(-1);  // prints "x is 4294967295"
    return 0;
}

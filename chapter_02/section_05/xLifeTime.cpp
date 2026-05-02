#include <iostream>

void doSomething()
{
    std::cout << "Hello!\n";
}

int main()
{
    int x{ 0 };  // x's lifetime begins here

    doSomething();  // x is still alive during this function call

    return 0;
} // x's lifetime ends here.

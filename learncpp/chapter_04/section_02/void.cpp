#include <iostream>

void writeValue(int x)
{
    std::cout << "The value of x is: " << x << '\n';
}

int getValue()
{
    int x{};
    std::cin >> x;
    return x;
}

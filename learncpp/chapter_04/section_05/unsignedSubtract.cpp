#include <iostream>

// g++ -std=c++17 unsignedSubtract.cpp -o unsignedSubtract && ./unsignedSubtract
int main()
{
    unsigned int x{ 2 };
    unsigned int y{ 3 };
    
    std::cout << x - y << '\n'; // prints 4294967295 (incorrect!)

    unsigned int u{ 2 };
    signed int s{ 3 };

    std::cout << u - s << '\n'; // prints 4294967295 (incorrect!)

    return 0;
}

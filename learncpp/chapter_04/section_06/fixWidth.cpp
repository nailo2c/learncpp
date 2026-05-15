#include <cstdint>  // for fixed-width integers
#include <iostream>

int main()
{
    std::int16_t x { 32767 }; 
    x = x + 1;
    std::cout << x << '\n'; // prints -32768

    return 0;
}

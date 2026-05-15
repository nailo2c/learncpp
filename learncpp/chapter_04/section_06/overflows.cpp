#include <iostream>

int main()
{
    int x { 32767 }; // x may be 16-bits or 32-bits
    x = x + 1;       // 32768 overflows if int is 16-bits
    std::cout << x << '\n';

    return 0;
}

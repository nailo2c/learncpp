#include <iostream>

int main()
{
    unsigned short x{ 0 };
    std::cout << "x was: " << x << '\n';

    x = -1;
    std::cout << "x is now: " << x << '\n';

    x = -2;
    std::cout << "x is now: " << x << '\n';

    return 0;
}

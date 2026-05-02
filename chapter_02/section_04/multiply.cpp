#include <iostream>

int add(int x, int y)
{
    return x + y;
}

int multiply(int z, int w)
{
    return z * w;
}

int main()
{
    std::cout << add(4, 5) << '\n';
    std::cout << add(1 + 2, 3 * 4) << '\n';

    int a{ 5 };
    std::cout << add(a, a) << '\n';
    
    std::cout << add(1, multiply(2, 3)) << '\n';
    std::cout << add(1, add(2, 3)) << '\n';

    return 0;
}

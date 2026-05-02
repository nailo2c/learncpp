#include <iostream>

int returnFive()
{
    return 5;
}

int main()
{
    std::cout << returnFive() << '\n';
    std::cout << returnFive() + 2 << '\n';

    returnFive();  // this will be ignored

    return 0;
}

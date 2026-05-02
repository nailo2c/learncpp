#include <iostream>

void returnNothing()
{
}

int returnFive()
{
    return 5;
}

int main()
{
    returnNothing();
    returnFive();

    std::cout << returnFive();
    std::cout << returnNothing();  // compile error, we can't call a function that returns void

    return;
}

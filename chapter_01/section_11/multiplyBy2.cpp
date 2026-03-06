#include <iostream>

int main()
{
    std::cout << "Enter an integer: ";

    int num{ };
    std::cin >> num; // get integer value from user's keyboard

    std::cout << "Double that number is: " << num * 2 << '\n';

    return 0;
}

#include <iostream>

int getValueFromUser()
{
    std::cout << "Enter a number: ";
    int input{};
    std::cin >> input;
    return input;
}

int doubleNumber(int x)
{
    return 2 * x;
}

int main()
{
    int input { getValueFromUser() };

    std::cout << doubleNumber(input) << '\n';
    return 0;
}

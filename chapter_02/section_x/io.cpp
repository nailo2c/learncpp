#include <iostream>

int readNumber()
{
    std::cout << "Enter a number: ";
    int input{};
    std::cin >> input;

    return input;
}

void writeAnswer(int answer)
{
    std::cout << "The answer is " <<  answer << '\n';
}

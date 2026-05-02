# Question #1

## What’s wrong with this program fragment?

```cpp
#include <iostream>

void multiply(int x, int y)
{
    return x * y;
}

int main()
{
    std::cout << multiply(4, 5) << '\n';

    return 0;
}
```

It should put `int` type for the function `multiply`.

# Question #2

## What two things are wrong with this program fragment?

```cpp
#include <iostream>

int multiply(int x, int y)
{
    int product { x * y };
}

int main()
{
    std::cout << multiply(4) << '\n';

    return 0;
}
```

1. Only pass one parameter to multiply function.
2. No return in multiply function.

# Question #3

24

# Question #4

```cpp
#include <iostream>

int doubleNumber(int x)
{
    return 2 * x;
}

int main()
{
    std::cout << doubleNumber(5);
    return 0;
}
```

# Question #5

```cpp
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
```

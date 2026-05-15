# Question #1

Consider the following program that we used above:

```cpp
#include <iostream>  // for std::cout and std::cin

int main()
{
    std::cout << "Enter a number: "; // ask user for a number
    int x{}; // define variable x to hold user input
    std::cin >> x; // get number from keyboard and store it in variable x
    std::cout << "You entered " << x << '\n';

    return 0;
}
```

The program expects you to enter an integer value, as the variable x that the user input will be put into is an integer variable.

Run this program multiple times and describe the output that results when you enter the following types of input:

a) A letter, such as `h`.

> Result: 0 is always printed.
> What's happening: An integer can't hold a letter, so extraction completely fails. `x` is assigned the value 0. (Because we value-initialize it, and extraction on line 7 failed, so x keeps 0.)

b) A number with a fractional part (e.g. 3.2). Try numbers with fractional parts less than 0.5 and greater than 0.5 (e.g. 3.2 and 3.7).

> 3 and 3
> Result: The fractional part is dropped (not round).
> What's  happening: Given the number 3.2 and 3 gets extracted, but `.` is an invalid character, so extraction stops here. The `.2` remains for a future extraction attempt.
> If you are wondering why this isn't a disallowed narrowing conversion, narrowing conversions are only disallowed during list-initialization (which happens on line 6). The extraction happens on line 7.

c) A small negative integer, such as -3.

> -3

d) A word, such as `Hello`.

> 0

e) A really big number (at least 3 billion).

> Result: `2147483647`.
> What's happening: `x` can only hold numbers up to a certain size. If you enter a value larger than the largest number `x` can hold, it will be set to the largest number that `x` can hold.

f) A small number followed by some letters, such as `123abc`.

> 123

g) A few letters followed by a small number, such as `abc123`.

> 0

h) `+5` (three spaces, followed by a plus symbol, and a 5).

> Result: 5.
> What's happening: The leading whitespace is skipped. Plus is a valid symbol at the start of a number (just as a minus sign would be), so it is extracted. The 5 is also extracted.

i) `5b6`

> 5

# Question #2


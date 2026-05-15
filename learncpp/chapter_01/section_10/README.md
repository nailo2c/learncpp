# Quesiton #1

## What is the difference between a statement and an expression?

Statements are used when we want the program to perform an action.
Expressions are used when we want the program to calculate a value.

# Question #2

a) `int x;`
- Statement does not contain expression.

b) `int x = 5;`
- Statement contains an expression.

c) `x = 5;`
- Expression statement.

d) `foo(); // foo is a function`
- Function calls are part of an experssion, so this is an expression statement.

e) `std::cout << x; // Hint: operator<< is a binary operator.`
- Expression statement.

# Question #3

Determine what values the following program outputs. Do not compile this program. Just work through it line by line in your head.

```cpp
#include <iostream>

int main()
{
	std::cout << 2 + 3 << '\n';

	int x{ 6 };
	int y{ x - 2 };
	std::cout << y << '\n';

	int z{};
	z = x;
	std::cout << z * x << '\n';

	return 0;
}
```

- 5, 4, 36

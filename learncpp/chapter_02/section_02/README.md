# Question #1

+ 1 a)  
16

+ 1 b)  
error, nested functions are not allowed  

+ 1 c)  
nothing

+ 1 d)  
```cpp
#include <iostream>

int getNumbers()
{
    return 5;
    return 7;
}

int main()
{
    std::cout << getNumbers() << '\n';
    std::cout << getNumbers() << '\n';

    return 0;
}
```
~~error~~. This function will print 5 twice.

+ 1 e)  
Error, the function name is invaild.

# Question #2


DRY means don't repeat yourself. Abstract the reusing parts as a function.

# Question #1

## What is a function prototype?

To write a forward declaration for a function is called a function prototype.

# Question #2

## What is a forward declaration?

Define the function body later or in other file is called forward declaration.

# Question #3

## How do we declare a forward declaration for functions?

For functions, a functiion declaration/prototype serves as a forward declaration.

# Question #4

Write the function declaration for this function (use the preferred form with names):
```cpp
int doMath(int first, int second, int third, int fourth)
{
     return first + second * third / fourth;
}
```

Ans:
```cpp
int doMath(int first, int second, int third, int fourth);
```

# Question #5

+ a)
    + compile failed on add function called, only two parameters be taked for add function.
+ b)
    + compile failed, because the arguments of function declaration is not align with the function definition.
+ c)
    + same as (b)
+ d)
    + Link success.
+ e)
    + Link success.
+ f)
    + compile success, the first add is not used.

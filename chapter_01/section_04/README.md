# Question #1

### What is the difference between initialization and assignment?

Initialization gives a variable an initial value at the point when it is created. Assignment gives a variable a value at some point after the variable is created.

# Question #2

### What form of initialization should you prefer when you want to initialize a variable with a specific value?

`Direct-list-initialization`

# Question #3

+ Default-initialization (e.g. `int x;`). In most cases, the variable is left with an indeterminate value.
+ Value-initialization (e.g. `int x{};`). It will assign 0 to x.

You should prefer value-initialization, as it initializes the variable to a consistent value.

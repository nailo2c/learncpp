#include <iostream>

void d()
{
    std::cout << "d() called\n";
}

void c()
{
    std::cout << "c() called\n";
}

void b()
{
	c();
	d();
}

void a()
{
	b();
}

int main()
{
	a();

	return 0;
}

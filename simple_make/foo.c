
#include <stdio.h>

void foo()
{
	printf("Foo from library!\n");
}

void bar()
{
	printf("Bar from library changed !\n");
}

void call_bar() 
{
	printf("Calling bar\n");
	bar();
}

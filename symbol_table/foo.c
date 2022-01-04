
#include <stdio.h>
#include "foo.h"

int somewhere_else = 0;

void in_main();

void foo2()
{
	printf("Foo2, assigning to somewhere_else\n");
	somewhere_else = 25;
}

void foo()
{
	printf("Foo! calling Foo2\n");
	foo2();
	in_main();
}


void foobar() 
{
	printf("Foobar from foo\n");
}

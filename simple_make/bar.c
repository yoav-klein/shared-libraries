#include <stdio.h>
#include "bar.h"

void foo();

void foo()
{
	printf("Foo from bar\n");	
}

void bar()
{
	printf("Bar from library changed !\n");
}

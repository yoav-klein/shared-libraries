
#include <stdio.h>
#include "foo.h"
#include "bar.h"

void foo()
{
	printf("Foo from library!\n");
}


void call_bar() 
{
	printf("Calling bar\n");
	bar();
}

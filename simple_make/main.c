
#include <stdio.h>

#include "foo.h"
#include "bar.h"
/*
void foo()
{
	printf("Main Foo\n");
}*/

void in_main()
{
	printf("Another function in main\n");
}

int main()
{
	foo();
	bar();
	
	foobar();
	
	return 0;
}

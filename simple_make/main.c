
#include <stdio.h>

#include "foo.h"
#include "bar.h"
/*
void foo()
{
	printf("Main Foo\n");
}*/

void here_in_main()
{
	printf("Here in main..\n");
}

int main()
{
	/*foo();*/
	bar();
	here_in_main();
	
	return 0;
}


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
	/*foo();*/
	bar();
	here_in_main();
	
	return 0;
}

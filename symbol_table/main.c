
#include <stdio.h>

#include "foo.h"

static void only_here()
{
	printf("Accessible only in this object\n");	
}

int glob_int = 10;
static int static_int = 20;
char *my_name = "Yoav";
extern int somewhere_else;

void in_main()
{
	printf("Another function in main\n");
}

int main()
{
	foo();
	
	printf("The value of somewhere_else is: %d\n", somewhere_else);
	in_main();
	
	only_here();
	return 0;
}


#include <stdio.h>
#include "foo.h"
#include "bar.h"

void foo()
{
	printf("Foo!\n");
}

static void foo_static()
{
	printf("Foo static\n");
}


void foobar() 
{
	printf("Foobar from foo\n");
}

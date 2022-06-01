
#include <stdio.h>
#include "foo.h"
#include "bar.h"

void foo()
{
	printf("Foo! Calling bar..\n");
	bar();
}


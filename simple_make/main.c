
#include <stdio.h>

void call_bar();

void foo();
void bar();

int main()
{
	foo();
	bar();
	
	call_bar();
	
	return 0;
}

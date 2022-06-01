
#include <iostream>
#include "foo.h"
#include "myprintf.h"

void foo()
{
    std::cout << "Foo calling myprintf";
    myprintf("From foo!");
}
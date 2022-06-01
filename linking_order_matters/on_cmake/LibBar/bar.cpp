
#include <iostream>
#include "bar.h"
#include "myprintf.h"

void bar()
{
    std::cout << "Bar calling myprintf" << std::endl;
    myprintf("From bar !");
}
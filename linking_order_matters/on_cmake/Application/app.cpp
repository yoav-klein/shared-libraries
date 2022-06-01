
#include <iostream>
#include "bar.h"
#include "foo.h"

int main()
{
    std::cout << "Main starting" << std::endl;

    foo();
    bar();

    return 0;
}
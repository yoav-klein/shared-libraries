
CC=g++
CFLAGS=-ansi -pedantic-errors -Wall -Wextra

libmyprintf.a: myprintf.o
	ar -rc $@ $^

myprintf.o: myprintf.cpp
	$(CC) $(CFLAGS) -c $^

.PHONY: clean
clean:
	@rm *.a *.o
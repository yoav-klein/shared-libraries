
GREEN=\e[32m
BOLD_GREEN=\e[32;1m
RESET=\e[0m
CC=gcc
CFLAGS=-c -fPIC -Wall -Wextra -ansi -pedantic-errors
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

SHELL=/bin/bash
LDFLAGS = -L. -lfoo

STATIC_LIBS=libfoo.a
SHARED_LIBS=libfoo.so

static: main.o $(STATIC_LIBS)
	$(CC) $(LDFLAGS) $^ -o $@


shared: main.o $(SHARED_LIBS)
	$(CC) $(LDFLAGS) -Wl,-rpath=\$$ORIGIN $^ -o $@ 

main.o: main.c
	@echo -e "$(GREEN)Compiling main.o$(RESET)"
	@$(CC) $(CFLAGS) main.c

libfoo.a: foo.o
	@echo -e "$(BOLD_GREEN)Creating libfoo.a$(RESET)"
	@ar -rc $@ $^

libfoo.so: foo.o
	@echo -e "$(BOLD_GREEN)Creating libfoo.so$(RESET)"
	@$(CC) -shared -Wl,-rpath=. -o $@ $^ 
		
foo.o: foo.c foo.h
	@echo -e "$(GREEN)Compiling foo.o$(RESET)"
	@$(CC) $(CFLAGS) $< 


.PHONY: clean	
clean:
	rm *.o *.so* *.out *.a shared static



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
LDFLAGS = -L. -lfoo -lbar

STATIC_LIBS=libfoo.a libbar.a
SHARED_LIBS=libbar.so libfoo.so

static: main.o $(STATIC_LIBS)
	$(CC) $(LDFLAGS) $^ -o $@

test:
	echo \$$OATH

shared: main.o $(SHARED_LIBS)
	$(CC) $(LDFLAGS) -Wl,-rpath=\$$ORIGIN $^ -o $@ 

main.o: main.c
	@echo -e "$(GREEN)Compiling main.o$(RESET)"
	@$(CC) $(CFLAGS) main.c

libfoo.a: foo.o
	@echo -e "$(BOLD_GREEN)Creating libfoo.a$(RESET)"
	@ar -rc $@ $^

libbar.a: bar.o
	@echo -e "$(BOLD_GREEN)Creating libbar.a$(RESET)"
	@ar -rc $@ $^

libbar.so: bar.o
	@echo -e "$(BOLD_GREEN)Creating libbar.so$(RESET)"
	@$(CC) -shared -o $@ $^
	
libfoo.so: foo.o
	@echo -e "$(BOLD_GREEN)Creating libfoo.so$(RESET)"
	@$(CC) -shared -Wl,-rpath=. -o $@ $^ 
	
bar.o: bar.c bar.h
	@echo -e "$(GREEN)Compiling bar.o$(RESET)"
	@$(CC) $(CFLAGS) $<
	
foo.o: foo.c foo.h
	@echo -e "$(GREEN)Compiling foo.o$(RESET)"
	@$(CC) $(CFLAGS) $< 


.PHONY: clean	
clean:
	rm *.o *.so* *.out *.a shared static


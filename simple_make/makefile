

CC=gcc
SONAME=libfoo.so
STATICNAME=libfoo.a
INCDIR=.

.PHONY: shared
shared: $(LIBNAME)

main_static.out: main.o $(STATICNAME)
	$(CC) main.o -L. -lfoo -o $@

main_shared.out: main.o $(SONAME)
	$(CC) main.o -L. -lfoo -o $@ -Wl,-rpath=.

main.o: main.c
	$(CC) -c main.c

$(SONAME): foo.o
	$(CC) -shared -o $@ $^

.PHONY: static
static: $(STATICNAME)
$(STATICNAME): foo.o
	ar -rc $@ $^

foo.o: foo.c
	$(CC) -c -fpic -o $@ $^ 

.PHONY: clean	
clean:
	rm *.o *.so* *.out *.a




CC=gcc
SONAME=libfoo.so
STATICNAME=libfoo.a
INCDIR=.


main_static.out: main.o $(STATICNAME)
	$(CC) main.o -L. -lfoo -o $@

main_shared.out: main.o $(SONAME)
	$(CC) main.o -L. -lfoo -o $@ -Wl,-rpath=$ORIGIN

main.o: main.c
	$(CC) -c main.c

.PHONY: shared
shared: $(SONAME)
$(SONAME): foo.o bar.o
	$(CC) -shared -o $@ $^

.PHONY: static
static: $(STATICNAME)
$(STATICNAME): foo.o bar.o
	ar -rc $@ $^

bar.o: bar.c
	$(CC) -c -fpic -o $@ $^
	
foo.o: foo.c
	$(CC) -c -fpic -o $@ $^ 

.PHONY: clean	
clean:
	rm *.o *.so* *.out *.a


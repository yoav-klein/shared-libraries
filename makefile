

CC=gcc
LIBNAME=libfoo.so

main.out: main.o $(LIBNAME)
	$(CC) main.o -L. -l:libfoo.so.0.1.0 -o $@ -Wl,-rpath=.

main.o: main.c
	$(CC) -c main.c

$(LIBNAME): foo.o
	$(CC) -shared -o $@ $^

foo.o: foo.c
	$(CC) -c -fpic -o $@ $^ 

.PHONY: clean	
clean:
	rm *.o *.so* *.out


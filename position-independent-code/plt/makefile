

CC=gcc
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CC += -g
endif

app: main.o libfoo.so libbar.so
	$(CC) -o $@ $^ -zlazy -Wl,-rpath=.

libfoo.so: foo.o
	$(CC) -shared $^ -o $@ 

libbar.so: bar.o
	$(CC) -shared $^ -o $@

foo.o: foo.c
	$(CC) -fPIC -c $< -o $@
	
bar.o: bar.c
	$(CC) -fPIC -c $< -o $@

main.o: main.c
	$(CC) -c -o $@ $^
	
.PHOYN: clean
clean:
	rm *.o *.so app

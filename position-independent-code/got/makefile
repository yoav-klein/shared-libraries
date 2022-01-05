
CC=gcc -g

app: main.o libfoo.so
	$(CC) -o $@ $^ -Wl,-rpath=.
	
libfoo.so: foo.c
	$(CC) -fPIC -shared $^ -o $@ 

main.o: main.c
	$(CC) -c -o $@ $^
	
.PHOYN: clean
clean:
	rm *.o *.so app

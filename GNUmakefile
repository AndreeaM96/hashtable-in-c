build: tema1
	
tema1: tema1.o libhash.so
	gcc tema1.o -o tema1 -L. -lhash

tema1.o: tema1.c
	gcc -c tema1.c -o tema1.o

clean:
	rm -rf tema1 tema1.o
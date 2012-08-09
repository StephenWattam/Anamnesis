CC=gcc
CFLAGS=-Wall

all: anamnesis.o
	$(CC) $(CFLAGS) anamnesis.o anamnesis-test.c -o anamnesis-test

anamnesis.o: anamnesis.c

clean:
	rm -rf *.o
	rm -rf anamnesis-test


all: protocol.o rsa.o
	gcc -g rsa.o protocol.o -lm

rsa.o: rsa.c rsa.h
	gcc -c rsa.c -o rsa.o -lm

protocol.o: protocol.c rsa.c rsa.h  certificate.h
	gcc -c protocol.c -o protocol.o -lm

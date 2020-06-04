SHELL := /bin/bash
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CC=g++
CFLAGS= -o 5.out -lssl -lcrypto
endif
ifeq ($(UNAME), Darwin)
CC=/usr/local/Cellar/gcc/9.3.0_1/bin/gcc-9
CFLAGS= -o 6ssl.out -lssl -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include
endif


build:
	@$(CC) main.c $(CFLAGS)

run: build
	@./6ssl.out

clean:
	rm *.out *.pem 
	rm -rf *.dSYM

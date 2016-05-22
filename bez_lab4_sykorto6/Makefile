SHELL := /bin/bash
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CC=g++
CODE= -o code.out
DECODE= -o decode.out
CFLAGS= -lssl -lcrypto
endif
ifeq ($(UNAME), Darwin)
CC=/usr/local/Cellar/gcc/5.3.0/bin/gcc-5
CODE= -o code.out
DECODE= -o decode.out
CFLAGS= -lssl -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include
endif


build:
	@$(CC) code.c $(CODE) $(CFLAGS)
	@$(CC) decode.c $(DECODE) $(CFLAGS)

test: build
	@./code.out message.txt pubkey.pem
	@./decode.out encrypted_message privkey.pem
	@diff decrypted_message message.txt && echo "Success"

clean:
	rm encrypted_message decrypted_message *.out *.dSYM



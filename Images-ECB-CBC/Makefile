SHELL := /bin/bash
CC=/usr/local/Cellar/gcc/5.3.0/bin/gcc-5
CFLAGS= -o 3.out -lssl -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include


build: 
	@$(CC) main.c $(CFLAGS)

runTask:
	@./3.out -e homer-simpson.bmp homer-simpson-encrypted-cbc.bmp cbc
	@./3.out -e homer-simpson.bmp homer-simpson-encrypted-ecb.bmp ecb
	@./3.out -e Mad_scientist.bmp Mad_scientist-encrypted-cbc.bmp cbc
	@./3.out -e Mad_scientist.bmp Mad_scientist-encrypted-ecb.bmp ecb

	@./3.out -d homer-simpson-encrypted-cbc.bmp homer-simpson-decrypted-cbc.bmp cbc
	@./3.out -d homer-simpson-encrypted-ecb.bmp homer-simpson-decrypted-ecb.bmp ecb
	@./3.out -d Mad_scientist-encrypted-cbc.bmp Mad_scientist-decrypted-cbc.bmp cbc
	@./3.out -d Mad_scientist-encrypted-ecb.bmp Mad_scientist-decrypted-ecb.bmp ecb

run: build runTask

	@echo "Mad Scientist"
	@shasum -a 256 Mad_scientist.bmp Mad_scientist-decrypted-cbc.bmp Mad_scientist-decrypted-ecb.bmp Mad_scientist-encrypted-cbc.bmp Mad_scientist-encrypted-ecb.bmp

	@echo "Homer Simpson"
	@shasum -a 256 homer-simpson.bmp homer-simpson-decrypted-cbc.bmp homer-simpson-decrypted-ecb.bmp homer-simpson-encrypted-cbc.bmp homer-simpson-encrypted-ecb.bmp


testByDiff: build runTask

	@diff homer-simpson.bmp homer-simpson-decrypted-cbc.bmp && echo "Success"
	@diff homer-simpson.bmp homer-simpson-decrypted-ecb.bmp && echo "Success"
	@diff Mad_scientist.bmp Mad_scientist-decrypted-ecb.bmp && echo "Success"
	@diff Mad_scientist.bmp Mad_scientist-decrypted-cbc.bmp && echo "Success"

uberTest: build
	@chmod +x randomTester.sh
	@./randomTester.sh


clean:
	@rm -rf 3.out 2>/dev/null 
	@rm -rf *crypted* 2>/dev/null 
	@rm *[0-9].bmp 2>/dev/null 
	@rm *.html 2>/dev/null 
SHELL := /bin/bash
CC=/usr/local/Cellar/gcc/5.3.0/bin/gcc-5
CFLAGS= -o 3.out -lssl -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include


build: 
	$(CC) main.c $(CFLAGS)

test: build
	./3.out -e homer-simpson.bmp homer-simpson-encrypted-cbc.bmp cbc
	./3.out -e homer-simpson.bmp homer-simpson-encrypted-ecb.bmp ecb
	./3.out -e Mad_scientist.bmp Mad_scientist-encrypted-cbc.bmp cbc
	./3.out -e Mad_scientist.bmp Mad_scientist-encrypted-ecb.bmp ecb

	./3.out -d homer-simpson-encrypted-cbc.bmp homer-simpson-decrypted-cbc.bmp cbc
	./3.out -d homer-simpson-encrypted-ecb.bmp homer-simpson-decrypted-ecb.bmp ecb
	./3.out -d Mad_scientist-encrypted-cbc.bmp Mad_scientist-decrypted-cbc.bmp cbc
	./3.out -d Mad_scientist-encrypted-ecb.bmp Mad_scientist-decrypted-ecb.bmp ecb


	diff homer-simpson.bmp homer-simpson-decrypted-cbc.bmp && echo "Success"
	diff homer-simpson.bmp homer-simpson-decrypted-ecb.bmp && echo "Success"
	diff Mad_scientist.bmp Mad_scientist-decrypted-ecb.bmp && echo "Success"
	diff Mad_scientist.bmp Mad_scientist-decrypted-cbc.bmp && echo "Success"

testRandom: build
	chmod +x randomTester.sh
	./randomTester.sh


clean:
	rm -rf 3.out
	rm -rf *crypted*
	rm *[0-9].bmp
	rm *.html
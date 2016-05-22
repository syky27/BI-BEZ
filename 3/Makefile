CC=/usr/local/Cellar/gcc/5.3.0/bin/gcc-5
CFLAGS= -o 3.out -lssl -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include


build: 
	$(CC) main.c $(CFLAGS)

test:
	./3.out -e homer-simpson.bmp homer-simpson-encrypted-cbc.bmp cbc
	./3.out -e homer-simpson.bmp homer-simpson-encrypted-ecb.bmp ecb
	./3.out -e Mad_scientist.bmp Mad_scientist-encrypted-cbc.bmp cbc
	./3.out -e Mad_scientist.bmp Mad_scientist-encrypted-ecb.bmp ecb

	./3.out -d homer-simpson.bmp homer-simpson-decrypted-cbc.bmp cbc
	./3.out -d homer-simpson.bmp homer-simpson-decrypted-ecb.bmp ecb
	./3.out -d Mad_scientist.bmp Mad_scientist-decrypted-cbc.bmp cbc
	./3.out -d Mad_scientist.bmp Mad_scientist-decrypted-ecb.bmp ecb





prg: 
		$(CC) $(CFLAGS) obrazek3.cpp -o obrazek
		@echo "USAGE: bez parametru spusti s obrazkem Mad_scientist.bmp"
		@echo "	      vlastni parametrizace:"
		@echo "	      make"
		@echo "	      make ARG=\"soubor.bmp\" run "

run: 
	./obrazek ${ARG} "ecb"
	./obrazek ${ARG} "cbc"
	sha256sum *.bmp
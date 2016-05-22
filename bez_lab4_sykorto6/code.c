// Syky.
// https://github.com/syky27/BI-BEZ

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include <arpa/inet.h>

#define MAX 4096

int Code(char *inputFile, char *keyFile)
{
    FILE *pub;
    FILE *fin;
    FILE * out_file;
    char *name = "encrypted_message";
    EVP_PKEY * pubkey;
    EVP_CIPHER_CTX ctx;

    const EVP_CIPHER *type = EVP_aes_128_cbc();

    unsigned char in[MAX];
    unsigned char bufferout[MAX + EVP_MAX_IV_LENGTH];
    int  inlen, bufferlen;

    if ((fin = fopen(inputFile, "rb")) == NULL) {
        printf("Error: Opening file to cypher\n");
        return 1;
    }

    inlen = fread (in, 1, MAX, fin);
    if ((pub = fopen(keyFile, "rb")) == NULL) {
        printf("Error: Opening key file\n");
        return 1;
    }

    pubkey = PEM_read_PUBKEY(pub, NULL, NULL, NULL);
    if (!pubkey) {
        printf("Error\n");
        return 1;
    }

    fclose(fin);
    fclose(pub);


    unsigned char * my_ek = (unsigned char *) malloc(EVP_PKEY_size(pubkey)); 
    int my_ekl; 
    unsigned char iv[EVP_MAX_IV_LENGTH]; 

    if ((out_file = fopen(name, "wb")) == NULL) {
        printf("Error\n");
        return 1;
    }

    EVP_CIPHER_CTX_init(&ctx);
    if (!EVP_SealInit( &ctx, type, &my_ek, &my_ekl, iv, &pubkey, 1)){
        printf("Error Buffer overflow maybe?\n");
        return 1;
    }

    uint32_t eklen_n = htonl(my_ekl);
    if (fwrite(&eklen_n, sizeof eklen_n, 1, out_file) != 1){
        printf("Error\n");
        return 1;
    }

    if (fwrite(my_ek, my_ekl, 1, out_file) != 1){
        printf("Error\n");
        return 1;
    }

    if (fwrite(iv, EVP_CIPHER_iv_length(type), 1, out_file) != 1){
        printf("Error\n");
        return 1;
    }

    if (!EVP_SealUpdate(&ctx, bufferout, &bufferlen, in, inlen)){
        printf("Error\n");
        return 1;
    }

    if (fwrite(bufferout, bufferlen, 1, out_file) != 1){
        printf("Error\n");
        return 1;
    }

    if (!EVP_SealFinal(&ctx, bufferout, &bufferlen)){
        printf("Error\n");
        return 1;
    }

    if (fwrite(bufferout, bufferlen, 1, out_file) != 1){
        printf("Error\n");
        return 1;
    }

    //free
    free(my_ek);
    fclose(out_file);
    EVP_PKEY_free(pubkey);
    EVP_CIPHER_CTX_cleanup(&ctx);
    EVP_cleanup();


    return 0;
}

void printUsage(void) {
    printf("Usage: executable message key");
    exit(1);
}

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Invalid Usage\n");
        printUsage();
    }
    return Code(argv[1], argv[2]);
}
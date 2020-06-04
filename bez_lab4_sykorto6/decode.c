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

int Decode(char *inputFile, char *keyFile)
{
    FILE *priv;
    FILE *fin;
    FILE * out_file;
    char *name = "decrypted_message";
    EVP_PKEY * privkey;
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    const EVP_CIPHER *type = EVP_aes_128_cbc();
    uint32_t eklen_n;
    unsigned char out[MAX + EVP_MAX_IV_LENGTH],  in[MAX];
    int outlen,  inlen;

    if ((priv = fopen(keyFile, "rb")) == NULL) {
        printf("Error: Opening file with private key\n");
        return 1;
    }

    privkey = PEM_read_PrivateKey(priv, NULL, NULL, NULL);
    if (!privkey) {
        printf("Error: Reading private key\n");
        return 1;
    }


    fclose(priv);

    unsigned char * my_ek = (unsigned char *) malloc(EVP_PKEY_size(privkey)); // allocate space for encrypted symmet. key
    int my_ekl; // enc. sym. key length
    unsigned char iv[EVP_MAX_IV_LENGTH]; // buffer for the init. vector

    if ((fin = fopen(inputFile, "rb")) == NULL) {
        printf("Error: Opening file to decrypt\n");
        return 1;
    }

    type = EVP_aes_128_cbc();

    if (fread(&eklen_n, sizeof eklen_n, 1, fin) != 1){
        printf("Error: Reading lenght of exponent\n");
        return 1;
    }

    my_ekl = ntohl(eklen_n);
    if (fread(my_ek, my_ekl, 1, fin) != 1){
        printf("Error: Reading exponent\n");
        return 1;
    }

    if (fread(iv, EVP_CIPHER_iv_length(type), 1, fin) != 1){
        printf("Error: Reading IV lenght\n");
        return 1;
    }

    EVP_CIPHER_CTX_init(ctx);
    if (!EVP_OpenInit(ctx, type, my_ek, my_ekl, iv, privkey)){
        printf("Error: Opening EVP\n");
        return 1;
    }

    inlen = fread(in, sizeof(unsigned char), sizeof (in), fin);
    if (inlen < 1){
        printf("Error: Reading cyphered text\n");
        return 1;
    }

    if ((out_file = fopen(name, "wb")) == NULL) {
        printf("Error\n");
        return 1;
    }

    if (!EVP_OpenUpdate(ctx, out, &outlen, in, inlen)){
        printf("Error: EVP Openupdate\n");
        return 1;
    }

    if (fwrite(out, outlen, 1, out_file) != 1){
        printf("Error\n");
        return 1;
    }

    if (!EVP_OpenFinal(ctx, out, &outlen)){
        printf("Error\n");
        return 1;
    }


    if (fwrite(out, outlen, 1, out_file) != 1){
        printf("Error\n");
        return 1;
    }

    //free
    free(my_ek);
    fclose(fin);
    fclose(out_file);
    EVP_PKEY_free(privkey);
    EVP_CIPHER_CTX_cleanup(ctx);
    EVP_cleanup();

    return 0;
}

void printUsage(void) {
    printf("Usage: executable cryptedText key");
    exit(1);
}

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Invalid Usage\n");
        printUsage();
    }
    return Decode(argv[1], argv[2]);
}

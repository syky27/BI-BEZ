#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>



void EncryptImage(const char* inputFile, const char* outputFile,const char* mod) {

    unsigned char key[EVP_MAX_KEY_LENGTH] = "Romane je to 54";  // klic pro sifrovani
    unsigned char iv[EVP_MAX_IV_LENGTH] = "inicial. vektor";  // inicializacni vektor
    int i;
    int tmpLength = 0;

    EVP_CIPHER_CTX ctx; // struktura pro kontext


    /* Sifrovani */

    if(!strcmp(mod,"cbc"))
        EVP_EncryptInit(&ctx, EVP_des_cbc(), key, iv);  // nastaveni kontextu pro sifrovani
    else
        EVP_EncryptInit(&ctx, EVP_des_ecb(), key, iv);  // nastaveni kontextu pro sifrovani

    unsigned char *ot;
    long input_file_size;
    FILE * pFile = fopen(inputFile, "rb");
    FILE * outFile = fopen(outputFile, "wb");

    fseek(pFile, 0, SEEK_END);
    input_file_size = ftell(pFile);
    rewind(pFile);

    ot = (unsigned char*)malloc(input_file_size * (sizeof(unsigned char)));
    fread(ot, sizeof(unsigned char), input_file_size, pFile);
    fclose(pFile);

    unsigned char *st = (unsigned char*)malloc(input_file_size * (sizeof(unsigned char)));
    int blockbyte = 54;
    int stLength = input_file_size;
    int otLength = input_file_size - blockbyte;

    fwrite (ot , sizeof(unsigned char), blockbyte, outFile);

    unsigned char *ot_tmp = ot + blockbyte;
    EVP_EncryptUpdate(&ctx,  st, &stLength, ot_tmp, otLength);  // sifrovani ot
    EVP_EncryptFinal(&ctx, &st[stLength], &tmpLength);  // ziskani sifrovaneho textu z kontextu
    stLength += tmpLength;

    fwrite (st , sizeof(unsigned char), stLength, outFile);

    fflush(outFile);
    fclose(outFile);

    free(ot);
    free(st);

}

void DecryptImage(const char* inputFile, const char* outputFile,const char* mod) {

    unsigned char key[EVP_MAX_KEY_LENGTH] = "Romane je to 54";  // klic pro sifrovani
    unsigned char iv[EVP_MAX_IV_LENGTH] = "inicial. vektor";  // inicializacni vektor
    int i;


    int tmpLength = 0;

    EVP_CIPHER_CTX ctx; // struktura pro kontext


    /* Sifrovani */

    if(!strcmp(mod,"cbc"))
        EVP_DecryptInit(&ctx, EVP_des_cbc(), key, iv);  // nastaveni kontextu pro sifrovani
    else
        EVP_DecryptInit(&ctx, EVP_des_ecb(), key, iv);  // nastaveni kontextu pro sifrovani


    unsigned char *ot;
    long input_file_size;
    FILE * pFile = fopen(inputFile, "rb");
    FILE * outFile = fopen(outputFile, "wb");
    fseek(pFile, 0, SEEK_END);
    input_file_size = ftell(pFile);
    rewind(pFile);


    ot = (unsigned char*)malloc(input_file_size * (sizeof(unsigned char)));
    fread(ot, sizeof(unsigned char), input_file_size, pFile);
    fclose(pFile);

    unsigned char *st = (unsigned char*)malloc(input_file_size * (sizeof(unsigned char)));
    int blockbyte = 54;
    int stLength = input_file_size;
    int otLength = input_file_size - blockbyte;

    fwrite (ot , sizeof(unsigned char), blockbyte, outFile);
    unsigned char *ot_tmp = ot + blockbyte;
    EVP_DecryptUpdate(&ctx,  st, &stLength, ot_tmp, otLength);  // sifrovani ot
    EVP_DecryptFinal(&ctx, &st[stLength], &tmpLength);  // ziskani sifrovaneho textu z kontextu
    stLength += tmpLength;

    fwrite (st , sizeof(unsigned char), stLength, outFile);

    fflush(outFile);
    fclose(outFile);

    free(ot);
    free(st);

}

void printUsage(void){
    printf("Usage: executable [-d || -e] inputFile outputFile [ecb || cbc]");
    exit(1);
}

int main(int argc, char** argv) {

    if (argc != 5){
        printUsage();
    } else {
        if (!strcmp(argv[1],"-d")) {
            if (strcmp(argv[4],"cbc") || strcmp(argv[4],"ecb")) {
                printUsage();
            }
            DecryptImage(argv[2], argv[3], argv[4]);
        } else if (!strcmp(argv[1],"-e")) {
            EncryptImage(argv[2], argv[3], argv[4]);
        } else {
            printf("Invalid option\n");
            printUsage();
        }
    }

    

    // EncryptImage("homer-simpson.bmp", "test.bmp","cbc");
    // DecryptImage("test.bmp", "test2.bmp","cbc");


    return 0;
}


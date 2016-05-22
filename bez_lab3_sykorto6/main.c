// Syky.
// https://github.com/syky27/BI-BEZ

#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>

void checkIfFileIsBitmap(FILE * file);
void checkIfFileHasExecute(FILE * file);
void printUsage(void);
void encryptImage(const char* inputFile, const char* outputFile, const char* mod);
void decryptImage(const char* inputFile, const char* outputFile,const char* mod);

void encryptImage(const char* inputFile, const char* outputFile, const char* mod) {

    unsigned char key[EVP_MAX_KEY_LENGTH] = "kjfahjkfhsajkhfkjas";
    unsigned char iv[EVP_MAX_IV_LENGTH] = "dkasjkd";
    int tmpLength = 0;
    EVP_CIPHER_CTX ctx;

    if(!strcmp(mod,"cbc")) {
        EVP_EncryptInit(&ctx, EVP_des_cbc(), key, iv);
    } else {
        EVP_EncryptInit(&ctx, EVP_des_ecb(), key, iv);
    }

    unsigned char *plainText;
    unsigned long long inputFileSize;

    FILE * ptrInputFile = fopen(inputFile, "rb");
    FILE * ptrOutputFile = fopen(outputFile, "wb");
    
    checkIfFileHasExecute(ptrInputFile);
    checkIfFileHasExecute(ptrOutputFile);
    checkIfFileIsBitmap(ptrInputFile);

    fseek(ptrInputFile, 0, SEEK_END);
    inputFileSize = ftell(ptrInputFile);
    rewind(ptrInputFile);

    plainText = (unsigned char*)malloc(inputFileSize * (sizeof(unsigned char)));
    fread(plainText, sizeof(unsigned char), inputFileSize, ptrInputFile);
    fclose(ptrInputFile);

    unsigned char *cypheredText = (unsigned char*)malloc(inputFileSize * (sizeof(unsigned char)));
    int blockbyte = 54;
    int cypheredTextLength = inputFileSize;
    int plainTextLength = inputFileSize - blockbyte;

    fwrite (plainText , sizeof(unsigned char), blockbyte, ptrOutputFile);

    unsigned char *plainText_tmp = plainText + blockbyte;
    EVP_EncryptUpdate(&ctx,  cypheredText, &cypheredTextLength, plainText_tmp, plainTextLength);
    EVP_EncryptFinal(&ctx, &cypheredText[cypheredTextLength], &tmpLength);
    cypheredTextLength += tmpLength;

    fwrite (cypheredText , sizeof(unsigned char), cypheredTextLength, ptrOutputFile);

    fflush(ptrOutputFile);
    fclose(ptrOutputFile);


    free(plainText);
    free(cypheredText);
    EVP_CIPHER_CTX_cleanup(&ctx); 


}

void decryptImage(const char* inputFile, const char* outputFile,const char* mod) {

    unsigned char key[EVP_MAX_KEY_LENGTH] = "kjfahjkfhsajkhfkjas";
    unsigned char iv[EVP_MAX_IV_LENGTH] = "dkasjkd";
    int tmpLength = 0;
    EVP_CIPHER_CTX ctx;

    if(!strcmp(mod,"cbc")){
        EVP_DecryptInit(&ctx, EVP_des_cbc(), key, iv);
    } else {
        EVP_DecryptInit(&ctx, EVP_des_ecb(), key, iv);
    }


    unsigned char *plainText;
    long inputFileSize;
    FILE * ptrInputFile = fopen(inputFile, "rb");
    FILE * ptrOutputFile = fopen(outputFile, "wb");

    checkIfFileHasExecute(ptrInputFile);
    checkIfFileHasExecute(ptrOutputFile);
    checkIfFileIsBitmap(ptrInputFile);


    fseek(ptrInputFile, 0, SEEK_END);
    inputFileSize = ftell(ptrInputFile);
    rewind(ptrInputFile);


    plainText = (unsigned char*)malloc(inputFileSize * (sizeof(unsigned char)));
    fread(plainText, sizeof(unsigned char), inputFileSize, ptrInputFile);
    fclose(ptrInputFile);

    unsigned char *cypheredText = (unsigned char*)malloc(inputFileSize * (sizeof(unsigned char)));
    int blockbyte = 54;
    int cypheredTextLength = inputFileSize;
    int plainTextLength = inputFileSize - blockbyte;

    fwrite (plainText , sizeof(unsigned char), blockbyte, ptrOutputFile);
    unsigned char *plainText_tmp = plainText + blockbyte;
    EVP_DecryptUpdate(&ctx,  cypheredText, &cypheredTextLength, plainText_tmp, plainTextLength);
    EVP_DecryptFinal(&ctx, &cypheredText[cypheredTextLength], &tmpLength);
    cypheredTextLength += tmpLength;

    fwrite (cypheredText , sizeof(unsigned char), cypheredTextLength, ptrOutputFile);

    fflush(ptrOutputFile);
    fclose(ptrOutputFile);

    free(plainText);
    free(cypheredText);
    EVP_CIPHER_CTX_cleanup(&ctx); 

}

void printUsage(void) {
    printf("Usage: executable [-d || -e] inputFile outputFile [ecb || cbc]");
    exit(1);
}

void checkIfFileIsBitmap(FILE * file) {
    char BMTestArray[2];
    if (fread(BMTestArray, 1, 2, file) == 2){
      if (!(BMTestArray[0] == 66 || BMTestArray[1] == 77)) {
          printf("Given file is not a bitmap\n");
          printUsage();
      }
   }
}

void checkIfFileHasExecute(FILE * file) {
    if (file == NULL) {
        perror("Error");
        exit(1);
    }
}


int main(int argc, char** argv) {
    if (argc != 5){
        printUsage();
    } else {
        if (!strcmp(argv[1],"-d")) {
            if (strcmp(argv[4],"cbc") && strcmp(argv[4],"ecb")) {
                printf("Invalid cypher type\n");
                printUsage();
            }
            decryptImage(argv[2], argv[3], argv[4]);
        } else if (!strcmp(argv[1],"-e")) {
            if (strcmp(argv[4],"cbc") && strcmp(argv[4],"ecb")) {
                printf("Invalid cypher type\n");
                printUsage();
            }
            encryptImage(argv[2], argv[3], argv[4]);
        } else {
            printf("Invalid option\n");
            printUsage();
        }
    }
    return 0;
}


// Syky.
// https://github.com/syky27/BI-BEZ

#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>

unsigned char key[EVP_MAX_KEY_LENGTH] = "D9YwLlaCrf5aJjBmsW4Oex46Ky3TTxwv79Zr0cr2Dz9SfsxRU4Hcol06GTvRClbJ";
unsigned char iv[EVP_MAX_IV_LENGTH] = "KH5nVwGtTDVS4FEt";

void checkIfFileIsBitmap(FILE * file);
void checkIfFileHasExecute(FILE * file);
void printUsage(void);
void encryptImage(const char* inputFile, const char* outputFile, const char* mod);
void decryptImage(const char* inputFile, const char* outputFile,const char* mod);

void encryptImage(const char* inputFile, const char* outputFile, const char* mod) {
    
    EVP_CIPHER_CTX ctx;
    if(!strcmp(mod,"cbc")) {
        EVP_EncryptInit(&ctx, EVP_des_cbc(), key, iv);
    } else {
        EVP_EncryptInit(&ctx, EVP_des_ecb(), key, iv);
    }

    FILE * ptrInputFile = fopen(inputFile, "rb");
    FILE * ptrOutputFile = fopen(outputFile, "wb");
    
    checkIfFileHasExecute(ptrInputFile);
    checkIfFileHasExecute(ptrOutputFile);
    checkIfFileIsBitmap(ptrInputFile);

    unsigned char * header = (unsigned char*)malloc(54 * (sizeof(unsigned char)));
    fread(header, sizeof(unsigned char), 54, ptrInputFile);
    fwrite (header, sizeof(unsigned char), 54, ptrOutputFile);

    unsigned char * inBuffer = (unsigned char*)malloc(32 * (sizeof(unsigned char)));
    unsigned char * outBuffer = (unsigned char*)malloc(32 * (sizeof(unsigned char)));

    int inBufferRead = 0;
    int actualReadSize = 0;
    while(!feof(ptrInputFile))  {
        actualReadSize = fread( inBuffer, sizeof(unsigned char), 32,  ptrInputFile);
        inBufferRead += actualReadSize;
        EVP_EncryptUpdate(&ctx, outBuffer, &inBufferRead, inBuffer, actualReadSize);
        fwrite(outBuffer, sizeof(unsigned char), inBufferRead, ptrOutputFile);
    }
    
    int tmpLength = 0;
    EVP_EncryptFinal(&ctx, outBuffer, &tmpLength);
    fwrite (outBuffer, sizeof(unsigned char), tmpLength, ptrOutputFile);
    
    fflush(ptrOutputFile);
    fclose(ptrOutputFile);
    fclose(ptrInputFile);

    free(header);
    free(outBuffer);
    free(inBuffer);
    EVP_CIPHER_CTX_cleanup(&ctx); 
}

void decryptImage(const char* inputFile, const char* outputFile,const char* mod) {

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



    unsigned char * header = (unsigned char*)malloc(54 * (sizeof(unsigned char)));
    fread(header, sizeof(unsigned char), 54, ptrInputFile);
    fwrite (header, sizeof(unsigned char), 54, ptrOutputFile);

    unsigned char * inBuffer = (unsigned char*)malloc(32 * (sizeof(unsigned char)));
    unsigned char * outBuffer = (unsigned char*)malloc(32 * (sizeof(unsigned char)));

    int inBufferRead = 0;
    int actualReadSize = 0;
    while(!feof(ptrInputFile))  {
        actualReadSize = fread( inBuffer, sizeof(unsigned char), 32,  ptrInputFile);
        inBufferRead += actualReadSize;
        EVP_DecryptUpdate(&ctx, outBuffer, &inBufferRead, inBuffer, actualReadSize);
        fwrite(outBuffer, sizeof(unsigned char), inBufferRead, ptrOutputFile);
    }
    
    int tmpLength = 0;
    EVP_DecryptFinal(&ctx, outBuffer, &tmpLength);
    fwrite (outBuffer, sizeof(unsigned char), tmpLength, ptrOutputFile);
    
    fflush(ptrOutputFile);
    fclose(ptrOutputFile);
    fclose(ptrInputFile);

    free(header);
    free(outBuffer);
    free(inBuffer);
    EVP_CIPHER_CTX_cleanup(&ctx); 

}

void printUsage(void) {
    printf("Usage: executable [-d || -e] inputFile outputFile [ecb || cbc]");
    exit(1);
}

void checkIfFileIsBitmap(FILE * file) {
    char BMTestArray[2];
    if (fread(BMTestArray, 1, 2, file) == 2){
      if (!(BMTestArray[0] == 'B' || BMTestArray[1] == 'M')) {
          printf("Given file is not a bitmap\n");
          rewind(file);
          printUsage();
      }
   }
   rewind(file);
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


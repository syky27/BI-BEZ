// Syky.

#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>


void checkIfFileIsBitmap(FILE * file);
void checkIfFileHasExecute(FILE * file);


void EncryptImage(const char* inputFile, const char* outputFile, const char* mod) {

    unsigned char key[EVP_MAX_KEY_LENGTH] = "kjfahjkfhsajkhfkjas";
    unsigned char iv[EVP_MAX_IV_LENGTH] = "dkasjkd";
    int tmpLength = 0;

    EVP_CIPHER_CTX ctx;

    if(!strcmp(mod,"cbc"))
        EVP_EncryptInit(&ctx, EVP_des_cbc(), key, iv);
    else
        EVP_EncryptInit(&ctx, EVP_des_ecb(), key, iv);

    unsigned char *ot;
    unsigned long long input_file_size;

    FILE * pFile = fopen(inputFile, "rb");
    FILE * outFile = fopen(outputFile, "wb");
    
    checkIfFileHasExecute(pFile);
    checkIfFileHasExecute(outFile);
    checkIfFileIsBitmap(pFile);

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
    EVP_EncryptUpdate(&ctx,  st, &stLength, ot_tmp, otLength);
    EVP_EncryptFinal(&ctx, &st[stLength], &tmpLength);
    stLength += tmpLength;

    fwrite (st , sizeof(unsigned char), stLength, outFile);

    fflush(outFile);
    fclose(outFile);

    free(ot);
    free(st);

}

void DecryptImage(const char* inputFile, const char* outputFile,const char* mod) {

    unsigned char key[EVP_MAX_KEY_LENGTH] = "kjfahjkfhsajkhfkjas";
    unsigned char iv[EVP_MAX_IV_LENGTH] = "dkasjkd";
    int i;


    int tmpLength = 0;

    EVP_CIPHER_CTX ctx;

    if(!strcmp(mod,"cbc"))
        EVP_DecryptInit(&ctx, EVP_des_cbc(), key, iv);
    else
        EVP_DecryptInit(&ctx, EVP_des_ecb(), key, iv);


    unsigned char *ot;
    long input_file_size;
    FILE * pFile = fopen(inputFile, "rb");
    FILE * outFile = fopen(outputFile, "wb");

    checkIfFileHasExecute(pFile);
    checkIfFileHasExecute(outFile);
    checkIfFileIsBitmap(pFile);


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
    EVP_DecryptUpdate(&ctx,  st, &stLength, ot_tmp, otLength);
    EVP_DecryptFinal(&ctx, &st[stLength], &tmpLength);
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
            DecryptImage(argv[2], argv[3], argv[4]);
        } else if (!strcmp(argv[1],"-e")) {
            if (strcmp(argv[4],"cbc") && strcmp(argv[4],"ecb")) {
                printf("Invalid cypher type\n");
                printUsage();
            }
            EncryptImage(argv[2], argv[3], argv[4]);
        } else {
            printf("Invalid option\n");
            printUsage();
        }
    }
    return 0;
}


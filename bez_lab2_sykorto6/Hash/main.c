
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <stdlib.h>
 
#define L 50



void hex(unsigned char *hash,int length)
{
  int i;
  for(i = 0; i < length; i++){
    printf("%02x ", hash[i]);
  }
  printf("\n");
}

void hashFunction(char *text,unsigned char hash[EVP_MAX_MD_SIZE]){

    int i;
    
  char hashFunction[] = "sha1";  // zvolena hashovaci funkce ("sha1", "md5" ...)
 
  EVP_MD_CTX ctx;  // struktura kontextu
  const EVP_MD *type; // typ pouzite hashovaci funkce
  //unsigned char hash[EVP_MAX_MD_SIZE]; // char pole pro hash - 64 bytu (max pro sha 512)
  int length;  // vysledna delka hashe
 
  /* Inicializace OpenSSL hash funkci */
  OpenSSL_add_all_digests();
  /* Zjisteni, jaka hashovaci funkce ma byt pouzita */
  type = EVP_get_digestbyname(hashFunction);
 
  /* Pokud predchozi prirazeni vratilo -1, tak nebyla zadana spravne hashovaci funkce */
  if(!type) {
    printf("Hash %s neexistuje.\n", hashFunction);
    exit(1);
  }
 
  /* Provedeni hashovani */
  EVP_DigestInit(&ctx, type);  // nastaveni kontextu
  EVP_DigestUpdate(&ctx, text, strlen(text));  // zahashuje text a ulozi do kontextu
  EVP_DigestFinal(&ctx, hash, (unsigned int *) &length);  // zjiskani hashe z kontextu
 
  /* Vypsani vysledneho hashe */
 // printf("Hash textu \"%s\" je: ", text);
  
  if(hash[0]==0xAA&&hash[1]==0xBB)
    hex(hash,length);
 
}


int main(int argc, char *argv[]){
 
    int i;
    char text[L];
    unsigned char hash[EVP_MAX_MD_SIZE];
    while(1)
    {
        for(i=0;i<L;i++)
        {
            text[i]=(rand() % 25) +'a';
        }
    hashFunction(text,hash);
    }
  exit(0);
}
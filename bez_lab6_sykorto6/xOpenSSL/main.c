#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <time.h>  
 

int Cypher(const char text[30] )
{
  int i;

  char hashFunction[] = "sha256";  // zvolena hashovaci funkce ("sha1", "md5" ...)

  EVP_MD_CTX *ctx;  // struktura kontextu
  ctx = EVP_MD_CTX_create();
  const EVP_MD *type; // typ pouzite hashovaci funkce
  unsigned char hash[EVP_MAX_MD_SIZE]; // char pole pro hash - 64 bytu (max pro sha 512)
  int length;  // vysledna delka hashe
 
  /* Inicializace OpenSSL hash funkci */
  OpenSSL_add_all_digests();
  /* Zjisteni, jaka hashovaci funkce ma byt pouzita */
  type = EVP_get_digestbyname(hashFunction);
 
  /* Pokud predchozi prirazeni vratilo -1, tak nebyla zadana spravne hashovaci funkce */
  if(!type) {
    printf("Sifra %s neexistuje.\n", hashFunction);
    exit(1);
  }
 
  /* Provedeni hashovani */
  EVP_DigestInit(ctx, type);  // nastaveni kontextu
  EVP_DigestUpdate(ctx, text, strlen(text));  // zahashuje text a ulozi do kontextu
  EVP_DigestFinal(ctx, hash, (unsigned int *) &length);  // zjiskani hashe z kontextu
 
  /* Vypsani vysledneho hashe */
  printf("Hash textu \"%s\" je: ", text);
  for(i = 0; i < length; i++){
    printf("%02x", hash[i]);
  }

  printf("\n");
    if(hash[0]==0x00)
      return 1;
  
  return 0;
}


int main(int argc, char *argv[]){
 
     srand (time(NULL));
    int i,j,nalezeno=0;
    char text[30];
     

      while(!nalezeno)
      {
          for(i=0;i<30;i++)
                text[i]=(rand()% 26)+'a';
          nalezeno=Cypher(text); 
      }
          
//"bizkomyoekxlhpgseqarbtchrklzax"

 
  exit(1);
}
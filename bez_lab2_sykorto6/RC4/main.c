#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>
 

void stringToHexa(unsigned char a [1024])
{
  int i;
  printf("HEX:");
  for(i=0;i<strlen(a);i++)
  printf("0x%02x, ", a[i]);
}
void conversion(unsigned char a [1024])
{
    int i;
    for(i=0;i<strlen((const char*) a);i++)
    {
        
        if(a[i]>=0&&a[i]<=9)
            a[i]-'0';
        else
            a[i]-'A';
    }
    
}


void Sifra( unsigned char a [1024])
{
  unsigned char ot[1024];
  strcpy(ot,a);
  unsigned char st[1024]="";  // sifrovany text
  unsigned char key[EVP_MAX_KEY_LENGTH] = "Pikachu je zluty";  // klic pro sifrovani
  unsigned char iv[EVP_MAX_IV_LENGTH] = "inicial. vektor";  // inicializacni vektor
  int i;
  int otLength = strlen((const char*) ot);
  int stLength = 0;
  int tmpLength = 0;
 
  EVP_CIPHER_CTX ctx; // struktura pro kontext
 
  printf("OT: %s\n", ot);
 
  /* Sifrovani */
  EVP_EncryptInit(&ctx, EVP_rc4(), key, iv);  // nastaveni kontextu pro sifrovani
  EVP_EncryptUpdate(&ctx,  st, &stLength, ot, otLength);  // sifrovani ot
  EVP_EncryptFinal(&ctx, &st[stLength], &tmpLength);  // ziskani sifrovaneho textu z kontextu
  stLength += tmpLength;
 
  printf ("Zasifrovano %d znaku.\n", stLength);
 
  /* Desifrovani */
  EVP_DecryptInit(&ctx, EVP_rc4(), key, iv);  // nastaveni kontextu pro desifrovani
  EVP_DecryptUpdate(&ctx, ot, &otLength,  st, stLength);  // desifrovani st
  EVP_DecryptFinal(&ctx, &ot[otLength], &tmpLength);  // ziskani desifrovaneho textu z kontextu
  otLength += tmpLength;
 
  /* Vypsani zasifrovaneho a rozsifrovaneho textu. */
  printf("ST:");
  stringToHexa(st);
//  printf("%s",st);
  printf("\n");
  printf("DT: %s\n" ,ot);
  
}

void Dekodovani(unsigned char nST [1024],unsigned char zOT [1024],unsigned char zST [1024])
{
    unsigned char nOT [1024];
 
    int min=strlen(zOT);
    int i;
    for(i=0;i<min;i++)
        nOT[i]=nST[i]^zOT[i]^zST[i];
    
    printf("Dekodovany text\n");
    nOT[min] = '\0';
    printf("%s \n",nOT);
}

int main(void) {
    
    Sifra("bezpecnost je dulezita vec!!!!");
    Sifra("zlato je na zapadnim pobrezi. ");
    
    printf("Cizi texty:\n");

    unsigned char nST [1024]={0xfc, 0xe3, 0x7c, 0x0a, 0x93, 0xaf, 0x07, 0x5c, 0x99, 0x84, 0x44, 0x9a, 0xc2, 0xc0, 0x8d, 0xc7, 0x0c, 0x84, 0x43, 0x14, 0x10, 0xb0, 0xd9, 0xeb, 0xb5, 0x4f, 0x99, 0xd6, 0x0a, 0x05};
    unsigned char zOT [1024]="Zasifrovana zprava pomoci RC4.";  
    unsigned char zST [1024]={0xf6, 0xf0, 0x60, 0x00, 0xd5, 0xb7, 0x1b, 0x43, 0xd8, 0x9e, 0x4a, 0x9a, 0xdc, 0xd5, 0x8c, 0xcf, 0x1c, 0x97, 0x0c, 0x12, 0x1e, 0xb1, 0x98, 0xa6, 0xfc, 0x0c, 0xa3, 0xff, 0x51, 0x05};
    Dekodovani(nST,zOT,zST);

  exit(0);
 }
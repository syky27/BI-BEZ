#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define MAX_BUFFER 1024

#include <openssl/ssl.h>


int main(int argc, char **argv){

	SSL_library_init();
        SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();

	SSL *ssl;
	SSL_CTX *ctx = NULL;

	char buffer[MAX_BUFFER]; //DEFINICE BUFFERU
	int sockfd, cout=0, n=0; 
	struct sockaddr_in server_addr; //


	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //POKUSI SE VYTVORIT SOCKET (TCP)
		printf("Chyba: Nelze vytvorit secket!\n");
		return 1;
	}

	memset(buffer, '0', sizeof(buffer)); //VYNULUJE BUFFER
	memset(&server_addr, '0', sizeof(server_addr)); //VYNULUJE STRUKTURU SERVER_ADDR

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(443); //PORT KOMUNIKACE
	server_addr.sin_addr.s_addr = inet_addr("147.32.232.248"); //IP ADRESA SERVERU

	if((connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)))< 0){ //PŘIPOJÍ SE
		printf("Chyba: Nelze se připojit!\n");
		return 1;
	}


	ctx = SSL_CTX_new(SSLv23_client_method());
	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, sockfd);

	//forbid cipher
	SSL_set_cipher_list(ssl, "ALL:!ECDHE-RSA-AES256-GCM-SHA384");
	SSL_connect(ssl);

	int length; 

	FILE *cer;

	cer = fopen("cer.pem", "wb");

	X509 * cert = SSL_get_peer_certificate(ssl);
	PEM_write_X509(cer, cert);
	
SSL_write(ssl, "GET /student/odkazy HTTP 1.1\r\nHost: fit.cvut.cz\r\n\r\n", strlen("GET /student/odkazy HTTP 1.1\r\nHost: fit.cvut.cz\r\n\r\n"));

	FILE *out;
	remove("out.txt");
	out = fopen("out.txt", "wa");

	while(length = SSL_read(ssl, buffer, sizeof(buffer)-1)){
		
	fwrite(buffer, sizeof(char), length, out);

	}

	fclose(out);
        
        //printf("Sifra: %s\n",SSL_CIPHER_get_name(SSL_get_current_cipher(ssl)));
        printf("Sifra: %s\n",SSL_get_cipher(ssl));
           
        int i=0;
        const char* text;
        printf("Vypis listu:\n");
        while(text=SSL_get_cipher_list(ssl,i))
        {
            printf("%s\n",text);
            i++;
        }
        
	SSL_shutdown(ssl);
	if (ssl != NULL){SSL_free(ssl);}
	if (ctx != NULL) {SSL_CTX_free(ctx);}

	return 0;
} 
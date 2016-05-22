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
    SSL *ssl;
    SSL_CTX *ctx = NULL;

    char buffer[MAX_BUFFER];
    int sockfd;
    struct sockaddr_in server_addr;


    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error: Could not create socket\n");
        return 1;
    }

    memset(buffer, '0', sizeof(buffer));
    memset(&server_addr, '0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(443);
    server_addr.sin_addr.s_addr = inet_addr("147.32.232.248");

    if((connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)))< 0){
        printf("Error: Could not connect\n");
        return 1;
    }

    ctx = SSL_CTX_new(SSLv23_client_method());
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1);
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    SSL_connect(ssl);

    int length; 
    FILE *cer;
    cer = fopen("cer.pem", "wb");
    X509 * cert = SSL_get_peer_certificate(ssl);
    PEM_write_X509(cer, cert);
    fclose(cer);

    BIO *o = BIO_new_fp(stdout,BIO_NOCLOSE);
    X509_print_ex(o, cert, XN_FLAG_COMPAT, X509_FLAG_COMPAT);

    SSL_write(ssl, "GET /student/odkazy HTTP 1.1\r\nHost: fit.cvut.cz\r\n\r\n", strlen("GET /student/odkazy HTTP 1.1\r\nHost: fit.cvut.cz\r\n\r\n"));

    FILE *out;
    remove("out.txt");
    out = fopen("out.txt", "wa");

    while((length = SSL_read(ssl, buffer, sizeof(buffer)))>0){
        fwrite(buffer, sizeof(char), length, out);
    }

    fclose(out);

    SSL_shutdown(ssl);
    close(sockfd);

    if (ssl != NULL){
        SSL_free(ssl);
    }
    
    if (ctx != NULL) {
        SSL_CTX_free(ctx);
    }
    return 0;
} 
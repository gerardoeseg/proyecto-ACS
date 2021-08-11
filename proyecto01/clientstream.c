#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

// puerto 3490 aleatorio (se pueden elegir desde 1024 en adelante)
#define PORT 3490
// max number of bytes we can get at once
#define MAXDATASIZE 300

int main(int argc, char *argv[]){
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    // estructura "hostent", apuntador "he"
    struct hostent *he;

    // connectors address information
    struct sockaddr_in their_addr;
    // if no command line argument supplied
    if(argc != 2){
        fprintf(stderr, "Client-Usage: %s host_servidor\n", argv[0]);
        // just exit
        exit(1);
    }

    // si lo que devuelve gethostbyname es NULL, entra al if 
    if((he=gethostbyname(argv[1])) == NULL){ 
        // argv[1] es el argumento desde linea de comandos (dominio)
        // se le asigna el valor de gesthostbyname a la variable "he", apuntador a estructura hostend
        // asignado el valor, se compara contra NULL, si es igual, entra al if
        perror("gethostbyname()"); 
        exit(1); // termina la ejecución con exit(1)
    }
    // si no es NULL, entra al else
    else
        printf("Client-The remote host is: %s\n", argv[1]);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket()");
        exit(1);
    }
    else 
        printf("Client-The socket() sockfd is OK...\n");

    // host byte order
    their_addr.sin_family = AF_INET;
    // short, network byte order
    printf("Server-Using %s and port %d...\n", argv[1], PORT);
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);

    // zero the rest of the struct
    memset(&(their_addr.sin_zero), '\0', 8);
    if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
        perror("connect()");
        exit(1);
    }
    else
        printf("Client-The connect() is OK...\n");

    if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1){
        perror("recv()");
        exit(1);
    }
    else
        printf("Client-The recv() is OK...\n");

    buf[numbytes] = '\0';
    printf("Client-Received: %s", buf);
    printf("Client-Closing sockfd\n");
    close(sockfd);
    return 0;
}
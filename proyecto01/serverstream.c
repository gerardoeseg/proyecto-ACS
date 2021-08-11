#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

/* the port users will be connecting to */
#define MYPORT 3490

/* how many pending connections queue will hold */
#define BACKLOG 10

void sigchld_handler(int s){
    while(wait(NULL) > 0);
}

int main(int argc, char *argv[ ]){

    /*  listen on sock_fd, new connection on new_fd 
     *  sockfd es el file descriptor 1, propio del servidor
     *  new_fd es el file descriptor 2, tiene la información propia del cliente
     */
    int sockfd, new_fd;

    /* my address information */
    struct sockaddr_in my_addr;

    /* connectors address information */
    struct sockaddr_in their_addr;
    int sin_size;
    struct sigaction sa;
    int yes = 1;

    // sockfd se inicia con la llamada de la función socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Server-socket() error lol!");
        exit(1);
    }
    else
        printf("Server-socket() sockfd is OK...\n");

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        perror("Server-setsockopt() error lol!");
        exit(1);
    }
    else
        printf("Server-setsockopt is OK...\n");

    /* host byte order */
    my_addr.sin_family = AF_INET;

    /* short, network byte order */
    my_addr.sin_port = htons(MYPORT);

    /* automatically fill with my IP */
    my_addr.sin_addr.s_addr = INADDR_ANY;
    printf("Server-Using %s and port %d...\n", inet_ntoa(my_addr.sin_addr), MYPORT);

    /* zero the rest of the struct */memset(&(my_addr.sin_zero), '\0', 8);
    if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
        perror("Server-bind() error");
        exit(1);
    }
    else
        printf("Server-bind() is OK...\n");

    if(listen(sockfd, BACKLOG) == -1){
        perror("Server-listen() error");
        exit(1);
    }

    printf("Server-listen() is OK...Listening...\n");
    /* clean all the dead processes */

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    // sigaction elimina los procesos que puedan quedar zombies
    if(sigaction(SIGCHLD, &sa, NULL) == -1){
        perror("Server-sigaction() error");
        exit(1);
    } 
    else
        printf("Server-sigaction() is OK...\n");

     // servidor entra en un ciclo infinito, en él se encuentra un accept()
    while(1){
        sin_size = sizeof(struct sockaddr_in);
        // cuando un cliente se conecta, es aceptado por accept() y se inicializa new_fd
        if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
            perror("Server-accept() error");
            continue;
        }
        else
            printf("Server-accept() is OK...\n");
        printf("Server-new socket, new_fd is OK...\n");
        printf("Server: Got connection from %s\n", inet_ntoa(their_addr.sin_addr));

        // se conecta el cliente y el servidor crea un hijo con fork(), el hijo entra al if
        if(!fork()){
            // cierra el sockfd, no lo necesita.
            close(sockfd);
            // hijo se comunica con el cliente, envía mensaje (37 bytes) a través del new_fd
            if(send(new_fd, "This is a test string from server!\n", 37, 0) == -1)
                perror("Server-send() error lol!");
            // cierra el descriptor de archivo
            close(new_fd);
            // termina
            exit(0);
        }
        else
            // padre entra al else, regresa al while a aceptar la conexión de otro cliente
            printf("Server-send is OK...!\n");

        /* parent doesnt need this */
        // el padre no se va a comunicar con el cliente, cierra el new_fd
        close(new_fd);
        printf("Server-new socket, new_fd closed successfully...\n");
    }
    return 0;
}
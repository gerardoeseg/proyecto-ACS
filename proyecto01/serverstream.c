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

#define MAXDATASIZE 300
/* the port users will be connecting to */
#define MYPORT 3490
/* how many pending connections queue will hold */
#define BACKLOG 10

/*
 *  Variables de la base de datos
 */
char numcta[9]; //primary key
char apPat[20]; 
char apMat[20];
char nombres[40];
char comando[6];			//INSERT o SELECT
int i;
char *array[10];
char buffer[100];

/*
 *  Funciones de la base de datos
 */
int insert_cmd()
{
	//Cadena que recibirá el nombre del archivo
	char nuevoArchivo[14];
	//Variable del archivo 
	FILE *nuevo;
	
	//El número de cuenta ingresado pasa como el nombre del nuevo archivo
	sprintf(nuevoArchivo, "%s.txt", numcta);
	
	//Se abre el archivo en modo escritura
	nuevo = fopen(nuevoArchivo, "w"); //FILE * fopen (const char *filename, const char *opentype);
	
	//El contenido de la variable "buffer" se escribe en el archivo
	fputs (buffer, nuevo);
	
	fclose(nuevo); //Cierre del archivo
	printf("\nINSERT EXITOSO\n");
	fflush(stdin);
}

int select_cmd()
{
	
	char filename[14];
	//Función que pasa el nombre de archivo 
	sprintf(filename, "%s.txt", numcta); //filename=numcta+.txt
	
	//Variable del archivo 
	FILE *archivo;
	char caracter;
	
	//Se abre el archivo en modo lectura
	archivo = fopen(filename,"r");
	
	//Si el archivo no se encuentra
	if (archivo == NULL){
        printf("\nNo existen datos para el num. de cuenta. \n\n");
    }
	//Si existe, se lee el archivo encontrado
    else{
        printf("\nEl contenido del archivo es: \n\n");
		//Se imprime el contenido del archivo caracter por caracter
        while((caracter = fgetc(archivo)) != EOF){
			printf("%c",caracter);
	    }
    }
    fclose(archivo); //Cierre del archivo
}

void sigchld_handler(int s){
    while(wait(NULL) > 0);
}

int main(int argc, char *argv[ ]){
   

    /*  listen on sock_fd, new connection on new_fd 
     *  sockfd es el file descriptor 1, propio del servidor
     *  new_fd es el file descriptor 2, tiene la información propia del cliente
     */
    int sockfd, new_fd;

    char entrada[MAXDATASIZE];
    int numbytes;


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

    // sigaction elimina los procesos que puedan quedar
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
            if(send(new_fd, "mensaje del servidor\n", 37, 0) == -1)
                perror("Server-send() error lol!");

            close(new_fd);  // cierra el descriptor de archivo
            exit(0);    // termina
        }
        // padre entra al else, regresa al while a aceptar la conexión de otro cliente
        else      
            printf("Server-send is OK...!\n");
        
        /// mensaje recibido
        if((numbytes = recv(new_fd, entrada, MAXDATASIZE-1, 0)) == -1){
            perror("recv()");
            exit(1);
        }
        else
            printf("Client-The recv() is OK...\n");

        entrada[numbytes] = '\0';
        printf("Server-Received: %s", entrada);

        ////////////

        char *token = strtok(entrada, " ");
	    if(token != NULL){
    		while(token != NULL){
			    array[i++]=token; //Guardando cada token en un arreglo
	            token = strtok(NULL, " ");
    	    }
        }

        //depuracion
        for (int j=0; j<i;j++) 
        	printf("Token[%i]: %s\n",j, array[j]); 
    
        /****ASIGNAR LOS TOKENS A LAS VARIABLES GLOBALES***/
        strcpy(comando, array[0]);
        printf("\ncomando: %s\n", comando); //depuracion

        strcpy(numcta,array[1]);
        printf("num cuenta: %s\n", numcta); //depuracion

        if((i>2)&&(i<9)) //Para evitar segmentation fault
        {
	        strcpy(apPat,array[2]);
	        printf("ap pat: %s\n", apPat); //depuracion
            strcpy(apMat,array[3]);
	        printf("ap mat: %s\n", apMat); //depuracion
            strcpy(nombres,array[4]);
	 	    for (int j=5; j<i; j++)
	 	    {
    	 		strcat(nombres, " ");
	 		    strcat(nombres,array[j]);
	 	    }
	        printf("nombre(s): %s\n", nombres); //depuracion
	    }

        //Cadena con el nombre completo
	    sprintf(buffer, "%s %s %s", apPat, apMat, nombres); 

        // REDIRIGIENDO A FUNCION CORRESPONDIENTE SEGUN EL COMANDO
        if(strcmp(comando,"INSERT")==0) {
    	    insert_cmd();
        }
        else if(strcmp(comando,"SELECT")==0){
    	    select_cmd();
        }
        else{
    	    printf("Syntax error\n");
    	    //main();
        }

        /* parent doesnt need this */
        // el padre no se va a comunicar con el cliente, cierra el new_fd
        close(new_fd);

        i = 0;
        printf("numero: i=%d", i);

        printf("\n\nServer-new socket, new_fd closed successfully...\n");
        fflush(stdin);   
    }
    return 0;
}
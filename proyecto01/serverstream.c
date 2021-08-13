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
char numcta[8]; //primary key
char apPat[20]; 
char apMat[20];
char nombres[40];
char *array[10];
char comando[6];			//INSERT o SELECT
char buffer[100];
char contenido[80];
char mensajeFinal[MAXDATASIZE];
char nuevoArchivo[14];      //Cadena que recibirá el nombre del archivo
char filename[13];  //SELECT
FILE *nuevo;
int i;

/*
 *  Funciones de la base de datos
 */


int insert_cmd()
{
	//El número de cuenta ingresado pasa como el nombre del nuevo archivo
	//Se abre el archivo en modo escritura
	nuevo = fopen(filename, "w"); //FILE * fopen (const char *filename, const char *opentype);
	
	//El contenido de la variable "buffer" se escribe en el archivo
	fputs (buffer, nuevo);
    
	fclose(nuevo); //Cierre del archivo
	sprintf(mensajeFinal, "INSERT EXITOSO, el archivo %s fue creado\n", filename);
    //printf("%s", mensajeInsert);
}

int select_cmd()
{
	nuevo = fopen(filename,"r");  //Se abre el archivo en modo lectura

	//Si el archivo no se encuentra
	if (nuevo == NULL)
        sprintf(mensajeFinal, "No existen datos para el num. de cuenta %s\n", numcta);
	//Si existe, se lee el archivo encontrado
    else{
        //printf("El contenido del archivo %s es: \n", filename);
		//Se imprime el contenido del archivo caracter por caracter
        while (feof(nuevo) == 0){
            fgets(contenido, 80, nuevo);
        }
        sprintf(mensajeFinal, "El contenido del archivo %s es:\n %s", filename, contenido);
    }
    fclose(nuevo); //Cierre del archivo
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
        if(!fork())
        {
            // cierra el sockfd, no lo necesita.
            close(sockfd);
            // hijo se comunica con el cliente, envía mensaje (37 bytes) a través del new_fd
            if(send(new_fd, "Conectado al servidor exitosamente\n", 37, 0) == -1)
                perror("Server-send() error lol!");
               // padre entra al else, regresa al while a aceptar la conexión de otro cliente
            else      
                printf("Server-send is OK...!\n");
                
            printf("Conectado con el cliente exitosamente\n");

            ////PETICION DEL CLIENTE
            printf("\nEsperando el mensaje del cliente:\n");
            if((numbytes = recv(new_fd, entrada, MAXDATASIZE-1, 0)) == -1){
                perror("recv()");
                exit(1);
            }
            else{
                printf("Client-The recv() is OK...\n");
                entrada[numbytes] = '\0';
                printf("Server-Received: %s\n", entrada);
            }
            ////FIN DE PETICION

            ////ANALISIS DEL MENSAJE  
            char *token = strtok(entrada, " ");
            if(token != NULL){
        		while(token != NULL){
			        array[i++]=token; //Guardando cada token en un arreglo
	                token = strtok(NULL, " ");
    	        }
            }

            for (int j=0; j<i;j++) 
        	    printf("Token[%i]: %s\n",j, array[j]); 

            strcpy(comando, array[0]);
            printf("comando: %s\n", comando);

            strcpy(numcta,array[1]);
            printf("num cuenta: %s\n", numcta); 

            if((i>2)&&(i<9)) //Para evitar segmentation fault
            {
	            strcpy(apPat,array[2]);
	            printf("ap pat: %s\n", apPat); 
                strcpy(apMat,array[3]);
	            printf("ap mat: %s\n", apMat); 
                strcpy(nombres,array[4]);
	 	        for (int j=5; j<i; j++){
    	 		    strcat(nombres, " ");
	 		        strcat(nombres,array[j]);
	 	        }
	            printf("nombre(s): %s\n", nombres);
	        } 
	        sprintf(buffer, "%s %s %s", apPat, apMat, nombres); // cadena con el nombre completo
            numcta[strcspn(numcta, "\n")] = 0;  // encuentra un salto de linea \n y lo elimina
            sprintf(filename, "%s.txt", numcta);    // cadena con el nombre de archivo

            // REDIRIGIENDO A FUNCION CORRESPONDIENTE SEGUN EL COMANDO
            if(strcmp(comando,"INSERT")==0) {
        	    insert_cmd();
            }
            else if(strcmp(comando,"SELECT")==0){
        	    select_cmd();
            }
            else{
        	    printf("Syntax error\n");
    	        exit(1);
            }
            ////FIN DEL ANALISIS          
            
            ////ENVIO DE RESULTADO
            if(send(new_fd, mensajeFinal, 80, 0) == -1)
                perror("Server-send() error lol!");
            else      
                printf("Server-send is OK...!\nRESULTADO ENVIADO\n");
            ////FIN DEL ENVIO

            close(new_fd);  // cierra el descriptor de archivo
            printf("\n\nServer-new socket, new_fd closed successfully...\n");
            printf("\nEsperando nueva conexion...\n");
            exit(0);    // termina

        }
        /* parent doesnt need this */
        // el padre no se va a comunicar con el cliente, cierra el new_fd
        close(new_fd);
        printf("\n\nServer-new socket, new_fd closed successfully...\n");
        fflush(stdin);   
    }
    return 0;
}
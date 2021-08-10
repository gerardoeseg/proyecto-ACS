#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXDATASIZE 100

char entrada[256];	//Mensaje del cliente 
char numcta[9]; //primary key
char apPat[20]; 
char apMat[20];
char nombres[40];

char buffer[MAXDATASIZE];


int insert_cmd(){
	
	//Cadena que recibirá el nombre del archivo
	char nuevoArchivo[14];

	//Variables del archivo 
	FILE *nuevo;
	char letras;

	//El número de cuenta ingresado pasa como el nombre del nuevo archivo
	sprintf(nuevoArchivo, "%s.txt", numcta);

	//Se crea el archivo 
	nuevo = fopen(nuevoArchivo, "w"); //FILE * fopen (const char *filename, const char *opentype);
	
	fputs (buffer, nuevo);
	fclose(nuevo);

	printf("\nRegistro exitoso!\n");
	fflush(stdin);
}

///////////////////////////////////////////////////////////////
int select_cmd()
{
	char filename[14];
	//Función que pasa el nombre de archivo 
	sprintf(filename, "%s.txt", numcta); //filename=numcta+.txt

	FILE *archivo;
	char caracter;
	
	archivo = fopen(filename,"r");
	
	if (archivo == NULL){
        printf("\nNo existen datos para el num. de cuenta. \n\n");
    }
    else{
        printf("\nEl contenido del archivo de prueba es: \n\n");
        while((caracter = fgetc(archivo)) != EOF){
			printf("%c",caracter);
	    }
    }
    fclose(archivo);
}
////////////////////////////////////////////////////////////////
int main(){	
	
	char entcpy[256], 	//Copia del mensaje para preservar el original
	comando[6];			//INSERT o SELECT
	
	int i=0;
	char *array[10];

	scanf( "%[^\n]" , entrada); //Lee el mensaje del cliente

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
	//Cadena con los datos finales
	sprintf(buffer, "%s %s %s %s", numcta, apPat, apMat, nombres); 
    //printf("%s\n", buffer);

/*
 * REDIRIGIENDO A FUNCION CORRESPONDIENTE SEGUN EL COMANDO
 */
    if(strcmp(comando,"INSERT")==0 || strcmp(comando,"insert")==0) {
    	insert_cmd();
    }
    else if(strcmp(comando,"SELECT")==0 || strcmp(comando,"select")==0){
    	select_cmd();
    }
    else{
    	printf("Syntax error\n");
    	main();
    }
	return 0;
}
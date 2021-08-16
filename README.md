# Proyecto de Arquitecturas Cliente-Servidor

## Universidad Nacional Autónoma de México

## Facultad de Ingeniería
-------------- 
## Arquitecturas Cliente-Servidor
### Profesor: Ing. Carlos Alberto Román Zamitiz
### Semestre 2021-2
#### Fecha: 13 de agosto de 2021
-------------
## Proyecto final

### Integrantes:
- Salinas Gutiérrez Gerardo
- Valeriano Barrios Cristian
----------- 
## Contenido
Basados en los códigos clientstream.c y serverstream.c

Para el proyecto final se realizaron ciertas modificaciones.

Como están los códigos originales, el servidor es el que envía el mensaje de texto al cliente. Ahora se modificaron para que sea el cliente el que eníe el mensaje inicial, pero no debe ser el mensaje actual que está fijo dentro del código. Deben capturar del teclado el mensaje que el cliente debe enviar al servidor.

La intención del proyecto es que hagan un cliente servidor de BD básico, programado en lenguaje C. El cliente envía un comando al servidor, éste realiza la operación correspondiente y envía la respuesta al cliente. 

El comando que el servidor debe ejecutar se realizará sobre archivod. **Por facilidad** los comandos no tendrán la sintaxis completa de SQL. Los comandos que el cliente debe enviar, son:

**"INSERT numcta apellido_paterno apellido_materno nombre(s)"**, donde numcta es el número de cuenta del alumno a insertar en la BD, seguido del apellido paterno, el apellido materno y el nombre o nombres. Para simular las tablas de la BD, ustedes deben tener un archivo por alumno. Para facilitar la ejecución del comando, el nombre del archivo será el número de cuenta. Cuando el servidor reciba el comando, debe partirlo en 3 cadenas para obtener "INSERT" en la primera cadena, en la segunda cadena el número de cuenta y en la tercera cadena el nombre completo del alumno. Posteriormente debe crear en el archivo el nombre completo del alumno y enviar un mensaje de éxito o error como respuesta al cliente, para que éste lo despliegue en su pantalla. Por ejemplo: 

*para facilitar la manipulación del archivo, éste puede llevar o no la extensión .txt*

**SELECT numcta**, donde numcta es el número de cuenta del alumno a consultar. Para simular las tablas de la BD, ustedes deben tener un archivo por alumno. Para facilitar la ejecución del comando, el nombre del archivo será el número de cuenta. Cuando el servidor reciba el comando, debe partirlo en 2 cadenas para obtener "SELECT" en la primera cadena y el número de cuenta en la segunda cadena. Posteriormente debe abrir el archivo cuyo nombre corresponda al número de cuenta pasado en el comado "SELECT", leer el archivo y enviar el contenido como respuesta al cliente, para que éste lo despliegue en su pantalla. Por ejemplo:

*para facilitar la manipulación del archivo, éste puede llevar o no la extensión .txt*

Por facilidad, el contenido del archivo, consistirá solamente en el nombre del alumno que corresponde a ese número de cuenta y que previamente debieron insertar con el comando "INSERT". 
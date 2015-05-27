//gcc main.c -lpthread -o p
/*
	Nombre Archivo:
      		main.c
     	Realizado por:
      		Erika Jeniffer Harker
      		David Calle
     	Objetivo:
      		Ejecucion del programa: Un Generador de Trazas para Evaluar el Desempeño de un Servidor de Blogs
     	Fecha Última Modificación:
      		Mayo 27 de 2015
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <math.h>
#include <unistd.h>

typedef void (*sighandler_t)(int);
sem_t escritura;
int num_blogs;
int flag;
int * terminar;
typedef struct
{
	int id;
	int tipo;
	float grafo[3][3];
	int tiempo_sesion;
}Usuario;
struct Log
{
	struct tm hora;
	int idMaquina;
	int tipoUsuario;
	int idSesion;
	int blog;
	char accion[50];
};
struct Nodo
{
	struct Log log;
	struct Nodo *siguiente;
};
typedef struct 
{
	struct Nodo *cabeza;
}Lista;

struct Nodo* crearNodo(struct Log log){
	struct Nodo* nuevo = malloc(sizeof(struct Nodo));
	nuevo->log=log;
	return nuevo;
}
Lista* crearLista(){
	Lista *lista= malloc(sizeof(Lista));
	lista->cabeza=NULL;
	return lista;
}
typedef struct {
	Usuario tipo;
	int num_blogs;
	int cant_seciones;
	int id_maquina;
	Lista*  logs;
} Parametros;
/*
	Función: 
		asignar_grafo
	Parámetros de Entrada: 
		grafo: es el grafo de probabilidades de cambio de estado
		tipo: es el tipo de usuario
	Valor de Salida:
		retorna un entero el cual represenat el tiempo de creacion de sesion
	Descripción:
		Modifica el grafo del usuario para que quede con las probabilidades especificas 
		del tipo de usuario y asigna e tiempo requerido para el cambio de sesion segun el
		tipo de usuario
*/
int asignar_grafo(float grafo[3][3], int tipo){
	switch (tipo){
		case 1:
			grafo[0][0] = 0.21;
			grafo[0][1] = 0.79;
			grafo[0][2] = 0;
			grafo[1][0] = 0.76;
			grafo[1][1] = 0.16;
			grafo[1][2] = 0;
			grafo[2][0] = 0;
			grafo[2][1] = 0;
			grafo[2][2] = 0;
			return 8;
			break;
		case 2:
			grafo[0][0] = 0.50;
			grafo[0][1] = 0.01;
			grafo[0][2] = 0;
			grafo[1][0] = 0.32;
			grafo[1][1] = 0.21;
			grafo[1][2] = 0;
			grafo[2][0] = 0;
			grafo[2][1] = 0;
			grafo[2][2] = 0;
			return 2;
			break;
		case 3:
			grafo[0][0] = 0.14;
			grafo[0][1] = 0.63;
			grafo[0][2] = 0.21;
			grafo[1][0] = 0.31;
			grafo[1][1] = 0.30;
			grafo[1][2] = 0.33;
			grafo[2][0] = 0.1;
			grafo[2][1] = 0;
			grafo[2][2] = 0.76;
			return 3;
			break;
	}
	
}
/*
	Función: 
		agregarALista
	Parámetros de Entrada: 
		lista: es el apuntador a la cabeza de la lista
		nodo: es el nodo que se va a agregar
	Valor de Salida:
		no retorna, sin embargo cambia la lista agregandole un nodo mas
	Descripción:
		Agraga el nodo al final de la lista
*/
void agregarALista(Lista* lista, struct Nodo* nodo){
	
	struct Nodo* aux=lista->cabeza;
	if(aux==NULL){
	
		lista->cabeza=nodo;
	}else{
	
		while(aux->siguiente!=NULL){
			aux= aux->siguiente;
		}
		aux->siguiente=nodo;
		
	}
}
/*
	Función: 
		itoa
	Parámetros de Entrada: 
		i: numero atransformar
	Valor de Salida:
		retorna el entero transformado en cadena
	Descripción:
		transforma el numero entero en una cadena de caracteres
*/
char * itoa(int i)
{
  	static char buffer[12];
	if (snprintf(buffer, sizeof(buffer), "%d", i) < 0)
		return NULL;
  	return strdup(buffer);
}
/*
	Función: 
		imprimirListaArchivo
	Parámetros de Entrada: 
		lista: es el apuntador a la cabeza de la lista
	Valor de Salida:
		Genera un archivo donde se guarda la informacion del log
	Descripción:
		Crea un archivo unico de cada proceso, el cual tendra como nombre
		el id del proceso. En este archivo se guar en forma de texto: Hora en que se
		realizo el cambio de estado, el id de la maquina, id de la secion, tipo de usuario
		accion que se realizao y blog en que se realizo la accion
*/
void imprimirListaArchivo(Lista* lista){
	int pid;
	char* nombreArchivo;
	pid=getpid();
	nombreArchivo= itoa(pid);
	FILE *f = fopen(nombreArchivo, "w");
 	if (f == NULL){
    		printf("Error al abrir el archivo \n");
    		exit(1);
	}else{
		// recorro la lista e imprimo los datos uno a uno
		struct Nodo* aux=lista->cabeza;
		while(aux!=NULL){
			struct Log log= aux->log;
			struct tm tm = log.hora;
			//impresión de la hora
			fprintf(f,"%d:%d:%d ",   tm.tm_hour, tm.tm_min, tm.tm_sec);
			if(log.blog!=-1){
				
				//idMaquina , numeroDeSesion, Tipo de Lector, operacion
				fprintf(f, "M%i, %i, T%i %s %i\n", log.idMaquina, log.idSesion, log.tipoUsuario, log.accion, log.blog);
			}else{
				//idMaquina , numeroDeSesion, Tipo de Lector, operacion
				fprintf(f, "M%i, %i, T%i, %s \n", log.idMaquina, log.idSesion, log.tipoUsuario, log.accion);
			
			}
			
			aux= aux->siguiente;
		}
	}
	fclose(f);
	free(nombreArchivo);

}
/*
	Función: 
		imprimir
	Parámetros de Entrada: 
		lista: es el apuntador a la cabeza de la lista
	Valor de Salida:
		imprime por pantalla el id de la maquina y el id de la sesion
	Descripción:
		Recorre toda la lista e imprime por pantalla el id de la maquina y el id de la sesion
*/
void imprimir(Lista* lista){
	struct Nodo* aux=lista->cabeza;
	while(aux!=NULL){
		printf("Id maquina: %i ID de la sesion: %i \n", aux->log.idMaquina,aux->log.idSesion);
		aux= aux->siguiente;
	}

}
/*
	Función: 
		eliminarLista
	Parámetros de Entrada: 
		lista: es el apuntador a la cabeza de la lista
	Valor de Salida:
		elimina la lista
	Descripción:
		elimina la lista, liberando la memoria que ocupaba cada nodo y destruyendo
		el apuntador de la cabeza en cual indica el innicio de la lista
*/
void eliminarLista(Lista *lista){
	struct Nodo* aux= lista->cabeza;
	while(aux!=NULL){
		struct Nodo* actual=aux;
		aux=aux->siguiente;
		free(actual);
	}
	free(lista);
}
/*
	Función: 
		meter_log
	Parámetros de Entrada: 
		lista: es el apuntador a la cabeza de la lista
		log: estructura que contiene, hora, id de la Maquina, tipo de Usuario, id de 
		     la Sesion,el numero del blog y la accion o estado en que se realizo el blog
	Valor de Salida:
		agrega el log a la lista
	Descripción:
		agrega el log a la lista por medio de la funcion "agregarALista"
*/
void meter_log (Lista * lista, struct Log log){
	struct Nodo * nodo =  crearNodo(log);	
	sem_wait(&escritura);
	agregarALista(lista, nodo);
	sem_post(&escritura);
}
/*
	Función: 
		accion_hilo
	Parámetros de Entrada: 
		p: son los parametro utilizados en esta funcion
			tipo: es el tipo de usuario que esta iniciando las sesiones
			num_blogs: cantidad maxima de blogs
			cant_seciones: cuantas sesiones se han creado en esa maquina
			id_maquina: id de la maquina que esta creando las sesiones
			logs: apuntador a la cabeza de la lista
	Valor de Salida:
		agrega el log a la lista
	Descripción:
		Dinamiza la sesion del usuario, haciendo que este cambie de estado segun el
		grafo asociado al tipo de usuario de la sesion, el cual contiene las probabilidades
		de cambio de estado. Al final agrega esta informacion a la lista de log
*/
void* accion_hilo (Parametros *p){
	struct Log log;
	time_t t;
	struct tm tm;
	int num_secion, Bactual, estado, testado, aux;
	float R;	
	num_secion = (*p).cant_seciones;
	Bactual = rand() % (*p).num_blogs;
	estado = 0;
	testado = 1;

	while (estado!=3&&(*terminar)==0){
		t = time(NULL);
		tm = *localtime(&t);
		log.hora = tm;
		log.idMaquina = (*p).id_maquina;
		log.tipoUsuario = (*p).tipo.tipo;
		log.idSesion = num_secion;
		log.blog = Bactual;
		switch (estado){
			case 0:
				strcpy(log.accion, "Read blog");
				break;		
			case 1:
				strcpy(log.accion, "Read same  blog");
				break;
			case 2:
				strcpy(log.accion, "Comment in blog");
				break;
		}
		
		sleep(testado);
		R = (float)rand()/(float)RAND_MAX;
		if (R<=(*p).tipo.grafo[estado][0]){
			do{
				aux = rand() % (*p).num_blogs;
			}while (aux != Bactual);
			Bactual = aux;
			estado = 0;
		}else if (R<=((*p).tipo.grafo[estado][1]+(*p).tipo.grafo[estado][0])){
			estado = 1;	
		}else if (R<=((*p).tipo.grafo[estado][2]+(*p).tipo.grafo[estado][1]+(*p).tipo.grafo[estado][0])){
			estado = 2;
		}else{
			estado = 3;
		}
	meter_log(p->logs, log);
	}
	t = time(NULL);
	tm = *localtime(&t);
	log.hora = tm;
	log.idMaquina = (*p).id_maquina;
	log.tipoUsuario = (*p).tipo.tipo;
	log.idSesion = num_secion;
	log.blog = -1;
	strcpy(log.accion, "terminar");
	meter_log(p->logs, log);
}
/*
	Función: 
		signalHandler1
	Parámetros de Entrada: 
		Ninguna
	Valor de Salida:
		Cambio de la variable terminar a 1
	Descripción:
		fucion activada por la señal SIGUSR1 para terminar la creacion de sesiones
		o hilos de cada proceso y evitar que los hilos sigan modificando la lista
*/
sighandler_t signalHandler1 (void){
	*terminar = 1;
	sem_wait(&escritura);
}
/*
	Función: 
		signalHandler
	Parámetros de Entrada: 
		Ninguna
	Valor de Salida:
		Cambio de la variable terminar a 1
	Descripción:
		fucion activada por la señal SIGALRM para continuar con el proceso
		padre y terminar la simulacion
*/
sighandler_t signalHandler (void){	
	*terminar = 1;
}

/*
	Función: 
		iniciar_computador
	Parámetros de Entrada: 
		tipo_usuario: Es el tipo de usuario que esta creando las sesiones
		id_maquina: id de la maquina que crea las sesiones
		cant_seciones: cantidad de sesiones que ha creado esa maquina
		lista: apuntador a la cabeza de la lista de logs		
	Valor de Salida:
		Crea un nuevo hilo que representa una sesion
	Descripción:
		inicializa los parametros necesarios para crear una nueva sesion, crea un
		nuevo hilo y espera cierto tiempo segun el tipo de usuario
*/
int iniciar_computador(Usuario tipo_usuario, int id_maquina, int cant_seciones, Lista * lista){	
	pthread_t id;
	Parametros p;
	int tiempo = tipo_usuario.tiempo_sesion;
	
	p.tipo = tipo_usuario;
	p.num_blogs = num_blogs;
	p.cant_seciones = cant_seciones;
	p.logs = lista;
	p.id_maquina= id_maquina;
	pthread_create(&id, NULL,(void *) accion_hilo, (void*)&p);
	sleep (tiempo);
}
/*
	Función: 
		cargarUsuarios
	Parámetros de Entrada: 
		usuarios: arreglo que guardara los tipos de usuarios
		cantidad: cantidad de maquinas ingresadas
	Valor de Salida:
		tipo de usuario para cada maquina
	Descripción:
		le pregunta al administrador del programa que tipo de usuario asigna a 
		cada maquina
*/
void cargarUsuarios(int *usuarios, int cantidad){
	int i, temporal;
	for (i = 0; i < cantidad; ++i){
	 	printf("Diga el tipo de usuario del computador (1,2,3) %i\n",(i+1) );
		
	 	scanf("%i", &temporal);
		if(temporal!=1&&temporal!=2&&temporal!=3){
			printf("El tipo de usuario %i no existe, vuelva a correr el programa", temporal);
			exit(0);
		}
	 	usuarios[i]=temporal;
	}

}
/*
	Función: 
		recopilarLogs
	Parámetros de Entrada: 
		pid: son los parametro utilizados en esta funcion
		cantidad_computadores: son los parametro utilizados en esta funcion
	Valor de Salida:
		archivo final con toda la traza de los procesos
	Descripción:
		Recopila todos los archivos que los procesos generaron en un solo archivo
		despues elimina los archivos generados por cada proceso para dejar un solo archivo
		principal
*/
void recopilarLogs(int *pid, int cantidad_computadores){
	int i;	
	FILE *fr, *salida;
	char  *nombreArchivo, linea[256];
	salida = fopen ("Analisis.log", "w");
	if (salida == NULL){
    		printf("Error al abrir el archivo \n");
    		exit(1);
	}
	printf("*************** Log Completo ******************\n");
	for(i=0; i< cantidad_computadores;i++){
		nombreArchivo= itoa(pid[i]);
		fr = fopen (nombreArchivo, "r");
		if( fr == NULL ){
      			perror("Error al abrir el archivo \n");
      			exit(EXIT_FAILURE);
   		}
		
		
		
    		while (fgets(linea, sizeof(linea), fr)) {
			fprintf(salida,"%s", linea);        		
			printf("%s", linea); 
    		}
	
		fclose(fr);
		//falta validar status
		remove(nombreArchivo);
		free(nombreArchivo);
		 
	}
	fclose(salida);
}
/*
	Función: 
		main
	Parámetros de Entrada: 
		argc: cantidad de parametros ingresados inicialmente
		argv: arrego de parametros necesarios para el funcionamiento del programa
		      numeroDeComputadores, numeroBlogs, runtime
	Valor de Salida:
		
	Descripción:
		Funcion principal
		realiza la creacion de procesos que representa cada maquina y se encarga de detener
		los proceso cuando el tiempo de simulacion termina
*/
int main(int argc, char *argv[]){
	
	int *pid,cantidad_computadores,i, status, blogs, runtime, tipo, *tipos_blogs, cant_seciones;
	Usuario u;

	Lista * lista;
	if(argc!=4){
		printf("cantidad de argumentos, no válida, debería ser  numeroDeComputadores, numeroBlogs, runtime\n");
		exit(0);
	}
	
	terminar = malloc (sizeof (int));
	*terminar = 0;	
	cantidad_computadores = atoi(argv[1]);
	num_blogs = atoi(argv[2]);
	runtime = atoi(argv[3]);
	for(i=1;i<4;i++){
		if(atoi(argv[i])<=0){
			printf("Ningún argumento debe ser negativo o letra\n");
			exit(0);
		}
	}
	if(cantidad_computadores>10){
		printf("No pueden haber más de 10 computadores\n");
		exit(0);
	}

	pid = malloc(sizeof(int) * cantidad_computadores);
	tipos_blogs = malloc(sizeof(int) * cantidad_computadores);
	cargarUsuarios(tipos_blogs, cantidad_computadores);

	for (i = 0; i < cantidad_computadores; ++i){
		if((pid[i]=fork()) == 0){
			lista= crearLista();	
			u.id = pid[i];
			u.tipo = tipos_blogs[i];
			u.tiempo_sesion = asignar_grafo(u.grafo, u.tipo);
			
			flag = 0;	
			cant_seciones = 0;	
			sem_init(&escritura, 0, 1);
			
			signal (SIGUSR1, (sighandler_t)signalHandler1);
			
			while (*terminar==0){
				iniciar_computador(u,i,cant_seciones,lista);
				cant_seciones++;			
			}
			imprimirListaArchivo(lista);
			exit(0);
		}	
	}
	signal (SIGALRM, (sighandler_t)signalHandler);
	alarm (runtime);
	pause();

	printf("-------------Timeout %i segundos---------- \n", runtime);
	for (i=0; i<cantidad_computadores; i++)
		kill (pid[i], SIGUSR1);


	for (i = 0; i < cantidad_computadores; ++i){	
		wait(&status);
	}
	//recopilar datos y guardar en un archivo
	recopilarLogs(pid,cantidad_computadores);
	free(pid);	
	free(tipos_blogs);
	free(terminar);
}
	

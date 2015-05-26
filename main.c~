//gcc main.c -lpthread -o p

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
char *itoa(int i)
{
  	static char buffer[12];
	if (snprintf(buffer, sizeof(buffer), "%d", i) < 0)
		return NULL;
  	return strdup(buffer);
}
void imprimirListaArchivo(Lista* lista){
	int pid;
	char* nombreArchivo;
	pid=getpid();
	nombreArchivo= itoa(pid);
	FILE *f = fopen(nombreArchivo, "w");
 	if (f == NULL){
    		printf("Error al abrir el archivo \n");
    		// No se si matar todo en caso de que falle o igual seguir
    		exit(1);
	}else{
		// recorro la lista e imprimo los datos uno a uno
		struct Nodo* aux=lista->cabeza;
		while(aux!=NULL){
			struct Log log= aux->log;
			struct tm tm = log.hora;
			//impresión de la hora

			if(log.blog!=-1){
				fprintf(f,"%d:%d:%d ",   tm.tm_hour, tm.tm_min, tm.tm_sec);
			//idMaquina , numeroDeSesion, Tipo de Lector, operacion
				fprintf(f, "M%i, %i, T%i %s %i\n", log.idMaquina, log.idSesion, log.tipoUsuario, log.accion, log.blog);
			}else{
				fprintf(f,"%d-%d-%d %d:%d:%d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			//idMaquina , numeroDeSesion, Tipo de Lector, operacion
				fprintf(f, "M%i, %i, T%i, %s \n", log.idMaquina, log.idSesion, log.tipoUsuario, log.accion);
			
			}
			
			aux= aux->siguiente;
		}
	}
	fclose(f);
	free(nombreArchivo);

}
void imprimir(Lista* lista){
	struct Nodo* aux=lista->cabeza;
	while(aux!=NULL){
		printf("Id maquina: %i \n", aux->log.idMaquina);
		aux= aux->siguiente;
	}

}
void eliminarLista(Lista *lista){
	struct Nodo* aux= lista->cabeza;
	while(aux!=NULL){
		struct Nodo* actual=aux;
		aux=aux->siguiente;
		free(actual);
	}
	free(lista);
}
void meter_log (Lista * lista, struct Log log){
	struct Nodo * nodo =  crearNodo(log);	
	sem_wait(&escritura);
	agregarALista(lista, nodo);
	sem_post(&escritura);
}

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

sighandler_t signalHandler1 (void){
	*terminar=1;
	sem_wait(&escritura);
}
sighandler_t signalHandler (void){	
	*terminar = 1;
}


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
	
	 	
	cant_seciones++;
   
    
}

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
			//u.tiempo_sesion;
			
			flag = 0;	
			cant_seciones = 0;	
			sem_init(&escritura, 0, 1);
			
			signal (SIGUSR1, (sighandler_t)signalHandler1);
			
			while (*terminar==0){
				iniciar_computador(u,i,cant_seciones,lista);			
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
	

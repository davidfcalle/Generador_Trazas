//gcc main.c -lpthread -o p

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
typedef void (*sighandler_t)(int);
sem_t escritura;
struct Usuario
{
	int id;
	double grafo[3][3];
	int tiempo_sesion;
};
struct Log
{
	struct tm hora;
	int idMaquina;
	int tipoUsuario;
	int idSesion;
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
	int tipo;
	int num_blogs;
	int cant_seciones;
	int id_maquina;
	Lista*  logs;
} Parametros;
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
	int num_secion, Bactual;
	num_secion = (*p).cant_seciones;
	Bactual = rand() % (*p).num_blogs;

//funcion genrar log
		
	struct Log log;
	log.idMaquina = p->id_maquina;
	log.tipoUsuario = 1;
	
	meter_log(p->logs, log);
	//pthread_exit(NULL);
}


sighandler_t signalHandler (void)
{
	
}
int iniciar_computador(int tipo_usuario, int id_maquina)
{	
	int cant_seciones;
	int num_blogs;
	Lista * lista= crearLista();
	
	cant_seciones = 0;	
	sem_init(&escritura, 0, 1);

	pthread_t id;
    int index;
	int tiempo=1;
	signal (SIGALRM, (sighandler_t)signalHandler);
	int cont =0;

	while (cont<5){
		printf("va a entrar\n");
		Parametros p;
		p.tipo = tipo_usuario;
		p.num_blogs = num_blogs;
		p.cant_seciones = cant_seciones;
		p.logs = lista;
		p.id_maquina= id_maquina;
		alarm (tiempo);
		pause ();
		pthread_create(&id, NULL,(void *) accion_hilo, (void*)&p);
		 	
		cant_seciones++;
		cont++;	
	} 
	printf("Fin computador\n");
	imprimir(lista);
	//buscar como espearar todos los hilos
	// escribir con nombre pid 
   
    
}

void cargarUsuarios(int *usuarios, int cantidad){
	int i, temporal;
	for (i = 0; i < cantidad; ++i){
	 	printf("Diga el tipo de usuario del computador %i\n",(i+1) );
	 	scanf("%i", &temporal);
	 	usuarios[i]=temporal;
	}

}

int main(int argc, char *argv[]){
	int *pid,cantidad_computadores,i, status, blogs, runtime, tipo, *tipos_blogs;

	
	if(argc!=4){
		printf("cantidad de argumentos, no válida, debería ser  numeroDeComputadores, numeroBlogs, runtime\n");
		exit(0);
	}

	
	cantidad_computadores = atoi(argv[1]);
	runtime = atoi(argv[2]);
	blogs = atoi(argv[3]);
	pid = malloc(sizeof(int) * cantidad_computadores);
	tipos_blogs = malloc(sizeof(int) * cantidad_computadores);
	cargarUsuarios(tipos_blogs, cantidad_computadores);

	for (i = 0; i < cantidad_computadores; ++i){
		if((pid[i]=fork()) == 0){
			printf("se creo un computador\n");	
			iniciar_computador(tipos_blogs[i],i);
			exit(0);
		}	
	}
	for (i = 0; i < cantidad_computadores; ++i){	
		wait(&status);
	}
	//recopilar datos y guardar en un archivo
	free(pid);	
	free(tipos_blogs);
}
	
/* segundo actual 
	struct timeval start,end;
	gettimeofday(&end, NULL);
	gettimeofday(&start, NULL);
		printf("%ld\n", ((end.tv_sec * 1000000 + end.tv_usec)
		  - (start.tv_sec * 1000000 + start.tv_usec))); 
	*/
	
	/* Tiempo
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	*/

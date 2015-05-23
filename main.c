#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
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
struct Lista
{
	struct Nodo *cabeza;
};

struct Nodo* crearNodo(struct Log log){
	struct Nodo* nuevo = malloc(sizeof(struct Nodo));
	nuevo->log=log;
	return nuevo;
}
struct Lista* crearLista(){
	struct Lista *lista= malloc(sizeof(struct Lista));
	lista->cabeza=NULL;
	return lista;
}

void agregarALista(struct Lista* lista, struct Nodo* nodo){
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
void eliminarLista(struct Lista *lista){
	struct Nodo* aux= lista->cabeza;
	while(aux!=NULL){
		struct Nodo* actual=aux;
		aux=aux->siguiente;
		free(actual);
	}
	free(lista);
}
/*
* ¿cómo ingresa el tipo de usuario de cada máquina?
* ¿Bloggers?
* se puede ahacer con archivos con nombre pid?
* toca implementar Lista?
* que pasaW con las sesiones y máquinas?
*/

void iniciar_computador(int tipo, int idMaquina){
	printf(" soy un PC de tipo %i\n",tipo);
	pthread_t hilo;
	struct Lista* lista = crearLista();
	// alarma para el tiempo del usuario, simular usuario y pasarlo como  parametro, mientras no haya pasada el runtime
	//recopilar datos y guardar en archivo
	eliminarLista(lista);

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
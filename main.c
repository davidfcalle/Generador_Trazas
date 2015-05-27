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
#include "generador.h"




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
			lista=(Lista*) (intptr_t)crearLista();	
			u.id = pid[i];
			u.tipo = tipos_blogs[i];
			u.tiempo_sesion = asignar_grafo(u.grafo, u.tipo);
			flag = 0;	
			cant_seciones = 0;	
			sem_init(&escritura, 0, 1);
			signal (SIGUSR1, (sighandler_t)signalHandler1);
			while ((*terminar)==0){
				iniciar_computador(u,i,cant_seciones,lista);
				cant_seciones++;			
			}
			imprimirListaArchivo(lista);
			eliminarLista(lista);
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
	

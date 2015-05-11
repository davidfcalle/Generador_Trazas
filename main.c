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
/*
* ¿cómo ingresa el tipo de usuario de cada máquina?
* ¿Bloggers?
* se puede ahacer con archivos con nombre pid?
* toca implementar Lista?
* que fuck con las sesiones y máquinas?
*/

void iniciar_computador(int tipo, int idMaquina){
	printf(" soy un PC de tipo %i\n",tipo);
	pthread_t hilo;

}

int main(int argc, char *argv[]){
	int *pid,cantidad_computadores,i, status, blogs, runtime, tipo;

	
	if(argc!=4){
		printf("cantidad de argumentos, no válida, debería ser  numeroDeComputadores, numeroBlogs, runtime\n");
		exit(0);
	}

	
	cantidad_computadores=atoi(argv[1]);
	runtime=atoi(argv[2]);
	blogs=atoi(argv[3]);
	//cargarUsuarios();
	pid = malloc(sizeof(int )* cantidad_computadores);
	for (i = 0; i < cantidad_computadores; ++i)
	{
		if((pid[i]=fork())==0){
			
				
		iniciar_computador(1,i);
			exit(0);
		}
		
	}
	for (i = 0; i < cantidad_computadores; ++i)
	{	
		wait(&status);
	}
	
	printf("Soy el padre y termino\n");
	 
	free(pid);
		
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
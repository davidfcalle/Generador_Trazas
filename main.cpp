#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>
using namespace std;
struct Log
{
	tm hora;
	int idMaquina;
	int tipoUsuario;
	int idSesion;
	char accion[50];
};
struct Usuario
{
	int id;
	double grafo[3][3];
	int tiempo_sesion;
	std::vector<Log> traza;

};

/*
* ¿cómo ingresa el tipo de usuario de cada máquina?
* ¿Bloggers?
* se puede ahacer con archivos con nombre pid?
* toca implementar Lista?
* que fuck con las sesiones y máquinas?
*/

void iniciar_computador(int tipo, int idMaquina){
	printf(" soy un PC de tipo %i\n",tipo );
	pthread_t hilo;
	Usuario usuario;
	std::vector<int> v;
	v.push_back(1);
	v.push_back(1);
	v.push_back(3);
	for (int i = 0; i < v.size(); ++i)
	{
		cout<<"con cout "<<getpid()<<" "<<v[i]<<endl;
	}
	//creo los hilos y los pongo andar
	//simular

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
	pid = new int[cantidad_computadores];
	for (i = 0; i < cantidad_computadores; ++i)
	{
		if((pid[i]=fork())==0){
			
				
		iniciar_computador(1,i);
		exit(0);
		}
		
	}
	for (i = 0; i < cantidad_computadores; ++i)
	{	
		wait(NULL);
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
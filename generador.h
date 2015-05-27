  /*
     Nombre Archivo:
      	generador.h
     Realizado por:
      	Erika Jeniffer Harker
      	David Calle
     Objetivo:
      	 Prototipo de las funciones que manipulan el log.
     Fecha Última Modificación:
      	Mayo 27 de 2015
    */
#ifndef GENERADOR_H
#define GENERADOR_H
sem_t escritura;
int num_blogs;
int flag;
int * terminar;

typedef void (*sighandler_t)(int);

typedef struct{
  int id;
  int tipo;
  float grafo[3][3];
  int tiempo_sesion;
}Usuario;

struct Log{
  struct tm hora;
  int idMaquina;
  int tipoUsuario;
  int idSesion;
  int blog;
  char accion[50];
};

struct Nodo{
  struct Log log;
  struct Nodo *siguiente;
};

typedef struct {
  struct Nodo *cabeza;
}Lista;




typedef struct {
  Usuario tipo;
  int num_blogs;
  int cant_seciones;
  int id_maquina;
  Lista*  logs;
} Parametros;

int asignar_grafo(float grafo[3][3], int tipo);
void agregarALista(Lista* lista, struct Nodo* nodo);
char * itoa(int i);
void imprimirListaArchivo(Lista* lista);
void imprimir(Lista* lista);
void eliminarLista(Lista *lista);
void meter_log (Lista * lista, struct Log log);
void* accion_hilo (Parametros *p);
sighandler_t signalHandler1 (void);
sighandler_t signalHandler (void);
int iniciar_computador(Usuario tipo_usuario, int id_maquina, int cant_seciones, Lista * lista);
void cargarUsuarios(int *usuarios, int cantidad);
void recopilarLogs(int *pid, int cantidad_computadores);
  
#endif
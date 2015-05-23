
#include <iostream>

using namespace std;


/*
	Función que me imprime el menú y me retorna la sección del usuario
*/

struct Cancion
{
	string titulo;
	string autor;
	float duracion;
	int reproducciones;

};

struct listaDeReproduccion
{
	string nombre;
	int cantidadDePistas;
	int cantidadPistasAgregadas;
	int actual;
	Cancion canciones[200];

};

struct Genero
{
	string nombre;
	Cancion canciones[15];
	int cantidadCanciones;
};

struct Biblioteca
{
	Cancion canciones [300];
	int cantidadCanciones;
	listaDeReproduccion cancionesLista [100];
	int cantidadListas;
	Genero generos [100];
	int cantidadGeneros;
	int posicionListaActual;


};

Cancion crearCancion(string titulo,string autor, float duracion){
	Cancion cancion;
	cancion.titulo=titulo;
	cancion.autor=autor;
	cancion.duracion=duracion;
	cancion.reproducciones=0;
	return cancion;
}

Biblioteca agregarCancion(Biblioteca biblioteca,Cancion cancion){
	biblioteca.canciones[biblioteca.cantidadCanciones] = cancion;
	biblioteca.cantidadCanciones=biblioteca.cantidadCanciones+1;
	return biblioteca;
}

Biblioteca llenar(){
    Biblioteca biblioteca;
    Cancion c1= crearCancion("Kevin", "Kevil Roldán",30);
    biblioteca=agregarCancion(biblioteca, c1);
    return biblioteca;

}



int existeCancion(Biblioteca biblioteca,string nombre){

	for (int i = 0; i < biblioteca.cantidadCanciones; ++i)
	{
		if (biblioteca.canciones[i].titulo==nombre){
			return i;
		}		
	}
	return -1;
}



Genero agregarCancionAGenero(Genero lista, Cancion cancion){
// no importa por ahora
}

listaDeReproduccion agregarCancionALista(Cancion cancion, listaDeReproduccion lista){
    int ultima=lista.cantidadPistasAgregadas;
    lista.canciones[ultima] = cancion;
    lista.cantidadPistasAgregadas= lista.cantidadPistasAgregadas+1;
    lista.cantidadDePistas= lista.cantidadDePistas+1;
    return lista;

}
Cancion crearCancionUsuario(){
    string nombre;
    string autor;
    float duracion;
    cout<<"creacion de cancion"<<endl;
    cout<<"Ingrese el nombre"<<endl;
    cin>>nombre;
    cout<<"Ingrese el autor"<<endl;
    cin>>autor;
    cout<<"Ingrese el duracion"<<endl;
    cin>>duracion;
    Cancion c;
    c= crearCancion(nombre, autor, duracion);
    c.reproducciones=0;
    return c;
}

listaDeReproduccion crearListaDeReproduccion(string nombre, Biblioteca biblioteca){
	listaDeReproduccion listica;
	listica.nombre= nombre;
	listica.cantidadDePistas=0;
	listica.cantidadPistasAgregadas=0;
	int seleccion;
	do{
        cout<<"¿Desea agregar una cancion a esta lista de reproduccion? 1. Si 2. No"<<endl;
        cin>>seleccion;
        if(seleccion!=2){
        	cout<<"Ingrese el nombre de la canción"<<endl;
        	string nombreC;
        	cin>>nombreC;
        	int posicionCancion=existeCancion(biblioteca, nombreC);
        	if (posicionCancion!=-1)
        	{
        		listica= agregarCancionALista(biblioteca.canciones[posicionCancion], listica);
        	}
            //Cancion cancion=crearCancionUsuario();// buscar canciòn
            //listica=agregarCancionALista(cancion,listica);

        }
	}while(seleccion!=2);

	return listica;

}


void reproducirCanciones(listaDeReproduccion lista){

}



char menu(){
	cout<<"Seleccione una opción"<<endl;
	cout<<"1. Seleccinoar una pista y agregarla a la cola de reproducción"<<endl;
	cout<<"2. Crear Lista de Reproduccion "<<endl;
	cout<<"3. Seleccionar Lista de Reproducción "<<endl;
	cout<<"4. Terminar Pista"<<endl;
	cout<<"5. Agregar canciones a Género"<<endl;
	cout<<"6. Top 10"<<endl;
	cout<<"7. Generos más escuchados"<<endl;
	cout<<"8. Buscar Pista por género"<<endl;



	cout<<"Seleccione una opción"<<endl;
	cout<<"Seleccione una opción"<<endl;
	char seleccion;
	cin>>seleccion;
	return seleccion;
}

bool esPrimo(int numero){
    for (int i = 2; i < numero; ++i)
    {
    	if(numero%i==0){
    		return false;
    	}
    }
    return true;
}


int main()
{
    Biblioteca biblioteca;
	biblioteca.cantidadCanciones = 0;
	biblioteca.cantidadListas = 0;
	biblioteca.cantidadGeneros = 0;
	biblioteca.posicionListaActual = -1;


	char seleccion= menu();
	while(seleccion!='s'){

		switch(seleccion){
			case '1':{
                cout<<"Llamo a agregarCancion"<<endl;
                break;
			}
            case '2':{
			    cout<<"ingrese el nombre de la lista"<<endl;
			    string nombre1;
			    cin>>nombre1;
                listaDeReproduccion milista=crearListaDeReproduccion(nombre1);

            break;
			 }
			case 'a':
				Cancion c= crearCancionUsuario();
				biblioteca=agregarCancion(biblioteca,c);
				cout<<"lo guarda"<<endl;
				cout<<"Ahora la bilbil tiene "<<biblioteca.cantidadCanciones<<endl;
			break;




		}
		seleccion= menu();

	}
	return 0;
}

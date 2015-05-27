all: Generador clean

Generador: Generador.o Main.o
	gcc generador.o main.o -lpthread -o ejecutable
Generador.o: generador.o generador.h
	gcc -c -lpthread generador.c
Main.o: main.o generador.o
	gcc -c -lpthread main.c
clean: 
	rm -rf */*.o
	rm -rf *.o
/***********************************************************************/
/* Nombre: arbol.h                                                               */
/* Contiene las declaraciones de todas las funciones y el tipo de estructura     */
/* empleado en el árbol del programa.                                            */
/*                                                                               */
/*                                                                               */
/***********************************************************************/
#ifndef _ARBOL_H
#define _ARBOL_H


#define FALSE 0
#define TRUE 1

#define ELIMINADO 1

#define PERMS 0600
#define DIR_CLAVE "/bin"
#define MUTEX "/semMUTEX"
#define S1 "/semS1"
#define SCLIENTES "/semSCLIENTES"
/*
 *  Control clientes activos
 */
#define TIEMPO_RESPUESTA 5
#define TIEMPO_ESPERA 20

/*
 *  Errores
 */
#define NO_ERROR 0
#define ERROR_NO_BAJA 1
#define ENCONTRADO 2
#define NO_ENCONTRADO 3
/*
 *  Opciones generales
 */
#define ALTA 0
#define INSERTAR 1
#define BORRAR 2
#define BUSCAR 3
#define TERMINAR 4

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/errno.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>



typedef struct NodoArbol
{
    int dato;
    struct NodoArbol *der;
    struct NodoArbol *izq;
} Nodo ;

struct mensaje_peticion
{
    long tipo;
    int codigo_operacion;
};

struct mensaje_respuesta
{
    long tipo;
    int codigo_error;
};











Nodo* Cargar(Nodo* raiz);
Nodo* InsertarElemento(Nodo* raiz,int d);
Nodo* Visualizar(Nodo* raiz);
Nodo* Buscar(Nodo* raiz,int d);
Nodo* Borrar(Nodo* raiz,int d);
Nodo* BuscarPadre(Nodo* raiz,int d);
FILE* Guardar(Nodo* raiz,FILE *fp1);
void GuardarFichero(Nodo* raiz);
int Contar(Nodo* raiz);
Nodo* CargarOrdenado(Nodo* raiz);
Nodo* InsertarOrdenado(int inicio,int final,int *datos,Nodo* raiz);


#endif

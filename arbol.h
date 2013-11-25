/***********************************************************************/
/* Nombre: arbol.h                                                               */
/* Contiene las declaraciones de todas las funciones y el tipo de estructura     */
/* empleado en el árbol del programa.                                            */
/*                                                                               */
/*                                                                               */
/***********************************************************************/
#include <semaphore.h>
#define FALSE 0
#define TRUE 1


/*************************************VARIABLES GLOBALES Y ESTRUCTURAS ************************************/
int esta_proceso_terminado;
int Q1,Q2,memo,max_clientes;

int *vector_clientes;
int num_clientes;

sem_t *s1, *mutex; // punteros para identificador de los semaforos


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




Nodo *raizarbol;













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
void cerrar_programa(int sig);
int baja(int pid,int *vector_clientes,int max_clientes);

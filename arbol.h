/*
 * Nombre: arbol.h
 * Contiene las declaraciones de todas las funciones y el tipo de estructura
 * empleado en el árbol del programa.
 *
 *
 */
#ifndef _ARBOL_H
#define _ARBOL_H


#define FALSE 0
#define TRUE 1

// Clientes máximos a la espera = clientes max * RANGO_CLIENTES_ESPERANDO
#define RANGO_CLIENTES_ESPERANDO 10

#define NO_ELIMINADO 0
#define ELIMINADO 1
/*
 *  Defines: Control memoria compartida
 */
#define PERMS 0600
#define DIR_CLAVE "/bin"
#define MUTEX "/semMUTEX"
#define S1 "/semS1"
#define SCLIENTES "/semSCLIENTES"
/*
 *  Defines: Control clientes activos
 */
 // TIEMPO_RESPUESTA es el tiempo máximo que se le permite al cliente tardar en responder
#define TIEMPO_RESPUESTA 5
// TIMPO_ESPERA es el tiempo entre comprobaciones de actividad de los clientes
#define TIEMPO_ESPERA 20
// TIEMPO_PANTALLA es el tiempo que se muestra en pantalla la respuesta del servidor a una petición del cliente (en el cliente)
#define TIEMPO_PANTALLA 2

/*
 *  Defines: Errores
 */
#define NO_ERROR 0
#define ERROR_NO_BAJA 1
#define ENCONTRADO 2
#define NO_ENCONTRADO 3
/*
 *  Defines: Opciones generales
 */
#define ALTA 0
#define INSERTAR 1
#define BORRAR 2
#define BUSCAR 3
#define TERMINAR 4
/*
 *  Defines: Archivo log
 */
 #define INICIAR 1
 #define OPERACION 2
 #define CIERRE 3
/*
 *  Librerias
 */
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
#include <time.h>


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



/*
 *  Declaraciones comunes
 */
int Q1;
int Q2;
int memo;
int max_clientes;
int *dato;
sem_t *s1;
sem_t *mutex;
struct mensaje_peticion peticion;
struct mensaje_respuesta respuesta;
/*
 *  Declaraciones servidor
 */
Nodo *raizarbol;
int *vector_clientes;
int num_clientes;
FILE *logfile;
/*
 *  Declaraciones control_clientes
 */
pthread_t hilo_c_clientes;
int Q_clientes_activos;
int matar_pid;
struct mensaje_peticion cliente_activo;
sem_t *sclientes;
sigset_t senyal_bloqueada;



/*
 *  Funciones main.c
 */
Nodo* cargar(Nodo* raiz);
Nodo* insertar_elemento(Nodo* raiz,int d);
Nodo* visualizar(Nodo* raiz);
Nodo* buscar(Nodo* raiz,int d);
Nodo* borrar(Nodo* raiz,int d);
Nodo* buscar_padre(Nodo* raiz,int d);
FILE* guardar(Nodo* raiz,FILE *fp1);
void guardar_fichero(Nodo* raiz);
int contar(Nodo* raiz);
Nodo* cargar_ordenado(Nodo* raiz);
Nodo* insertar_ordenado(int inicio,int final,int *datos,Nodo* raiz);
int preparar_entorno();
int inicializar_cola(char nombre[], char id);
int inicializar_memoria_compartida(char nombre[], char id);
sem_t *inicializar_semaforo(char nombre[], int valor_inicial);
int *inicializar_clientes(int numero_clientes);
int *get_inicio_memoria_compartida(int memoria_compartida);
int es_cola_correcta(int cola);
int es_memoria_correcta(int memoria_compartida);
int es_semaforo_correcto(sem_t *semaforo);
int son_clientes_correctos(int *clientes);
int es_arbol_correcto(Nodo *arbol);
void borrar_cola(int cola);
void cerrar_clientes(int *clientes, int numero_clientes);
void cerrar_semaforo(sem_t *semaforo, char nombre[]);
void liberar_memoria_compartida(int memoria_compartida);
void cerrar_programa(int sig);
void *control_clientes(void *parametro);
void matar_cliente_inactivo(int sig);
int baja(int pid);
void operacion_arbol();
void registrando_en_logfile(int tipo_operacion);
/*
 *  Funciones cliente.c
 */
void atender_peticion(int sig);
int abrir_cola(char nombre[], char id);
int abrir_memoria_compartida(char nombre[], char id);

#endif

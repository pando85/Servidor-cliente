/*
 * Nombre: auxiliar_cliente.c
 * Contiene las funciones auxiliares del cliente:
 * atender_peticion()
 * abrir_cola
 * abrir_memoria_compartida()
 *
 */



#include "../arbol.h"




void atender_peticion(int sig)
{
    cliente_activo.tipo = getpid();
    cliente_activo.codigo_operacion = TRUE;
    msgsnd(Q_clientes_activos, &cliente_activo,sizeof(int),0);
}

int abrir_cola(char nombre[], char id)
{
    int cola;

    key_t llave = ftok(nombre, id);
    if(llave < 0)
    {
        printf("¡Error! ftok fallo con errno = %d\n",errno);
        exit(-1);
    }

    cola = msgget(llave, 0);
    if(cola < 0)
    {
        printf("¡Error! No esta ejecutado el servidor.\n");
        exit(-1);
    }
    return cola;
}

int abrir_memoria_compartida(char nombre[], char id)
{
    int memoria;

    key_t keymemo=ftok(nombre,id);
    if(keymemo < 0)
    {
        printf("¡Error! Fallo recuperando token de mem. compartida. [ERROR %d]\n", errno);
        exit(-1);
    }

    memoria = shmget(keymemo,sizeof(int),PERMS);
    if(memoria < 0)
    {
        printf("¡Error! No se pudo acceder a la memoria compartida, [ERROR %d]\n", errno);
        exit(-1);
    }
    return memoria;
}


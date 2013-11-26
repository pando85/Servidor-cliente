/*****************************************************************/
/* Nombre: cliente.c                                                      */
/* Contiene el cliente del programa con el menú:                          */
/* main()                                                                 */
/*                                                                        */
/*****************************************************************/

#include "../arbol.h"



int Q_clientes_activos;
struct mensaje_peticion cliente_activo;

void atender_peticion(int sig);
int abrir_cola(char nombre[], char id);
int abrir_memoria_compartida(char nombre[], char id);
/*****************************************************************/
/* Nombre: main()                                                         */
/* Descripción: Carga el menú principal.                                  */
/* Argumentos:                                                            */
/* Valor Devuelto: 0 si se ha ejecutado con éxito el programa.            */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/


int main()
{
    int *dato_enviado;
    int dato;
    int err;

    // Identificadores colas y mem compartida
    int Q1;
    int Q2;
    int memo;


    sem_t *s1;
    sem_t *mutex;

    struct mensaje_peticion peticion;
    struct mensaje_respuesta respuesta;

    // ABRIENDO LAS COLAS
    printf("Abriendo colas...\n");
    Q1 = abrir_cola(DIR_CLAVE,'3');
    Q2 = abrir_cola(DIR_CLAVE,'4');
    Q_clientes_activos = abrir_cola(DIR_CLAVE,'7');


    // ABRIENDO LA MEMORIA COMPARTIDA
    printf("Abriendo memoria compartida...\n");
    memo = abrir_memoria_compartida(DIR_CLAVE,'5');

    // ABRIENDO LOS SEMAFOROS
    printf("Abriendo semaforos...\n");
    s1=sem_open(S1,  0);

    if(s1==SEM_FAILED)
    {
        printf("¡Error! No se pudo abrir el semaforo, fallo con errno = %d\n",errno);
        exit(-1);
    }

    mutex=sem_open(MUTEX,0);
    if(mutex==SEM_FAILED)
    {
        printf("¡Error! No se pudo abrir el semaforo, fallo con errno = %d\n",errno);
        exit(-1);
    }

    // Bloqueo si num_clientes>nmax_clientes
    printf("Esperando hueco en el servidor...\n");
    sem_wait(s1);

    dato_enviado=shmat(memo,0,0);

    signal (SIGUSR1,atender_peticion);

    // Petición de alta
    printf("Petición de alta en el servidor...\n");
    peticion.tipo=getpid();
    peticion.codigo_operacion=0;
    msgsnd(Q1, &peticion,sizeof(int),0);
    msgrcv(Q2,&respuesta,sizeof(int),getpid(),0);


    while(1)//peticion.codigo_operacion!=4
    {
        /************************MENÚ*********************************/
        printf("\n\n\n\n");
        printf("1. Insertar elemento por valor\n");
        printf("2. Borrar elemento en árbol\n");
        printf("3. Buscar elemento por valor\n");
        printf("4. Salir\n");
        printf("\n\n\n");

        __fpurge(stdin);
        err=scanf("%d",&peticion.codigo_operacion);

        if(peticion.codigo_operacion==1 || peticion.codigo_operacion==2 || peticion.codigo_operacion==3)
        {

            printf("\nIntroduzca el dato:");
            __fpurge(stdin);
            err=scanf("%d",&dato);
        }
        if(err==0)
        {
            printf("\nNo se ha introducido ningún entero, por favor inténtelo de nuevo.\n");
        }
        else
        {
            sem_wait(mutex);
            msgsnd(Q1, &peticion,sizeof(int),0);
            *dato_enviado=dato;
            sem_post(mutex);
            msgrcv(Q2,&respuesta,sizeof(int),getpid(),0);

            switch(respuesta.codigo_error)
            {
            case NO_ERROR:
                break;

            case ENCONTRADO:
                printf("El dato se encuentra en el árbol.\n");
                sleep(1);
                break;

            case NO_ENCONTRADO:
                printf("El dato no se encuentra en el árbol.\n");
                sleep(1);
                break;

            case ERROR_NO_BAJA:
                printf("¡ERROR! No se pudo efectuar la baja.\n");
                sleep(1);
                break;

            }

        }

    }
    return 0;
}


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

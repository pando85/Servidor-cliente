
#ifndef CABECERA_INCLUIDA
#define CABECERA_INCLUIDA
#include "../arbol.h"
#endif

/*****************************************************************/
/* Nombre: main()                                                         */
/* Descripción: Carga el menú principal.                                  */
/* Argumentos: 	Nº máximo de clientes.                                    */
/* Valor Devuelto: 0 si se ha ejecutado con éxito el programa.            */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/


int main()
{
    int *dato,d,err; // dato para insertar/buscar/borrar , err para buscar errores
    int Q1,Q2,memo; // Identificadores colas y mem compartida

    key_t llave1, llave2,keymemo; // llaves para la creacion de colas y memoria compartida
    sem_t *s1, *mutex; // punteros para identificador de los semaforos

    struct mensaje_peticion peticion;
    struct mensaje_respuesta respuesta;

    // ABRIENDO LAS COLAS
    llave1=ftok("/bin",'3');
    if(llave1==-1)
    {
        printf("¡Error! ftok fallo con errno = %d\n",errno);
        exit(-1);
    }
    llave2=ftok("/bin",'4');
    if(llave2==-1)
    {
        printf("¡Error! ftok fallo con errno = %d\n",errno);
        exit(-1);
    }
    Q1= msgget(llave1, 0);
    if(Q1==-1)
    {
        printf("¡Error! No esta ejecutado el servidor Q1.\n");
        exit(-1);
    }
    Q2= msgget(llave2, 0);
    if(Q2==-1)
    {
        printf("¡Error! No esta ejecutado el servidor Q2.\n");
        exit(-1);
    }

    // ABRIENDO LA MEMORIA COMPARTIDA
    keymemo=ftok("/bin",'5');
    if(keymemo==-1)
    {
        printf("¡Error! ftok fallo con errno = %d\n",errno);
        exit(-1);
    }
    memo=shmget(keymemo,sizeof(int),PERMS);
    if(memo==-1)
    {
        printf("¡Error! No se pudo acceder a la memoria compartida, fallo con errno = %d\n",errno);
        exit(-1);
    }

    // ABRIENDO LOS SEMAFOROS
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

    // Bloqueo si n_clientes>nmax_clientes
    printf("Esperando hueco en el servidor...\n");
    sem_wait(s1);

    dato=shmat(memo,0,0);

    // Petición de alta
    printf("Petición de alta en el servidor...\n");
    peticion.tipo=getpid();
    peticion.codigo_operacion=0;
    msgsnd(Q1, &peticion,sizeof(int),0);
    msgrcv(Q2,&respuesta,sizeof(int),getpid(),0);

    /************************MENÚ*********************************/
    while(1)//peticion.codigo_operacion!=4
    {
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
            err=scanf("%d",&d);
        }
        if(err==0)
        {
            printf("\nNo se ha introducido ningún entero, por favor inténtelo de nuevo.\n");
        }
        else
        {
            sem_wait(mutex);
            msgsnd(Q1, &peticion,sizeof(int),0);
            *dato=d;
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

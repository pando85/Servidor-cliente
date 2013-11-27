/*
 * Nombre: cliente.c
 * Contiene la función principal del cliente:
 * main()
 *
 */

#include "../arbol.h"



/*
 * Nombre: main()
 * Descripción: Contiene el cliente con todas sus funciones.
 * Argumentos: 	Ninguno.
 * Valor Devuelto: Ninguno.
 * Alexander Gil Casas. 2013.
 */

int main()
{
    int *dato_enviado;
    int dato;
    int err;

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
    peticion.codigo_operacion=ALTA;
    msgsnd(Q1, &peticion,sizeof(int),0);
    msgrcv(Q2,&respuesta,sizeof(int),getpid(),0);


    while(1)//peticion.codigo_operacion!=4
    {
        /************************MENÚ*********************************/
        printf("\n\n\n\n");
        printf("%d. Insertar elemento por valor\n",INSERTAR);
        printf("%d. Borrar elemento en árbol\n",BORRAR);
        printf("%d. Buscar elemento por valor\n",BUSCAR);
        printf("%d. Salir\n",TERMINAR);
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
                sleep(TIEMPO_PANTALLA);
                break;

            case NO_ENCONTRADO:
                printf("El dato no se encuentra en el árbol.\n");
                sleep(TIEMPO_PANTALLA);
                break;

            case ERROR_NO_BAJA:
                printf("¡ERROR! No se pudo efectuar la baja.\n");
                sleep(TIEMPO_PANTALLA);
                break;

            }

        }

    }
    return 0;
}

/*
 * Nombre: main.c
 * Contiene la función principal del programa con el menú:
 * main()
 *
 */




#include "arbol.h"


/*
 * Nombre: main()
 * Descripción: Contiene el servidor con todas sus funciones.
 * Argumentos: 	Nº máximo de clientes.
 * Valor Devuelto: 0 si se ha ejecutado con éxito el programa.
 * Alexander Gil Casas. 2013.
 */


int main(int argc, char *argv[])
{
    int error;


    num_clientes = 0;
    // Comprobación si se ha introducido el número máximo de clientes
    if(argc!=2)
    {
        printf("\n¡Error! Debe introducir el nº maximo de clientes como argumento.\n\n");
        exit(-1);
    }
    max_clientes=atoi(argv[1]);
    if(max_clientes==0)
    {
        printf("\n¡Error! Debe introducir el nº maximo de clientes como argumento.\n\n");
        exit(-1);
    }

    if (!preparar_entorno())
        cerrar_programa(SIGINT);


    error = pthread_create(&hilo_c_clientes, NULL,control_clientes, NULL);
    if(error != 0)
    {
        perror("\n¡ERROR!No se puede crear el hilo para el control de clientes inactivos.\n\n");
    }

    sigemptyset(&senyal_bloqueada);
    sigaddset(&senyal_bloqueada,SIGALRM);
    error = pthread_sigmask(SIG_BLOCK, &senyal_bloqueada, NULL);
    if(error != 0)
    {
        perror("\n¡ERROR!No se puede crear el hilo para el control de clientes inactivos.\n\n");
    }

    while(1)
    {
        msgrcv(Q1,&peticion,sizeof(int),0,0);
        respuesta.tipo=peticion.tipo;
        switch(peticion.codigo_operacion)
        {
        case ALTA:
            printf("Peticion de alta del cliente %d/%d, con pid %ld\n",num_clientes+1,max_clientes,peticion.tipo);
            vector_clientes[num_clientes]=peticion.tipo;
            sem_wait(sclientes);
            num_clientes++;
            sem_post(sclientes);
            respuesta.codigo_error=NO_ERROR;
            break;


        case INSERTAR:
            sem_wait(mutex);
            raizarbol=insertar_elemento(raizarbol,*dato);
            respuesta.codigo_error=NO_ERROR;
            sem_post(mutex);
            break;

        case BORRAR:
            sem_wait(mutex);
            respuesta.codigo_error=NO_ERROR;
            raizarbol=borrar(raizarbol,*dato);
            sem_post(mutex);
            break;


        case BUSCAR:
            sem_wait(mutex);
            if(buscar(raizarbol,*dato)!=NULL)
            {
                respuesta.codigo_error=ENCONTRADO;
            }
            else
            {
                respuesta.codigo_error=NO_ENCONTRADO;
            }
            sem_post(mutex);
            break;


        case TERMINAR:
            if(baja(peticion.tipo)==ELIMINADO)
            {
                sem_wait(sclientes);
                num_clientes--;
                sem_post(sclientes);
                printf("Peticion de baja del cliente %ld, clientes conectados %d/%d\n",peticion.tipo,num_clientes,max_clientes);
                respuesta.codigo_error=NO_ERROR;
                sem_post(s1);
            }
            else
            {
                respuesta.codigo_error=ERROR_NO_BAJA;
            }
            break;



        }

        msgsnd(Q2, &respuesta, sizeof(int),0);

        printf("\n\n");
        visualizar(raizarbol);
        printf("\n\n");


    }

    return 0;

}


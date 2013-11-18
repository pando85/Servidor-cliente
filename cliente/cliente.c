

#include <stdio.h>
#include <stdlib.h>
#include "../arbol.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/errno.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>


#define PERMS 0600
#define MUTEX "/raro123"
#define S1 "/ouyea123"
/*****************************************************************/
/* Nombre: main()                                                         */
/* Descripción: Carga el menú principal.                                  */
/* Argumentos: 	Nº máximo de clientes.                                    */
/* Valor Devuelto: 0 si se ha ejecutado con éxito el programa.            */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/


int main()
{
    int opcion; // opción del menu
    int d,err; // dato para insertar/buscar/borrar , err para buscar errores
    int Q1,Q2,memo; // Identificadores colas y mem compartida

    key_t llave1, llave2,keymemo; // llaves para la creacion de colas y memoria compartida
    sem_t *s1, *mutex; // punteros para identificador de los semaforos



    struct mens_peticion peticion;
	struct mens_respuesta respuesta;



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
        printf("¡Error! No esta ejecutado el servidor.\n");
        exit(-1);
    }
	Q2= msgget(llave2, 0);
    if(Q2==-1)
    {
        printf("¡Error! No esta ejecutado el servidor.\n");
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
    sem_wait(s1);


    // Petición de alta
    peticion.tipo=getpid();
    peticion.cod_op=0;
    msgsnd(Q1, &peticion,sizeof(int),0);
    msgrcv(Q2,&respuesta,sizeof(int),getpid(),0);



    /************************MENÚ*********************************/
    while(opcion!=4)
    {
        printf("\n\n\n\n");
        printf("1. Insertar elemento por valor\n");
        printf("2. Busca elemento en árbol\n");
        printf("3. Borrar elemento por valor\n");
        printf("4. Salir\n");

        __fpurge(stdin);
        scanf("%d",&opcion);
        printf("\n\n\n");

        switch(opcion)
        {

        case 1:
            printf("Introduzca el dato que desea insertar:");
            fflush(stdin);
            err=scanf("%d",&d);
            if(err==0)
                printf("\nNo se ha introducido ningún entero, por favor inténtelo de nuevo.\n");
            else
 //               raiz=InsertarElemento(raiz,d);
            break;
        case 2:
            printf("Introduzca el dato que desea buscar:");
            fflush(stdin);
            err=scanf("%d",&d);
            if(err==0)
            {
                printf("\nNo se ha introducido ningún entero, por favor inténtelo de nuevo.\n");
                break;
            }

 //           if (Buscar(raiz,d)==NULL) //Resultado de buscar el dato
//            {
 //               printf("\nEl dato no se encuentra en el árbol.");
  //          }
   //         else
    //        {
     //           printf("\nEl dato se encuentra en el árbol.");
      //      }
            break;
        case 3:
            printf("Introduzca el dato que desea borrar:");
            fflush(stdin);
            err=scanf("%d",&d);
            if(err==0)
                printf("\nNo se ha introducido ningún entero, por favor inténtelo de nuevo.\n");
            else
    //            raiz=Borrar(raiz,d);
            break;

        }
    }
    return 0;
}

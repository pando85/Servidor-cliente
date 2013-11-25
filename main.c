/*****************************************************************/
/* Nombre: main.c                                                         */
/* Contiene la función principal del programa con el menú:                */
/* main()                                                                 */
/*                                                                        */
/*****************************************************************/



#ifndef CABECERA_INCLUIDA
#define CABECERA_INCLUIDA
#include "arbol.h"
#endif





/*****************************************************************/
/* Nombre: main()                                                         */
/* Descripción: Carga el menú principal.                                  */
/* Argumentos: 	Nº máximo de clientes.                                    */
/* Valor Devuelto: 0 si se ha ejecutado con éxito el programa.            */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/


int main(int argc, char *argv[])
{
    int *dato,i; // dato para insertar/buscar/borrar , err para buscar errores,Q1 cola 1,Q2 cola 2,max_clientes,longitud mensaje
    key_t llave1, llave2,keymemo; // llaves para la creacion de colas y memoria compartida

    struct mensaje_peticion peticion;
    struct mensaje_respuesta respuesta;

    raizarbol=NULL;// Árbol vacío

    num_clientes=0; // Inicializamos el numero de clientes
    esta_proceso_terminado= FALSE;
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

    printf("Creando colas...\n");

    // CREANDO LAS COLAS
    llave1=ftok("/bin",'3');
    if(llave1==-1)
    {
        perror(strerror(errno));
        exit(-1);
    }
    llave2=ftok("/bin",'4');
    if(llave2==-1)
    {
        perror(strerror(errno));
        exit(-1);
    }
    Q1 = msgget(llave1, PERMS | IPC_CREAT);
    if(Q1==-1)
    {
        perror(strerror(errno));
        exit(-1);
    }
    Q2 = msgget(llave2, PERMS | IPC_CREAT);
    if(Q2==-1)
    {
        perror(strerror(errno));
        exit(-1);
    }

    printf("Creando memoria compartida...\n");
    // Creo memoria compartida
    keymemo=ftok("/bin",'5');
    if(keymemo==-1)
    {
        perror(strerror(errno));
        exit(-1);
    }
    memo=shmget(keymemo,sizeof(int),IPC_CREAT | PERMS);
    if(memo==-1)
    {
        perror(strerror(errno));
        exit(-1);
    }

    printf("Creando semaforos...\n");

    // Creo e inicializo semaforo mutex a 1
    mutex=sem_open(MUTEX, O_CREAT, PERMS, 1);
    if(mutex==SEM_FAILED)
    {
        perror(strerror(errno));
        exit(-1);
    }

    // Creo e inicializo semaforo s1 a max_clientes
    s1=sem_open(S1,O_CREAT,PERMS,max_clientes);
    if(s1==SEM_FAILED)
    {
        perror(strerror(errno));
        exit(-1);
    }

    //Vector con los pids de los clientes
    vector_clientes=(int *)malloc(max_clientes*sizeof(pid_t));

    printf("Cargando datos del arbol desde el fichero...\n");
    // Cargar fichero
    raizarbol=CargarOrdenado(raizarbol);

    if(raizarbol==NULL)
    {
        printf("¡Error! No se pudo cargar el arbol.\n");
        esta_proceso_terminado=TRUE;
    }

    // Ctrl-C cierra_programa
    signal (SIGINT,cerrar_programa);

    // Dato apunta a la memoria compartida
    dato=shmat(memo,0,0);

    while(1)
    {
        msgrcv(Q1,&peticion,sizeof(int),0,0);
        respuesta.tipo=peticion.tipo;
        switch(peticion.codigo_operacion)
        {
            // ALTA
        case 0:
            printf("Peticion de alta del cliente %d/%d, con pid %d\n",num_clientes+1,max_clientes,peticion.tipo);
            vector_clientes[num_clientes]=peticion.tipo;
            num_clientes++;
            respuesta.codigo_error=NO_ERROR;
            break;


            // INSERTAR
        case 1:
            sem_wait(mutex);
            raizarbol=InsertarElemento(raizarbol,*dato);
            respuesta.codigo_error=NO_ERROR;
            sem_post(mutex);
            break;


            // BORRAR
        case 2:
            sem_wait(mutex);
            raizarbol=Borrar(raizarbol,*dato);
            respuesta.codigo_error=NO_ERROR;
            sem_post(mutex);
            break;


            // BUSCAR
        case 3:
            sem_wait(mutex);
            if(Buscar(raizarbol,*dato)!=NULL)
            {
                respuesta.codigo_error=ENCONTRADO;
            }
            else
            {
                respuesta.codigo_error=NO_ENCONTRADO;
            }
            sem_post(mutex);
            break;


            // TERMINAR
        case 4:
            if(baja(peticion.tipo,vector_clientes,max_clientes)==ELIMINADO)
            {
                num_clientes--;
                printf("Peticion de baja del cliente %d, clientes conectados %d/%d\n",peticion.tipo,num_clientes,max_clientes);
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
        Visualizar(raizarbol);
        printf("\n\n");


    }
    // Guardar fichero
    GuardarFichero(raizarbol);

    // Borrar colas
    msgctl(Q1, IPC_RMID, 0);
    msgctl(Q2, IPC_RMID, 0);



    // Cerrar clientes
    for(i=0; i<num_clientes; i++)
    {
        kill(vector_clientes[i],SIGINT);
    }

    // Liberar memoria
    free(vector_clientes);


    // Cerrar y borrar semaforos
    sem_close(mutex);
    sem_close(s1);
    sem_unlink(MUTEX);
    sem_unlink(S1);

    // Destruye la memoria compartida
    shmctl(memo, IPC_RMID, 0);


    return 0;
}




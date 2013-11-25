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

int Q1 = -1;
int Q2 = -1;
int memo = -1;
int max_clientes;
int *dato = NULL;
sem_t *s1 = NULL;
sem_t *mutex = NULL;

Nodo *raizarbol = NULL;
int *vector_clientes = NULL;
int num_clientes = 0;


// Cabeceras de funciones
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

/*****************************************************************/
/* Nombre: main()                                                         */
/* Descripción: Carga el menú principal.                                  */
/* Argumentos: 	Nº máximo de clientes.                                    */
/* Valor Devuelto: 0 si se ha ejecutado con éxito el programa.            */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/


int main(int argc, char *argv[])
{
    int i;


    struct mensaje_peticion peticion;
    struct mensaje_respuesta respuesta;

    raizarbol=NULL;

    num_clientes=0;

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
        cerrar_programa(2);


    while(1)
    {
        msgrcv(Q1,&peticion,sizeof(int),0,0);
        respuesta.tipo=peticion.tipo;
        switch(peticion.codigo_operacion)
        {
            // ALTA
        case 0:
            printf("Peticion de alta del cliente %d/%d, con pid %ld\n",num_clientes+1,max_clientes,peticion.tipo);
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
        Visualizar(raizarbol);
        printf("\n\n");


    }

    return 0;
}


int preparar_entorno()
{
    printf("Creando colas...\n");
    Q1 = inicializar_cola("/bin", '3');
    if(!es_cola_correcta(Q1))
    {
        printf("\nNo se pudo crear la cola 1.\n\n");
        return FALSE;
    }

    Q2 = inicializar_cola("/bin", '4');
    if (!es_cola_correcta(Q2))
    {
        printf("\nNo se pudo crear la cola 2.\n\n");
        return FALSE;
    }

    printf("Creando memoria compartida...\n");
    memo = inicializar_memoria_compartida("/bin", '5');
    if(!es_memoria_correcta(memo))
    {
        printf("\nNo se pudo inicializar la memoria compartida.\n\n");
        return FALSE;
    }

    printf("Creando semaforos...\n");
    // Creo e inicializo semaforo mutex a 1
    mutex = inicializar_semaforo(MUTEX, 1);
    if (!es_semaforo_correcto(mutex))
    {
        printf("\nError inicializando semaforo MutEx.\n\n");
        return FALSE;
    }

    // Creo e inicializo semaforo s1 a max_clientes
    s1 = inicializar_semaforo(S1, max_clientes);
    if(!es_semaforo_correcto(s1))
    {
        printf("\nError inicializando semaforo S1.\n\n");
        return FALSE;
    }


    //Vector con los pids de los clientes
    vector_clientes = inicializar_clientes(max_clientes);
    if (!son_clientes_correctos(vector_clientes))
    {
        printf("\nError inicializando espacio de clientes.\n\n");
        return FALSE;
    }

    printf("Cargando datos del arbol desde el fichero...\n");
    // Cargar fichero
    raizarbol=CargarOrdenado(raizarbol);

    if(!es_arbol_correcto(raizarbol))
    {
        printf("¡Error! No se pudo cargar el arbol.\n");
        return FALSE;
    }

    // Ctrl-C cierra_programa
    signal (SIGINT,cerrar_programa);

    // Dato apunta a la memoria compartida
    dato = get_inicio_memoria_compartida(memo);

    return TRUE;
}

int inicializar_cola(char nombre[], char id)
{
    int cola;

    key_t llave = ftok(nombre, id);
    if(llave < 0)
    {
        fprintf(stderr, "\nError creando token de cola.\n\n");
        return -1;
    }

    cola = msgget(llave, PERMS | IPC_CREAT);
    if(cola < 0)
    {
        fprintf(stderr, "\nError durante la creación de la cola.\n\n");
        return -1;
    }
    return cola;
}

int es_cola_correcta(int cola)
{
    return cola >= 0;
}

int inicializar_memoria_compartida(char nombre[], char id)
{
    int memoria;

    key_t keymemo=ftok(nombre,id);
    if(keymemo < 0)
    {
        printf("¡Error! Fallo recuperando token de mem. compartida. [ERROR %d]\n", errno);
        return -1;
    }

    memoria = shmget(keymemo, sizeof(int), IPC_CREAT | PERMS);
    if(memoria < 0)
    {
        printf("¡Error! No se pudo acceder a la memoria compartida, [ERROR %d]\n", errno);
        return -1;
    }
}

int es_memoria_correcta(int memoria_compartida)
{
    return memoria_compartida >= 0;
}

sem_t *inicializar_semaforo(char nombre[], int valor_inicial)
{
    sem_t *mutex;

    mutex = sem_open(nombre, O_CREAT, PERMS, valor_inicial);
    if(mutex == SEM_FAILED)
    {
        printf("¡Error! No se pudo abrir el semaforo, fallo con errno = %d\n",errno);
        return NULL;
    }
    return mutex;
}

int es_semaforo_correcto(sem_t *semaforo)
{
    return semaforo != NULL;
}

int *inicializar_clientes(int numero_clientes)
{
    size_t tamanyo_memoria = sizeof(pid_t) * (size_t)numero_clientes;
    return (int *)malloc(tamanyo_memoria);
}

int son_clientes_correctos(int *clientes)
{
    return clientes != NULL;
}

int *get_inicio_memoria_compartida(int memoria_compartida)
{
    return (int *)shmat(memoria_compartida, 0, 0);
}

int es_arbol_correcto(Nodo *arbol)
{
    return arbol != NULL;
}


void borrar_cola(int cola)
{
    msgctl(cola, IPC_RMID, 0);
}

void cerrar_clientes(int *clientes, int numero_clientes)
{
    if (!son_clientes_correctos(clientes))
        return;

    int i;
    for(i = 0; i < numero_clientes; i++)
    {
        kill(clientes[i], SIGINT);
    }

    free(vector_clientes);
}

void cerrar_semaforo(sem_t *semaforo, char nombre[])
{
    if (!es_semaforo_correcto(semaforo))
        return;
    sem_close(semaforo);
    sem_unlink(nombre);
}

void liberar_memoria_compartida(int memoria_compartida)
{
    if (!es_memoria_correcta(memoria_compartida))
        return;

    shmctl(memoria_compartida, IPC_RMID, 0);
}




void cerrar_programa(int sig)
{
    printf("Guardando datos del arbol en el fichero...\n");
    GuardarFichero(raizarbol);

    printf("Cerrando clientes...\n");
    cerrar_clientes(vector_clientes, num_clientes);

    printf("Cerrando colas...\n");
    borrar_cola(Q1);
    borrar_cola(Q2);

    printf("Cerrando semaforos...\n");
    cerrar_semaforo(mutex, MUTEX);
    cerrar_semaforo(s1, S1);

    printf("Cerrando memoria compartida...\n");
    liberar_memoria_compartida(memo);
    exit(0);
}




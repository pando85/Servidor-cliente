/*****************************************************************/
/* Nombre: main.c                                                         */
/* Contiene la función principal del programa con el menú:                */
/* main()                                                                 */
/*                                                                        */
/*****************************************************************/




#include "arbol.h"


/*****************************************************************/
/* Nombre: main()                                                         */
/* Descripción: Carga el menú principal.                                  */
/* Argumentos: 	Nº máximo de clientes.                                    */
/* Valor Devuelto: 0 si se ha ejecutado con éxito el programa.            */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/

int preparar_entorno()
{
    printf("Creando colas...\n");
    Q1 = inicializar_cola(DIR_CLAVE, '3');
    if(!es_cola_correcta(Q1))
    {
        printf("\nNo se pudo crear la cola 1.\n\n");
        return FALSE;
    }

    Q2 = inicializar_cola(DIR_CLAVE, '4');
    if (!es_cola_correcta(Q2))
    {
        printf("\nNo se pudo crear la cola 2.\n\n");
        return FALSE;
    }
    Q_clientes_activos = inicializar_cola(DIR_CLAVE, '7');
    if (!es_cola_correcta(Q_clientes_activos))
    {
        printf("\nNo se pudo crear la cola clientes-activos.\n\n");
        return FALSE;
    }

    printf("Creando memoria compartida...\n");
    memo = inicializar_memoria_compartida(DIR_CLAVE, '5');
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
    // Semaforo para el control de clientes
    sclientes = inicializar_semaforo(SCLIENTES, 1);
    if(!es_semaforo_correcto(sclientes))
    {
        printf("\nError inicializando semaforo SCLIENTES.\n\n");
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

void cerrar_programa(int sig)
{
    printf("Cerrando el control de clientes inactivos...\n");


    printf("Guardando datos del arbol en el fichero...\n");
    GuardarFichero(raizarbol);

    printf("Cerrando clientes...\n");
    cerrar_clientes(vector_clientes, num_clientes);

    printf("Cerrando colas...\n");
    borrar_cola(Q1);
    borrar_cola(Q2);
    borrar_cola(Q_clientes_activos);

    printf("Cerrando semaforos...\n");
    cerrar_semaforo(mutex, MUTEX);
    cerrar_semaforo(s1, S1);
    cerrar_semaforo(sclientes,SCLIENTES);

    printf("Cerrando memoria compartida...\n");
    liberar_memoria_compartida(memo);
    exit(0);
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
    return memoria;
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





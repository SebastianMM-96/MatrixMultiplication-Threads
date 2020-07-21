#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include "matrix_library.h"

//Defines
#define TRUE 1
#define FALSE 0
#define bool int
#define MAX_COLS_COUNT 10000
#define MAX_ROWS_COUNT 10000
//maximo numero de hilos es el maximo de numero de filas * columnas
#define MAX_THREAD_NUMBER 100000000


int A[MAX_ROWS_COUNT][MAX_COLS_COUNT];
int B[MAX_ROWS_COUNT][MAX_COLS_COUNT];
int C[MAX_ROWS_COUNT][MAX_COLS_COUNT];
int A_cols  = -1, B_cols = -1 , A_rows = -1 , B_rows = -1 , C_cols = -1 , C_rows = -1;
/*Apuntadores de los archivos donde se encuentran las matrices*/
char* fileA;
char* fileB;
char* fileC;
/*Variable que manda los errores que puede haber en la compilacion*/
enum error_codes error;
/*definimos la variable de hilos, el cual contendra el maximo de los hilos.*/
/*Es el tipo de datos que almacena el identificador de un hilo*/
pthread_t threads[MAX_THREAD_NUMBER];

int main(int argc , char** argv)
{
    /*Asignamos memoria a nuestroas archivos.*/
    fileA = malloc(100);
    fileB = malloc(100);
    fileC = malloc(100);

    if(argc < 4)  //si los archivos no estan definidos
    {
        strcpy(fileA , "a.txt");
        strcpy(fileB , "b.txt");
        strcpy(fileC , "c");
    }
    else
    {
        strcpy(fileA , argv[1]);
        strcpy(fileB , argv[2]);
        strcpy(fileC , argv[3]);
    }
    /*Errores que pueden presentarse si los archivos no se encuentran en el mismo directorio*/
    if(read_matrix_A()==FALSE)
    {
        error = FAILED_READ_MATRIX_A;
        print_error(error);
        exit(1);
    }
    else if(read_matrix_B()==FALSE)
    {
        error = FAILED_READ_MATRIX_B;
        print_error(error);
        exit(1);
    }
    /*Si las columnas de la matriz A son diferentes de las filas de la matriz B*/

    else if(A_cols!= B_rows)
    {
        /*No se puede multiplicar*/
        error = INVALID_ARGUMENTS;
        print_error(error);
        exit(1);
    }
    else
    {
        printf("Usando thread por cada elemento de la matriz.\n");
        first_method();
        printf("Usando thread por cada fila de la matriz\n");
        second_method();
        return 0;
    }
}

/*Calculamos la matriz C teniendo un thread que calcule cada elemento*/
void first_method()
{
    //Cada hilo calcula un elemento, necesita C_rows * C_cols threads
    struct timeval stop, start;
    gettimeofday(&start, NULL); //Se empieza a tomar el tiempo de ejecucion
    int i;
    int number_of_threads = C_cols * C_rows;
    printf("Numero de thread usados: %d\n" , number_of_threads);
    
    for(i = 0 ; i < number_of_threads ; i ++)
    {
        thread_element_params *thread_data = malloc(sizeof(thread_element_params));
        thread_data->row = i/C_cols;
        thread_data->column = i%C_cols;
        
        int state = pthread_create(&threads[i] , NULL , calculate_element , (void*) thread_data);
        /* Crea un nuevo hilo que se ejecuta concurrentemente con el hilo que hace la llamada. 
        El nuevo hilo ejecuta la función start_routine pasando arg como primer  argumento.*/
        
        if(state)
        {
            error = FAILED_TO_CREATE_THREAD;
            print_error(error);
            exit(-1);
        }
    }
    
    for(i = 0 ; i < number_of_threads ; i++)
    {
        pthread_join(threads[i],NULL);
        /*suspende la ejecución del hilo que hace la llamada hasta que el hilo identificado 
        por th termina (explícitamente o implícitamente).
        */

    }
    
    print_result(1);
    gettimeofday(&stop, NULL); //end checking time
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
    return;
}

//La función de arranque para el método que calcula por elemento
void* calculate_element(void* threadarg)
{
    int value = 0;
    int i;
    
    thread_element_params *my_data;
    my_data = (thread_element_params *)threadarg;
    
    int row = my_data->row;
    int column = my_data->column;
    
    for(i = 0 ; i < A_cols ; i ++)
    {
        value += (A[row][i] * B[i][column]);
    }
    
    C[row][column] = value;
    pthread_exit(NULL);
    /*termina la ejecución del hilo que hace la llamada.*/
}

/*Calcula la matriz C haciendo que un hilo calcule cada fila*/
void second_method()
{
    //Cada hilo calcula una fila, número de hilos = C_rows
    struct timeval stop, start;
    gettimeofday(&start, NULL); //Empezamos a tomar el tiempo
    int number_of_threads = C_rows;
    int i;
    printf("Numero de threads usados: %d\n" , number_of_threads);
    
    for(i = 0 ; i < number_of_threads ; i ++)
    {
        int state;
        int thread_data = i;
        state = pthread_create(&threads[i] , NULL , calculate_row , (void*)thread_data);
        
        if(state)
        {
            error = FAILED_TO_CREATE_THREAD;
            print_error(error);
            exit(-1);
        }
    }
    
    for(i = 0 ; i < number_of_threads ; i++)
    {
        pthread_join(threads[i],NULL);
    }
   
    print_result(2);
    gettimeofday(&stop, NULL); //Terminamos de tomar el tiempo
    
    /*Imprimimos el tiempo que tomo a los hilos calcular.*/
    printf("Tomaron: %lu segundos\n", stop.tv_sec - start.tv_sec);
    printf("Tomaron: %lu microsegundos\n", stop.tv_usec - start.tv_usec);
    return;
}


//La función de inicio para el hilo por método de fila
void* calculate_row(void* threadarg)
{
    int i , j;
    int row = threadarg;
    for(i = 0 ; i < C_cols ; i ++)
    {
        int value = 0;
        for(j = 0 ; j < C_rows ; j++)
        {
            value += (A[row][j] * B[j][i]);
        }
        C[row][i] = value;
    }
    pthread_exit(NULL);
}

/*Funcion para imprimir la matriz*/
//Imprime la matriz C en el archivo de C
void print_result(int method_id)
{
    int i,j;
    char* file = malloc(100);
    strcpy(file , fileC);
    if(method_id == 1){
        strcat(file , "_1");
    }
    else strcat(file, "_2");
    strcat(file , ".out");
    freopen(file , "w" , stdout);
    for(i = 0 ; i  < C_rows ; i++)
    {
        for(j = 0 ; j < C_cols ; j ++)
        {
            printf("%d" , C[i][j]);
            if(j<C_cols-1)printf("\t");
        }
        printf("\n");
    }
    
    freopen("/dev/tty" , "a" , stdout);
}

/*Funcion para leer la matriz A del archivo a.txt*/
bool read_matrix_A()
{
    char* input_line = malloc(200);
    char* temp = malloc(200);
    if(!freopen(fileA , "r" , stdin))
    {
        error = FILE_A_NOT_FOUND;
        print_error(error);
        exit(1);
    }
    else
    {
        fgets(input_line , 200 , stdin);
        temp = strtok(input_line , "=\t ");
        while(temp!= NULL)
        {
            if(atoi(temp)!=0 || temp[0]=='0')
            {
                if(A_rows==-1)
                {
                    A_rows = atoi(temp);
                    C_rows = atoi(temp);
                }
                else A_cols = atoi(temp);
            }
            temp = strtok(NULL , "=\t ");
        }
        int i , j;
        for(i = 0 ; i < A_rows ; i++)
        {
            fgets(input_line , 200 , stdin);
            temp = strtok(input_line , " \t");
            for(j = 0 ; j  < A_cols ; j++)
            {
                if(atoi(temp)!=0 || temp[0]=='0')
                {
                    A[i][j] = atoi(temp);
                }
                else return FALSE;
                if(temp!=NULL)temp = strtok(NULL, "\t ");
            }
        }
        return TRUE;
    }
}

/*Funcion para leer la matriz B del archivo b.txt*/
bool read_matrix_B()
{
    char* input_line = malloc(200);
    char* temp = malloc(200);
    if(!freopen(fileB , "r" , stdin))
    {
        error = FILE_B_NOT_FOUND;
        print_error(error);
        exit(1);
    }
    else
    {
        fgets(input_line , 200 , stdin);
        temp = strtok(input_line , "=\t ");
        while(temp!= NULL)
        {
            if(atoi(temp)!=0 || temp[0]=='0')
            {
                if(B_rows==-1)
                    B_rows = atoi(temp);
                else
                {
                    B_cols = atoi(temp);
                    C_cols = atoi(temp);
                }
            }
            temp = strtok(NULL , "=\t ");
        }
        int i , j;
        for(i = 0 ; i < B_rows ; i++)
        {
            fgets(input_line , 200 , stdin);
            temp = strtok(input_line , " \t");
            for(j = 0 ; j  < B_cols ; j++)
            {
                if(atoi(temp)!=0 || temp[0]=='0')
                {
                    B[i][j] = atoi(temp);
                }
                else return FALSE;
                if(temp!=NULL)temp = strtok(NULL, "\t ");
            }
        }
        return TRUE;
    }
}

//Imprime el mensaje apropiado para cada error
/*Esto se encuentra en la libreria */
void print_error(enum error_codes error)
{
    switch(error)
    {
    case FAILED_READ_MATRIX_A:
            printf("Fallo en leer la matriz A\n");
        break;
    case FAILED_READ_MATRIX_B:
        printf("Fallo en leer la matriz B\n");
        break;
    case FAILED_TO_CREATE_THREAD:
        printf("Fallo en thread (creacion)\n");
        break;
    case INVALID_ARGUMENTS:
        printf("No se pueden multiplicar estas matrices: columnas de A deben ser iguales a filas de B\n");
        break;
    case FILE_A_NOT_FOUND:
        printf("El archivo de la matriz A no se encontro\n");
        break;
    case FILE_B_NOT_FOUND:
        printf("El archivo de la matriz A no se encontro\n");
        break;
    }
}
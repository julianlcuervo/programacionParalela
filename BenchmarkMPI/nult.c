#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

/*
    Ejecución 
    mpicc nult.c -o nult
    mpirun -f hostfile -n "número de precesadores" ./nult
*/

int main(int argc, char *argv[])
{
    char *a = argv[1];

    srand(time(NULL));
    //recibe el tamaño de la martiz
    int valor = atoi(a);

    int size, rank;
    MPI_Status status;
    //Inicia MPI
    MPI_Init(&argc, &argv);
    //numero de procesadores
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //numero del procesador
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *filas = NULL;
    int *columnas = NULL;
    columnas = malloc((valor * valor) * sizeof(int));
    int *filasGlo = NULL;
    filasGlo = malloc(((valor / size) * valor) * sizeof(int));
    int *columnasGLo = NULL;
    columnasGLo = malloc(((valor / size) * valor) * sizeof(int));
    int *rfinal = NULL;
    double time[size];

    //El siguiente codigo se ejecuta en el procesador 0
    if (rank == 0)
    {
        //espacio de memoria para el resultado de la multiplicacion de matrices
        rfinal = malloc(((valor / size) * valor) * sizeof(int));

        //inicializa la primera matriz
        int **matriz;
        matriz = (int **)malloc(valor * sizeof(int *));

        for (int i = 0; i < valor; i++)
            matriz[i] = (int *)malloc(valor * sizeof(int));

        //Llena la primera matriz con números random hasta el 9
        for (int i = 0; i < valor; i++)
        {
            for (int j = 0; j < valor; j++)
            {
                int ran = rand() % 10;
                matriz[i][j] = ran;
            }
        }

        //imprime la matriz1
        for (int i = 0; i < valor; i++)
        {
            for (int j = 0; j < valor; j++)
            {
                printf("%i ", matriz[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        //Se define la segunda matriz de punteros
        int **matriz2;
        matriz2 = (int **)malloc(valor * sizeof(int *));

        for (int i = 0; i < valor; i++)
            matriz2[i] = (int *)malloc(valor * sizeof(int));

        //Llena la segunda matriz con números random hasta el 9
        for (int i = 0; i < valor; i++)
        {
            for (int j = 0; j < valor; j++)
            {
                int ran = rand() % 10;
                matriz2[i][j] = ran;
            }
        }

        //imprime matriz2
        for (int i = 0; i < valor; i++)
        {
            for (int j = 0; j < valor; j++)
            {
                printf("%i ", matriz2[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        //define arreglo para filas
        filas = malloc((valor * valor) * sizeof(int));

        int x = 0;
        //guarda en el arreglo las filas de la matriz1
        for (int i = 0; i < valor; i++)
        {
            for (int j = 0; j < valor; j++)
            {
                filas[x] = matriz[i][j];
                x += 1;
            }
        }

        int y = 0;
        //guarda en el arreglo las filas de la matriz2
        for (int i = 0; i < valor; i++)
        {
            for (int j = 0; j < valor; j++)
            {
                columnas[y] = matriz2[i][j];
                y += 1;
            }
        }
    }

    //envia de forma equitativa un numero de filas de la matriz1 a todos los procesadores
    MPI_Scatter(filas, (valor / size) * valor, MPI_INT, filasGlo, (valor / size) * valor, MPI_INT, 0, MPI_COMM_WORLD);
    //envia las filas de la matriz2 a cada uno de los procesadores
    MPI_Bcast(columnas, (valor * valor), MPI_INT, 0, MPI_COMM_WORLD);

    //inicializa matriz
    int **m1;
    m1 = (int **)malloc(((valor / size) * valor) / valor * sizeof(int *));

    for (int i = 0; i < ((valor / size) * valor) / valor; i++)
        m1[i] = (int *)malloc(valor * sizeof(int));

    //guarda en la matriz m1 una parte de las filas de la matriz1 en cada procesador
    int o = 0;
    for (int i = 0; i < ((valor / size) * valor) / valor; i++)
    {
        for (int j = 0; j < valor; j++)
        {
            m1[i][j] = filasGlo[o];
            o += 1;
        }
    }

    //iniciliaza matriz
    int **m2;
    m2 = (int **)malloc(valor * sizeof(int *));

    for (int i = 0; i < valor; i++)
        m2[i] = (int *)malloc(valor * sizeof(int));

    //guarda en la matriz m2 las filas de la matriz2
    int p = 0;
    for (int i = 0; i < valor; i++)
    {
        for (int j = 0; j < valor; j++)
        {
            m2[i][j] = columnas[p];
            p += 1;
        }
    }

    //inicializa matriz
    int **matrizmult;
    matrizmult = (int **)malloc(((valor / size) * valor) / valor * sizeof(int *));

    for (int i = 0; i < ((valor / size) * valor) / valor; i++)
        matrizmult[i] = (int *)malloc(valor * sizeof(int));

    //Se inicializa el tiempo de inicio y final en cada procesador
    struct timeval start, end;
    //Inicia el Benchmark
    gettimeofday(&start, NULL);

    //multiplicacion de matrices
    for (int i = 0; i < ((valor / size) * valor) / valor; i++)
    {

        for (int j = 0; j < valor; j++)
        {

            matrizmult[i][j] = 0;

            for (int k = 0; k < valor; k++)
            {
                //Guarda el resultado de la multiplicacion entre matriz y matriz2
                matrizmult[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    gettimeofday(&end, NULL);
    double segundos;
    //Guarda el resultado del tiempo de ejecucion del algoritmo de multiplicacion en segundos de cada procesador
    segundos = ((end.tv_sec - start.tv_sec) * 1000000u +
                end.tv_usec - start.tv_usec) /
               1.e6;
    //Imprime el tiempo
    printf("%f", segundos);
    printf("\n");

    //inicializa arreglo
    int *resultado = NULL;
    resultado = malloc(((valor / size) * valor) * sizeof(int));

    //guarda en el arreglo las filas de la matriz resultante
    int z = 0;
    for (int i = 0; i < ((valor / size) * valor) / valor; i++)
    {
        for (int j = 0; j < valor; j++)
        {
            resultado[z] = matrizmult[i][j];
            z += 1;
        }
    }

	MPI_Gather(&segundos, 1, MPI_DOUBLE, time, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        double promedio=0;
        printf("\n");
        for (int j = 0; j < size; j++)
        {
            promedio+=time[j];
        }
        printf("\n");
        printf("\n");
        printf("Promedio total: %f \n",promedio/size);

    }


    //finalia MPI
    MPI_Finalize();
    return (0);
}

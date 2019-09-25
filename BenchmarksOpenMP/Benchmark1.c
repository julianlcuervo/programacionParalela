#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <time.h>
#include <omp.h>
//Recibe ./nombredelarchivo y el tamaño de la matriz
int main(int argc, char *argv[])
{
    //Guarda el tamaño de la matriz como char
    char *a = argv[1];

    srand(time(NULL));
    //Guarda el valor en un entero
    int valor = atoi(a);

    //Se define la primera matriz de punteros
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

    //Se define la matriz de punteros que guarda el resultado la multiplicacion de matrices
    int **matrizmult;
    matrizmult = (int **)malloc(valor * sizeof(int *));

    for (int i = 0; i < valor; i++)
        matrizmult[i] = (int *)malloc(valor * sizeof(int));

    //Se inicializa el tiempo de inicio y final
    struct timeval start, end;
    //Inicia el Benchmark
    gettimeofday(&start, NULL);
//La multiplicacion de matrices se ejecuta mediante programacion paralela
#pragma omp parallel for
    for (int i = 0; i < valor; i++)
    {

        for (int j = 0; j < valor; j++)
        {

            matrizmult[i][j] = 0;

            for (int k = 0; k < valor; k++)
            {
                //Guarda el resultado de la multiplicacion entre matriz y matriz2
                matrizmult[i][j] += matriz[i][k] * matriz2[k][j];
            }
        }
    }
    //Termine el Benchmark
    gettimeofday(&end, NULL);
    double segundos;
    //Guarda el resultado del tiempo de ejecucion del algoritmo de multiplicacion en segundos
    segundos = ((end.tv_sec - start.tv_sec) * 1000000u +
                end.tv_usec - start.tv_usec) /
               1.e6;
    //Imprime el tiempo
    printf("%f", segundos);
    printf("\n");
}

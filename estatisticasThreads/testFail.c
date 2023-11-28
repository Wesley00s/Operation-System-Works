#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREADS 2

// Estrutura para armazenar os dados das threads
struct DadosThread
{
    int tam;
    int *valores;
    int inicio;
    int fim;
};

typedef struct DadosThread Dados;

// Protótipos das funções
void merge(int *arr, int inicio, int meio, int fim);
void mergeSort(int *arr, int inicio, int fim);
void *threadMerge(void *args);

int main(int argc, char *argv[])
{
    int tam = argc - 1;
    int meio = tam / 2;

    int *valores1 = (int *)malloc(meio * sizeof(int)); // Correção no tamanho
    int *valores2 = (int *)malloc((tam - meio) * sizeof(int));

    Dados dados1 = {meio, valores1, 0, meio - 1};
    Dados dados2 = {tam - meio, valores2, meio, tam - 1};

    // Preenche o array com valores da linha de comando
    for (int i = 0; i < tam; i++)
    {
        if (i < meio)
            valores1[i] = atoi(argv[i + 1]);
        else
            valores2[i - meio] = atoi(argv[i + 1]);
    }

    // Cria threads
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, threadMerge, &dados1);
    pthread_create(&thread2, NULL, threadMerge, &dados2);

    // Esperar threads terminarem
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Exibir resultados
    printf("Vetor ordenado: ");
    for (int i = 0; i < meio; i++)
    {
        printf("%d ", valores1[i]);
    }
    for (int i = 0; i < (tam - meio); i++)
    {
        printf("%d ", valores2[i]);
    }
    printf("\n\n");

    free(valores1); // Libera a memória alocada
    free(valores2); // Libera a memória alocada

    return 0;
}

void merge (int *arr, int inicio, int meio, int fim) // Mescla
{
    int tamD = meio - inicio + 1;
    int tamE = fim - meio;

    int D[tamD], E[tamE];

    for (int i = 0; i < tamE; i++)
    {
        E[i] = arr[inicio + i];
    }
    for (int i = 0; i < tamD; i++)
    {
        D[i] = arr[meio + 1 + i];
    }

    int i = 0, j = 0, k = inicio;

    while (i < tamE && j < tamD)
    {
        if (E[i] <= D[j])
        {
            arr[k] = E[i];
            i++;
        }
        else
        {
            arr[k] = D[j];
            j++;
        }
        k++;
    }

    while (i < tamD)
    {
        arr[k] = E[i];
        i++;
        k++;
    }
}

void mergeSort (int *arr, int inicio, int fim)
{
    if (inicio < fim)
    {
        int meio = inicio + (fim - inicio) / 2;

        mergeSort(arr, inicio, meio);
        mergeSort(arr, meio + 1, fim);

        merge(arr, inicio, meio, fim);
    }
}

void *threadMerge (void *args)
{
    Dados *dados = (Dados *)args;

    mergeSort(dados->valores, dados->inicio, dados->fim);

    return NULL;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREADS 2

// Estrutura para armazenar os dados das threads
struct DadosThread
{
    int tam;      // Tamanho do array
    int *valores; // Ponteiro para o array de valores
};

typedef struct DadosThread Dados;

// Protótipos das funções
void *bubbleSort1(void *arg);
void *bubbleSort2(void *arg);
void mergeArrays(int arr1[], int n1, int arr2[], int n2, int res[]);

int main(int argc, char *argv[])
{
    pthread_t threads[THREADS];
    Dados dados[THREADS];

    int tam = argc - 1; // Tamanho do array obtido dos argumentos
    int meio = tam / 2; // Tamanho da metade do array
    int *arrMerged = (int *)malloc(tam * sizeof(int)); // Array para armazenar a mesclagem final
    int *valores = (int *)malloc(tam * sizeof(int));   // Array para armazenar os valores

    // Preenche o array de valores a partir dos argumentos da linha de comando
    for (int i = 0; i < tam; i++)
    {
        valores[i] = atoi(argv[i + 1]);
    }

    // Configuração dos dados para cada thread
    for (int i = 0; i < THREADS; i++)
    {
        dados[i].valores = valores;
        dados[i].tam = tam;
    }

    // Criação das threads
    pthread_create(&threads[0], NULL, bubbleSort1, &dados[0]);
    pthread_create(&threads[1], NULL, bubbleSort2, &dados[1]);

    // Espera pelas threads terminarem
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Exibição dos resultados das threads
    printf("\nThread 1: ");
    for (int i = 0; i < meio; i++)
    {
        printf("%d ", dados[0].valores[i]);
    }

    printf("\nThread 2: ");
    for (int i = meio; i < tam; i++)
    {
        printf("%d ", dados[1].valores[i]);
    }

    // Mescla as partes ordenadas do array
    mergeArrays(dados[0].valores, meio, dados[1].valores + meio, tam - meio, arrMerged);

    // Exibe o resultado do merge
    printf("\nResultado do merge das partes do array: ");
    for (int i = 0; i < tam; i++)
    {
        printf("%d ", arrMerged[i]);
    }

    printf("\n\n");

    // Liberação de memória alocada
    free(valores);
    free(arrMerged);

    return 0;
}

// Função de Bubble Sort para a 1ª metade do array
void *bubbleSort1(void *arg)
{
    Dados *dados = (Dados *)arg;
    int meio = dados->tam / 2;
    int aux = 0;

    for (int i = 0; i < meio; i++)
    {
        for (int j = 0; j < meio - 1; j++)
        {
            if (dados->valores[j] > dados->valores[j + 1])
            {
                aux = dados->valores[j];
                dados->valores[j] = dados->valores[j + 1];
                dados->valores[j + 1] = aux;
            }
        }
    }

    pthread_exit(NULL);
}

// Função de Bubble Sort para a 2ª metade do array
void *bubbleSort2(void *arg)
{
    Dados *dados = (Dados *)arg;
    int meio = dados->tam / 2;

    int aux = 0;

    for (int i = meio; i < dados->tam; i++)
    {
        for (int j = meio; j < dados->tam - 1; j++)
        {
            if (dados->valores[j] > dados->valores[j + 1])
            {
                aux = dados->valores[j];
                dados->valores[j] = dados->valores[j + 1];
                dados->valores[j + 1] = aux;
            }
        }
    }

    pthread_exit(NULL);
}

// Função para mesclar dois arrays ordenados
void mergeArrays(int arr1[], int n1, int arr2[], int n2, int res[])
{
    int i = 0, j = 0, k = 0;

    while (i < n1 && j < n2)
    {
        if (arr1[i] < arr2[j])
        {
            res[k++] = arr1[i++];
        }
        else
        {
            res[k++] = arr2[j++];
        }
    }

    // Adiciona os elementos restantes do arr1, se houver
    while (i < n1)
    {
        res[k++] = arr1[i++];
    }

    // Adiciona os elementos restantes do arr2, se houver
    while (j < n2)
    {
        res[k++] = arr2[j++];
    }
}

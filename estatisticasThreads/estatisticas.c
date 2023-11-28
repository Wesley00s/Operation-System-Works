#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREADS 3

// Estrutura para armazenar os dados das theads
struct DadosThread
{
    int tam;
    float *valores;
    float resultado;
};

typedef struct DadosThread Dados;

// Protótipos das funções
void *calcMedia(void *arg);
void *calcMenor(void *arg);
void *calcMaior(void *arg);

int main(int argc, char *argv[])
{
    pthread_t threads[THREADS];
    Dados dados[THREADS];

    int tam = argc - 1; // Armazena o tamanho do array a partir do contador de argumentos
    float *valores = (float *)malloc(tam * sizeof(float)); // Aloca a menória necessária

    // Preenche o array com valores da linha de comando
    for (int i = 0; i < tam; i++)
    {
        valores[i] = atof(argv[i + 1]); // Converte string para float
    }

    // Configurar dados para cada thread
    for (int i = 0; i < THREADS; i++)
    {
        dados[i].valores = valores;
        dados[i].tam = tam;
    }

    // Cria threads
    pthread_create(&threads[0], NULL, calcMaior, &dados[0]);
    pthread_create(&threads[1], NULL, calcMedia, &dados[1]);
    pthread_create(&threads[2], NULL, calcMenor, &dados[2]);

    // Esperar threads terminarem
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Exibir resultados
    printf("Maior: %.2f\n", dados[0].resultado);
    printf("Média: %.2f\n", dados[1].resultado);
    printf("Menor: %.2f\n", dados[2].resultado);

    free(valores); // Libera a memória alocada

    return 0;
}

void *calcMaior(void *arg) // Cacula o maior valor
{
    Dados *dados = (Dados *)arg;
    dados->resultado = dados->valores[0];

    for (int i = 1; i < dados->tam; i++)
    {
        if (dados->valores[i] > dados->resultado)
        {
            dados->resultado = dados->valores[i];
        }
    }
    pthread_exit(NULL);
}

void *calcMenor(void *arg) // Calcula o menor valor
{
    Dados *dados = (Dados *)arg;
    dados->resultado = dados->valores[0];

    for (int i = 1; i < dados->tam; i++)
    {
        if (dados->valores[i] < dados->resultado)
        {
            dados->resultado = dados->valores[i];
        }
    }
    pthread_exit(NULL);
}

void *calcMedia(void *arg) // Calcula o valor médio
{
    Dados *dados = (Dados *)arg;
    float soma = 0;

    for (int i = 0; i < dados->tam; i++)
    {
        soma += dados->valores[i];
    }
    dados->resultado = soma / dados->tam;
    pthread_exit(NULL);
}
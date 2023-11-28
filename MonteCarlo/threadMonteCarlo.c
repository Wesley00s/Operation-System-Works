#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

// Número de threads e pontos a serem gerados
#define NUM_THREADS 4
#define NUM_POINTS 1000000

// Estrutura de dados para armazenar informações do cálculo de Monte Carlo
typedef struct
{
    int pontosNoCirculo;      // Contagem de pontos dentro do círculo para cada thread
    int totalPontosNoCirculo; // Contagem total de pontos dentro do círculo para todas as threads
} DadosMonteCarlo;

// Função executada por cada thread
void *monteCarlo(void *data);

int main()
{
    // Inicialização da semente para o gerador de números aleatórios
    srand((unsigned int)time(NULL));

    // Declaração das threads e das estruturas de dados associadas a cada thread
    pthread_t threads[NUM_THREADS];
    DadosMonteCarlo monteCarloData[NUM_THREADS];

    // Criação e execução das threads
    for (long t = 0; t < NUM_THREADS; t++)
    {
        // Inicializa a contagem de pontos dentro do círculo para cada thread
        monteCarloData[t].pontosNoCirculo = 0;
        // Inicializa a contagem total de pontos dentro do círculo para todas as threads
        monteCarloData[t].totalPontosNoCirculo = 0;

        // Cria uma thread, associando a função monteCarlo e a estrutura de dados específica para cada thread
        pthread_create(&threads[t], NULL, monteCarlo, (void *)&monteCarloData[t]);
    }

    // Aguarda o término de todas as threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Soma os resultados das threads para obter o total de pontos dentro do círculo
    int totalPontosNoCirculo = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        totalPontosNoCirculo += monteCarloData[i].pontosNoCirculo;
    }

    // Calcula e exibe a estimativa de π
    double PI = 4.0 * totalPontosNoCirculo / NUM_POINTS;
    printf("Valor estimado de π: %f\n", PI);

    return 0;
}

// Função executada por cada thread
void *monteCarlo(void *data)
{
    // Converte o ponteiro genérico para o tipo específico de estrutura de dados
    DadosMonteCarlo *monteCarloData = (DadosMonteCarlo *)data;
    int contagemLocal = 0;

    // Gera pontos aleatórios e verifica se estão dentro do círculo
    unsigned int semente = rand();
    for (int i = 0; i < NUM_POINTS / NUM_THREADS; i++)
    {
        double x = (double)rand_r(&semente) / RAND_MAX;
        double y = (double)rand_r(&semente) / RAND_MAX;

        // Verifica se o ponto está dentro do círculo
        if (pow(x, 2) + pow(y, 2) <= 1.0)
        {
            contagemLocal++;
        }
    }

    // Atualiza a contagem de pontos dentro do círculo na estrutura de dados da thread
    monteCarloData->pontosNoCirculo = contagemLocal;
    // Atualiza a contagem total de pontos dentro do círculo para todas as threads
    monteCarloData->totalPontosNoCirculo += contagemLocal;

    // Termina a thread
    pthread_exit(NULL);
}

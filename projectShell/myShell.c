/*
Curso: Análise e Desenvolvimento de Sistemas
Prática 03 - Sistemas Operacionais: Implementação de um shell
Semestre: 2
Data: 25/10/2023
Prof: Caio Figueredo
Aluno: Wesley Rodrigues de Sousa
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Contém declarações para várias funções do sistema relacionadas
ao POSIX (Portable Operating System Interface). Aqui, chdir
(para mudar o diretório de trabalho) é usado.*/
#include <unistd.h>

/*Define tipos de dados utilizados em chamadas de sistema.
Neste código, é usado para declarar pid_t, que é o tipo de
dados para IDs de processo.*/
#include <sys/types.h>

/*Contém declarações relacionadas à espera de processos.
Aqui, waitpid é usado para esperar que um processo
filho termine no processo pai.*/
#include <sys/wait.h>

#define MAX_LINE 80       // Define o tamanho máximo da linha de comando.
#define MAX_COMMANDS 1000 // Define a quantidade máxima de comandos que podem listados no histórico.

int main(void)
{
    char input[MAX_LINE];              // Vetor para aramzenar a entrada do usuário.
    int should_run = 1;                // Flag para controlar quando o shell deve ser fechado.
    char hist[MAX_COMMANDS][MAX_LINE]; // Matriz para armazenar o histórico dos comandos.
    int command_count = 0;             //  Contador de comandos no histórico.

    // Loop principal do shell.
    while (should_run)
    {
        printf("msh >> ");

        // Obtém a entrada do usuário
        fgets(input, MAX_LINE, stdin);
        input[strcspn(input, "\n")] = 0; // Remove a quebra de linha.

        strcpy(hist[command_count], input); // Copia o comando para o histórico.

        // Verifica se o comando é para encerrar o shell.
        if (strcmp(input, "exit") == 0)
        {
            should_run = 0;
            break;
        }

        // Tokeniza a entrada em argumentos
        char *args[MAX_LINE / 2 + 1]; // Este array será usado para armazenar os argumentos tokenizados.
        int arg_count = 0;            // Rastreador do número de argumentos.

        /* Usa a função strtok para obter o primeiro token da string input usando
        um espaço em branco como delimitador. Retorna um ponteiro para o primeiro
        token encontado. */
        char *token = strtok(input, " ");
        while (token != NULL) //  Inicia um loop que continua enquanto houver tokens.
        {
            args[arg_count++] = token; // Armazena o token atual no array args e incrementa o contador de argumentos.
            token = strtok(NULL, " "); // Obtém o próximo token da string.
        }

        args[arg_count] = NULL; // Coloca um ponteiro nulo no final do array args, indicando o final dos argumentos.

        // Verifica se é um comando 'cd' (mudar diretório)
        if (args[0] != NULL && strcmp(args[0], "cd") == 0)
        {
            /* Tratamento especial para o comando 'cd'. Pois o comando "cd"
            (change directory) não pode ser executado como um processo
            separado usando execvp como outros comandos. Em vez disso,
            ele precisa ser tratado internamente pela próprio shell,
            pois altera o diretório de trabalho do próprio processo do shell. */
            if (arg_count > 1 && args[1] != NULL)
            {
                if (chdir(args[1]) != 0) // O comando "cd" é implementado usando a função chdir (change directory) do sistema.
                {
                    perror("Erro ao executar 'cd'"); // Se a mudança de diretório não for bem-sucedida.
                }
            }
            else
            {
                /* Se o usuário fornecer argumentos incorretos ou nenhum argumento
                para o comando "cd", o shell imprime uma mensagem de uso correto.*/
                fprintf(stderr, "Uso correto: cd [diretório]\n");
            }
        }
        else
        {
            if (strcmp(input, "history") == 0)
            {
                // Loop para imprimir o histórico.
                for (int i = 0; i < command_count; i++)
                {
                    printf("%d - %s\n", i + 1, hist[i]);
                }
            }
            else
            {
                // Cria um processo filho.
                pid_t pid = fork();

                if (pid == 0) // Código do processo filho.
                {
                    /* Remove "&" do último argumento, se presente.
                    A remoção do "&" do último argumento garante que execvp interprete
                    corretamente os argumentos fornecidos, mesmo quando o
                    comando é executado em segundo plano. */
                    if (arg_count > 0 && strcmp(args[arg_count - 1], "&") == 0)
                    {
                        args[arg_count - 1] = NULL; // Atribui NULL ao último caractere, indicando asssim, o fim do comando.
                    }

                    // Executa o comando
                    execvp(args[0], args);

                    // Se execvp retornar, houve um erro
                    perror("Erro ao executar o comando");
                    exit(EXIT_FAILURE);
                }
                else if (pid < 0)
                { // Se fork() retornar um valor negativo, indica que a criação do processo filho falhou.
                    perror("Erro ao criar processo filho");
                }
                /*Se "&" não estiver presente, o pai espera que o processo filho termine usando waitpid.
                Isso impede que o shell avance para a próxima entrada de comando até que o processo filho termine.*/
                else if (arg_count > 0 && strcmp(args[arg_count - 1], "&") != 0) // Código do processo pai
                {
                    // Espera o processo filho terminar, a menos que "&" esteja presente
                    waitpid(pid, NULL, 0);
                }
            }
        }
        command_count++;
    }

    return 0;
}

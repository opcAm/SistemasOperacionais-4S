#define _GNU_SOURCE

#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>

#define STACK_SIZE 1024*64

// Estrutura que faz a troca de dados entre o processo pai e a thread
typedef struct threadData {
    char info[100]; // Buffer para guardar a informação
} SubroutineData;

// funcao executada pela nova thread
int subroutine(void* arg) {
    // Transforma o argumento para o tipo correto (threadData*)
    threadData* sharedData = (threadData*)arg;
    // Exibe a msg recebida do processo principal
    printf("thread: Lendo informação do processo principal: '%s'\n", sharedData->info);

    // Modifica msg pra enviar pro processo pai
    strcpy(sharedData->info, "bom dia paizao");
    // Anuncia a finalização da threas
    printf("thread: Finalizando\n");
    return 0; // Retorna sucesso
}

// Função principal
int main() {
    void* stack; // Ponteiro para a stack da thread
    pid_t pid; // Identificador do processo da thread
    SubroutineData sharedData; 

    // Reserva espaço para a stack da thread
    stack = malloc(STACK_SIZE);
    if (stack == 0) {
        perror("malloc: erro ao alocar stack");
        exit(1); // Finaliza se não conseguir alocar
    }

    // Define a informação a ser enviada para a sub-rotina
    strcpy(sharedData.info, "bom dia, filhotao");

    // Indica a criação da thread
    printf("Iniciando thread filho\n");
    // Cria a thread com a chamada clone
    // Passa a função threadFunction, o topo da pilha, flags e a estrutura de dados como argumento
    pid = clone(&subroutine, (char*)stack + STACK_SIZE,
                SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &sharedData);
    if (pid == -1) {
        perror("clone");
        exit(2); // Finaliza se a criação da thread falhar
    }

    // Aguarda o término da thread
    pid = waitpid(pid, NULL, 0);
    if (pid == -1) {
        perror("waitpid");
        exit(3); // Finaliza se a espera falhar
    }

    // Lê a informação alterada pela thread
    printf("Processo principal lendo informação modificada: '%s'\n", sharedData.info);

    // Libera o espaço da stack da thread
    free(stack);
    // Informa o término da thread e a liberação da stack
    printf("thread filho concluída e stack liberada.\n");
    return 0; // Indica sucesso
}

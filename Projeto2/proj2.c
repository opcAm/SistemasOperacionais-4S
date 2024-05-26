#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct conta {
    char nome[10];
    int saldo;
};
typedef struct conta conta;

struct transacao {
    conta *recebe;
    conta *envia;
    int valor;
//ponteiro
    int *contador; 
};
typedef struct transacao transacao;

conta conta1;
conta conta2;
pthread_mutex_t semaforo;

void *transferir(void *args) {
    transacao *t = (transacao *)args;
    pthread_mutex_lock(&semaforo);

    printf("---------------------------------------\n");
    if (t->envia->saldo - t->valor >= 0) {
        printf("Transação de %s para %s\n", t->envia->nome, t->recebe->nome);
        printf("Valor da transação: $%d,00\n", t->valor);

        printf("%s - Saldo Anterior: $%d,00\n", t->envia->nome, t->envia->saldo);
        t->envia->saldo = t->envia->saldo - t->valor;
        printf("%s - Saldo Posterior: $%d,00\n", t->envia->nome, t->envia->saldo);

        printf("%s - Saldo Anterior: $%d,00\n", t->recebe->nome, t->recebe->saldo);
        t->recebe->saldo = t->recebe->saldo + t->valor;
        printf("%s - Saldo Posterior: $%d,00\n", t->recebe->nome, t->recebe->saldo);

        // Incrementa a contagem de transações bem-sucedidas
        (*(t->contador))++;
    } else {
        printf("Saldo Insuficiente\n");
    }

    pthread_mutex_unlock(&semaforo);
    free(t);
    return NULL;
}

int main(void) {
    srand(time(NULL));
    strcpy(conta1.nome, "Conta 1");
    conta1.saldo = 1000;
    strcpy(conta2.nome, "Conta 2");
    conta2.saldo = 1000;

    pthread_mutex_init(&semaforo, NULL);

    int totalDeTransacoes = 10;
    int transacoes_sucesso = 0; // Contador de transações bem-sucedidas
    int totalDeLoops = totalDeTransacoes / 100 + (totalDeTransacoes % 100 != 0);
    int count = 0;

}

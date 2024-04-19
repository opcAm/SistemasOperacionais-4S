// Inclui as bibliotecas necessárias para funções de entrada/saída, manipulação de processos e semáforos
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

// Define uma constante para o número máximo de pessoas
#define MAX_PESSOAS 10000

// Define uma estrutura para representar um passageiro, contendo tempo e direção
typedef struct {
    int tempo, direcao;
} Passageiro;

// Define uma estrutura para representar o estado da escada rolante
typedef struct {
    int tempoFinal;
    int direcaoAtual; // -1 indica que a escada está parada
    int contador[2]; // Contadores para pessoas esperando em cada direção
    Passageiro fila[2][MAX_PESSOAS]; // Filas de espera para cada direção
    int fimFila[2]; // Indices para o fim da fila em cada direção
} EstadoEscada;

// Função que cada processo filho executará para processar um passageiro
void processarPassageiro(Passageiro p, EstadoEscada *estado, sem_t *semDir, sem_t *semTempo) {
    // Aguarda (bloqueia) o semáforo da direção para acesso exclusivo
    sem_wait(semDir);
    // Verifica se a escada está parada ou na mesma direção do passageiro
    if (estado->direcaoAtual == -1 || estado->direcaoAtual == p.direcao) {
        estado->direcaoAtual = p.direcao;
        estado->tempoFinal = p.tempo + 10; // Atualiza o tempo final adicionando 10
        estado->contador[p.direcao]++; // Incrementa o contador da direção do passageiro
    } else {
        // Adiciona o passageiro à fila da direção oposta se a direção for diferente
        estado->fila[1-p.direcao][estado->fimFila[1-p.direcao]++] = p;
    }
    // Libera o semáforo da direção
    sem_post(semDir);

    // Aguarda (bloqueia) o semáforo do tempo
    sem_wait(semTempo);
    // Verifica se não há mais ninguém na direção atual
    if (estado->contador[estado->direcaoAtual] == 0) {
        estado->direcaoAtual = 1 - estado->direcaoAtual; // Muda a direção
        // Processa todos os passageiros na fila da nova direção atual
        for (int i = 0; i < estado->fimFila[estado->direcaoAtual]; i++) {
            estado->tempoFinal += 10;
            estado->contador[estado->direcaoAtual]++;
        }
        estado->fimFila[estado->direcaoAtual] = 0; // Reseta o fim da fila para a direção atual
    }
    // Libera o semáforo do tempo
    sem_post(semTempo);
}

// Função principal
int main() {
    // Cria e abre um segmento de memória compartilhada
    int shm_fd = shm_open("/estadoEscada", O_CREAT | O_RDWR, 0666);
    // Configura o tamanho do segmento de memória compartilhada
    ftruncate(shm_fd, sizeof(EstadoEscada));
    // Mapeia o segmento de memória compartilhada
    EstadoEscada *estado = mmap(NULL, sizeof(EstadoEscada), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Inicializa o estado da escada rolante
    estado->tempoFinal = 0;
    estado->direcaoAtual = -1;
    memset(estado->contador, 0, sizeof(estado->contador));
    memset(estado->fimFila, 0, sizeof(estado->fimFila));

    // Cria semáforos para direção e tempo
    sem_t *semDir = sem_open("/semDir", O_CREAT, 0666, 1);
    sem_t *semTempo = sem_open("/semTempo", O_CREAT, 0666, 1);

    // Cria um array para armazenar os passageiros e lê o número de passageiros
    Passageiro passageiros[MAX_PESSOAS];
    int numPassageiros;
    scanf("%d", &numPassageiros);

    // Cria processos filhos para processar cada passageiro
    for (int i = 0; i < numPassageiros; i++) {
        scanf("%d %d", &passageiros[i].tempo, &passageiros[i].direcao);
        if (fork() == 0) {
            processarPassageiro(passageiros[i], estado, semDir, semTempo);
            exit(0);
        }
    }

    // Espera todos os processos filhos terminarem
    while (wait(NULL) > 0);

    // Imprime o momento final de parada da escada
    printf("O momento final de parada da escada rolante é %d\n", estado->tempoFinal);

    // Limpa e fecha os recursos utilizados
    munmap(estado, sizeof(EstadoEscada));
    close(shm_fd);
    shm_unlink("/estadoEscada");
    sem_close(semDir);
    sem_unlink("/semDir");
    sem_close(semTempo);
    sem_unlink("/semTempo");

    return 0;
}

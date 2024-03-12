#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int main() {
    int fd[2]; // Pipe para comunicação de pai para filho
    pid_t pid;
    char msg_pai[] = "Pode ir na padaria, por favor?";
    char read_buffer[BUFFER_SIZE];

    // Cria o pipe
    if (pipe(fd) == -1) {
        perror("Pipe falhou");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork falhou\n");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { 
      //  pai
        close(fd[0]); // Fecha lado de leitura do pipe no pai
        // Envia mensagem ao filho
        if (write(fd[1], msg_pai, sizeof(msg_pai)) == -1) {
            perror("msg falhou");
            exit(EXIT_FAILURE);
        }
        printf("Pai: Mensagem enviada ao filho.\n");
        close(fd[1]); // Fecha lado de escrita do pipe
        waitpid(pid, NULL, 0); // Espera pelo processo filho específico terminar
        printf("Pai: Finalizando processo.\n");
    } else { 
      //  filho
        close(fd[1]); // Fecha lado de escrita do pipe no filho
        // Lê mensagem do pai
        ssize_t bytes_read = read(fd[0], read_buffer, sizeof(read_buffer));
        if (bytes_read == -1) {
            perror("leitura falhou");
            exit(EXIT_FAILURE);
        }
        printf("Filho: Mensagem do pai: %s\n", read_buffer);
        close(fd[0]); // Fecha leitura do pipe
        printf("Filho: Ok, pai. Estou indo para a padaria.\n");
        exit(EXIT_SUCCESS);
    }

    return 0;
}


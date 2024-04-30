#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


// matriz
int matriz[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
int vetor[3] = {1, 2, 3};
int resultado[3];

int indices[3];

// func que cada thread executará para calcular o produto da matriz pelo vetor
void *calculaProduto(void *param) {
    int index = *(int *)param;  
    resultado[index] = 0;
    for (int col = 0; col < 3; col++) {
        // multiplica cada elemento da linha da matriz pelo elemento do vetor e soma ao resultado
        resultado[index] += matriz[index][col] * vetor[col];
    }
    return NULL;  
}

int main() {
    pthread_t ids[3];  //identificadores das threads

    // cria thread para cada linha da matriz
    for (int i = 0; i < 3; i++) {
        indices[i] = i; 
        pthread_create(&ids[i], NULL, calculaProduto, (void *)&indices[i]);  
    }

    // espera todas as threads terminarem
    for (int i = 0; i < 3; i++) {
        pthread_join(ids[i], NULL);  // espera a thread especificada terminar
    }

    printf("Resultado da multiplicação: \n");
    for (int i = 0; i < 3; i++) {
        printf("%d ", resultado[i]);  // cada elemento do vetor resultado
    }
    printf("\n");
    return 0;
}

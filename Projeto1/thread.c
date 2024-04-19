#include <stdio.h>
#include <pthread.h>

// Define matrizes para armazenar os tempos de chegada das pessoas para cada direção
int tempos_chegada[2][10000];
// Vetor para armazenar a quantidade de pessoas em cada direção
int contadores_direcoes[2] = {0, 0};
// Variável para armazenar a direção atual de operação da escada
int direcao_atual;
// Variável para armazenar o tempo atual no contexto da execução da escada
int tempo_atual;
// Variável para armazenar o tempo final de execução após processar todos os passageiros
int tempo_final;

// Função executada pela thread que simula a operação da escada rolante
void *escada_rolante() {

  // Variável para armazenar a direção oposta à direção atual
  int direcao_oposta;
  // Vetor para armazenar o índice atual de processamento em cada direção
  int indices[2] = {0, 0};
  // Variável para manter registro do último tempo em que uma pessoa começou a usar a escada
  int ultimo_tempo_partida = 0;

  // Define o tempo inicial com base no primeiro passageiro na direção inicial
  tempo_atual = tempos_chegada[direcao_atual][0];

  // Processa enquanto houver pessoas em qualquer uma das direções
  while (indices[0] < contadores_direcoes[0] || indices[1] < contadores_direcoes[1]) {
    // Calcula a direção oposta
    direcao_oposta = 1 - direcao_atual;
    
    // Verifica se a pessoa atual pode partir no tempo registrado
    int partida_valida = tempos_chegada[direcao_atual][indices[direcao_atual]] <= ultimo_tempo_partida;

    if (partida_valida) {
      // Atualiza o tempo atual com o tempo de chegada da pessoa sendo processada
      tempo_atual = tempos_chegada[direcao_atual][indices[direcao_atual]];
      // Incrementa o índice para a direção atual
      indices[direcao_atual]++;
    } 
    else {
      // Define o novo tempo atual baseado no maior entre o último tempo de partida e a chegada da próxima pessoa na direção oposta
      int proximo_tempo_partida_oposta = tempos_chegada[direcao_oposta][indices[direcao_oposta]];
      tempo_atual = (ultimo_tempo_partida > proximo_tempo_partida_oposta) ? ultimo_tempo_partida : proximo_tempo_partida_oposta;
      // Incrementa o índice para a direção oposta
      indices[direcao_oposta]++;
    }

    // Atualiza o último tempo de partida adicionando 10 segundos ao tempo atual
    ultimo_tempo_partida = tempo_atual + 10;
    // Alterna a direção atual para a próxima iteração
    direcao_atual = 1 - direcao_atual;
  }

  // Define o tempo final adicionando 10 segundos ao último tempo de partida
  tempo_final = ultimo_tempo_partida + 10;
}

int main() {

  // Abre o arquivo de entrada para leitura
  FILE *arquivo_entrada, *arquivo_saida;
  arquivo_entrada = fopen("input.txt", "r");
  if (arquivo_entrada == NULL) {
    printf("Erro ao abrir o arquivo de entrada.\n");
    return 0;
  }

  // Lê o número de pessoas e armazena os tempos de chegada e direções
  int numero_pessoas, tempo, direcao_entrada;
  fscanf(arquivo_entrada, "%d", &numero_pessoas);

  for (int i = 0; i < numero_pessoas; i++) {
    fscanf(arquivo_entrada, "%d %d", &tempo, &direcao_entrada);
    if (i == 0) {
      direcao_atual = direcao_entrada; // Define a direção inicial baseada na primeira pessoa
    }
    tempos_chegada[direcao_entrada][contadores_direcoes[direcao_entrada]] = tempo;
    contadores_direcoes[direcao_entrada]++;
  }

  // Fecha o arquivo de entrada após a leitura
  fclose(arquivo_entrada);

  // Cria uma thread para processar a simulação da escada rolante
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, escada_rolante, NULL);
  pthread_join(thread_id, NULL);

  // Abre o arquivo de saída para escrever o resultado da simulação
  arquivo_saida = fopen("output.txt", "w");
  if (arquivo_saida == NULL) {
    printf("Erro ao abrir o arquivo de saída.\n");
    return 0;
  }

  // Escreve o tempo final no arquivo de saída
  fprintf(arquivo_saida, "%d\n", tempo_final);
  // Fecha o arquivo de saída
  fclose(arquivo_saida);

  return 0;
}

# Amanda Oliveira Pires Conde
## RA: 10409108

### Como compilar:
```
Instalar o editor vim:
sudo dnf install vim

Abrir e editar o programa com o vim:
vim projeto2.c

Instalar o compilador gcc:
sudo dnf install gcc

Compilar o arquivo .c
gcc -o proj2 projeto2.c

Executar o programa:
./proj2
```
## Comprovando os resultados que foram propostos:


![image](https://github.com/opcAm/SistemasOperacionais-4S/assets/134961931/aa0483a3-3d4c-49f4-951d-73c373b7e964)
![image](https://github.com/opcAm/SistemasOperacionais-4S/assets/134961931/3e756dc9-e4ee-4fa3-90ff-690671f256dd)

## A solução cumpre os tópicos abaixo:
1- A conta to pode receber mais de uma transferência simultânea;

2- A conta from pode enviar mais de uma transferência simultânea;

3- A conta from não pode enviar dinheiro se não tiver mais saldo;

4- A conta to pode trocar de ordem com a conta from, ou seja, a conta que enviava pode receber e a conta que recebia pode enviar;

5- Poderão ser realizadas até 100 transações simultâneas de transferência.

# Principais elementos do código:
## Definição das Estruturas de Dados:

conta:
<img width="165" alt="Captura de Tela 2024-05-26 às 18 37 31" src="https://github.com/opcAm/SistemasOperacionais-4S/assets/134961931/28db2a4d-7bfd-47ab-869d-9838dea6c7f8">

transacao:
<img width="278" alt="Captura de Tela 2024-05-26 às 18 38 38" src="https://github.com/opcAm/SistemasOperacionais-4S/assets/134961931/1a905e9d-8651-49e5-a3de-82b5d9434c4b">

## Declaração de Contas e Mutex:
```Conta conta_A; e Conta conta_B;```: Duas contas bancárias iniciais.
```pthread_mutex_t mutex;```: Um mutex para garantir a sincronização das operações de transferência.

## Função de Transferência:
```void *realizar_transferencia(void *argumento)```: Função executada por threads que realiza a transferência de dinheiro entre contas com uso de mutex para evitar condições de corrida.

## Função Principal (main):

- Inicialização da conta;
- Inicialização do mutex;
- Configuração de variáveis para controle de transações;
- Loop principal para criação e execução de threads;
- Esperar pelas threads para finalizar;
- Destruição do mutex;
- Impressão dos resultados finais;
- Finaliza o programa.

## Solução:
O código implementa um sistema de transferência de dinheiro entre duas contas usando threads e mutex para garantir a sincronização. As contas são inicializadas com saldos e nomes, e threads são criadas para simular transações. O mutex garante que apenas uma thread execute uma transferência por vez, prevenindo condições de corrida e assegurando a integridade dos dados. O saldo é verificado antes de cada transferência, impedindo transações insuficientes. O sistema permite simular múltiplas transações simultâneas de forma segura.

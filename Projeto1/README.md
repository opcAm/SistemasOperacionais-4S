### 1. Qual a estratégia que você utilizou para evitar que duas pessoas acessem a escada rolante ao mesmo tempo em cada abordagem?

Thread- Para prevenir acessos simultâneos à escada rolante, o código utilizando as threads gerencia o acesso através de variáveis globais que controlam o tempo de ocupação e a direção atual da escada. Quando uma pessoa inicia o uso da escada, o tempo de ocupação é atualizado para refletir um intervalo durante o qual ninguém mais pode entrar, e a direção é mantida constante. A lógica implementada garante que enquanto uma pessoa está na escada, seu tempo e direção permanecem bloqueados para outros, e a transição de direção só ocorre quando o tempo expira


Processos- O código de processos utiliza uma estratégia similar de prevenção de acesso simultâneo, com a direção e o tempo de ocupação controlados. No entanto, a comunicação entre os processos é realizada por meio de pipes, uma abordagem que isola mais os dados mas introduz latência e complexidade adicional. Quando um processo filho altera o estado da escada, como a extensão do tempo de ocupação, ele precisa enviar essas informações de volta ao processo pai através do pipe, o que pode introduzir atrasos.


### 2. Como garantir que somente uma das direções está ativa de cada vez em cada uma das abordagens?
-Threads

A exclusividade da direção é controlada também por uma variável global direction, que é acessada e modificada pelas threads. A mudança de direção só é permitida após o término do período de uso, evitando que pessoas em direções opostas usem a escada ao mesmo tempo.

-Processos

A garantia de que apenas uma direção está ativa de cada vez é assegurada pelo uso sincronizado do pipe. Mudanças na direção são efetuadas somente depois que todos os dados relevantes são transmitidos e recebidos, garantindo que não haja conflito entre as direções de uso da escada.


### 3. Discorra sobre as diferenças entre as implementações utilizando threads e processos e diga qual foi mais eficiente na solução do problema, justificando sua resposta.

Comparando as implementações com threads e processos para controlar o acesso à escada rolante, a abordagem com threads se destaca pela eficiência. Threads compartilham memória e sincronizam variáveis globais rapidamente, resultando em menos overhead e uma resposta mais ágil. Por outro lado, processos exigem comunicação via pipes, introduzindo atrasos e complexidade, além de consumirem mais recursos devido ao isolamento de memória. Portanto, para este caso específico, a implementação com threads é mais eficiente, proporcionando um controle de concorrência mais direto e eficaz.

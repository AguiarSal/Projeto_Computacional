# Projeto_Computacional
Este projeto é uma réplica do jogo clássico Space Invaders. Ele é implementado em C++ usando apenas bibliotecas padrão de C/C++ e a API do Windows (Windows.h). Nenhuma outra biblioteca externa é utilizada, garantindo que o projeto seja construído do zero.
## Indice

+ Introdução
+ Recursos
+ Instalação
+ Uso
## Introdução

A réplica do Space Invaders é um projeto que visa recriar o jogo de arcade clássico usando práticas modernas de C++. O jogo inclui um loop de jogo totalmente funcional, gerenciamento de sprites, detecção de colisão e renderização usando a API Direct3D.
## Recursos

+ **Programação Orientada a Objetos:** Utilizada extensivamente para diversos elementos do jogo.
+ **Gerenciamento de Janela Personalizado:** Lida com a criação de janelas, processamento de entrada e renderização usando a API do Windows.
+ **Manipulação de Sprites:** Gerencia os sprites do jogo, incluindo o carregamento de texturas e a renderização.
+ **Gerenciamento de Cena:** Lida com diferentes objetos do jogo, incluindo objetos estáticos e em movimento, e gerencia suas interações.
+ **Integração com Direct3D:** Usa Direct3D para renderizar objetos do jogo, garantindo um desempenho gráfico suave.
+ **Detecção de Colisão:** Implementa a detecção de colisão para várias formas (pontos, retângulos, círculos) para gerenciar a lógica do jogo.
+ **Uso de Estruturas de Dados (Listas):** As listas foram utilizadas para organizar e gerenciar objetos na cena do jogo. Elas permitem que o código trate de maneira eficiente a adição, remoção, atualização e detecção de colisões entre objetos estáticos e móveis.
## Instalação

Para construir e executar o projeto, você precisa ter um ambiente Windows com as ferramentas de desenvolvimento necessárias instaladas.
### Pré-requisitos

    Visual Studio com ferramentas de desenvolvimento C++
    Windows SDK

### Construindo o Projeto
1. Clone o repositório:
`git clone https://github.com/seuusuario/space-invaders-replica.git`
2. Abra o projeto no Visual Studio.
3. Construa a solução selecionando Build > Build Solution no menu.

### Executando o Projeto

1. Após construir a solução, execute o executável a partir do Visual Studio pressionando F5 ou selecionando Debug > Start Debugging.

## Uso

### Controles
+ **Teclas de Seta:** Move a nave do jogador para a esquerda e direita.
+ **Barra de Espaço:** Atira.

### Mecânica do Jogo

+ **Nave do Jogador:** Controlada pelo jogador para atirar nos invasores.
+ **Invasores:** Movem-se pela tela e descem em direção ao jogador. O jogador deve atirar neles antes que cheguem ao fundo.
+ **Pontuação:** Pontos são atribuídos para cada invasor destruído.

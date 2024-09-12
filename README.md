# Projeto HADOUKEN

Autores: Matheus Barros, Lucas Oliveira, Renato Laffranchi Falcão

Desenvolvolvimento de um controle remoto baseado nas tradicionais maquinas
arcade.

## Ideia

<!--  Descreva aqui em poucas palavras qual a ideia do seu controle. Se ele vai ser de jogo ou de aplicação -->
O nosso projeto consiste em construir um controle para jogos de Arcade, utilizando o Microcontrolador SAME70-XPLD, periféricos, componentes eletrônicos e os conhecimentos adquiridos no curso de Computação Embarcada.

## Nome

<!-- De um nome ao seu controle! -->
Controle Ryu

## Usuários

<!-- Descreva aqui quem seriam os possíveis usuários deste controle. -->
Amantes dos jogos de Arcade que querem sentir aquela boa nostalgia ou Gamers mais modernos que apreciam a tecnologia retrô.

## Software/Jogo

<!-- Qual software que seu controle vai controlar? -->
O software a ser controlado será o Emulador MAME para jogar, em especial, o jogo Street Fighter, com a possibilidade de jogar também os outros jogos disponibilizados pelo emulador.

## Jornada dos usuários

Caso 1: O usuário se encontra em uma faixa etária na qual durante sua juventude eram populares os chamados fliperamas, máquinas que continham um visor, alguns botões e talvez o elemento mais icônico, o joystick.
Hoje, com a popularização de consoles e computadores capazes de rodar os mais complexos jogos, é difícil achar uma máquina de fliperama, especialmente em cidades menores. A alternativa que sobra para o usuário, nostálgico da experiência que marcou parte de sua vida, são emuladores para computador.
Tal solução, entretanto, se demonstra incompleta, uma vez que jogar com um teclado difere muito da experiência original. Como solução, surge o controle a ser desenvolvido pelo time que visa ao máximo reviver os controles físicos dos antigos fliperamas.

Caso 2: O usuário se encontra em uma faixa etária na qual fliperamas já não são/eram mais populares na sua juventude. Mesmo com muitas opções de entretenimento, o usuário gostaria de experimentar algo diferente do que é oferecido por consoles e computadores. Procura então por jogos mais antigos e se depara com emuladores de fliperama, porém ainda se encontra insatisfeito por não conseguir ter toda a experiência da máquina de jogos. O controle a ser desenvolvido pelo time vem para completar o que faltava para que o usuário fuja do comum e tenha uma imersão completa em outra forma de jogar.

## Comandos/ Feedbacks

<!-- 
Quais são os comandos/ operacões possíveis do seu controle?

Quais os feedbacks que seu controle vai fornecer ao usuário?
-->

Comandos que podem ser realizados com o controle:

- Ações com os botões X, Y , A , B, RB, LB, RT e LT. Possum funcionalidades dentro do jogo, como por exemplo pular, bater e soltar poderes especiais.

- "Coin Button", que simula o crédito colocado na máquina de fliperama para iniciar o jogo.

- Movimentação do personagem no jogo.

- Controle do volume do computador em 4 níveis com o uso de um potenciômetro na lateral da
  caixa (entrada analógica)

Feedbacks que podem ser devolvidos pelo controle:

- Acender LEDs em diferentes cores quando determinados botões forem clicados.

## In/OUT

<!--
Para cada Comando/ Feedback do seu controle, associe qual sensores/ atuadores pretende utilizar? Faca em formato de lista, exemplo:

- Avanca música: Push button amarelo
- Volume da música: Fita de LED indicando potência do som
-->

Entradas Digitais:

- Botões de ação: 8 botões de pressão coloridos.
- Botão de energia: "Coin Button".
- Joystick: movimentação do personagem.

Entrada Analógica:

- Potenciômetro para alterar o volume do jogo.

Saídas Digitais:

- Acender LEDs em diferentes cores quando determinados botões forem clicados.

## Design
<img src="https://user-images.githubusercontent.com/15271557/198908819-728bbee0-77de-4ec5-8e0c-bd3db5ab3a85.png" width="500">

### Relação de pinos utilizados

Nome|Pino|Input/Output
---|---|---
Botão X|PC31|Input
Botão Y|PA19|Input
Botão A|PB3|Input
Botão B|PB2|Input
Botão RB|PC19|Input
Botão LB|PC13|Input
Botão RT|PD11|Input
Botão LT|PD27|Input
Botão Coin|PA21|Input
Joystick UP|PD11|Input
Joystick DOWN|PB10|Input
Joystick LEFT|PB11|Input
Joystick RIGHT|PB12|Input
Potenciômetro|PB11|Input
LEDs|PD28|Output




## Vídeos

### Em Uso [Link](https://youtu.be/yCXOoS1pK4o)

[<img src="https://user-images.githubusercontent.com/15271557/198908619-ab1cbdab-7999-437f-9007-fee7f0ce5355.png" width="500">](https://youtu.be/yCXOoS1pK4o)

### Interação do Usuário [Link](https://youtu.be/Ns-hOR85Mq0)

[<img src="https://user-images.githubusercontent.com/15271557/198908966-eee990e9-d1f7-45cf-b982-692938ca1332.png" width="500">](https://youtu.be/Ns-hOR85Mq0)


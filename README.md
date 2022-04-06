# 22a - EMB - AV1

- A prova é prática, com o objetivo de avaliar sua compreensão a cerca do conteúdo ministrado pela disciplina. 
- É permitido consulta a todo material pessoal (suas anotações, códigos) e publico (github e internet), mas não pode consultar outros alunos.
- **Lembre que e' boa prática a cada 30 minutos fazer um commit no seu código!**
- Duração total: 2h + 1h extra

>  Criar o repositório de entrega pelo link:
>  - https://classroom.github.com/a/cFcTMHtX
>
>  Use como base o código disponível neste repositório.

Você vai precisar:

- ▶️ Kit SAME70-XPLD
- ▶️ Conectar o OLED1 ao EXT-1

### Ideia geral

![](https://m.media-amazon.com/images/I/81Qlt6DNbCL._AC_SX522_.jpg)

Nesta avaliacão vocês vão desenvolver um protótipo de um controlador para um micro-ondas, que em linhas gerais irá possuir as seguintes interfaces:

- Display OLED
- Botão de +5 segundos
- Botão de +10 segundos
- Sensor de porta aberta
- Controle da luz interna 
- Sistema de potência (liga micro-ondas)
- Buzzer 

A seguir iremos detalhar um pouco mais como deve ser feito o protótipo e o que estará sendo avaliado.

## Micro-ondas

O funcionamento do micro-ondar é similar com qualquer outro, o usuário primeiramente precisa acrescentar um tempo utilizando os botões de `+5 segundos` ou `+10 segundos` (botões 3 e 2 do OLED). O Micro-ondas liga instantaneamente depois do tempo configurado e somente se a porta estiver fechada. Oo LED 2 da placa OLED informa que o micro-ondas está ligado. O sistema de potência deve ficar aceso pelo tempo em que o sistema ficar ligado, e deve desligar passado o tempo ou se a portar for aberta.

A seguir detalhes do funcionamento:

![](diagrama.png)

Requisitos funcionais:

- **Display OLED:** Exibe o tempo configurado pelo usuário, deve ser atualizado de segundo em segundo quando o micro-ondas estiver ligado. Por padrão deve exibir `00:00`.
- **Cronometro:** Deve ser o valor configurado pelo usuário, sendo que o valor máximo deve ser 60 minutos : 60 segundos. 
- **Sensor de porta aberta:** Em qualquer momento se a portar do micro-ondas for aberta o sistema de potência deve parar. Junto com ele o contador de tempo, a luz interna deve acender.
    - `OLED BUTTON 1`
    - Ação nas bordas: descida E subida
- **Botão de +10 segundos:** Adiciona 10 segundos ao cronometro, deve funcionar com o micro-ondas em operação.
   - `OLED BUTTON 2`
   - Ação na borda de descida 
- **Botão de +5 segundos:** Adiciona 5 segundos ao cronometro, deve funcionar com o micro-ondas em operação.
   - `OLED BUTTON 3`
   - Ação na borda de descida 
- **Luz interna:** É a luz interna do Micro-ondas, acesa sempre que a porta estiver aberta ou o micro-ondas ligado
    - `OLED LED 1`
- **Potência:** Indica que o micro-ondas está ligado
    - `OLED LED 2`
    - [ ] Aceso: Micro-ondas ligado
    - Apagado: Micro-ondas desligado
- **Buzzer:** Deve piscar a uma frequência de 10Hz até o usuário abrir a porta do micro-ondas ou adicionar um novo tempo.
    - `OLED LED 3`
- **Ligando o micro-ondas:** O Micro-ondas deve começar operar instantaneamente quando um valor de tempo estiver definido (t > 0s) e a porta fechada.

-------------------

🎥 ⚠️ ❗ Assista o video no youtube: https://www.youtube.com/watch?v=_OgLylrLAV4&feature=youtu.be 🎥 ⚠️ ❗

-------------------

Requisitos de software:

- Não atualiza OLED em IRQ
- Todos os botões com IRQ
- Lógica implementada na função main
- Usar TC com 1hz para a contagem do cronometro
- Usar TC para fazer o buzzer piscar após o fim do tempo

## Entrega

Fazer um commit e push no repositório e preencher o formulário a seguir:

📰 https://forms.gle/Km3EaeCuPNZaYqBB8

## Aumentando a nota

Para aumentar a nota vocês devem fazer os itens extras
seguindo a tabela a seguir:

| Nota | Itens                                         |
|------|-----------------------------------------------|
| C    | Requisitos do micro-ondas                     |
| C+   | Um item do básico                             |
| B    | Um item do avançado                           |
| B+   | Um item do avançado e um item do básico       |
| A    | Dois itens do avançado e um item do básico    |
| A+   | Dois itens do avançado e dois itens do básico |

## Extra básico 

- B0: Incremento rápido quando os botões ficarem pressionados
- B1: Adicionar forma de cancelar operação: Apertar os dois botões (+10 e + 5) ao mesmo tempo limpa o valor do cronometro e desliga o micro-ondas
- B2: Código estruturado em funções:
    ```
    /**
     * Incrementa contadores de minuto e segundo (p_minuto, p_segundo)
     * dado um valor de incremento em segundos (inc_segundos)
     *
     * retorno: a função retorna se o incremento foi um sucesso ou não 
     * (estourou o tempo)
     **/
     int inc_counters(int *p_minuto, int *p_segundo, int inc_segundos);
    
     /**
     * Decrementa contadores de minuto e segundo (p_minuto, p_segundo)
     * dado um valor de decremento em segundos (dec_segundos)
     *
     * retorno: a função retorna se o decrementro foi um sucesso ou não 
     * (00:00)
     **/
     int dec_counters(int *p_minuto, int *p_segundo, int dec_segundos);
     
     /**
      * Atualiza OLED com os valores de minuto e segundo
      **/
      void update_oled(int minuto, segundo);
    ```
    
## Extra avançado

Implemente fazendo uso do periférico RTT.

- A0: Luz interna fica acesa por apenas 15 segundos após o fim do tempo (usar RTT).
- A1: Buzzer apita por 10 segundos e fica 10 segundos sem apitar, repete o ciclo (usar RTT)
- A2: O micro-ondas liga somente após passado 3 segundos da configuração de um novo tempo (usar RTT).

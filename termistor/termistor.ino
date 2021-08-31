/*Código Desenvolvido por Lucas Mire Muffato
https://github.com/lmuffato/

/* Esse códio é para os termistores NTC, ou seja, os resistores que diminuem a
resistência elétrica com o aumento da temperatura.

O termistor usado foi um Termistor NTC, Modelo TTC 104 da marca TSK.


As informações sobre a Resistência Nonimal, a Temperatura Nominal e o Coeficiente Beta do Termistor
são obtivos através do datasheet do modelo a ser utilizado.

O datasheet do Termistor está no link abaixo:
https://datasheetspdf.com/pdf-file/768616/TKS/TTC-104/1

// <-- ESQUEMA ELETRÔNICO -->

Para proteger o termistor e diminuir a tensão, será utilizado um resistor de 10k Ohm,
ligado em série com o termistor, conforme o esquema abaixo: 

  |------T------|
  |             |
  |      |--R---|
  |      |      |
  5v    GND    A0

5v: Saída de 5v do arduino;
GND: Referência de tensão 0;
R: Resistor de 100k Ohm;
T: Termistor (sensor de temperatura);
A0: Porta analógica 0.

// <-- CONSIDERAÇÕES INICIAIS -->

A entrada analógica A0 reconhece o valor da tensão, sendo 1023 para o 5v.

Convertendo o valor analógico em em tensão, utilizando uma proporção:
Se 1023 equivale a 5 volts, o valor lido no pino analógico será uma tensão proporcional:

(Va) Valor de Entrada = Va
(Vf) Tensão de Saída = Vf

[ Vf = (( Va . 5 )/1023) ]

// <-- DETERMINANDO A RESISTÊNICA NO TERMISTOR -->

Para determinar a tensão de saída, será utilizado a Lei de Ohm em um circuito de resistência em série.

(RT) = Resistência do Termistor = RT
(R1) = Resistência do Resistor Associado em série com o Termistor; (nesse caso, um resistor de 10 k)
(Vi) = Tensão de Entrada = 5 v
(Vf) = Tensão de Saída = Vf

[ RT = ((R1 . Vi) / Vf) - R1 ]

// <-- DETERMINANDO A TEMPERATURA -->

A Temperatura no Termissor não varia de maneira linear, ou seja, ela não aumenta na mesma proporção
da variação de resistência.

A relação da temperatura e da resistência do Termistor é descrita pela Equação do Fator Beta,
que é a função utilizada para determinar a variação da resistência do Termistor
pela variação da Temperatura:

(T)  = Temperatura atual no termissor;
(T0) = Temperatura Nominal ( 25° Celcius );
(R0) = Resistência Nominal ( 100Ohm a 25° Celcius );
(RT) = Resistência atual do Resistor;
(B)  = Fator Beta;
(ln) = Logaritimo Natural (Log na base "e")

Tk = 1/( (1 / (T0 + 273.15) ) + (( log(RT / R0) )/B) );

*/

// <-- AQUI INICIA O CÓDIGO DO PRGRAMA -->

/* Os códigos antes do void setup() determinam os parâmetros fixos de como o
arduino funcionará após a inicialização. */

// <-- INCLUI A BIBLIOTECA DO DISPLAY DE LCD -->

/* A biblioteca do Display de LCD já trás os códigos prontos para a utilização de comandos do
display de LCD, dispensando a necessidade de ter que criar códigos para as funções básicas 
como escrever letras, textos ou números. */

#include <LiquidCrystal.h>

/* Determinar quais pinos digitais da placa do Arduino serão usados, na ordem
sequencial da esquerda para a direita. */

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// <-- INICIA O CÓDIGO QUE SERÁ EXECUTADO APENAS UMA VEZ -->
void setup()

{
//Abre a porta serial e define a taxa de dados para 9600 bites por segundo.

/*Esse comando faz o arduino se comunicar com o computador, enviando qualquer informação
desejada para o computador a uma taxa de 9600 bites por segundo.
Esse comando serve para exibir qualquer informação desejada no monitor serial do arduino, afinal
sem abrir uma comunicação, o monitor serial não terá o que exibir.

A escolha da taxa de bites por segundo, depende da quantidade de informação que será enviada.
Quanto maior a quantida de infomração, maior deve ser a taxa.*/

Serial.begin(9600);
  
// Ativa o display, usando as 16 colunas e as 2 linhas
lcd.begin(16, 2);

// Limpa a tela do LCD de quaisquer dados anteriores.
lcd.clear();

}

// Inicia o código que será repetido infinitamente.
void loop()

{
// Início do Código do loop.  

// <-- CRIAÇÃO DAS VARIÁVEIS -->

/*
float -> Significa que a variável terá um valor decimal;
Int -> Significa que a variável terá um valor inteiro;
*/  
float T;                    // (T)  = Temperatura atual no termissor graus celsius
float Tk;                   // (T)  = Temperatura atual no termissor kelvin
float Va;                   // (Va) = Valor de Entrada
float Vf;                   // (Vf) = Tensão de Saída
float RT;                   // (RT) = Resistência atual do Termistor

// Aqui determina que o valor da variável "Va" será o mesmo valor lido na porta analógica A0.
Va = analogRead(A0);

//Criação de variáveis para determinação do valor médio.
float soma = 0;             // Soma das Amostras.
int n = 5;                  // Número de amostras.
float Vm = 0;               // Média Aritimética das Amostras.
float amostra[n];           // Amostra.
  
// As amostras começam a ser contadas a partir do número zero.
// Enquanto o número de amostras coletadas não é igual ao número determinado, coleta-se mais uma amostra.
for (n=0; n< 5; n++)

// Cada amostra terá o valor da porta analógica A0.
{
  amostra[n] = analogRead(A0);
 // O programa espera 10 milissegundos para coletar a próxima amostra.
 delay(10);
}

// Soma as amostras começando da amostra nº zero, somando a próxima amostra até que o número da amostra seja o número 5.
for (n=0; n< 5; n++){soma += amostra[n];}

// Faz a média aritimética das amostras somando o valor das cinco amostras e dividindo pelo número de amostras coletadas. 
Vm = (soma/5);

// <-- CÁLCULO PELO COEFICIENTE BETA -->

// Criação das variáveis correspondentes aos parametros de acordo com cada situação:

float B  = 4400;            // (B)  = Fator Beta.
float T0 = 25;              // (T0) = Temperatura Nominal em graus celsius.
float R0 = 100;             // (R0) = Resistência Nominal em K Ohm.
float Ri = 10;              // (R0) = Resistência do Resistor Associado em Série com o Termistor K OhmK.
float Vi = 5;               // (Vi) = Valor da Tensão Inicial ou Tensão de Entrada.

// Convertendo o valor da média de cinco amostras do valor analógico em tensão:
Vf = ((Vm * Vi) / 1023);                               // [ Vf = ( ( Vm . Vi ) / 1023 ) ]

// Determinando a Resistência do Termissor pela Diferença de Tensão:
//RT = ((( Ri * Vi) / Vf) - Ri);                       // [ RT = (( R1 . Vi )/ Vf) - R1 ]

RT = ((Ri*(Vi - Vf))/Vf);

// Determinando a Temperatura no Termistor pela Equação do Fator Beta:
/*
A equação foi formulada com a temperatura em  kelvins, e por essa razão é preciso converter 
a temperatura de graus celsius para kelvins. Para isso, isso adiciona-se 273.15 na temperatura
nominal do termistor.
*/ 

Tk = 1/( (1 / (T0 + 273.15) ) + (( log(RT / R0) )/B) );

/* OBS.: No arduíno a função log(x) defeine a função de logarítimo natural.
Para utilizar outra base, como 10, é necessário especificar como log10. */

// A temperatura encontrada é convertida de kelvins para graus celsius.
T = (Tk - 273.15);

// <-- EXIBIR A TEMPERATURA NO DYSPLAY DE LCD -->

// Move o Cursor do LDC para Coluna 0 e a linha 0.
lcd.setCursor(0, 0);

// Ordena que o LDC escrever o texto entre aspas.
lcd.print("Temperatura:");

// Move o Cursor do LDC para Coluna 0 e a linha 1.
lcd.setCursor(0, 1);

// Ordena que o LDC escreve o valor da variável "T".
lcd.print(T);

// escreve um espaço em branco logo depois do dado anterior.
lcd.print(" ");

// escreve um "C" logo depois do dado anterior.
lcd.print("C");

// <-- EXIBIR A TEMPERATURA NA PORTA SERIAL (NO COMPUTADOR) -->

// Ordena que o programa escreva "Temperatura: " em uma linha na tela serial do aplicativo do arduino no computador.
Serial.print("Temperatura por Fator Beta: ");

// Ordena que o programa escreva o valor da variável "T" na mesma linha na tela serial do aplicativo do arduino no computador.
Serial.println(T);

// Ordena que o programa escreva "Tensão: " em uma nova linha tela serial do aplicativo do arduino no computador.
Serial.print("Tensao ");

// Ordena que o programa escreva o valor da variável "Vf" na mesma linha tela serial do aplicativo do arduino no computador.
Serial.println(Vf);

// Ordena que o programa escreva "Valor Analógico: " em uma nova linha tela serial do aplicativo do arduino no computador.
Serial.print("Valor Analogico: ");

// Ordena que o programa escreva o valor da variável "Va" na mesma linha tela serial do aplicativo do arduino no computador.
Serial.println(Va);

// Ordena que o programa escreva "Valor Analógico: " em uma nova linha tela serial do aplicativo do arduino no computador.
Serial.print("Resistência do Termistor: ");

// Ordena que o programa escreva o valor da variável "Va" na mesma linha tela serial do aplicativo do arduino no computador.
Serial.println(RT);


// Espera 500 milissegundos, ou 0,5 segundos.
delay(1000);
}
// Final do Código do loop.
// A partir o conchetes o código se inicia novamente.

// Bibliotecas
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h> 
#include <Servo.h>
#include <DHT.h>

//Definições das entradas e funções do display
#define endereco  0x27
#define colunas   20
#define linhas    4
// configurações para o sensor DHT22 
#define DHTPIN 6 //PINO DIGITAL UTILIZADO PELO DHT22
#define DHTTYPE DHT22 //DEFINE O MODELO DO SENSOR (DHT22 / AM2302)
DHT dht(DHTPIN, DHTTYPE); //PASSA OS PARÂMETROS PARA A FUNÇÃO
// definindo portas para os componentes
#define ldr A2
#define LEDV 11 
#define LEDA 10
#define POTV A0
#define POTA A1
#define chave 12
#define servo 9

//Variaveis de controle
bool flag = false;
bool flagLed = false;
//variavel LDR
int R;
//Variavel potenciometros que controlam os leds V= vermelho A= azul
unsigned int ValorPotV;
unsigned int ValorPotA;

unsigned long delayIntervalo;
//Variavel para aparecer em porcentagem a intensidade dos leds
unsigned int porcentagemLedV;
unsigned int porcentagemLedA;
//configuração do display
LiquidCrystal_I2C lcd(endereco, colunas, linhas);
//configuração teclado matricial
const byte LINHAS = 4; 
const byte COLUNAS = 4; 

const char TECLAS_MATRIZ[LINHAS][COLUNAS] = {
 
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

const byte PINOS_LINHAS[LINHAS] = {13, 8, 7, 0}; // Pinos de conexao com as linhas do teclado
const byte PINOS_COLUNAS[COLUNAS] = {5, 4, 3, 2}; // Pinos de conexao com as colunas do teclado

Keypad teclado_personalizado = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS); // Inicia teclado
// criando variavel para o controle do servo motor e instanciando objetos
Servo servo1;
// criando variavel para o sensor de temp e umidade e instanciando objetos


void setup() {
  Serial.begin(9600);

  servo1.attach(servo);

  dht.begin();
  delay(2000);
  bool liga = digitalRead(chave);
  if (liga == true){
    inicio();
  }
  iniciaPortas(); // chama a função para iniciar as portas do arduino
}

void loop() {

  bool liga = digitalRead(chave);
  char leitura_teclas = teclado_personalizado.getKey(); // Atribui a variavel a leitura do teclado
  R =map (analogRead(ldr), 8, 1015, 1000, 0); // Lê o valor do LDR
  
  if (liga == true) {
    if (leitura_teclas == '2') { // caso aperte a tecla 2 mostrara a lista de nutrientes da mistura
      flagLed = false;
      nutrientes();
  }
  
    if (leitura_teclas == '3') {
    lcd.clear();
    lcd.print("Umidade: "); //IMPRIME O TEXTO NO DISPLAY
    lcd.print(dht.readHumidity()); //IMPRIME O VALOR DE UMIDADE MEDIDO
    lcd.print("%"); 
    lcd.setCursor(0, 1);
    lcd.print("Temperatura: ");
    lcd.print(dht.readTemperature(), 0); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO E REMOVE A PARTE DECIMAL
    lcd.println("C");
    delay(2000);
    }
  
    if (leitura_teclas == 'A') { // Aqui é adicionado a mistura A de 10 em 10gm apartir do movimento do servo motor

        lcd.clear();
        lcd.print("Dose KNO3 = 200g");
        lcd.setCursor(0, 1);

        lcd.print("Valor atual :");

          for (int i = 0; i < 20; i++){
          
          int doseA = doseA + 10;
          lcd.setCursor(13, 1 );
          lcd.print(doseA);

          servo1.write(90);
          delay (1000);
          servo1.write(0);
          delay(1000);
        
        
    }}

    if (leitura_teclas == 'B') { // Aqui é adicionado a mistura B de 10 em 10gm apartir do movimento do servo motor

        lcd.clear();
        lcd.print("Dose K2 SO4 = 200g");
        lcd.setCursor(0, 1);
        lcd.print("Valor atual :");

        for(int i = 0; i < 20; i++){
       
        int doseB = doseB + 10;
          lcd.setCursor(13, 1);
          lcd.print(doseB);

          servo1.write(90);
          delay (1000);
          servo1.write(0);
          delay (1000);
    }}

    if (leitura_teclas == 'C') {  // Aqui é adicionado a mistura C de 10 em 10gm apartir do movimento do servo motor

        lcd.clear();
        lcd.print("Dose Fe = 200g");
        lcd.setCursor(0, 1);
        lcd.print("Valor atual :");

        for (int i = 0; i < 20; i++){
          
        int doseC = doseC + 10;
          lcd.setCursor(13, 1 );
          lcd.print(doseC);

          servo1.write(90);
          delay (1000);
          servo1.write(0);
          delay(1000);
    }}
    if (flagLed == true){  //condicional para verificar se a função display esta ativda
      display();
    }
  
  
      if (flag == false) { // condicional para ligar 
      inicio();
      flag = true;
      }

    if (leitura_teclas == '1') {  // mostra o controle dos leds e a intensidade deles e do ldr
      flagLed = true;
      lcd.clear();
      }
       
    } else { //condicional para desligar
        desliga();
      }
} 
void iniciaPortas(){  // inicia as portas do arduino
  pinMode(ldr, INPUT);
  pinMode(LEDV, OUTPUT);
  pinMode(LEDA, OUTPUT);
  pinMode(POTV, INPUT);
  pinMode(POTA, INPUT);

}
void inicio() {  //mensagem inicial do display
   lcd.init();
  lcd.backlight();
  lcd.print("Estufa hidroponica");
  lcd.setCursor(0, 1);
  lcd.print("1-LEDS");
  lcd.setCursor(0, 2);
  lcd.print("2-nutrientes");
  lcd.setCursor(0, 3);
  lcd.print("3-Umidade e temp");
 
}

void desliga(){ // apaga o display
  lcd.clear();
  lcd.noBacklight();
  flag = false;
}

void nutrientes() { // lista de nutrientes
  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Composto A : KNO3");
    lcd.setCursor(0, 1);
    lcd.print("Composto B : K2 SO4");
    lcd.setCursor(0, 2);
    lcd.print("Composto C : Fe"); 
}

void display() { //  Função responsavel pelo controle dos leds, aparecer no display e informação de luminosidade
  ValorPotV = analogRead(POTV);
  ValorPotV = map (ValorPotV, 0, 1023, 0, 255);
  analogWrite(LEDV, ValorPotV);
  lcd.setCursor(0, 0);
  porcentagemLedV = map(ValorPotV, 0, 255, 0, 100);
  lcd.print("LED VERMELHO");
  lcd.setCursor(13, 0);
  lcd.print(porcentagemLedV);
  lcd.setCursor(16,0);
  lcd.print("%");

  ValorPotA = analogRead(POTA);
  ValorPotA = map (ValorPotA, 0, 1023, 0, 255);
  analogWrite(LEDA, ValorPotA);
  lcd.setCursor(0, 1);
  porcentagemLedA = map(ValorPotA, 0, 255, 0, 100);
  lcd.print("LED AZUL");
  lcd.setCursor(13, 1);
  lcd.print(porcentagemLedA);
  lcd.setCursor(16, 1);
  lcd.print("%");
  lcd.setCursor(0, 2);
  lcd.print("Luminosidade:");
  lcd.setCursor(13, 2);
  lcd.print(R);
  lcd.print("Lm");

}
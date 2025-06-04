// Detecção precoce de enchentes usando IoT ~ Projeto da Hydromind
// <LiquidCrystal.h> é a biblioteca para usar o display LCD 16x2
#include <LiquidCrystal.h>
// "DHT.h" é a biblioteca para usar o sensor de temperatura e umidade DHT22
#include "DHT.h"
#define DHTPIN A0                       // Inicializando o pino para o DHT22
#define DHTTYPE DHT22                   // Declarando o tipo de sensor DHT para funcionamento correto
LiquidCrystal lcd(2,3,4,5,6,7);         // Criando uma instância da biblioteca LiquidCrystal
DHT dht(DHTPIN, DHTTYPE);               // Criando uma instância da biblioteca DHT para o sensor DHT22
const int in=8;                         // Pino ECHO do sensor ultrassônico HC-SR04
const int out=9;                        // Pino TRIG do sensor ultrassônico HC-SR04

// Definindo os pinos para os LEDs e o buzzer
const int verde=10;
const int amarelo=11;
const int vermelho=12;
const int buzzer=13;

void setup()
{
  // Inicia a comunicação serial com taxa de 9600
  Serial.begin(9600);
  // Inicializa o display LCD com 16 colunas e 2 linhas
  lcd.begin(16, 2);
  // Define os pinos dos componentes como entrada ou saída
  pinMode(in, INPUT);
  pinMode(out, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(amarelo, OUTPUT);
  pinMode(vermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);
  // Inicializa o sensor DHT
  dht.begin();
  // Define os estados iniciais dos LEDs e do buzzer como desligados (LOW)
  digitalWrite(verde,LOW);
  digitalWrite(amarelo,LOW);
  digitalWrite(vermelho,LOW);
  digitalWrite(buzzer,LOW);
  // Mensagem de inicialização no display LCD
  lcd.setCursor(0, 0);
  lcd.print("Sensor de Agua");
  lcd.setCursor(0,1);
  lcd.print("Monitor de Risco");
  // Aguarda 5 segundos e limpa o LCD
  delay(5000); 
  lcd.clear();
}

void loop()
{
  // Lê temperatura e umidade do sensor DHT22
  float T = dht.readTemperature();
  float H = dht.readHumidity();
  // Verifica se os dados do sensor são válidos
  if (isnan(H) && isnan(T)) { 
    lcd.print("ERROR");
    return;
  }
  float f = dht.readTemperature(true);

  // Leitura da distância do sensor ultrassônico HC-SR04
  long dur;
  long dist;
  long per;
  digitalWrite(out,LOW);
  delayMicroseconds(2);
  digitalWrite(out,HIGH);
  delayMicroseconds(10);
  digitalWrite(out,LOW);
  dur = pulseIn(in, HIGH);
  dist = (dur * 0.034) / 2;

  // Mapeia o valor da distância para uma porcentagem
  per = map(dist, 10.5, 2, 0, 100);

  // Garante que a porcentagem esteja dentro dos limites
  if(per < 0)
  {
    per = 0;
  }
  if(per > 100)
  {
    per = 100;
  }

  // Mostra os dados dos sensores na serial
  Serial.print("Umidade: ");
  Serial.print(H);
  Serial.print("%  Temperatura: ");
  Serial.print(T);
  Serial.print("°C  Nivel de Agua: ");
  Serial.println(String(per) + "%");

  // Mostra temperatura e umidade no display LCD
  lcd.setCursor(0,0);
  lcd.print("Temperatura:");
  lcd.setCursor(0,1);
  lcd.print("Umidade    :");
  lcd.setCursor(12,0);
  lcd.print(T);
  lcd.setCursor(12,1);
  lcd.print(H);
  delay(1000);

  // Mostra o nível de água no display
  lcd.clear();
  lcd.print("Nivel Agua: ");
  lcd.print(String(per));
  lcd.print("% ");

  // Verifica o nível de água e define o alerta
  if(dist <= 3)
  {
      lcd.setCursor(0,1);
      lcd.print("Alerta VERMELHO");
      digitalWrite(vermelho, HIGH);
      digitalWrite(verde, LOW);
      digitalWrite(amarelo, LOW);
      digitalWrite(buzzer, HIGH);
      delay(2000);
      digitalWrite(buzzer, LOW);
      delay(2000);
      digitalWrite(buzzer, HIGH);
      delay(2000);
      digitalWrite(buzzer, LOW);
      delay(2000);
  }
  else if(dist <= 10)
  {
    lcd.setCursor(0,1);
    lcd.print("Alerta AMARELO ");
    digitalWrite(amarelo, HIGH);
    digitalWrite(vermelho, LOW);
    digitalWrite(verde, LOW);
    digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(buzzer, LOW);
    delay(3000);
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print("Alerta VERDE   ");
    digitalWrite(verde, HIGH);
    digitalWrite(amarelo, LOW);
    digitalWrite(vermelho, LOW);
    digitalWrite(buzzer, LOW);
    delay(3000);
  }
}
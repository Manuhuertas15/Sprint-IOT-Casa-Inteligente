//bibliotecas
#include <DHT11.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
//#include <Servo.h>
#include <ESP32Servo.h>

const int buzzer = 18;

#define DHTPINO 23    //Definimos qual sera o pino pars o sensor
#define DHTIPO DHT11  //Variavel do tipo DHT

DHT dht(DHTPINO, DHTIPO);  //inicializa o objeto DHT com o pino e o tipo
LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo motorPorta;

//Variaveis globais sao da funcao acenderLEDAoDeclararPresenca
const int PIR = 5;  //Pino Digital que o PIR está plugado
const int ledVerde = 4;

const int rele = 15;


/*void acenderLEDAoDetectarPresenca() {
  int estadoPIR = digitalRead(PIR);  //Le o pino Digital 2

  //INDENTACAO (ATALHO) - SHIFT + ALT + F
  // HIGH - VALE O VALOR DE 1
  // LOW - VALE O VALOR DE 0

  if (estadoPIR == HIGH) {
    digitalWrite(ledVerde, HIGH);//Fala ao programa para o pino 13 "ACENDER"
    Serial.println("LED ligado");
    
  } else {
    digitalWrite(ledVerde, LOW);//Fala ao programa para o pino 13 "ACENDER"
    Serial.println("LED apagado");
    
  }
  delay(1000);
}*/

void alarme_dois_tons() {
  int freqAlta = 2000;
  int freqBaixa = 800;
  int duracaoTom = 250;

  tone(buzzer, freqAlta, duracaoTom);
  delay(duracaoTom);
  tone(buzzer, freqBaixa, duracaoTom);
  delay(duracaoTom);
  delay(5000);
}

/*void verificarVazamentoDeGas() {
 int estadoMQ135 = analogRead(MQ135);
 Serial.println("Concentracao de gas: " + String(estadoMQ135));
 
 //if - verificar a intensidade do valor do estadoMQ135 >= 600
 //sim - ativar o alarme
 //nao - desativar alarme - noTone(buzzer);

  if (estadoMQ135 >= 400) {
    alarme_dois_tons();
  }
  else{
    noTone(buzzer);
  }
  delay(1000);
}*/

void verificarTemperaturaEUmidade() {
  float umidade = dht.readHumidity();         //Le a umidade
  float temperatura = dht.readTemperature();  //Le a temperatura em Ceusius

  //Serial.println("Temperatura: " + String(temperatura) + "C");

  if (temperatura <= 21.00 || temperatura >= 24.00) {
    alarme_dois_tons();
  } else {
    noTone(buzzer);
  }

  lcd.clear();  //limpa o blacklight do lcd
  lcd.setCursor(0, 0);
  lcd.print("Umidade: " + String(umidade) + "%");
  lcd.setCursor(0, 1);
  lcd.print("Temperatura: " + String(temperatura) + "C");
  delay(2000);
}

void abrirPorta() {
  motorPorta.write(20);
  delay(5000);
}

void fecharPorta() {
  motorPorta.write(182);
  delay(5000);
}

/*void trancarPorta(){
  digitalWrite(rele, LOW);
  Serial.println("Porta trancada");
  delay(2000);
}

void destrancarPorta(){
  digitalWrite(rele, HIGH);
  Serial.println("Porta destrancada");
  delay(2000);
}*/

void setup() {
  Serial.begin(115200);

  //motor.attach(servoMotor);
  pinMode(buzzer, OUTPUT);
  motorPorta.attach(26);
  pinMode(rele, OUTPUT);

  //
  digitalWrite(rele, LOW);

  //
  motorPorta.write(182);

  //
  dht.begin();

  lcd.init();       //lcd.begin();
  lcd.backlight();  //liga o lcd

  Serial.println("Calibrando os sensores");
  delay(1500);
  Serial.print(".");
  delay(1500);
  Serial.print(".");
  delay(1500);
  Serial.print(".");
  delay(1500);
  Serial.print(".");
  delay(1500);
  Serial.println(".");
  delay(1500);
  Serial.println("Sensores calibrados!!!");
  delay(500);
  Serial.println("  *TESTE LIBERADO*");
}

void loop() {
  //acenderLEDAoDetectarPresenca();

  //verificarVazamentoDeGas();

  verificarTemperaturaEUmidade();

  abrirPorta();

  fecharPorta();

  //destrancarPorta();

  //trancarPorta();
}
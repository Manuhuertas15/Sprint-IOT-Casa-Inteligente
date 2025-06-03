const int PIR = 2;
const int ledVerde = 12;
const int buzzer = 10;
const int MQ135 = A0; //MQ = sensor de gás 

void ledPresenca(){
  int estadoPIR = digitalRead(PIR);// Ler pino digital 2

  // HIGH = 1
  // LOW = 0
  if (estadoPIR == HIGH) {
    digitalWrite(ledVerde, HIGH);
  }
  else {
    digitalWrite(ledVerde, LOW);
  }
  //delay(1000);
}

void gasConcentrado(){
  int estadoMQ135 = analogRead(MQ135);
  Serial.println(estadoMQ135);
  if (estadoMQ135 >= 500) {
    digitalWrite(buzzer, HIGH);
  }
  else {
    digitalWrite(buzzer, LOW);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(ledVerde, OUTPUT);

  pinMode(MQ135, INPUT);
  
  pinMode(buzzer, OUTPUT);

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
  // Somente chamada de funcoes
  ledPresenca();
  gasConcentrado();
}

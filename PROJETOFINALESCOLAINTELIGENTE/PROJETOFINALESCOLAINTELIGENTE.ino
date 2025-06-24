//bibliotecas
#include <DHT11.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
//#include <Servo.h>
#include <ESP32Servo.h>
#include <PubSubClient.h>
#include <WiFi.h>

// --- WiFi & MQTT ---
const char* ssid = "ProjetosIoT_Esp32";//sua rede wifi
const char* password = "Sen@i134";//senha da sua rede wifi
const char* mqtt_server = "172.16.39.118";//endereço do broker público
const int mqtt_port = 1883;//porta do broker público, geralmente 1883

//Tópicos
const char* topic_led2 = "babytechhome/quarto2/LED";
const char* topic_led = "babytechhome/quarto/LED";
const char* topic_umid = "babytechhome/quarto/umidade";
const char* topic_temp = "babytechhome/quarto/temperatura";
const char* topic_porta = "babytechhome/sala/porta";
//const char* topic_buzzer = "babytchhome/quarto2/buzzer";

//Variaveis globais sao da funcao acenderLEDAoDeclararPresenca
const int PIR = 5;  //Pino Digital que o PIR está plugado
const int ledQuarto = 4;
const int ledQuarto2 = 14;
const int buzzer = 18;
const int rele = 15;
#define DHTPINO 23    //Definimos qual sera o pino pars o sensor
#define DHTIPO DHT11  //Variavel do tipo DHT
float temperatura; 
float umidade;

DHT dht(DHTPINO, DHTIPO);  //inicializa o objeto DHT com o pino e o tipo
LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo motorPorta;
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long ultimaLeitura = 0;

// --- Funções WiFi e MQTT ---
void conectarWiFi() {//verifica conexão wifi para somente depois iniciar o sistema
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void reconectarMQTT() {//verifica e reconecta a conexão com o broker mqtt
  while (!client.connected()) {
    Serial.print("Reconectando MQTT...");
    String clientId = "ESP32Client-";
    clientId += String(WiFi.macAddress());
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado!");
      //client.subscribe(topic_led);//conecta ao topico do led assim que estabelecer ligação com o broker
      client.subscribe(topic_porta);//conecta ao topico da porta assim que estabelecer ligação com o broker
    } else {
      Serial.print("Falha: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

/**
  Função para tratamento das mensagens de callback/retorno do broker de cada tópico subscrito (led, porta, etc.)

  char* é uma cadeia de caracteres em C como um vetor onde cada caractter/letra está em uma posição, 
  diferente de uma String em C++ que pode ser lida completamente
*/
void tratarMensagem(char* topic, byte* payload, unsigned int length) {//
  String mensagem = "";
  for (int i = 0; i < length; i++) {//concatena todas os char* para se ter o texto completo em String
    mensagem += (char)payload[i];
  }

  Serial.printf("Mensagem recebida [%s]: %s\n", topic, mensagem.c_str());
  
  /*
    Verifica se o tópico recebido é o topico da porta
  é uma função da linguagem C que compara duas strings (topic e topic_porta)
  */
  //porta
  if (strcmp(topic, topic_porta) == 0) {//tópico atual é o da porta?
    if (mensagem == "abrir") {
     
      abrirPorta();
       Serial.println("Porta ABERTA");

    } else if (mensagem == "fechar") {
      
      fecharPorta();
      Serial.println("Porta FECHADA");
    }
  }
}

void alarme_dois_tons() {
  int freqAlta = 2000;
  int freqBaixa = 800;
  int duracaoTom = 300;

  for (int i = 0; i< 2; i++) {
    tone(buzzer, freqAlta, duracaoTom);
    tone(buzzer, freqBaixa, duracaoTom);
    delay(500);
    tone(buzzer, freqAlta, duracaoTom);
    tone(buzzer, freqBaixa, duracaoTom);
  }
  noTone(buzzer);  // Desliga o buzzer ao final
}

void verificarTemperaturaEUmidade() {
  float temperatura = dht.readTemperature();//lê a temperatura do sensor
  float umidade = dht.readHumidity();// lê a umidade do sensor

  if (isnan(temperatura) || isnan(umidade)) {//avisa no console se deu erro
    Serial.println("Erro ao ler DHT!");
    return;
  }

  Serial.printf("Temp: %.1f °C | Umid: %.1f %%\n", temperatura, umidade);//mostra temperatura e umidade no console

  char tempStr[10], umidStr[10];
  dtostrf(temperatura, 4, 1, tempStr); //converte o valor da temperatura do sensor que para string (ele vem float do sensor)
  dtostrf(umidade, 4, 1, umidStr); //converte o valor da umidade do sensor que para string (ele vem float do sensor)
  client.publish(topic_temp, tempStr);//publica a temperatura no tópico, lá no Broker Server
  client.publish(topic_umid, umidStr);//publica a umidade no tópico, lá no Broker Server
  
  if (temperatura <= 21.00 || temperatura >= 25.00) {
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
  delay(500);
}

void fecharPorta() {
  motorPorta.write(182);
  delay(500);
}

void setup() {
  Serial.begin(115200);

  //motor.attach(servoMotor);
  pinMode(buzzer, OUTPUT);
  //pinMode(rele, OUTPUT);
  pinMode(ledQuarto, OUTPUT);
  pinMode(ledQuarto2, OUTPUT);

  //digitalWrite(rele, LOW);

  motorPorta.write(182);
  motorPorta.attach(26);
  
  dht.begin();

  lcd.init();       //lcd.begin();
  lcd.backlight();  //liga o lcd

  conectarWiFi();//conecta no wifi
  client.setServer(mqtt_server, mqtt_port);//conecta no broker server
  client.setCallback(tratarMensagem);//trata as mensagens recebidas do broker

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
  if (!client.connected()) reconectarMQTT();//se não tem conexão com o broker, tenta reconectar
  client.loop(); //mantém a conexão com o broker serve sempre aberta

  if (millis() - ultimaLeitura > 3000) {
    ultimaLeitura = millis();   //contador de milisegundos
    verificarTemperaturaEUmidade(); //Dados do sensor de umidade e temperatura
  }
}
// C++ code
//
int ledVerde = 10;
int ledAmarelo = 9;
int ledVermelho = 7;
int escolha;

void setup()
{
  Serial.begin(9600);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  
  do
  {
    Serial.println(" ___________________________________________");   
    Serial.println("| ESCOLHA UMA DAS OPCOES PARA SER REALIZADA |");
    Serial.println("|___________________________________________|"); 
    Serial.println();
    Serial.println(" 1) Ligar Led Verde"); 
    Serial.println(" 2) Desligar Led Verde");
    Serial.println(" 3) Ligar Led Amarelo");
    Serial.println(" 4) Desligar Led Amarelo");
    Serial.println(" 5) Ligar Led Vermelho");
    Serial.println(" 6) Desligar Led Vermelho");
    Serial.println(" 7) Ligar Todos os Leds");
    Serial.println(" 8) Desligar Todos os Leds");
    Serial.println(" 0) Sair");
    while(!Serial.available());
    escolha =  Serial.parseInt();
    
    delay(1000);
    Serial.println();
    
    switch(escolha)
    {
      case 1:
      digitalWrite(ledVerde, HIGH);
      Serial.println("Led VERDE aceso.");
      break;
      case 2:
      digitalWrite(ledVerde, LOW);
      Serial.println("Led VERDE apagado.");
      break;
      case 3:
      digitalWrite(ledAmarelo, HIGH);
      Serial.println("Led AMARELO aceso.");
      break;
      case 4:
      digitalWrite(ledAmarelo, LOW);
      Serial.println("Led AMARELO apagado.");
      break;
      case 5:
      digitalWrite(ledVermelho, HIGH);
      Serial.println("Led VERMELHO aceso.");
      break;
      case 6:
      digitalWrite(ledVermelho, LOW);
      Serial.println("Led VERMELHO apagado.");
      break;
      case 7:
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledAmarelo, HIGH);
      digitalWrite(ledVermelho, HIGH);
      Serial.println("Led VERDE, AMARELO E VERMELHO acesos.");
      break;
      case 8:
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledAmarelo, LOW);
      digitalWrite(ledVermelho, LOW);
      Serial.println("Led VERDE, AMARELO E VERMELHO apagados.");
      break;
      case 0:
      Serial.print("DESLIGANDO");
      delay(500);
      Serial.print(".");
      delay(500);
      Serial.print(".");
      delay(500);
      Serial.println(".");
      break;
      default:
      Serial.println("Nao temos esta opcao.");
      break;
    }
    delay(3000);
  }
  while(escolha >= 1 && escolha <= 8); 
  
}

void loop()
{
 /* digitalWrite(10, HIGH);
  delay(1000); 
  digitalWrite(10, LOW);
  delay(1000); 
  digitalWrite(9, HIGH);
  delay(1000); 
  digitalWrite(9, LOW);
  delay(1000); 
  digitalWrite(8, HIGH);
  delay(1000); 
  digitalWrite(8, LOW);
  delay(1000); */
  
}
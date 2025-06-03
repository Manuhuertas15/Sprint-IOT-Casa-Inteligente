// C++ code
//
int ledInterno = 13;
int sensorUmidade = 7;

void setup()
{
  pinMode(13, OUTPUT);
  pinMode (sensorUmidade, INPUT);
}

void loop()
{
  digitalWrite(13, HIGH);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(13, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
}
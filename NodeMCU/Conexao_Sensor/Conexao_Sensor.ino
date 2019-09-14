#include <SimpleDHT.h>

//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2 DIGITAL 

int pinDHT11 = 2;


SimpleDHT11 dht11(pinDHT11);

void setup() {
  Serial.begin(115200);
  Serial.print("Iniciando Setup");
}

void Ler_Dados_Sensor()
{
    Serial.println("=================================");
  
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  err = dht11.read(&temperature, &humidity, NULL); 
  
  if (err != SimpleDHTErrSuccess) 
  {
    Serial.print("Erro ao ler info dos sensores"); 
    Serial.println(err);
    delay(1000);
    return;
  }
  
  Serial.print((int)temperature); 
  Serial.print(" C");
  Serial.print(" /// ");
  Serial.print((int)humidity); 
  Serial.println(" H");
  
  // DHT11 sampling rate is 1HZ.
  delay(1500);
}



void loop() {
  Ler_Dados_Sensor();

}

#include <SimpleDHT.h>
#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "www.smartcare.pe.hu";
int pinDHT11 = 2;
EthernetClient client;
SimpleDHT11 dht11(pinDHT11);
//      Conexões no Arduino
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2 DIGITAL 


void setup() {
  Serial.begin(9600);
  Serial.println("========= Iniciando Setup =========");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP"); 
    while(Ethernet.begin(mac) !=1)
    {
      Serial.println("Trying Connection"); 
    }
  }
 else{
      Serial.print(" - DHCP IP ");
      Serial.println(Ethernet.localIP());
    }
}

void POST_SEND(int temp, int humi)
{
  int t = 0;  // TEMPERATURE VAR
  int h = 0;  // HUMIDITY VAR
  String data;
  h = humi;
  t = temp;
  data = String("temp=") + t + "&hum=" + h + "&presenca=0"+"&id_sensor=4";
 //data = "{\"status\": {\"led_is\": \"on\"}}";
   
  if (client.connect(server,80)) { 
    Serial.println("========= POST =========");
    Serial.print(" - Conectado ao Servidor \n");
    // Make a HTTP request:                 
    client.println("POST /index.php/entrada/retorno HTTP/1.1");           
    client.println("Host: www.smartcare.pe.hu");
    client.println("Content-Type: application/x-www-form-urlencoded");//x-www-form-urlencoded
    client.println("Connection: close");
    client.println("Access-Control-Allow-Origin: *");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);
    client.println();
    Serial.println("~ POST Info ~");
    Serial.print(data+"\n"); 
    Serial.print("~ Data enviados ~ \n");
  }
 else{
  Serial.print("Not Connected \n");
  } 

  if (client.connected()) { 
    client.stop();  // DISCONNECT FROM THE SERVER
  }

  delay(3000); 
}

void Ler_Dados_Sensor()
{
    Serial.println("========= Leitura =========");
  
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
  Serial.print("Temperatura: "); 
  Serial.print((int)temperature); 
  Serial.println(" C");
  Serial.print("Umidade: "); 
  Serial.print((int)humidity); 
  Serial.println(" H");

  POST_SEND((int)temperature, (int)humidity);
  // DHT11 sampling rate is 1HZ.
  delay(12000);
}



void loop() {
  Ler_Dados_Sensor();

}

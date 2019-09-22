#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <SimpleDHT.h>

//      VCC: 5V or 3V
//      GND: GND
//      DATA: D4
int pinDHT11 = 2; // D4 data
SimpleDHT11 dht11(pinDHT11);


#ifndef STASSID
#define STASSID "POCOPHONE"
#define STAPSK "guigui10";
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "www.smartcare.pe.hu";
const uint16_t port = 80;

int temp = 0;
int humid = 0;

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(13, OUTPUT);// Seta o pino 13 (D7)
  pinMode(5, OUTPUT); // Seta o pino 5 (D1)
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

   Temp_Humid_Read();
   
  ///////////////////////////////////////////////
  Serial.println("connecting to "+(String)host+":"+(String)port);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }
  // This will send a string to the server
  if (client.connected()) {

    String data = String("temp=")+ temp +  "&hum=" + humid + "&presenca=0"+"&id_sensor=4";

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
    Serial.print(" - POST Info : ");
    Serial.println(data+"\n");
    Serial.print(" - Dados Enviados \n");
  }

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();
  delay(8000);// execute once every 5 minutes, don't flood remote service
}

void Temp_Humid_Read()
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
    PiscarLed(13); // Pin Failure
    return;
  }

  Serial.print((int)temperature);
  Serial.print(" C");
  Serial.print(" /// ");
  Serial.print((int)humidity);
  Serial.println(" H");
  temp =  (int)temperature;
  humid = (int)humidity;
  PiscarLed(5); // Pin Sucess

  // DHT11 sampling rate is 1HZ.
  //delay(1500);
}
void PiscarLed(int pin)
{
  digitalWrite(pin, HIGH); // Acende o Led
  delay(1000); 
  digitalWrite(pin, LOW); // Apaga o Led
  delay(1000); 
}

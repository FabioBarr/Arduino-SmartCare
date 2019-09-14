#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID ".:ESTUDIO:."
#define STAPSK "maverick";
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "www.smartcare.pe.hu";
const uint16_t port = 80;

void setup() {

  Serial.begin(115200);
  
  Serial.println();
  Serial.println();
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
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }
 

  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
    String data = String("temp=10") +  "&hum=12" +  + "&presenca=0"+"&id_sensor=4";

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

  // wait for data to be available
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

  delay(30000); // execute once every 5 minutes, don't flood remote service
}

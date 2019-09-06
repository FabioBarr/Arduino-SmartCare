#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "www.ptsv2.com";
EthernetClient client;

int t = 0;  // TEMPERATURE VAR
int h = 0;  // HUMIDITY VAR
String data;

void setup() { 
  Serial.begin(9600);

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP"); 
  }
 else{
      Serial.print("  DHCP assigned IP ");
      Serial.println(Ethernet.localIP());
    }

  h = 7;
  t = 6;

  data = "";
  }

 
void loop(){
  
  data = String("temp1=") + t + "&hum1=" + h;
    
  if (client.connect(server,80)) { 
    Serial.print("connected \n");
    // Make a HTTP request:
    Serial.print("Making a HTTP request \n");                    
    client.println("POST /t/md86d-1567266602/post HTTP/1.1");           
    client.println("Host: www.ptsv2.com");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);
    client.println();
    Serial.print(data+"\n"); 
    Serial.print("Data sent... \n");
  }
 else{
  Serial.print("Not Connected \n");
  } 

  if (client.connected()) { 
    client.stop();  // DISCONNECT FROM THE SERVER
  }

  delay(5000); // WAIT FIVE MINUTES BEFORE SENDING AGAIN
}

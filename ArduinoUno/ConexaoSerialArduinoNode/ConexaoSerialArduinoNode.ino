#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno(3,4);

void setup() {
  Serial.begin(115200);
  ArduinoUno.begin(4800);

}

void loop() {
  while (ArduinoUno.available()>0)
  {
    float val = ArduinoUno.parseFloat();
    if(ArduinoUno.read()=='\n')
    {
    Serial.println();
    }
    delay(30);
  }

}

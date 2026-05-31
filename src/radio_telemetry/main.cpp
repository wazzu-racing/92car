#include <SPI.h>
#include <RH_RF95.h>
#include <Arduino.h>

#define RFM95_CS    8
#define RFM95_INT   7
#define RFM95_RST   4

#define RF95_FREQ 915.0
#define RF95_TX 23

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  while (!Serial) delay(1);
  delay(100);

  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);


  while (!rf95.init()) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }

  if (!rf95.setFrequency(RF95_FREQ)) {
    while (1);
  }

  rf95.setTxPower(RF95_TX, false);
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");
  Serial.println("Setup!!!");

}

void loop() {
  if (rf95.available()) {
    uint8_t buf[(4*53) + 3 + 1];
    uint8_t len = (4*53) + 3 + 1;

    if (rf95.recv(buf, &len)) {
      digitalWrite(LED_BUILTIN, HIGH);    
     
      for (int i = 0; i < len; i++) {
        Serial.print((char)buf[i]); 
      }      
    } 
  }
}
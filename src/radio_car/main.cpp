// Feather9x_TX
// -*- mode: C++ -*-

#include <SPI.h>
#include <RH_RF95.h>


#define RFM95_CS    8
#define RFM95_INT   7
#define RFM95_RST   4

#define RF95_FREQ 915.0
#define RF95_TX 23

RH_RF95 rf95(RFM95_CS, RFM95_INT);

uint8_t input[503];
uint8_t seek = 0;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial1.begin(9600);
  Serial.begin(9600);
  while (!Serial1) delay(1);
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
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  }

  if (!rf95.setFrequency(RF95_FREQ)) {
    while (1);
  }

  rf95.setTxPower(RF95_TX, false);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial1.available()) {
    seek += Serial1.readBytesUntil('\n', input + seek, 500 - seek);
    while (!Serial1.available()) delay(1);
    char a = Serial1.read();
    while (!Serial1.available()) delay(1);
    char b = Serial1.read();


    if (a != '\n' || b != '\n') {
      Serial.println("AHHHHHHAHHHHHHAHHHHHHAHHHHHHAHHHHHHAHHHHHHAHHHHHHAHHHHHHAHHHHHHAHHHHHHAHHHHHHAHHHHHH");
      input[++seek] = '\n';
      input[++seek] = a;
      input[++seek] = b;
      if (seek > 500) seek = 0;
      return;
    }

    input[++seek] = '\n';
    input[++seek] = '\n';
    input[++seek] = '\n';
    seek++;
    for (int i=0;i<seek;i++) {
      Serial.print((char)input[i]);
    }
    Serial.println();
    digitalWrite(LED_BUILTIN, HIGH);

    rf95.send(input, seek);

    seek = 0;
    digitalWrite(LED_BUILTIN, LOW);
  }
}

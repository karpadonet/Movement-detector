#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define BUZZER_PIN 5 

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";


void setup() {
  //setting up the pins and the nfr connection
  //pinMode(BUZZER_PIN, OUTPUT); // Set buzzer - pin 5 as an output

  //initialize the radio object with ACK message enabled
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,address);
  

  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setRetries(1,5);
  

  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  radio.writeAckPayload(1, "ack!", 5);

}

void loop() {

  if (radio.available()) {
    char txt[32] = "";
    radio.read(&txt, sizeof(txt));
    Serial.println(txt);
    radio.writeAckPayload(1, "ack!", 5);
  }

}
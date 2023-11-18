#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define BUZZER_PIN 5 
int count = 0;
boolean flag = false;

//NRF24 connection
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

//The arduino setup function defines the pinmode for the buzzer 
//and the SPI connection to the NRF24 board
void setup() {

  //initialize the radio object
  Serial.begin(9600); //baud rate
  radio.begin(); 
  radio.openReadingPipe(1,address);
  
  //setting up the ACK reply for the NRF24 messages comming
  //from a different NRF24
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setRetries(1,5);
  

  radio.setPALevel(RF24_PA_MIN); //power level
  radio.startListening(); //enable receiving messages
  radio.writeAckPayload(1, "ack!", 5); //send first ACK message

}

//The arduino loop function is responsible for the buzzer functionality
//when a "movement" msg arrives, the buzzer starts to buzz for 10 seconds
void loop() {

  //check if a message arrived
  if (radio.available()) {
    char txt[32] = "";
    radio.read(&txt, sizeof(txt));
    radio.writeAckPayload(1, "ack!", 5); //send ack to any message arriving
    //if a "movement" message arrived start buzzing
	if (strcmp(txt, "Movement") == 0) {
      flag = true;       
    }
  }

  if (flag)
  {
     tone(BUZZER_PIN, 1000); // Send 1KHz sound signal
     count++;
	
	//continue buzzing for 400 cycles of 25[ms] = 10 seconds
     if (count == 400) {
        noTone(BUZZER_PIN);     // Stop sound
        count = 0;
        flag = false;
      }
  }

  delay(25); // run logic each 25[ms]

}

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define HELLO_MSG_TIME 40
#define MAX_COUNT 10
#define WAIT 1000
#define SENSOR_PIN 7
#define LED_PIN 8
int count = 0;
int led_count = 0;
int hello_msg_counter = 0;
boolean led_status = false;

//NRF24 connection
RF24 radio(9,10); //CE, CSN
//an array of the addresses that the modules will use to communicate amongst themselves
const byte address[6] = "00001";

//The arduino setup function defines the pinmode for the motion sensor, LED
//and the SPI connection to the NRF24 board
void setup() {

  //setting up the pins 
  pinMode(LED_PIN,OUTPUT);
  pinMode(SENSOR_PIN,INPUT);
  digitalWrite(LED_PIN,LOW);
  
  //initialize the radio object
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.setRetries(5,5); 
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
 

}


/*
The arduino loop function for the motion detector is performing two functions periodically:
1. Sample the motion detector pin status. If the sensor detect motion, send a notification via NRF24
2. Send a "Hello" message to sample the connection status with the buzzer device. If an ACK messgage does
not arrive for more than MAX_COUNT = 10 times, the status LED will blink until an ACK message is recieved
*/
void loop() {

//sending "hello" msg every 1[sec] and counting it
if (hello_msg_counter == HELLO_MSG_TIME)
{
  const char txt[] = "Hello";
  radio.write(&txt, sizeof(txt));
  count++;

    //handle nrf24 ACK message reception
  if (radio.isAckPayloadAvailable())
  {
    unsigned char rx_buff[5];
    radio.read(rx_buff, 5);
    Serial.println("Got ACK");
    count = 0;
  }

  hello_msg_counter = 0;
}

if (count >= 10 ) 
{
  //light is blinking
  if (led_count == 40)
  {
    if (led_status) 
    {
      digitalWrite(LED_PIN,LOW);
      led_status = false;
    }
    else
    {
      led_status = true;
      digitalWrite(LED_PIN,HIGH);
    }
    led_count = 0;
  }
  led_count++;
}
else 
{
  //light is on
  digitalWrite(LED_PIN,HIGH);

}

//trying to detect movement - periodically sampling the SENSOR_PIN
//if movement was detected we notify the buzzer with the NRF24 communication module
if (digitalRead(SENSOR_PIN)) 
{
    const char txt[] = "Movement";
    radio.write(&txt, sizeof(txt));
}

//global delay - 25[ms]
hello_msg_counter++;
delay(25);
}

/*BASE 00
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
          == Base/ Master Node 00==
  by Dejan, www.HowToMechatronics.com
  Libraries:
  nRF24/RF24, https://github.com/nRF24/RF24
  nRF24/RF24Network, https://github.com/nRF24/RF24Network
*/

// IN THIS EXAMPLE BASE00 TAKE SENSOR FROM 01 SEND INSTRUCTION TO 02.

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
RF24 radio(10, 9);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node02 = 02;
int tempo;
int ref;
int LEDACTIVATOR[2];
void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);

}
void loop() {
LEDACTIVATOR[0]=0; // NON ACCENDI IL LED
LEDACTIVATOR[1]=0;  
  network.update();  
tempo=millis();
ref=tempo;
//===== Receiving Response =====//
while(tempo-ref<1000){
 while ( network.available() ) {    // Is there any incoming data from 01
    RF24NetworkHeader header;
    network.read(header, &LEDACTIVATOR, sizeof(LEDACTIVATOR)); // Read the incoming data 

  Serial.println(header.from_node); 
   Serial.print(LEDACTIVATOR[0]);
   Serial.println(LEDACTIVATOR[1]);
 
  }
  
tempo=millis();
}

  //===== Sending =====//

  // MESSAGE at Node 01 ATTIVATI  
  RF24NetworkHeader header2(node01);     // (Address where the data is going)
   const int NUM = 1;//CODICE LEGGI
  bool ok1 = network.write(header2, &NUM, sizeof(NUM)); // Send the data
 
  // MESSAGE at Node 02 GESTISCI LED
  RF24NetworkHeader header3(node02);     // (Address where the data is going)
  bool ok2 = network.write(header3, &LEDACTIVATOR, sizeof(LEDACTIVATOR)); // Send the data

  
 
  
delay(100);




}

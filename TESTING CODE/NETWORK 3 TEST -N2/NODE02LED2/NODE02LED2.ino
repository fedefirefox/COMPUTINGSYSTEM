/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
            == Node 01    
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
RF24 radio(10, 9);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 02;  // Address of our node in Octal format ( 04,031, etc)
const uint16_t base00 = 00;    // Address of the other node in Octal format
int tempoM;
int refM;
void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
 pinMode(4,OUTPUT);
 pinMode(6,OUTPUT);
}
void loop() { 
  network.update();
  //===== Receiving =====//
  tempoM=millis();
  refM=tempoM;
  while(tempoM-refM<1000){
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
     int incomingData[2];
    network.read(header, &incomingData, sizeof(incomingData));

   Serial.println(header.from_node); 

if (incomingData[0]==1){
digitalWrite(6,HIGH);
}
else if(incomingData[0]==0){digitalWrite(6,LOW);
}

else { Serial.println("ERRORE");}

  
if (incomingData[1]==1){
digitalWrite(4,HIGH);
}
else if(incomingData[1]==0){digitalWrite(4,LOW);
}
  
  }
  tempoM=millis();
  } // in questo modo passa un pò di tempo ad aspettare una risposta ignorando il resto del programma
delay(100);
}

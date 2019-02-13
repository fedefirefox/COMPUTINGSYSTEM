/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
            == Node 01    
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
RF24 radio(10, 9);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;  // Address of our node in Octal format ( 04,031, etc)
const uint16_t base00 = 00;    // Address of the other node in Octal format
int tempoM;
int refM;
int rosso=4;
int verde=6;
void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
 pinMode(rosso, INPUT);
 pinMode(verde,INPUT);
}
void loop() {
  network.update();
  //===== Receiving =====//
  tempoM=millis();
  refM=tempoM;
  while(tempoM-refM<1000){
  while ( network.available() ) {     // Is there any incoming data? THE MASTER CALL YOU % IL MASTER CHIAMA E VERIFICA QUALE PULSANTE STAI ACCENDENDO
    RF24NetworkHeader header;
    int DATA;
    network.read(header, &DATA, sizeof(DATA));
  

   
  //===== Sending back =====// contiuna mandare per 10ms se ricevuto qualcosa
if(DATA==1){
int Rosso,Verde;
Rosso=digitalRead(rosso);
Verde=digitalRead(verde);
Serial.println(Rosso);
Serial.println(Verde);
int DataBack[2]={Verde,Rosso}; // 00 due spenti 01 rossoacceso verde spento

  
  RF24NetworkHeader header8(base00);
  bool ok = network.write(header8, &DataBack, sizeof(DataBack)); // Send the data


}
  
  }
  tempoM=millis();
  } // in questo modo passa un pò di tempo ad aspettare una risposta ignorando il resto del programma
delay(100);
}

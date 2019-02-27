/*BASE 00
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
          == Base/ Master Node 00 ==
  by Dejan, www.HowToMechatronics.com
  Libraries:
  nRF24/RF24, https://github.com/nRF24/RF24
  nRF24/RF24Network, https://github.com/nRF24/RF24Network
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
RF24 radio(10, 9);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t  bath= 01;      // Address of the other node in Octal format
const uint16_t screen = 02;

int tempo,ref;
int data[3]; 


void setup() {

  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_250KBPS);
 radio.setChannel(124);
  



}
int temp[2]={555,250};
int ACK[3];
int SCHERMO=10;
void loop() {

  
  // FASE DI LETTURA DALLO SPAZIO CARICA IN BUFFER SEGNALI   
  tempo=millis();
  ref=tempo;
 ACK[0]=1;

//===== Receiving Response =====// LEGGE ON OFF DA SCHERMO

while(tempo-ref<1000){
  network.update(); 
   
 while ( network.available() ) {    // Is there any incoming data from 01
  
    
    // PARTO POSITIVO DICENDO TUTTO OK! CAMBIA SOLO SE IL PACCHETTO SMENTISCE
   // POMPA=ACK;
    // SCHERMO=ACK; // STESSA FILOSOFIA VA TUTTO BENE! 
// LED 13 SPENTO
    
    RF24NetworkHeader header;
    network.read(header, &ACK, sizeof(ACK)); // Read the incoming data 

 
   // GESTIONE DEL ACK MANDATO PER SCELTE FUTURE
   if (header.from_node== bath)// pompa)
   {SCHERMO=1111;}
   else if (header.from_node==screen)//SCHERMO)
   SCHERMO=ACK[0];
 {Serial.print("LO STATO (ON=1_OFF=0) è ..."); Serial.println(SCHERMO);}  //LED 13 ACCESO LO SCHERMO E' VIVO

   
  }
  
tempo=millis();
}











  //===== Sending =====// CREO IL PACCHETTO per lo schermo 
data[0]=SCHERMO;
data[1]=temp[0];
data[2]=temp[1];



delay(200);

int i;
  for (i=0;i<15;i++){


  // MESSAGE at Node 02 SCHERMO!!
  RF24NetworkHeader header3(screen);     // (Address where the data is going)

  bool ok2 = network.write(header3, &data, sizeof(data)); // Send the data
  Serial.println("mando");
delay(100); // MANDA PACCHETTI!!!
  }











}

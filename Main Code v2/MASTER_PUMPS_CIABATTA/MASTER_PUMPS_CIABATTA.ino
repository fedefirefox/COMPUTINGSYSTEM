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

    int ACK,POMPA,SCHERMO;

int A[3]={A1,A2,A3};

void setup() {

  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_250KBPS);
radio.setChannel(124);

// DEFINIZIONE PIN
pinMode(A1,INPUT);
pinMode(A2,INPUT);
pinMode(A3,INPUT);















}
void loop() {









  
  // FASE DI LETTURA DALLO SPAZIO CARICA IN BUFFER SEGNALI   
  tempo=millis();
  ref=tempo;
 ACK=1;

//===== Receiving Response =====//

while(tempo-ref<1000){
  network.update(); 
 while ( network.available() ) {    // Is there any incoming data from 01
  
    
    ACK=1; // PARTO POSITIVO DICENDO TUTTO OK! CAMBIA SOLO SE IL PACCHETTO SMENTISCE
   // POMPA=ACK;
    // SCHERMO=ACK; // STESSA FILOSOFIA VA TUTTO BENE! 
// LED 13 SPENTO
    
    RF24NetworkHeader header;
    
    network.read(header, &ACK, sizeof(ACK)); // Read the incoming data 

   Serial.println(header.from_node); // STAMPA IL NODO DA CUI STAI RICEVNDO
  
   // GESTIONE DEL ACK MANDATO PER SCELTE FUTURE
   if (header.from_node== bath)// pompa)
   {POMPA=ACK;}
   else if (header.from_node==screen)//SCHERMO)
 {SCHERMO=ACK;}  //LED 13 ACCESO LO SCHERMO E' VIVO

   
  }
  
tempo=millis();
}


// CONTROLLO CRITICO CIABATTA





















// FASE DI LETTURA DATI
int i;
int temp[2]={0,0}; 
for (i=0;i<2;i++){
temp[i]=analogRead(A[i]); // LETTURA 0-1023 0-5V su 2 pin 


}


  //===== Sending =====// CREO IL PACCHETTO per lo schermo 
data[0]=ACK;
data[1]=temp[0];
data[2]=temp[1];



  for (i=0;i<10;i++){

  // MESSAGE at Node 01  BATH. 
  RF24NetworkHeader header2(bath);     // (Address where the data is going)
  const int NUM = 1;//CODICE LEGGI, PARLA!
  bool ok1 = network.write(header2, &NUM, sizeof(NUM)); // Send the data
 

  
  
  // MESSAGE at Node 02 SCHERMO!!
  RF24NetworkHeader header3(screen);     // (Address where the data is going)

  bool ok2 = network.write(header3, &data, sizeof(data)); // Send the data
  
delay(10); // MANDA PACCHETTI!!!
  }











}


#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
RF24 radio(10, 9);     
// DA CAMBIARE IL PROTOCOLLO DI ACQUISIZIONE DATI!!!!




RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 04;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t  master= 00;      // Address of the other node in Octal format

int tempo,ref;

  int data[3]; // mandato
  int incomingData[3]; // ricevuto 
int checkscreen;
int expmode=0;
void setup() {

  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_250KBPS);
radio.setChannel(124);











  
}

void loop() {

  // FASE DI LETTURA DALLO SPAZIO CARICA IN BUFFER SEGNALI   
  tempo=millis();
  ref=tempo;

// Definisco delle variabili di controllo di arrivo del pacchetto
checkscreen=0;


while(tempo-ref<1000 && tempo-ref>-0.01){
  network.update(); 

 while ( network.available() ) {


 RF24NetworkHeader header;
 
 // leggo I DATI E DISCRIMINO LA PROVENIENZA! 
 network.read(header, &incomingData, sizeof(incomingData));
  Serial.println(header.from_node);
 
//Provenienza da schermo
if(header.from_node==2){
  checkscreen=1;
  expmode=incomingData[0];
  Serial.print("Ricevuto da schermo..Mode exp:");
  Serial.println(expmode);//0=OFF
  }


 }
   
tempo=millis();
 delay(10);
 }




 

// FASE DI LETTURA DATI
int i;
int temp[2]={0,0}; 















//VALUTA MESSAGGIO DI ERRORE DA MANDARE

data[0]=1;// TUTTO ok
data[1]=temp[0];
data[2]=temp[1];

if(expmode==0){ 
  for (i=0;i<10;i++){

  // MESSAGE at Node 01  BATH. __TO DECIDE

  
  
  // MESSAGE at Node 02 SCHERMO!!
  RF24NetworkHeader header3(master);     // (Address where the data is going)

  bool ok2 = network.write(header3, &data, sizeof(data)); // Send the data
  
delay(10); // MANDA PACCHETTI!!!
  }
}


}

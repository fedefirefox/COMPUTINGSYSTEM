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
const uint16_t pump = 01;      // Address of the other node in Octal format
const uint16_t screen = 02;
int esponente[2];
int pompa[2];
int tempo,ref;
  int data[5];

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



// CONTROLLO CRITICO





}
void loop() {
  
  network.update(); // FASE DI LETTURA DALLO SPAZIO CARICA IN BUFFER SEGNALI   
  tempo=millis();
  ref=tempo;
 ACK=1;

//===== Receiving Response =====//

while(tempo-ref<1000){
 while ( network.available() ) {    // Is there any incoming data from 01
  
    
    ACK=1; // PARTO POSITIVO DICENDO TUTTO OK! CAMBIA SOLO SE IL PACCHETTO SMENTISCE
    POMPA=ACK;
    SCHERMO=ACK; // STESSA FILOSOFIA VA TUTTO BENE! 
// LED 13 SPENTO
    
    RF24NetworkHeader header;
    
    network.read(header, &ACK, sizeof(ACK)); // Read the incoming data 

   Serial.println(header.from_node); // STAMPA IL NODO DA CUI STAI RICEVNDO
  
   // GESTIONE DEL ACK MANDATO PER SCELTE FUTURE
   if (header.from_node== pump)// pompa)
   {POMPA=ACK;}
   else if (header.from_node==screen)//SCHERMO)
 {SCHERMO=ACK;}  //LED 13 ACCESO LO SCHERMO E' VIVO

   
  }
  
tempo=millis();
}


// ORA LA FASE DI LETTURA E' TERMINATA





// FASE DI LETTURA DATI
int i;
for (i=0;i<3;i++){
double temp[3]; 
temp[i]=analogRead(A[i]); // LETTURA 0-1023 0-5V su 2 pin

// ConversioneVolt_mbar (int pompa[i], int& pompa[i] , int& esponente[i]) {}
temp[i]=temp[i]*5/1023;
temp[i]=pow(10,(temp[i]-3.4251)/0.3027);
if (temp[i]<100){
  int j=0;// itera finchè pressure non diventa composto da 123.123 conta il numero di volte hai fatto per 10 
  while(temp[i]<100){
        
        temp[i]=temp[i]*10;
        j++;
  }
esponente[i]=2-j;
pompa[i]=(int) temp[i];
Serial.println(pompa[i]); 

}




// QUA DA AGGIUNGERE LA CONVERSIONE NEL CASO DI PRESSIONE MAGGIORE DI 100mbar! 







}


  //===== Sending =====// CREO IL PACCHETTO 
data[0]=ACK;
data[1]=pompa[0];
data[2]=esponente[0];

data[3]=pompa[1];
data[4]=esponente[1];


  

  // MESSAGE at Node 01  PUMP. 
  RF24NetworkHeader header2(pump);     // (Address where the data is going)
  const int NUM = 1;//CODICE LEGGI, PARLA!
  bool ok1 = network.write(header2, &NUM, sizeof(NUM)); // Send the data
 

  
  
  // MESSAGE at Node 02 SCHERMO!!
  RF24NetworkHeader header3(screen);     // (Address where the data is going)

  bool ok2 = network.write(header3, &data, sizeof(data)); // Send the data





  
delay(100);




}

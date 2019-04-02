
#include <AltSoftSerial.h>//libreria necessaria se non uso i pin rx e tx
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>



//set up a new serial port
AltSoftSerial mySerial;
RF24 radio(10, 9); 
RF24Network network(radio);
const uint16_t this_node = 01; 
const uint16_t master = 00;

void setup()  {
  mySerial.begin(19200);//velocità voluta dal refrigeratore, 8 bit, no parity bit , 1 bit di stop
  Serial.begin(9600);
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_250KBPS);
radio.setChannel(124);
   
 }
int tempo,ref;
void loop(){
  int data[5];
  int i=0;
  int k;
  int on;
  int notgood=0;
  int fault=0;
  byte response[20];
  int checkscreen,expmode;
  byte turnon[]={0xca, 0x0, 0x01, 0x20,0x81 ,0x08,0x01,0x02,0x02,0x02,0x02,0x02,0x02,0x02};//turn on the unit
  byte readstatus[]={0xca, 0x0, 0x01, 0x09,0x0};//lettura dello stato
  byte resptoreadstatus[]={0xca, 0x00, 0x01, 0x09, 0x05};//risposta alla richiesta dello stato


//mando il comando di lettura dello stato
  sendcommand(readstatus,sizeof(readstatus));
  delay(1000);//aspetto la risposta

//ascolto la risposta
  //inizio:
  if (mySerial.available()){
  i=0;
  while(mySerial.available() > 0 ){
  response[i]=mySerial.read();//legge il primo byte disponibile nel buffer
  
  //stampo la risposta
  Serial.print(response[i],HEX);
  
  delay(2);//aspetto il caricamento del buffer
    i++;
  }

  
//  la prima parte della sequenza è nota, la verifico
  for(k=0;k<5;k++){
  if(response[k]!=resptoreadstatus[k]){
    Serial.println("la prima parte non è corretta");
    while(mySerial.available() > 0 ){
    mySerial.read();}//svuoto il buffer prima di ripetere il comando
    goto endofall;}}
    
    
    
//verifico il bit di checksum
  if(response[i-1]!=checksum(1,i-2,response)){
  Serial.println("checksum errato");
  goto endofall;
  }
  else {
  Serial.println("checksum verificato"); }

 
//leggo le informazioni sullo stato
       switch (response[8] & (byte)0x0e) {
         case (byte)0x0e:
             on=1;
             Serial.println("GOOD!");
            break;
         case (byte)0x0a:
             on=1;
             notgood=1;
             Serial.println("anomalo");
            break;
            
         case (byte)0x08:
              Serial.println("anomalo");
              on=1;
              notgood=1;
             break;
         case (byte)0x00:
              Serial.println("spento");
              on=0;
             break;
            
      }

      
//leggo informazioni sull'unità
          switch (response[8] & (byte)0xf0) {
            case (byte)0x10:
                 on=1;
                 fault=1;
                 Serial.println("unit faulted");
                break;
            case (byte)0x20:
                 on=0;
                 Serial.println("unit stopping");
                break;
            
           default:
                 Serial.print("altro");
                break; 
            
      }
      
      
  endofall:
  delay(2000);
  //  goto inizio; 
} 

//se è spento lo accendo
if(!on){
sendcommand(turnon,sizeof(turnon));
delay(1000);
}



// FASE TX E RX

tempo=millis();
  ref=tempo;


while(tempo-ref<1000 && tempo-ref>-0.01){
  network.update();


 while ( network.available() ) {

int incomingData[3];
 RF24NetworkHeader header;
network.read(header, &incomingData, sizeof(incomingData));
  Serial.println(header.from_node);
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


//send info
//
//on 
//notgood
// error
data[0]=on; // QUA CREAZIONE DEL PACCHETTO per il master
data[1]=notgood;
data[2]=1;
data[3]=0;

  
  // MESSAGE at Node 00 MASTER!! ok or not ok
  if(expmode==0 || notgood==1){ // INVIA IL PACCHETTO LO STESSO SE ERRORE FATALE
  int i;
  for(i=0;i<2;i++){
  RF24NetworkHeader header3(master);     // (Address where the data is going)

  bool ok2 = network.write(header3, &data, sizeof(data));
  delay(10);
  }
  }


}




 byte checksum(int inizio,int fine,byte vector[])  {
  byte sum=0x0;
  for (int j=inizio;j<fine+1;j++){
  sum=sum+vector[j];
  }
  return(sum xor 0xFF);}





  
  void sendcommand(byte vector[],int dim)  {
  for (int j=0;j<dim;j++){
  mySerial.write(vector[j]);
   
  }
  mySerial.write(checksum(1,dim-1,vector));
  }

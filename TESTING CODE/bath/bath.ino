
#include <AltSoftSerial.h>//libreria necessaria se non uso i pin rx e tx

//set up a new serial port
AltSoftSerial mySerial;

void setup()  {
  mySerial.begin(19200);//velocità voluta dal refrigeratore, 8 bit, no parity bit , 1 bit di stop
  Serial.begin(9600);
  
   
 }

void loop(){
  
  int i=0;
  int k;
  int on;
  int notgood=0;
  int fault=0;
  byte response[20];
  
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




//send info
//
//on 
//notgood
// error




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

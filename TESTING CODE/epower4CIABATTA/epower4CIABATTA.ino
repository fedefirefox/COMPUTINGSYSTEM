#include <AltSoftSerial.h>

AltSoftSerial altSerial;
int turnon=0;
int turnoff=0;

void setup()  {

  altSerial.begin(9600);//velocità voluta dalla ciabatta, 8 bit, no parity bit , 1 bit di stop
  Serial.begin(9600);
  
 }

void loop(){
char response[100];
int i;
char respexpon[]={'P','0','0','=','1'};
char respexpoff[]={'P','0','0','=','0'};
//

//codice di comunicazione che imposta i valori di turnon e turnoff;

//



if(turnon){

  resendon:
  altSerial.print("/P00=1");
  delay(1000);
  
  if (altSerial.available()){
  i=0;
  while(altSerial.available() > 0 ){
  response[i]=altSerial.read();
  Serial.print(response[i]);
  if(response[i]!=respexpon[i]){
  goto resendon;}
  }
  
  altSerial.print("\r");
  turnon=0;
  }
   
  
}
if(turnoff){

  resendoff:
  altSerial.print("/P00=0");
  delay(1000);
  
  if (altSerial.available()){
  i=0;
  while(altSerial.available() > 0 ){
  response[i]=altSerial.read();
  Serial.print(response[i]);
  if(response[i]!=respexpoff[i]){
  goto resendoff;}
  }
  
  altSerial.print("\r");
  turnoff=0;
  }
   
 }  
  
} 

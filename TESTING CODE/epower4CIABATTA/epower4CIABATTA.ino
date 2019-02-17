#include <AltSoftSerial.h>

AltSoftSerial altSerial;
int turnon=0;
int turnoff=0;
int ison[]={0,0,0,0};

void setup()  {
  altSerial.begin(9600);//velocità voluta dalla ciabatta, 8 bit, no parity bit , 1 bit di stop
  Serial.begin(9600);
  
  }

void loop(){
  

char response[100];
int i;
int j;
int k;
char respturn[]={'P','0'};
char state[]={'R','0'};

resendstate:
//mando un comando base iniziale
  altSerial.print("\r");
  //svuoto il buffer
  for(k=0;k<1000;k++){
    while(altSerial.available() > 0 ){
     altSerial.read();}
     delay(2);
  }


//verifico lo stato delle 4 prese
for(j=1;j<5;j++){

  altSerial.print("/R0");
  altSerial.print(j);
  delay(20);

if(altSerial.available()){
 //ascolto la risposta alla richiesta di conoscere lo stato 
  i=0;
  while(altSerial.available() > 0 ){
  response[i]=altSerial.read();
  Serial.print(response[i]);
  Serial.print(',');
  i++;
  delay(2);
  }
  //verifico che la risposta sia corretta altrimenti rimando il comando iniziale
    for (k=0;k<2;k++){
 if(response[k]!=state[k]){
  Serial.print("sbaglio 10");
//    while(altSerial.available() > 0 ){
//     altSerial.read();
//     delay(2);}
     goto resendstate;}
     }
 if(response[2]!=j+'0'){
  Serial.print("sbaglio 20");
//    while(altSerial.available() > 0 ){
//     altSerial.read();
//     delay(2);}
    
  goto resendstate;}
 
  Serial.print("prima fase");
  //se sono qui ho ricevuto la risposta corretta, mando il comando di conferma
  altSerial.print("\r");
  delay(20);

  //ascolto la risposta
  i=0;
  while(altSerial.available() > 0 ){
  response[i]=altSerial.read();
  Serial.print(response[i]);
  Serial.print('.');
  i++;
  delay(2);
  }

  //verifico se la risposta è corretta
    for(k=2;k<4;k++){
 if(response[k]!=state[k-2]){Serial.print("sbaglio 1");
//    while(altSerial.available() > 0 ){
//     altSerial.read();
//     delay(2);}
     goto resendstate;}
     }
     
 if(response[4]!=j+'0'){Serial.print("sbaglio 2");
//    while(altSerial.available() > 0 ){
//     altSerial.read();
//     delay(2);}
     
  goto resendstate;}
  
     if(response[5]!='='){Serial.print("sbaglio 3");
//    while(altSerial.available() > 0 ){
//     altSerial.read();
//     delay(2);}
     
  goto resendstate;}

  //salvo lo stato delle prese
      ison[j-1]=response[6]- '0';
      Serial.print(ison[j-1]);

      //svuoto il buffer
  for(k=0;k<10;k++){
    while(altSerial.available() > 0 ){
     altSerial.read();}
     delay(2);
  }
}
     }


  
  
  
  
//

//codice di comunicazione che imposta i valori di turnon e turnoff;
turnon=random(0,2);
turnoff=!turnon;
//


  
  
  
  

if(turnon){
  
  resendon:
//mando un comando base iniziale
  altSerial.print("\r");
  //svuoto il buffer
  for(k=0;k<1000;k++){
    while(altSerial.available() > 0 ){
     altSerial.read();}
     delay(2);
  }

//accendo tutte le tre prese
for(j=1;j<4;j++){
if(!ison[j-1]){
  altSerial.print("/P0");
  altSerial.print(j);
  altSerial.print("=1");
  delay(20);

if(altSerial.available()){
 //ascolto la risposta alla richiesta di accendere le prese 
  i=0;
  while(altSerial.available() > 0 ){
  response[i]=altSerial.read();
  Serial.print(response[i]);
  Serial.print(',');
  i++;
  delay(2);
  }
  //verifico che la risposta sia corretta altrimenti rimando il comando iniziale
    for (k=0;k<2;k++){
 if(response[k]!=respturn[k]){
  Serial.print("sbaglio 10");
    
     goto resendon;}
     }
 if(response[2]!=j+'0'){
  Serial.print("sbaglio 20");
  goto resendon;}

  if(response[3]!='='){
  Serial.print("sbaglio 30");
  goto resendon;} 

  if(response[4]!='1'){
  Serial.print("sbaglio 40");
  goto resendon;} 

  
  Serial.print("prima fase");
  //se sono qui ho ricevuto la risposta corretta, mando il comando di conferma
  altSerial.print("\r");
  delay(20);
  
      //svuoto il buffer
  for(k=0;k<10;k++){
    while(altSerial.available() > 0 ){
     altSerial.read();}
     delay(2);
  }
}
     }
}
}


if(turnoff){
  
  resendoff:
//mando un comando base iniziale
  altSerial.print("\r");
  //svuoto il buffer
  for(k=0;k<1000;k++){
    while(altSerial.available() > 0 ){
     altSerial.read();}
     delay(2);
  }

//spengo tutte le tre prese
for(j=1;j<4;j++){
if(ison[j-1]) {
  altSerial.print("/P0");
  altSerial.print(j);
  altSerial.print("=0");
  delay(20);

 if(altSerial.available()){
 //ascolto la risposta alla richiesta di spegnere le prese 
  i=0;
  while(altSerial.available() > 0 ){
  response[i]=altSerial.read();
  Serial.print(response[i]);
  Serial.print(',');
  i++;
  delay(2);
  }
  //verifico che la risposta sia corretta altrimenti rimando il comando iniziale
    for (k=0;k<2;k++){
 if(response[k]!=respturn[k]){
  Serial.print("sbaglio 10");
    
     goto resendoff;}
     }
 if(response[2]!=j+'0'){
  Serial.print("sbaglio 20");
  goto resendoff;}

  if(response[3]!='='){
  Serial.print("sbaglio 30");
  goto resendoff;} 

  if(response[4]!='0'){
  Serial.print("sbaglio 40");
  goto resendoff;} 

  
  Serial.print("prima fase");
  //se sono qui ho ricevuto la risposta corretta, mando il comando di conferma
  altSerial.print("\r");
  delay(20);
  
      //svuoto il buffer
  for(k=0;k<10;k++){
    while(altSerial.available() > 0 ){
     altSerial.read();}
     delay(2);
  }

     }
}}
}



 
} 


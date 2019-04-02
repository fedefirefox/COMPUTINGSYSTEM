#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>



RF24 radio(37, 35);               // nRF24L01 (CE,CSN)
RF24Network network(radio);    
const uint16_t this_node = 02;  // Address of our node in Octal format ( 04,031, etc)
const uint16_t base00 = 00; 
const uint16_t base04 = 04; 
const uint16_t bath = 01; 
int counter=0;
int counterNR=0;
int counterbath=0;

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
//#define LCD_CS A3 // Chip Select goes to Analog 3
//#define LCD_CD A2 // Command/Data goes to Analog 2
//#define LCD_WR A1 // LCD Write goes to Analog 1
//#define LCD_RD A0 // LCD Read goes to Analog 0

//#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define YELLOW  0xFFE0
#define WHITE   0xFFFF



#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */

/******************* UI details */
#define BUTTON_X 10
#define BUTTON_Y 100
#define BUTTON_W 50
#define BUTTON_H 40
#define BUTTON_SPACING_Y 30
#define BUTTON_TEXTSIZE 3

// text box where title goes
#define title_X 10
#define title_Y 10
#define title_w 300
#define title_h 50
#define title_SIZE 3



#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

#define TS_MINX 130
#define TS_MAXX 905

#define TS_MINY 75
#define TS_MAXY 930
#define MINPRESSURE 10
#define MAXPRESSURE 1000


Adafruit_GFX_Button buttons[4]; // DICHIARAZIONE DELLE CAMERE
/* create 4 buttons, corresponding to the 4 vacuum chambers */
char buttonlabels[4][4] = {"1", "2", "3","4" };

// Questi parametri servono a definire il bottone on/off della modalità experiment
Adafruit_GFX_Button expmode; // DUCHIARAZIONE DI UN BOTTONE 
char ON[3] = "ON";
char OFF[4] = "OFF";


int on_off = 0; // questa variabile serve a capire quale bottone si deve disegnare on, verde oppure off, rosso
int change = 0; // il bottone viene ridisegnato all'interno del loop solo se vi è stata una modifica nello stato del bottone
                          
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD screen"));

  tft.reset();// ???? reset schermata 

  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  }else if(identifier == 0x7783) {
    Serial.println(F("Found ST7781 LCD driver"));
  }else if(identifier == 0x8230) {
    Serial.println(F("Found UC8230 LCD driver"));  
  }
  else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else if(identifier==0x9481)
  {     
       Serial.println(F("Found 0x9481 LCD driver"));
  }
  else if(identifier==0x9486)
  {     
       Serial.println(F("Found 0x9486 LCD driver"));
  }
  else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9486;
    
  }
  
  tft.begin(identifier);
  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());
  tft.setRotation(0);
  tft.fillScreen(BLACK);// schermo nero partenza

  tft.drawRect(title_X, title_Y, title_w, title_h, WHITE); // disegno rettangolo titolo

    unsigned long start = micros();
   tft.setCursor(title_X+25, title_Y+title_h/3);
    tft.setTextColor(ILI9341_BLUE);    tft.setTextSize(2);
    // Scrivo il titolo in alto
   tft.println("Camere a vuoto (mbar)");



    // Diesegno i bottoni 1,2,3,4 corrispondenti alle 4 camere a vuoto
    for (uint8_t row=0; row<4; row++) {
        buttons[row].initButton(&tft, BUTTON_X+BUTTON_W, 
                  BUTTON_Y+row*(BUTTON_H+BUTTON_SPACING_Y),    // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, ILI9341_BLUE, WHITE,
                  buttonlabels[row], BUTTON_TEXTSIZE); 
        buttons[row].drawButton();
    }


   // Disegno il bottone on/off per la modalità esperimento
   tft.setCursor(30, 385);
    tft.setTextColor(WHITE);    tft.setTextSize(2.5);
   tft.println("Experiment");

   tft.setCursor(30, 410);
    tft.setTextColor(WHITE);    tft.setTextSize(2.5);
   tft.println("mode is:");

   
expmode.initButton(&tft, 240, 410, 110, 70, WHITE, RED, WHITE, OFF, 3);
      expmode.drawButton();


//comunicazione
Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
   radio.setDataRate(RF24_250KBPS);
 radio.setChannel(124);
  
}



double pompa[4]={666,666,666,666}; // Codice di not received yet
int esponente[4]={0,0,0,0};
int i;
int head;
double temp[4];
int statoBath=0;
int acceso=1;
void loop() {

  int tempo,ref;
  // ricezione

  tempo=millis(); // DA GESTIRE IL MILLIS
ref=tempo;
int out=0;
 int network_avail=0;

  while(tempo-ref<500 && tempo-ref>-0.01 && out==0){ // regolare tempo in base a esigenze
network.update();
Serial.println("Waiting...");


  while ( network.available() ) {     // Is there any incoming data?   NON GESTISCE ROBUSTAMENTE PIù DATI????? BEH ALLORA MANDA TUTTO DA UN SOLO MASTER A SCHERMO! DOMANI GESTISCI 3 arduini!!!!!
    RF24NetworkHeader header;
     Serial.print("Ricevuto messaggio");
            
             int incomingData[5];
      network.read(header, &incomingData, sizeof(incomingData));
       temp[0]=incomingData[1];
       temp[1]=incomingData[2];
       temp[2]=incomingData[3];
       temp[3]=incomingData[4]; // se temp ricevuto è -1 NON AGGIORNARE LO SCHERMO
        network_avail=1;
        out=1;
         if(header.from_node==00){counterNR=0;}
        
        
if(header.from_node==01){counterbath=0;
        statoBath=incomingData[1]; //good or not good
        acceso=incomingData[0];}
         
  } 
   
 
  tempo=millis();

  }

// IN INCOMING DATA[0] HO CODICE DI ERRORE!
//IL QUALE GESTISCE UN ALERT MODE

// trasformazione di incoming Data QUA poi ci sarà da discernere tra i vari casi se il messaggio è inviato da master 1 o master 2 
// per semplicità codice del caso singolo. l'idea è quella di shiftare banalmente
// QUA DA AGGIUNGERE LA GESTIONE DEI 2 INDIRIZZI SMART Ways!

if(network_avail){ //aggiorna le cose solo se hai ricevuto

 

int j=0;
// MODIFICA SOLO I PRIMI DUE GLI ALTRI VANNO GIA' BENE
for (i=0;i<2;i++){
  

if (temp[i]>0){
 temp[i]=temp[i]*5/1023;
 temp[i]=pow(10,(temp[i]-3.4229)/0.3025);

   if (temp[i]<1){
                   j=0;
  // itera finchè pressure non diventa composto da 123.123 conta il numero di volte hai fatto per 10 
  while(temp[i]<1){
  temp[i]=temp[i]*10;
        j++;}
}






esponente[i]=-j;
pompa[i]= temp[i];
  }                      

//else if(pompa[i]=666); // DA GESTIRE IL CASO PRESSIONI ALTE!!!!!!!
else if (temp[i]<0) {
pompa[i]=666;   //messaggio di errore
esponente[i]=0;
Serial.println("MODIFICA FATTA");

}



// attenzione alLO ZERO MACCHINA DI 10^-12 !!! segnalare altro
if(j<-12 ){
pompa[i]=666;   //messaggio di errore
esponente[i]=0;}

}


}



if(counterNR>3 || on_off){ // GESTISCO COSI' la MANCANZA DI AGGIORNAMENTO
pompa[0]=666;  
pompa[1]=666;  
pompa[2]=666;  
pompa[3]=666;  
}

counterNR++;


// QUA GESTISTI TEMP[2] e temp[3]












  // network_avail è un booleano =1 se è stato ricevuto un messaggio
  // Se network_avail=1, devo sovrascrivere lo schermo, e quindi dicsegno un rettangolo nero per coprire i dati precedenti
  if (network_avail || on_off || counterNR==5) {tft.fillRect(BUTTON_X+130, BUTTON_Y-20, 200, 4*BUTTON_SPACING_Y+4*BUTTON_H, ILI9341_BLACK);}
  



  // Ora scrivo i nuovi dati sullo schermo
  
   tft.setCursor(BUTTON_X+140, BUTTON_Y-12);
    tft.setTextColor(WHITE);    tft.setTextSize(3);
  if (pompa[0]!=666){
     tft.print(pompa[0]); tft.print("e");
   tft.println(esponente[0]);}
   else{tft.println("NR");}

   tft.setCursor(BUTTON_X+140, BUTTON_Y-12+BUTTON_SPACING_Y+BUTTON_H);
    tft.setTextColor(WHITE);    tft.setTextSize(3);
 
  if (pompa[1]!=666){
     tft.print(pompa[1]); tft.print("e");
   tft.println(esponente[1]);}
   else{tft.println("NR");}
   
   tft.setCursor(BUTTON_X+140, BUTTON_Y-12+2*BUTTON_SPACING_Y+2*BUTTON_H);
    tft.setTextColor(WHITE);    tft.setTextSize(3);
if (pompa[2]!=666){
     tft.print(pompa[2]); tft.print("e");
   tft.println(esponente[2]);}
   else{tft.println("NR");}


  

   tft.setCursor(BUTTON_X+140, BUTTON_Y-12+3*BUTTON_SPACING_Y+3*BUTTON_H);
    tft.setTextColor(WHITE);    tft.setTextSize(3);

if (pompa[3]!=666){
     tft.print(pompa[3]); tft.print("e");
   tft.println(esponente[3]);}
   else{tft.println("NR");}



  


  tempo=millis();
   ref=tempo;
// QUA IMPLEMENTA UN ALGORITMO INTELLIGENTE

   while(tempo-ref<5000 && tempo-ref>-0.1){

digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);



 if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
   {

      p.x = p.x + p.y;       
      p.y = p.x - p.y;            
      p.x = p.x - p.y;   
      p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
      p.y = tft.height()-(map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));


      if ((expmode.contains(p.x, p.y))&&p.x>10) 
      {
          Serial.print("Pressing: "); Serial.println(0);
          expmode.press(true);  // tell the button it is pressed
      } else {
          expmode.press(false);  // tell the button it is NOT pressed
      }



   } else {
    
   expmode.press(false);  // tell the button it is NOT pressed
   }


   
   if (expmode.justReleased()) {
       Serial.print("Released: "); Serial.println(0);
       expmode.drawButton();  // draw normal
       change = 1;
       on_off = !on_off; // ON==1 
   } else {
    change=0;
   }

   if (change) {
      if (on_off){
      expmode.initButton(&tft, 240, 410, 110, 70, WHITE, ILI9341_DARKGREEN, WHITE, ON, 3);
      expmode.drawButton();
      counter=0;
   } else {
      expmode.initButton(&tft, 240, 410, 110, 70, WHITE, RED, WHITE, OFF, 3);
      expmode.drawButton();
       counter=0;
   }
   }


  tempo=millis();
   }



// MANDA INDIETRO LO STATO DELLO SCHERMO! SE è ON manda 2 volte e poi basta fino a nuovo ordine



 // MESSAGE from Node 02 to 00 && 04 && bath!!
 int data[3];
data[0]=on_off;
data[1]=0;
data[2]=0;
Serial.println(on_off);
// NON INVIARE NULLA SE SEI IN OFF
 if(counter<2  || on_off==0 ){ // qua idealmente mandi per 3/4 loop e poi interrompi la comunicazione fino a nuovo ordine on_off=1
 
 for (i=0;i<2;i++){
  RF24NetworkHeader header3(base00);     // (Address where the data is going)

 bool ok2 = network.write(header3, &data, sizeof(data)); // Send the data
  delay(20);
  
  RF24NetworkHeader header4(base04);     // (Address where the data is going)

 bool ok = network.write(header4, &data, sizeof(data)); // Send the data
  delay(20);

 RF24NetworkHeader header5(bath);     // (Address where the data is going)

 bool ok3 = network.write(header5, &data, sizeof(data)); // Send the data
  delay(100);
  
 counter ++;
 }
 
 }


// QUI IMPLEMENTA L'ALERT MODE 
if(statoBath=1){//errore FATALE
tft.fillScreen(RED);
  tft.setCursor(title_X+25, title_Y+title_h/3);
    tft.setTextColor(ILI9341_BLUE);    tft.setTextSize(5);
    // Scrivo il titolo in alto
   tft.println("FATAL ERROR IN CHILLER... RESTART IT");
delay(60000);
setup();// GO BACK TO SETUP
}


if(counterbath>10){
  tft.fillScreen(BLACK);
  tft.setCursor(title_X+25, title_Y+title_h/3);
    tft.setTextColor(ILI9341_BLUE);    tft.setTextSize(5);
    // Scrivo il titolo in alto
   tft.println("NOT RECEIVING FROM CHILLER...CHECK IT");
delay(60000);
setup();// GO BACK TO SETUP
  }//SCRIVI NON STO RICEVENDO NOTIZIE

counterbath++;




   
   }
   
   

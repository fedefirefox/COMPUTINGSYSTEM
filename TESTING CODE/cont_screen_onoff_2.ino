#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
//#define LCD_CS A3 // Chip Select goes to Analog 3
//#define LCD_CD A2 // Command/Data goes to Analog 2
//#define LCD_WR A1 // LCD Write goes to Analog 1
//#define LCD_RD A0 // LCD Read goes to Analog 0

//#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */

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



Adafruit_GFX_Button buttons[4];
/* create 15 buttons, in classic candybar phone style */
char buttonlabels[4][4] = {"1", "2", "3","4" };

Adafruit_GFX_Button expmode;
char ON[3] = "ON";
char OFF[4] = "OFF";

int on_off = 1;
int change = 0;
                             
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD screen"));

  tft.reset();

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
  tft.fillScreen(BLACK);

  tft.drawRect(title_X, title_Y, title_w, title_h, WHITE);

    unsigned long start = micros();
   tft.setCursor(title_X+25, title_Y+title_h/3);
    tft.setTextColor(ILI9341_BLUE);    tft.setTextSize(2);
   tft.println("Camere a vuoto (mbar)");




    for (uint8_t row=0; row<4; row++) {
        buttons[row].initButton(&tft, BUTTON_X+BUTTON_W, 
                  BUTTON_Y+row*(BUTTON_H+BUTTON_SPACING_Y),    // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, ILI9341_BLUE, WHITE,
                  buttonlabels[row], BUTTON_TEXTSIZE); 
        buttons[row].drawButton();
    }


   tft.setCursor(30, 385);
    tft.setTextColor(WHITE);    tft.setTextSize(2.5);
   tft.println("Experiment");

   tft.setCursor(30, 410);
    tft.setTextColor(WHITE);    tft.setTextSize(2.5);
   tft.println("mode..");
   
  expmode.initButton(&tft, 240, 410, 110, 70, WHITE, ILI9341_DARKGREEN, WHITE, ON, 3);
  expmode.drawButton();
  
}


#define MINPRESSURE 10
#define MAXPRESSURE 1000


void loop() {
  // put your main code here, to run repeatedly:



  
   tft.setCursor(BUTTON_X+140, BUTTON_Y-12);
    tft.setTextColor(WHITE);    tft.setTextSize(3);
   tft.println("23.4 e-3");

   tft.setCursor(BUTTON_X+140, BUTTON_Y-12+BUTTON_SPACING_Y+BUTTON_H);
    tft.setTextColor(WHITE);    tft.setTextSize(3);
   tft.println("23.4 e-3");

   tft.setCursor(BUTTON_X+140, BUTTON_Y-12+2*BUTTON_SPACING_Y+2*BUTTON_H);
    tft.setTextColor(WHITE);    tft.setTextSize(3);
   tft.println("23.4 e-3");

   tft.setCursor(BUTTON_X+140, BUTTON_Y-12+3*BUTTON_SPACING_Y+3*BUTTON_H);
    tft.setTextColor(WHITE);    tft.setTextSize(3);
   tft.println("23.4 e-3");


  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  
 // p = ts.getPoint(); 
  /*
  if (ts.bufferSize()) {
    
  } else {
    // this is our way of tracking touch 'release'!
    p.x = p.y = p.z = -1;
  }*/
  
  // Scale from ~0->4000 to tft.width using the calibration #'s
  /*
  if (p.z != -1) {
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    Serial.print("("); Serial.print(p.x); Serial.print(", "); 
    Serial.print(p.y); Serial.print(", "); 
    Serial.print(p.z); Serial.println(") ");
  }*/
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
       on_off = !on_off;
   } else {
    change=0;
   }

   if (change) {
      if (on_off){
      expmode.initButton(&tft, 240, 410, 110, 70, WHITE, ILI9341_DARKGREEN, WHITE, ON, 3);
      expmode.drawButton();
   } else {
      expmode.initButton(&tft, 240, 410, 110, 70, WHITE, RED, WHITE, OFF, 3);
      expmode.drawButton();
   }
   }
   
  

}

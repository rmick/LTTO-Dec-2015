

//////////////////////Include libraries////////////////////////
#include <Adafruit_GFX.h>
#include <SWTFT.h>
#include <TouchScreen.h>
#include <EEPROM.h>
//#include <EnableInterrupt.h>

//////////////////////Setup Touchscreen///////////////////////
#define YP A1
#define XM A2
#define YM 7
#define XP 6

SWTFT tft;    //Defines all control and data lines for LCD shield.

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define MinValTouchX  947
#define MinValTouchY  958
#define MaxValTouchX  90
#define MaxValTouchY  137

////////////////////Define common colours//////////////////////
#define  BLACK    0x0000
#define  BLUE     0x001F
#define  RED      0xF800
#define  GREEN    0x07E0
#define  CYAN     0x07FF
#define  MAGENTA  0xF81F
#define  YELLOW   0xFFE0
#define  WHITE    0xFFFF

////////////////////My Variables //////////////////////////////

const bool TRUE =  1;
const bool FALSE = 0;

byte numLives =   EEPROM.read(0);
byte medicDelay = EEPROM.read(2); 
byte teamID =     EEPROM.read(4);         
byte playerID =   EEPROM.read(6);
bool hostile =    EEPROM.read(8);
       


const byte IR_LED = 13;
const byte IR_RECEIVE_PIN = 11;

//DeBug use only
unsigned long irTime;
const bool deBug = 1;

int timer1counter;

const char MEDIC            = 'm';
const char PINPAD           = 'p';
const char TAGGER           = 't';
const char CONFIG           = 'c';
const char NONE             = 'n';
const char SET_TEAM         = 'i';
const char SET_MEDIC_DELAY  = 'd';
const char SET_HOSTILE      = 'h';
const char CLEAR_SCORE      = 'r';

char state = PINPAD;
char lastState = NONE;

byte buttonCount;
bool touchGood = 0;




  
///////////////////////////////////////////////////////////////

void setup()
{
  if (deBug) Serial.begin(250000);
  pinMode (IR_LED, OUTPUT);
  pinMode (IR_RECEIVE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IR_RECEIVE_PIN), ISRpulse, CHANGE);
  
  /////////////////Setup the LCD screen////////////////////////
  tft.reset();
  uint16_t identifier = tft.readID();
  //if (deBug) Serial.println(identifier);
  tft.begin(identifier); 
  //tft.setRotation(0);
  tft.fillScreen(BLACK);            // It fails first time, so do it here before we start the program
}

////////////////////// MAIN LOOP ///////////////////////////////////////////////////////////////////////// MAIN LOOP ////////////////////////

void loop()
{ 
  if      (state == MEDIC)            MedicMode();
  else if (state == PINPAD)           PinPadMode();
  else if (state == TAGGER)           TaggerMode();
  else if (state == CONFIG)           ConfigMode();
  else if (state == SET_TEAM)         SetTeam();
  else if (state == SET_MEDIC_DELAY)  SetMedicDelay();
  else if (state == SET_HOSTILE)    SetHostile();
  else if (state == CLEAR_SCORE)    ClearScore();
}

////////////////////// END MAIN LOOP//////////////////////////////////////////////////////////////////// END MAIN LOOP //////////////////////

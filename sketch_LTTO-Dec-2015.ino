
//////////////////////Include libraries////////////////////////
#include <EnableInterrupt.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
//#include <QueueArray.h>
#include <EEPROM.h>
#include <SWTFT.h>

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

const byte eeMEDIC_COUNT =   0;
const byte eeMEDIC_DELAY =   2; 
const byte eeTEAM_ID =       4;         
const byte eePLAYER_ID =     6;
const bool eeHOSTILE =       8;
const byte eeRELOAD_AMOUNT = 10;
const byte eeMAX_RELOADS =   12;
const byte eeSHIELDS_TIMER = 14;

byte medicCount =   EEPROM.read(eeMEDIC_COUNT);
byte medicDelay =   EEPROM.read(eeMEDIC_DELAY); 
byte teamID =       EEPROM.read(eeTEAM_ID);         
byte playerID =     EEPROM.read(eePLAYER_ID);
bool hostile =      EEPROM.read(eeHOSTILE);
byte reloadAmount = EEPROM.read(eeRELOAD_AMOUNT);
byte maxReloads =   EEPROM.read(eeMAX_RELOADS);
byte shieldsTimer = EEPROM.read(eeSHIELDS_TIMER);
byte shotCount = reloadAmount;

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

const byte  ARRAY_LENGTH = 24;
int8_t      messageIR         [ARRAY_LENGTH];
uint16_t    messageIRpulse    [ARRAY_LENGTH];
uint16_t    messageISRdelay   [ARRAY_LENGTH];
uint16_t    messageISRelapsed [ARRAY_LENGTH];

struct irMessage
{
  char type;
  byte byteMsb;
  byte byteLsb;
};

irMessage receivedIRmessage;

///////////////////////////////////////////////////////////////

void setup()
{
  if (deBug) Serial.begin(250000);
  pinMode (IR_LED, OUTPUT);
  pinMode (IR_RECEIVE_PIN, INPUT_PULLUP);
  enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE);
  receivedIRmessage.type = '_';
  
  /////////////////Setup the LCD screen////////////////////////
  tft.reset();
  uint16_t identifier = tft.readID();
  //if (deBug) Serial.println(identifier);
  tft.begin(identifier); 
  //tft.setRotation(0);
  tft.fillScreen(BLACK);            // It fails first time, so do it here before we start the program

//Initialises the EEPROM on first upload/run.
if (maxReloads = 255)   { maxReloads =    0;  EEPROM.write(eeMAX_RELOADS, 0);  }
if (medicDelay = 255)   { medicDelay =   10;  EEPROM.write(eeMEDIC_DELAY, 10); }
if (medicCount = 255)   { medicCount =    0;  EEPROM.write(eeMEDIC_COUNT,  0); }
if (shieldsTimer = 255) { shieldsTimer = 30;  EEPROM.write(eeSHIELDS_TIMER, 15); }  
if (reloadAmount = 255) { reloadAmount = 15;  EEPROM.write(eeRELOAD_AMOUNT, 15); shotCount = reloadAmount; }
}

////////////////////// MAIN LOOP ///////////////////////////////////////////////////////////////////////// MAIN LOOP ////////////////////////

void loop()
{ 
  ////////////////////////
  if      (state == MEDIC)            MedicMode();
  else if (state == PINPAD)           PinPadMode();
  else if (state == TAGGER)           TaggerMode();
  else if (state == CONFIG)           ConfigMode();
  else if (state == SET_TEAM)         SetTeam();
  else if (state == SET_HOSTILE)      SetHostile();
  else if (state == CLEAR_SCORE)      ClearScore();
  else if (state == SET_MEDIC_DELAY)  SetMedicDelay();
  //////////////////////////////////

  static byte healthCount = 0;
  static byte badMessageCount = 0;

  if (receivedIRmessage.type != '_')
  {
    if (receivedIRmessage.type == 'T')  // && receivedIRmessage.byteMsb == 0 && receivedIRmessage.byteLsb == 0)
    {
      healthCount++;
      Serial.print(F("\n ----------------------------- BANG ! - # ")); Serial.print(healthCount); Serial.println(F(" -----------------------------"));
      Serial.println();
    }
    else if (receivedIRmessage.type == 'B')
    {
      Serial.print("b");
    }
    else
    {
      badMessageCount++;
      Serial.print(F("\n--- bad Tag message - # "));   Serial.print(badMessageCount); Serial.println(F(" ---"));
      Serial.println();
    }
    ClearIRarray();
  }
}

////////////////////// END MAIN LOOP//////////////////////////////////////////////////////////////////// END MAIN LOOP //////////////////////

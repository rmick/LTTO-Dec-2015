//////////////////////Include libraries////////////////////////
#include <Adafruit_GFX.h>
#include <SWTFT.h>
#include <TouchScreen.h>
#include <EEPROM.h>

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

const bool True = 1;
const bool False = 0;

int numLives = EEPROM.read(0);
int chargeDelay = EEPROM.read(2);   // TODO : can be changed via config screen - need to store in Flash.        
bool touchGood = 0;
bool keyNumNew = 0;

const byte IrLED = 13;
const byte IrReceivePin = 11;

//DeBug use only
unsigned long irTime;
const bool deBug = 1;

int timer1counter;

const char Medic = 'm';
const char PinCode = 'p';
const char Tagger = 't';
const char Config = 'c';
const char Null = 'n';

char state = PinCode;
char lastState = Null;

byte ButtonCount;
  
///////////////////////////////////////////////////////////////

void setup()
{
  if (deBug) Serial.begin(250000);
  pinMode (IrLED, OUTPUT);
  pinMode (IrReceivePin, INPUT_PULLUP);
//  attachInterrupt(digitalPinToInterrupt(IrReceivePin), ISRpulse, CHANGE);
  
  /////////////////Setup the LCD screen////////////////////////
  tft.reset();
  uint16_t identifier = tft.readID();
  if (deBug) Serial.println(identifier);
  tft.begin(identifier); 
  //tft.setRotation(0);
  tft.fillScreen(BLACK);            // It fails first time, so do it here before we start the program

                chargeDelay = 2;     /////////////////////////////////////////////////////////////////////////TEMPORARY

//////////////////////TIMER 1 INTERUPT///////////////////////////

  // initiliase timer1
  noInterrupts();
  TCCR1A = 0;               // clear the TCCR1A register (known starting point)
  TCCR1B = 0;               // clear the TCCR1A register (known starting point)

  // set Timer1counter to the correct value for the interrupt interval
  //timer1counter = 64911; //preload timer 65536-16MHz/256/100Hz
  //timer1counter = 64286; //preload timer 65536-16MHz/256/50Hz
  //timer1counter = 34286; //preload timer 65536-16MHz/256/2Hz
  timer1counter = 1; 

  TCNT1 = timer1counter;                              // preload timer
  TCCR1B |= (1 << CS12);                              // 256 prescaler
  //TCCR1B |= (1 << CS12); TCCR1B |= (1 << CS10);     // 1024 prescaler
  TIMSK1 |= (1 << TOIE1);     // enable timer overflow interupt     
  interrupts();

}


ISR(TIMER1_OVF_vect)          // interrrupt service routine
  {
    // if (deBug) Serial.println(" Tick");
    digitalWrite(13, !digitalRead(13));
    TCNT1 = timer1counter;    //re-preload the timer
  }
//////////////////////MAIN LOOP//////////////////////////////////


void loop()
{
  if (state == Medic)
  {
    // Run the medic screen code
    if (lastState != Medic)
    {
      if (deBug) Serial.println(F("Medic-Mode"));
      lastState = Medic;
      DrawMedicScreen();
    }
    ReadMedicScreen();
  }
  else if (state == PinCode)
  {
    // Run the pinpad code
    if (lastState != PinCode)
    {
      if (deBug) Serial.println(F("PinCode-Mode"));
      lastState = PinCode;
      PinPadScreen();
    }
  }
  else if (state == Tagger)
  {
    // Run the Tagger mode code
    if (lastState != Tagger)
    {
      if (deBug) Serial.println(F("Tagger-Mode"));
      lastState = Tagger;
      DrawTaggerScreen();
    }
    TaggerMode();
  }
  else if (state == Config)
  {
    // Run the setup screen code
    if (lastState != Config)
    {
      if (deBug) Serial.println(F("Config-Mode"));
      lastState = Config;
      SetUp();
    }
  }
  else
  {
    Serial.println(F("How did we get here? State is not valid !"));
  }
    
  ///////////////////////////////////

}

void GetTouchInput()
{
  touchGood = 0;
  TSPoint p = ts.getPoint();
  // Sharing pins, so we need to reset the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > 10 && p.z <1000) touchGood = 1;      // z = pressure
  //return touchGood;
}

int CountDigits(int num)
{
  int count=0;
  while(num)
  {
    num=num/10;
    count++;
  }
  return count;
}

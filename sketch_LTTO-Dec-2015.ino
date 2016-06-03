
////////////////////// Include libraries ////////////////////////

#include <EnableInterrupt.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <EEPROM.h>

#include <SWTFT.h>
SWTFT tft;

//#include <MCUFRIEND_kbv.h>
//MCUFRIEND_kbv tft;

////////////////////// Setup Touchscreen ////////////////////////

#define YP A1
#define XM A2
#define YM 7
#define XP 6

//    //Defines all control and data lines for LCD shield.

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define MIN_VAL_TOUCH_X  947
#define MIN_VAL_TOUCH_Y  958
#define MAX_VAL_TOUCH_X   90
#define MAX_VAL_TOUCH_Y  137

//////////////////// Define common colours //////////////////////

#define  BLACK    0x0000
#define  BLUE     0x001F
#define  RED      0xF800
#define  GREEN    0x07E0
#define  CYAN     0x07FF
#define  MAGENTA  0xF81F
#define  YELLOW   0xFFE0
#define  WHITE    0xFFFF
#define  GREY     0x5050

//////////////////// Global Constants //////////////////////////////

//const bool true =  1;
//const bool false = 0;

const byte eeMEDIC_COUNT =   0;
const byte eeMEDIC_DELAY =   2; 
const byte eeTEAM_ID =       4;         
const byte eePLAYER_ID =     6;
const bool eeHOSTILE =       8;
const byte eeRELOAD_AMOUNT = 10;
const byte eeMAX_RELOADS =   12;
const byte eeSHIELDS_TIMER = 14;
const byte eePIN_CODE = 16;         // NB. It is a 4 byte array, so next available is 20 !
// const byte ee???? = 20;

//////////////////// Medic Variables ////////////////////////////////

byte medicCount =   EEPROM.read(eeMEDIC_COUNT);
byte medicDelay =   EEPROM.read(eeMEDIC_DELAY);
bool hostile =      EEPROM.read(eeHOSTILE);
byte pinCode[4] =   {EEPROM.read(eePIN_CODE), EEPROM.read(eePIN_CODE+1), EEPROM.read(eePIN_CODE+2), EEPROM.read(eePIN_CODE+3) };

//////////////////// Tagger Variables ///////////////////////////////

byte teamID =       EEPROM.read(eeTEAM_ID);         
byte playerID =     EEPROM.read(eePLAYER_ID);
byte reloadAmount = EEPROM.read(eeRELOAD_AMOUNT);
byte maxReloads =   EEPROM.read(eeMAX_RELOADS);
byte shieldsTimer = EEPROM.read(eeSHIELDS_TIMER);
byte playerHealth = 50;
byte tagCount = reloadAmount;
bool shieldsUp = false;
bool friendlyFire = false;
byte scoreGrid[25];       // One extra so we can ignore base0 issues

const byte IR_LED             = 13;
const byte IR_RECEIVE_PIN     = 11;
const byte DE_BUG_TIMING_PIN  = 12;

//#define DEBUG



  ////---------------------------------------------------------------------------------------------------------
  //  Set ISR variables
  
    volatile byte irPacketLength = 0;
    volatile byte countISR = 0;

    // DeBug use only
    volatile byte shortPulseLengthError = 0;
    volatile byte arrayOverLengthError = 0;

  ////---------------------------------------------------------------------------------------------------------
  //  Set CreateIRmessage Debug variables

     int badMessage_CountISRshortPacket = 0;
     int badMessage_InvalidPacketType = 0;
     int badMessage_non3_6Header = 0;


      bool debugStartHost = false;


/////////////////////// State machine Constants ////////////////////////////

const char MEDIC            = 'm';
const char PINPAD           = 'p';
const char TAGGER           = 't';
const char CONFIG1          = '1';
const char NONE             = 'n';
const char SET_TEAM         = 'i';
const char SET_MEDIC_DELAY  = 'd';
const char SET_HOSTILE      = 'h';
const char CLEAR_SCORE      = 'r';
const char GAME_OVER        = 'g';
const char CONFIG2          = '2';
const char CHANGE_PIN       = 'x';
const char CONFIRM_PIN      = 'y';
const char SCORES           = 'z';
const char HOST             = 'H';
char state = MEDIC;
char lastState = NONE;

byte buttonCount;
bool touchGood = 0;


  ////---------------------------------------------------------------------------------------------------------
  //  Set up the irMessage data structure and then create an instance of it 'decodedIRmessage'

    struct decodedIR             //  Most of these are accessed via ISR so must be declared volatile
    {
      volatile bool           newMessage;           //  true = Yes there is a new message waiting
      volatile char           type;                 //  T, B, P, D, C
      volatile unsigned int   rawDataPacket;        //  The undecoded datapacket
      volatile unsigned int   messageOverwritten;   //  This counts messages that are overwritten by a new packet before being read by the main loop if(DecodeIR() ) call
      unsigned int            TeamID;               //  Team 0 = No Team, then 1,2,3
      unsigned int            PlayerID;             //  Player 1 thru 8
      unsigned int            ShotStrength;         //  Mega = 1 thru 4
      // TODO: These are yet to be implemented in DecodeIR()
      bool                    TagReceivedBeacon;    //  true = Beacon sent due to Tagger being tagged (confirmation)
      int                     PacketByte;           //  The undecoded Packet number in Hex
      String                  PacketName;           //  The simple name for the packet (e.g. Announce Game)  - source   wiki.lazerswarm.com
      String                  DataType;             //  What the current DataByte contains (e.g. GameID, TaggerID, ShieldTime, PackedByte1, etc)
      long int                DataByte;             //  Data Byte (in Hex)
      uint8_t                 CheckSumRxByte;       //  CheckSum value (in Hex)
      bool                    CheckSumOK;           //  true = The Checksum matches the data packets
    };

    decodedIR decodedIRmessage;

    static uint8_t  byteCount = 0;                                   // Used to keep track of which datapacket is what.

    uint8_t   assignToTeam = 0;
    uint8_t   assignToPlayer = 0;
    uint8_t   CheckSumRx = 0;

    

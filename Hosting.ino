//
//
/*
Announce Game
Sent By: Host
Signature Type: Packet
Signature Count: 10-14 (including checksum)

The Announce Game packet is sent by the host at 1.5 second intervals (from start to start) until it receives a Request Join Game packet from a joining tagger.



- Packet Type (9 bits) - FIRST bit must be 0
    0x002 - Announce Custom Lazer Tag Game
    0x003 - Announce Custom Lazer Tag (2 Teams) Game
    0x004 - Announce Custom Lazer Tag (3 Teams) Game
    0x005 - Announce Hide and Seek Game
    0x006 - Announce Hunt the Prey Game
    0x007 - Announce Kings (2 Teams) Game
    0x008 - Announce Kings (3 Teams) Game
    0x009 - Announce Own the Zone Game
    0x00A - Announce Own the Zone (2 Teams) Game
    0x00B - Announce Own the Zone (3 Teams) Game
    0x00C - Announce Special Game
 
- Game ID (8 bits)

- Game length in minutes (8 bits) BCD

- Tags (8 bits) BCD

- Reloads (8 bits) BCD, 0xFF for unlimited

- Shields (8 bits) BCD

- Mega-Tags (8 bits) BCD, 0xFF for unlimited

- Flags 1 (8 bits)
    Neutralize players when tagged 10 times (1 bits)  ************ This is the one for Medic Stations to work *******
    Limited reloads (1 bits)
    Limited mega-tags (1 bits)
    Team Tags (1 bits)
    Medic Mode (1 bits)
    Slow Tags (1 bits)
          Maximum of one tag every other second
    Hunt the Prey (1 bits)
    Hunt direction (1 bits)
          0 - Team 1 hunts first
          1 - Team 2 hunts first)

- Flags 2 (8 bits)
    Contested zones (1 bits)
    Zones can be associated with teams (1 bits)
          "Number of teams" must be greater than 1.
          If this is set to 0, zones associated with teams will not affect players.
    Neutralize players for 15 seconds when tagged (1 bits)
    Supply zones unneutralize players (1 bits)
    Supply zones refill tags (1 bits)
    Hostile zones (1 bits)
          "Supply zones unneutralize players" must be 0.
          "Supply zones refill tags" must be 0.
    Number of teams (2 bits)
    
- Game Name (32 bits) only for packet type 0x00C

- Checksum (9 bits) - FIRST bit must be 1

 * 
 */

 ///////////////////////////////////////////////////////////////////////////////

  byte hostedGameType           = 0x02;
  byte hostedGameID             = 0xAF;
  byte hostedGameTime           = 0x15;
  byte hostedTagsAvailable      = 0x50;
  byte hostedReloadsAvailable   = 0xFF;
  byte hostedShieldTime         = 0x15;
  byte hostedMegaTags           = 0xFF;
  byte hostedPackedFlags1       = 0x00;
  byte hostedPackedFlags2       = 0x01;
  byte ascii1              = 0;
  byte ascii2              = 0;
  byte ascii3              = 0;
  byte ascii4              = 0;

  bool patent = TRUE;

//////////////////////////////////////////////////////////////////////

void HostMode()
{
  DrawHostMode();
  
  static unsigned long hostTimer = micros();

  char* Action = GetButtonPress();
  if      (Action == "Host 2 Teams")  AnnounceCustomGame();
  else if (Action == "Join Game")     RequestJoinGame();
  else if (Action == "Make Coffee")   LinearNumbersTest();
  else if (Action == "EXIT")          state = CONFIG2;
}

///////////////////////////////////////////////////////////////////////////////

void DrawHostMode()
{
  if (lastState != state)
  {
    #ifdef DEBUG Serial.println(F("HostMode Screen"));
    #endif
    DrawScreen(HOST, "HOST GAME", MAGENTA, WHITE, 3);
    DrawButton( 20,  50, 200, 55, BLACK,  "Host 2 Teams", 2, WHITE);
    DrawButton( 20, 120, 200, 55, BLACK,  "Join Game",  2, WHITE);
    DrawButton( 20, 210, 200, 55, BLACK,  "Make Coffee", 2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT",       2, BLACK);
    if (deBug) PrintButtonArray();
  }
}

///////////////////////////////////////////////////////////////////////////////

 void AnnounceCustomGame()
 {
  disableInterrupt(IR_RECEIVE_PIN);
  
  //  Game sample from Patent
  byte hostedCheckSum;
  if (patent)
  {
    hostedGameType           = 0x0C;
    hostedGameID             = 0x2C;
    hostedGameTime           = 15;
    hostedTagsAvailable      = 0x50;
    hostedReloadsAvailable   = 0xFF;
    hostedShieldTime         = 0x45;
    hostedMegaTags           = 0x12;
    hostedPackedFlags1       = 0x28;
    hostedPackedFlags2       = 0xA2;
    ascii1              = 0x32;
    ascii2              = 0x5A;
    ascii3              = 0x4F;
    ascii4              = 0x4E;
    hostedCheckSum      = 0xE6;
  }
  
  Serial.println(F("\nAnnounce Custom Game"));
  SendIR('P', hostedGameType);
  SendIR('D', hostedGameID);
  SendIR('D', ConvertDecToBCD(hostedGameTime));
  SendIR('D', hostedTagsAvailable);
  SendIR('D', hostedReloadsAvailable);
  SendIR('D', hostedShieldTime);
  SendIR('D', hostedMegaTags);
  SendIR('D', hostedPackedFlags1);
  SendIR('D', hostedPackedFlags2);
  if (patent)
  {
    SendIR('D', ascii1);
    SendIR('D', ascii2);
    SendIR('D', ascii3);
    SendIR('D', ascii4);
    SendIR('C', hostedCheckSum);
    Serial.print(F("\nCalc : Correct :\t"));
    Serial.print(CalculateCheckSum());
    Serial.print(F(": "));
    Serial.print(hostedCheckSum);
  }
  else SendIR('C', CalculateCheckSum());

 
  enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE);
}

///////////////////////////////////////////////////////////////////////////////

void RequestJoinGame()
{
  Serial.println(F("\nRequest Join Game"));
  SendIR('P', 0x10);
  SendIR('D', hostedGameID);
  SendIR('D', 0x42);
  SendIR('D', 0x00);
  SendIR('C', 0x98);  //Junk for now I just need a 9 bit packet.
}

///////////////////////////////////////////////////////////////////////////////

void LinearNumbersTest()
{
  Serial.println(F("\nLinearNumbersTest"));
  SendIR('P', 0x01);
  SendIR('D', 0x02);
  SendIR('D', 0x03);
  SendIR('D', 0x04);
  SendIR('C', 0x05);  //Junk for now I just need a 9 bit packet.
}

///////////////////////////////////////////////////////////////////////////////

uint8_t CalculateCheckSum()
{
  uint8_t checkSum;
  checkSum =  hostedGameType + ConvertDecToBCD(hostedGameID)+ hostedGameTime + hostedTagsAvailable
              + hostedReloadsAvailable + hostedShieldTime + hostedMegaTags
              + hostedPackedFlags1 + hostedPackedFlags2 + ascii1 + ascii2
              + ascii3 + ascii4;
  return checkSum;
}

///////////////////////////////////////////////////////////////////////////////

byte ConvertDecToBCD(byte dec)
{
  if (dec == 0xff) return dec;
  return (byte) (((dec/10) << 4) | (dec %10) );
}

byte ConvertBCDtoDec(byte bcd)
{
  if (bcd == 0xff) return bcd;
  return (byte) (((bcd >> 4) & 0xF) *10) + (bcd & 0xF);
}
///////////////////////////////////////////////////////////////////////////////

void SendText()
{
  SendIR('P', ConvertBCDtoDec(80));
  SendIR('D', ConvertBCDtoDec(48));
  SendIR('D', ConvertBCDtoDec(45));
  SendIR('D', ConvertBCDtoDec(0x4C));
  SendIR('D', ConvertBCDtoDec(0x4C));
  SendIR('D', ConvertBCDtoDec(0x4F));
  SendIR('C', ConvertBCDtoDec(0xF4));
}


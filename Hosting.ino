//////////////////////////////////////////////////////////////////////

void HostMode()
{
  DrawHostMode();
  
  static unsigned long hostTimer = millis();
  static bool hostingActive = FALSE;

  if(debugStartHost) hostingActive = TRUE;
    
  char const* Action = GetButtonPress();
  if      (Action == "Host 2 Teams")    hostingActive = TRUE;
  else if (Action == "Assign Player")   AssignPlayer();
  else if (Action == "CountDown")       StartCountDown();
  else if (Action == "EXIT")            state = CONFIG2;




  if (millis() - hostTimer > 1500 && hostingActive)
  {
    AnnounceCustomGame();
    hostTimer = millis();
  }
  //if (PacketByte
  

 
}





 
///////////////////////////////////////////////////////////////////////////////

void DrawHostMode()
{
  if (lastState != state)
  {
    #ifdef DEBUG
      Serial.println(F("HostMode Screen"));
    #endif
    DrawScreen(HOST, "HOST GAME", MAGENTA, WHITE, 3);
    DrawButton( 20,  50, 200, 55, BLACK,  "Host 2 Teams",   2, WHITE);
    DrawButton( 20, 120, 200, 55, BLACK,  "Assign Player",  2, WHITE);
    DrawButton( 20, 210, 200, 55, BLACK,  "Countdown",      2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT",           2, BLACK);

    debugStartHost = FALSE;
  }
}

///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////

  uint8_t   checkSumCalc = 0;

  ////--------------------------------------------------
  //  Host 2Teams Game (captured packets from LTTO host)
  byte hostedGameType           = 0x03;
  byte hostedGameID             = 0xED;
  byte hostedGameTime           = 0x10;
  byte hostedTagsAvailable      = 0x99;
  byte hostedReloadsAvailable   = 0xFF;
  byte hostedShieldTime         = 0x15;
  byte hostedMegaTags           = 0x10;
  byte hostedPackedFlags1       = 0x38;
  byte hostedPackedFlags2       = 0x02;

///////////////////////////////////////////////////////////////////////////////

void AnnounceCustomGame()
 {
  Serial.println(F("\nAnnounce 2 Teams Game"));
  SendIR('P', hostedGameType);
  SendIR('D', hostedGameID);
  SendIR('D', hostedGameTime);
  SendIR('D', hostedTagsAvailable);
  SendIR('D', hostedReloadsAvailable);
  SendIR('D', hostedShieldTime);
  SendIR('D', hostedMegaTags);
  SendIR('D', hostedPackedFlags1);
  SendIR('D', hostedPackedFlags2);
  SendIR('C', checkSumCalc);
}

///////////////////////////////////////////////////////////////////////////////

uint8_t taggerID = 0;

void AssignPlayer()
{
  Serial.println(F("\nAssignPlayer"));
  SendIR('P', 0x01);
  SendIR('D', hostedGameID);
  SendIR('D', taggerID);
  SendIR('D', B01110);
  SendIR('C', checkSumCalc);
}

///////////////////////////////////////////////////////////////////////////////

void StartCountDown()
{
  static byte CountDownTime = 0x0A;           //TODO: his needs to be BCD not HEX
  Serial.println(F("\nStartCountDown"));
  SendIR('P', 0x00);
  SendIR('D', hostedGameID);
  SendIR('D', CountDownTime);
  SendIR('D', 0x08);
  SendIR('D', 0x08);
  SendIR('D', 0x00);
  SendIR('C', checkSumCalc);
  CountDownTime--;
  delay (1000);
  if (CountDownTime >0) StartCountDown();
}




///////////////////////////////////////////////////////////////////////////////

void RequestJoinGame()
{
  Serial.println(F("\nRequest Join Game"));
  SendIR('P', 0x10);
  SendIR('D', 0x7A);
  SendIR('D', 0x42);
  SendIR('D', 0x00);
  SendIR('C', checkSumCalc);
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
  SendIR('P', 0x80);
  SendIR('D', 0x48);
  SendIR('D', 0x45);
  SendIR('D', 0x4C);
  SendIR('D', 0x4C);
  SendIR('D', 0x4F);
  SendIR('C', 0xF4);
}


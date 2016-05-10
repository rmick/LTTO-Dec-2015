//////////////////////////////////////////////////////////////////////
static bool hostingActive = FALSE;


void HostMode()
{
  DrawHostMode();
  
  static unsigned long hostTimer = millis();

  if(debugStartHost) hostingActive = TRUE;
    
  char const* Action = GetButtonPress();
  if      (Action == "Host 2 Teams")
  {
    assignToTeam = 1;
    assignToPlayer = 1;
    hostingActive = TRUE;
  }
  else if (Action == "Start Game")   StartCountDown();
  else if (Action == "EXIT")         state = CONFIG2;


  if (millis() - hostTimer > 1500 && hostingActive)
  {
    AnnounceCustomGame();
    hostTimer = millis();
  }

  if (decodedIRmessage.CheckSumOK == TRUE)  ActionCompletePacketandData();
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
    DrawButton( 20, 120, 200, 55, BLACK,  "",               2, WHITE);
    DrawButton( 20, 210, 200, 55, BLACK,  "Start Game",     2, GREY );
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT",           2, BLACK);

    debugStartHost = FALSE;
  }
}

///////////////////////////////////////////////////////////////////////////////

////--------------------------------------------------
  //  Hosting variables

  uint8_t   checkSumCalc = 0;
  uint8_t   taggerID = 0;
 

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
  Serial.println(F("\n\tAnnounce 2 Teams Game"));
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
bool gameIDmatch = FALSE; 


void ActionRequestJoinGameDataByte()
{
    if      (byteCount == 1 && decodedIRmessage.DataByte == hostedGameID)     gameIDmatch = TRUE;
    else if (byteCount == 2)                                                  taggerID = decodedIRmessage.DataByte;
    else if (byteCount == 3)                                                  if(decodedIRmessage.DataByte == 0) TeamAndPlayerAutoSelect();
    // now wait for the CheckSum and then AssignPlayer() 
}

///////////////////////////////////////////////////////////////////////////////



void ActionCompletePacketandData()
{
Serial.print(F("\nCheckSum correct !"));
  if      (decodedIRmessage.PacketName == "RequestJoinGame" && gameIDmatch == TRUE && taggerID != 0)     AssignPlayer();
  else if (decodedIRmessage.PacketName == "AckPlayerAssign" && gameIDmatch == TRUE && taggerID != 0)
    {
      CheckTaggerAssignedCorrectly();
      //TOO: Now add it to the list of known taggers for Countdown flags
      Serial.print(F("\n\tThe tagger has Ack'd our AssignPlayer()"));
      //SendAckToTagger();
    }
}    
///////////////////////////////////////////////////////////////////////////////
bool AckPlayerAssignOK = FALSE;

void ActionAcknowledgePlayerAssignDataByte()
{
   Serial.print(F("\tyep"));
   if      (byteCount == 1 && decodedIRmessage.DataByte == hostedGameID)
   {
    if(hostedGameID == decodedIRmessage.DataByte) gameIDmatch = TRUE;
    Serial.print(F(" -1"));
   }
   else if (byteCount == 2)                                                  
   {
    if (taggerID == decodedIRmessage.DataByte) AckPlayerAssignOK = TRUE;
    Serial.print(F(" -2"));
   }
   // now wait for the CheckSum and then CheckTaggerAssignedCorrectly() 
}


///////////////////////////////////////////////////////////////////////////////

void CheckTaggerAssignedCorrectly()
{
  if (gameIDmatch && AckPlayerAssignOK) Serial.print(F("Got 'im !"));
  else Serial.print(F("\nNope !!!"));
}





byte assignToTeamAndPlayer = 0;


void TeamAndPlayerAutoSelect()              //TODO: Need a way to stop Team X assignments via user intervention (eg. Only 4 players on Team 1 instead of 8)
{
  assignToTeamAndPlayer = assignToTeam << 3;                                //  TeamID is 1 based
  
  assignToTeamAndPlayer = assignToTeamAndPlayer + (assignToPlayer-1);       //  PlayerID is 0 based
  assignToPlayer++;

  if (assignToPlayer > 8)
  {
    if (assignToTeam == 2)  StartCountDown();
    else
    {
      assignToTeam++;
      assignToPlayer = 1;
    } 
  } 
}

///////////////////////////////////////////////////////////////////////////////

void AssignPlayer()                              // This is the reply to:    0x010 - Request Join Game
{
  Serial.println(F("\nAssignPlayer - "));
  SendIR('P', 0x01);
  SendIR('D', hostedGameID);
  SendIR('D', taggerID);
  SendIR('D', assignToTeamAndPlayer);
  SendIR('C', checkSumCalc);
}

///////////////////////////////////////////////////////////////////////////////

void StartCountDown()
{
  hostingActive = FALSE;
  static byte CountDownTime = 0x0A;           //  TODO: his needs to be BCD not HEX
  Serial.print(F("\nCountDown : "));
  Serial.print(CountDownTime);
  SendIR('P', 0x00);
  SendIR('D', hostedGameID);
  SendIR('D', CountDownTime);
  SendIR('D', 0x08);                          //  TODO: This is packed byte of valid Team1 Player ID's  - Needs to be real data eventually.
  SendIR('D', 0x08);                          //  TODO: This is packed byte of valid Team1 Player ID's  - Needs to be real data eventually.
  SendIR('D', 0x00);                          //  TODO: This is packed byte of valid Team1 Player ID's  - Needs to be real data eventually.
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


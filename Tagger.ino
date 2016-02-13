//  void TaggerMode()
//  void FireLaser(byte megaTag)
//  void SetShields()
//  void Reload()
//  void DrawTaggerScreen()
//  void DrawTaggerScreenShieldsUp()
//  void DecodeIR()
//  void GameOver()

///////////////////////////////////////////////////////////////////////////////

void TaggerMode()
{ 
  DrawTaggerScreen();
  
  char* Action = GetButtonPress();
  if      (Action == "Tag 1" && shieldsUp == FALSE)   FireLaser(B0000000);
  else if (Action == "Tag 2" && shieldsUp == FALSE)   FireLaser(B0000001);
  else if (Action == "Tag 3" && shieldsUp == FALSE)   FireLaser(B0000010);
  else if (Action == "Tag 4" && shieldsUp == FALSE)   FireLaser(B0000011);
  else if (Action == "Shields")                     SetShields();
  else if (Action == "ReLoad")                      Reload();                    
  else if (Action == "EXIT")                        state = CONFIG;

  if (shieldsUp) UpdateShieldsTimer();
}

void UpdateShieldsTimer()
{
  //Start a timer           // TODO:
  //check if 1 second has expired and then update display and variable
}
///////////////////////////////////////////////////////////////////////////////

void FireLaser(byte tagPower)
{
  if (tagCount == 0) return;
  //tft.fillScreen(YELLOW);

  //Add TeamID, PlayerID and megaTag into the message data

  SendIR('T', tagPower);
  
  tagCount = tagCount - (tagPower+1);
  if (tagCount >= 250 && tagCount <= 255) tagCount = 0;
  lastState = NONE;
  DrawTaggerScreen();
}
///////////////////////////////////////////////////////////////////////////////

void SetShields()
{
//  if (remainingShields) ;    // Need to add some smarts here.  //TODO:
  shieldsUp = !shieldsUp;
  lastState = NONE;
  if (!shieldsUp) DrawTaggerScreen();
  else if (shieldsUp) DrawTaggerScreenShieldsUp();

                                                                  //TODO: Shields countdown timer.
}

///////////////////////////////////////////////////////////////////////////////

void Reload()
{
  // if reLoadAmount is 0 then unlimited reloads, else kep count.
  
  tft.fillScreen(BLACK);
  delay (500);
  tagCount = reloadAmount;
  reloadAmount--;
  EEPROM.write(eeRELOAD_AMOUNT, reloadAmount);
  lastState = NONE;
  DrawTaggerScreen();
}

///////////////////////////////////////////////////////////////////////////////

void DrawTaggerScreen()
{
  if (lastState != state)
  {
    if (deBug) Serial.println(F("DrawTaggerScreen"));
    DrawScreen(TAGGER, "TAG MODE", GREEN, BLUE, 3);
    //lastState = state;
    DrawButton(  5,  40,  100, 55, WHITE,  "Tag 1",   2, BLACK);
    DrawButton(  5, 100,  100, 55, WHITE,  "Tag 2",   2, BLACK);
    DrawButton(  5, 160,  100, 55, WHITE,  "Tag 3",   2, BLACK);
    DrawButton(  5, 220,  100, 55, WHITE,  "Tag 4",   2, BLACK);
    DrawButton(135,  40,  100, 55, BLACK,  "ReLoad",  2, WHITE);
    DrawButton(135, 220,  100, 55, RED,    "Shields", 2, GREEN);
    DrawButton( 70, 290,  100, 30, YELLOW, "EXIT",    2, BLACK);
    if (deBug) PrintButtonArray();

    DrawTextLabel( 165,   98, GREEN,  String(tagCount),     3, BLACK, 2);
    DrawTextLabel( 160,  145, YELLOW, String(playerHealth), 4, BLACK, 2);
    DrawTextLabel( 165,  195, GREEN,  String(shieldsTimer), 3, RED,   2);
  }
}

///////////////////////////////////////////////////////////////////////////////

void DrawTaggerScreenShieldsUp()
{
  if (lastState != state)
  {
    if (deBug) Serial.println(F("DrawTaggerScreen-ShieldsUp"));
    DrawScreen(TAGGER, "TAG MODE", GREEN, BLUE, 3);
    lastState = state;
    DrawButton(  5,  40,  100, 55, GREEN,  "Tag 1",   2, BLACK);
    DrawButton(  5, 100,  100, 55, GREEN,  "Tag 2",   2, BLACK);
    DrawButton(  5, 160,  100, 55, GREEN,  "Tag 3",   2, BLACK);
    DrawButton(  5, 220,  100, 55, GREEN,  "Tag 4",   2, BLACK);
    DrawButton(135,  40,  100, 55, GREEN,  "ReLoad",  2, WHITE);
    DrawButton(135, 220,  100, 55, RED,    "Shields", 2, GREEN);
    if (deBug) PrintButtonArray();

    DrawTextLabel( 165,  195, GREEN, String(shieldsTimer), 3, RED, 2);
  }
}


///////////////////////////////////////////////////////////////////////////////

void DecodeIR()
{
  if (deBug) Serial.println(F("DecodeIR()"));
  static byte badMessageCount = 0;  

  byte shooterTeamID;
  byte shooterPlayerID;
  byte shooterShotPower;
 
  //  Check Type, if Tag, What Team/Player/Mega.
  //  If same team then check FF flag before actioning.
  //  Adjust health and then update score table (numberOfHitsByPlayerXX++ x Mega)
  
  if (receivedIRmessage.type == 'T')
  {
    Serial.print("\nTag");
    // Find TeamID of shooter
    shooterTeamID = receivedIRmessage.dataPacket & B01100000;             // TeamID = 1 thru 3  (0 = NoTeams)
    shooterTeamID = shooterTeamID >> 5;
    Serial.print(F("\tTeamID: "));
    Serial.print(shooterTeamID);
    // Find PlayerID
    shooterPlayerID = (receivedIRmessage.dataPacket & B00011100)+1;       // PlayerID = 1 thru 8
    shooterPlayerID = shooterPlayerID >> 2;
    Serial.print(F("\tPlayerID: "));
    Serial.print(shooterPlayerID);
    // Find Mega power
    shooterShotPower = (receivedIRmessage.dataPacket & B00000011)+1;
    Serial.print(F("\tShotStrength: "));
    Serial.println(shooterShotPower);

    //Process the data  // TODO: maybe move this to a separate function !!!
    if (shieldsUp == TRUE)                        { ClearIRarray(); return; }    // TODO: signal a shot was blocked.
    if (teamID == shooterTeamID && teamID != 0)   { ClearIRarray(); return; }
    
    playerHealth = playerHealth - shooterShotPower;
    if (playerHealth <= 255 && playerHealth >= 250) playerHealth = 0;
    DrawTextLabel( 160,  145, YELLOW, String(playerHealth), 4, BLACK, 2);
    //Check if they are dead !
    if (playerHealth == 0)
      {
        state = GAME_OVER;
      }
  }
    
  else if (receivedIRmessage.type == 'B')
  {
    //Serial.print("Bcn-");
  }
    
  else
  {
    badMessageCount++;                // TODO: Is this logical here, or am I off with the fairies.
    //Serial.print("Error-");
  }    
  ClearIRarray();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver()
{
  tft.fillScreen(BLACK);
  DrawTextLabel ( 0, 100, BLACK, "Game", 6, RED, 0);
  DrawTextLabel ( 0, 180, BLACK, "Over", 6, RED, 0);
}


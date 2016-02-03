//  void TaggerMode()
//  void FireLaser(byte megaTag)
//  void SetShields()
//  void Reload()
//  void DrawTaggerScreen()
//  void DrawTaggerScreenShieldsUp()
//  void DecodeIR()

///////////////////////////////////////////////////////////////////////////////

void TaggerMode()
{ 
  DrawTaggerScreen();
  
  if (receivedIRmessage.type != '_') DecodeIR();
  
  char* Action = GetButtonPress();
  if      (Action == "Tag 1" && shieldsUp == FALSE)   FireLaser(B0000000);
  else if (Action == "Tag 2" && shieldsUp == FALSE)   FireLaser(B0000001);
  else if (Action == "Tag 3" && shieldsUp == FALSE)   FireLaser(B0000010);
  else if (Action == "Tag 4" && shieldsUp == FALSE)   FireLaser(B0000011);
  else if (Action == "Shields")                     SetShields();
  else if (Action == "ReLoad")                      Reload();                    
  else if (Action == "EXIT")                        state = PINPAD;

  if (shieldsUp) UpdateShieldsTimer();
}

void UpdateShieldsTimer()
{
  //Start a timer           // TODO:
  //check if 1 second has expired and then update display and variable
}
///////////////////////////////////////////////////////////////////////////////

void FireLaser(byte megaTag)
{
  if (shotCount == 0) return;
  long unsigned int shotTime = micros();
  //tft.fillScreen(YELLOW);
  Serial.println("\nShot");
  SendIR('T', megaTag);
  shotCount = shotCount - (megaTag+1);
  if (shotCount >= 250 && shotCount <= 255) shotCount = 0;
  lastState = NONE;
  Serial.print((micros()-shotTime)/1000); Serial.println("mS.");
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
  shotCount = reloadAmount;
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

    DrawTextLabel( 165,   98, 0,      String(shotCount),    3, BLACK, 2);
    DrawTextLabel( 160,  145, YELLOW, String(playerHealth), 4, BLACK, 2);
    DrawTextLabel( 165,  195, 0,      String(shieldsTimer), 3, RED,   2);
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
    //DrawButton(135,  40,  100, 55, BLACK,  "ReLoad",  2, WHITE);
    DrawButton(135, 220,  100, 55, RED,    "Shields", 2, GREEN);
    DrawButton( 70, 290,  100, 30, YELLOW, "EXIT",    2, BLACK);
    if (deBug) PrintButtonArray();

    //DrawTextLabel( 150,  110, 0, String(shotCount),    3, BLACK, 3);
    DrawTextLabel( 150,  170, 0, String(shieldsTimer), 3, BLACK, 3);
  }
}


///////////////////////////////////////////////////////////////////////////////

void DecodeIR()
{
  
  static byte badMessageCount = 0;  

  byte shooterTeamID;
  byte shooterPlayerID;
  byte shooterShotPower;
 
  //  Check Type, if Tag, What Team/Player/Mega.
  //  If same team then check FF flag before actioning.
  //  Adjust health and then update score table (numberOfHitsByPlayerXX++ x Mega)
  
  if (receivedIRmessage.type == 'T')
  {
    Serial.print("\nT");
    // Find TeamID of shooter
    shooterTeamID = receivedIRmessage.dataPacket & B01100000;
    shooterTeamID = shooterTeamID << 5;
    Serial.print(F("\tTeamID: "));
    Serial.print(shooterTeamID);
    // Find PlayerID
    shooterPlayerID = receivedIRmessage.dataPacket & B00011100;
    shooterPlayerID = shooterPlayerID << 3;
    Serial.print(F("\tPlayerID: "));
    Serial.print(shooterPlayerID);
    // Find Mega power
    shooterShotPower = (receivedIRmessage.dataPacket & B00000011)+1;
    Serial.print(F("\tShotStrength: "));
    Serial.println(shooterShotPower);

    //Process the data  // TODO: maybe move this to a separate function !!!
    if (shieldsUp == TRUE) // TODO: signal a shot was blocked.
    if (teamID = shooterTeamID) return;
    playerHealth = playerHealth - shooterShotPower;
    if (playerHealth <= 255 && playerHealth >= 250) playerHealth = 0;
    DrawTextLabel( 160,  145, YELLOW, String(playerHealth), 4, BLACK, 2);
    //Check if they are dead !
    if (playerHealth == 0)
      {
        state = GAME_OVER;
        //return;
      }
        
  }
    
  else if (receivedIRmessage.type == 'B')
  {
    Serial.print("b");
  }
    
  else
  {
    badMessageCount++;                // TODO: Is this logical here, or am I off with the fairies.
    Serial.print("-BAD-");
    //Serial.print(F("\n--- bad Tag message - # "));   Serial.print(badMessageCount); Serial.println(F(" ---"));
    //Serial.println();
  }    
  ClearIRarray();
}



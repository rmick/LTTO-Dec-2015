#include <Arduino.h>

//  void TaggerMode()
//  void FireLaser(byte megaTag)
//  void SetShields()
//  void Reload()
//  void DrawTaggerScreen()
//  void DrawTaggerScreenShieldsUp()
//  void DecodeTagIR()
//  void GameOver()





/*
byte tagPower = 1;

///////////////////////////////////////////////////////////////////////////////

void TaggerMode()
{ 
  DrawTaggerScreen();
  
  char const* Action = GetButtonPress();
  if      (Action == "Team")      { teamID++;   if (teamID == 4)    teamID = 0;   DrawTextLabel(50, 90,  GREEN, String(teamID),   2, BLACK, 1); }
  else if (Action == "Player")    { playerID++; if (playerID == 9)  playerID = 1; DrawTextLabel(50, 175, GREEN, String(playerID), 2, BLACK, 1); }
  else if (Action == "TagPower")  { tagPower++; if (tagPower == 5)  tagPower = 1; DrawTextLabel(50, 260, GREEN, String(tagPower), 2, BLACK, 1); }
  else if (Action == "Fire")      FireLaser();
  else if (Action == "Shields")   SetShields();
  else if (Action == "ReLoad")    Reload();
  else if (Action == "Scores")    state = SCORES;                    
  else if (Action == "EXIT")      state = CONFIG1;

  if (shieldsUp) UpdateShieldsTimer();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UpdateShieldsTimer()
{
  static uint16_t shieldsTimer = micros();
  
  //Start a timer           // TODO:
  //check if 1 second has expired and then update display and variable
}
///////////////////////////////////////////////////////////////////////////////

void FireLaser()
{
  unsigned int fireMessage;
  
  if (tagCount == 0) return;          //TODO: Make a click noise
  if (tagCount < tagPower) return;    //TODO: Make a failed noise
  tft.fillScreen(YELLOW);

  ltto.sendTag(teamID, playerID, tagPower);
  
//  //Assemble the fireMessage
//  fireMessage = teamID;
//  fireMessage = fireMessage << 3;
//  fireMessage = fireMessage + (playerID-1);
//  fireMessage = fireMessage << 2;
//  fireMessage = fireMessage + (tagPower-1);
//
//  ltto.sendIR('T', fireMessage);
  
  tagCount = tagCount - (tagPower);
  if (tagCount >= 250 && tagCount <= 255) tagCount = 0;
  lastState = NONE;
  DrawTaggerScreen();
}
///////////////////////////////////////////////////////////////////////////////
uint16_t shieldsTimeMicros;

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
  static uint8_t remainingReloads = maxReloads;
  // if maxReloads is 0 then unlimited reloads, else keep count.
  
  //if (remainingReloads >0)
  {
    tft.fillScreen(BLACK);
    delay (500);
    tagCount = reloadAmount;
    remainingReloads--;
    lastState = NONE;
    DrawTaggerScreen();
  }
}

///////////////////////////////////////////////////////////////////////////////

void DrawTaggerScreen()
{
  if (lastState != state)
  {
    #ifdef DEBUG
      Serial.println(F("\nDrawTaggerScreen"));
    #endif
    DrawScreen(TAGGER, "TAG MODE", GREEN, BLUE, 3);
    lastState = state;
    DrawButton(  5,  30,  100, 55, WHITE,  "Team",     2, BLACK);
    DrawButton(  5, 115,  100, 55, WHITE,  "Player",   2, BLACK);
    DrawButton(  5, 200,  100, 55, WHITE,  "TagPower", 2, BLACK);
    DrawButton(135,  30,  100, 55, BLACK,  "ReLoad",   2, WHITE);
    DrawButton(135, 125,  100, 55, YELLOW, "Fire",     2, YELLOW);
    DrawButton(135, 200,  100, 55, RED,    "Shields",  2, GREEN);
    DrawButton(  5, 290,   50, 30, YELLOW, "Scores",   1, BLACK);
    DrawButton( 70, 290,  100, 30, YELLOW, "EXIT",     2, BLACK);

    DrawTextLabel(  50, 90,  GREEN,  String(teamID),       2, BLACK, 1);
    DrawTextLabel(  50, 175, GREEN,  String(playerID),     2, BLACK, 1);
    DrawTextLabel(  50, 260, GREEN,  String(tagPower),     2, BLACK, 1);
    DrawTextLabel( 165,  88, GREEN,  String(tagCount),     3, BLACK, 2);
    DrawTextLabel( 150, 130, YELLOW, "Health",             2, BLACK, 0);
    DrawTextLabel( 170, 155, YELLOW, String(playerHealth), 3, BLACK, 2);
    DrawTextLabel( 165, 260, GREEN,  String(shieldsTimer), 3, RED,   2);
  }
}
///////////////////////////////////////////////////////////////////////////////

void DrawTaggerScreenShieldsUp()
{
  if (lastState != state)
  {
    #ifdef DEBUG
      Serial.println(F("DrawTaggerScreen-ShieldsUp"));
    #endif
    DrawScreen(TAGGER, "TAG MODE", GREEN, BLUE, 3);
    lastState = state;
    DrawButton(135, 200,  100, 55, RED,    "Shields",  2, GREEN);
    DrawTextLabel( 165, 260, GREEN,  String(shieldsTimer), 3, RED,   2);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void ProcessRxTag(byte taggedTeamID, byte taggedPlayerID, byte taggedMegaPower)
{
    if (teamID == taggedTeamID && friendlyFire == false && teamID != 0)
    {
      decodedIRmessage.newMessage = false;
      Serial.print(F("\nFriendlyFire"));
      return;
    }

    if (shieldsUp == true)
    {
      decodedIRmessage.newMessage = false;
      tft.fillScreen(WHITE);
      lastState = NONE;
      DrawTaggerScreenShieldsUp();
      Serial.print(F("\nShields Blocked A Shot"));
      return;
      }

    //Take the hit
    //tft.fillScreen(RED);
    //lastState = NONE;
    //DrawTaggerScreen();
    playerHealth = playerHealth - taggedMegaPower;
    if (playerHealth <= 255 && playerHealth >= 250) playerHealth = 0;
    if (state == TAGGER) DrawTextLabel( 170, 155, YELLOW, String(playerHealth), 3, BLACK, 2);

    //Update the scoreGrid
    byte currentHitIndex;
    currentHitIndex = taggedPlayerID;
    if (teamID != 0)  currentHitIndex = currentHitIndex + (teamID * 8);
    scoreGrid[currentHitIndex] = scoreGrid[currentHitIndex] + taggedMegaPower;
    
    //Check if they are dead !
    if (playerHealth == 0)
      {
        state = GAME_OVER;
      }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayScores()
{
  static bool scoresActive;
  if (!scoresActive)
  {
    tft.fillScreen(BLACK);
    DrawButton( 70, 290,  100, 30, YELLOW, "EXIT", 2, BLACK);
    scoresActive = true;
    tft.setTextColor(YELLOW);
    tft.setTextSize(1);
    for (uint8_t teamPlayerCount = 1; teamPlayerCount <25; teamPlayerCount++)
    {
      tft.setCursor(60, (teamPlayerCount*11)+5);
      byte displayPlayerID = teamPlayerCount%8; if (displayPlayerID == 0) displayPlayerID = 8;
      tft.print("Team "); tft.print(teamPlayerCount/8); tft.print(", Player "); tft.print(displayPlayerID); tft.print(": ");
      tft.println(scoreGrid[teamPlayerCount]);

      //Serial.print(F("Team ")); Serial.print(teamPlayerCount/8); Serial.print(F(", Player ")); Serial.print(displayPlayerID); Serial.print((F": "));
      //Serial.println(scoreGrid[teamPlayerCount]);
    }
  }

  char const* Action = GetButtonPress();
  if (Action == "EXIT")  { scoresActive = false; state = TAGGER; lastState = NONE; DrawTaggerScreen(); Serial.print(F("ExitScores")); }

  
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GameOver()
{
  tft.fillScreen(BLACK);
  DrawTextLabel ( 0, 100, BLACK, "Game", 6, RED, 0);
  DrawTextLabel ( 0, 180, BLACK, "Over", 6, RED, 0);
}


*/




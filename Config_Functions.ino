#include <Arduino.h>

//  void SetTeam()
//  void DrawSetTeam()
//  void SetMedicDelay()
//  void DrawMedicDelay()
//  void SetHostile()
//  void ClearScore()
//  void DrawClearScore()

////////////////////////////////////////////////////////////////////

void SetTeam()
{
  DrawSetTeam();
 
  char const* Action = GetButtonPress();
  if      (Action == "No Team")  { teamID = 0; lastState = NONE; }
  else if (Action == "Team 1")   { teamID = 1; lastState = NONE; }
  else if (Action == "Team 2")   { teamID = 2; lastState = NONE; }
  else if (Action == "Team 3")   { teamID = 3; lastState = NONE; }
  else if (Action == "EXIT")     { EEPROM.write(eeTEAM_ID, teamID); state = CONFIG1; }
}

////////////////////////////////////////////////////////////////////

void DrawSetTeam()
{
  if (lastState != state)
  {
    #ifdef DEBUG
      Serial.println(F("SetTeamIDscreen"));
    #endif
    DrawScreen(SET_TEAM, "CHOOSE TEAM", MAGENTA, WHITE, 3);
    DrawButton( 20,  40, 200, 55, BLACK,  "No Team", 2, WHITE);
    DrawButton( 20, 100, 200, 55, BLACK,  "Team 1",  2, WHITE);
    DrawButton( 20, 160, 200, 55, BLACK,  "Team 2",  2, WHITE);
    DrawButton( 20, 220, 200, 55, BLACK,  "Team 3",  2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT",    2, BLACK);

    uint16_t tickPosition = (teamID*60)+55;                       // Place a Yellow 'tick' in the active button
    tft.fillRoundRect(180, tickPosition, 25, 25, 5, YELLOW);
  }
}

////////////////////////////////////////////////////////////////////

void SetMedicDelay()
{
  DrawMedicDelay();
  
  char const* Action = GetButtonPress();
  if      (Action == "Up")      { medicDelay++; DrawTextLabel( 0,  140, MAGENTA, String(medicDelay), 4, BLACK, 3); }
  else if (Action == "Down")    { medicDelay--; DrawTextLabel( 0,  140, MAGENTA, String(medicDelay), 4, BLACK, 3); }
  else if (Action == "EXIT")    { EEPROM.write(eeMEDIC_DELAY, medicDelay); state = CONFIG1; }
}

////////////////////////////////////////////////////////////////////

void DrawMedicDelay()
{
  if (lastState != state)
  {
    #ifdef DEBUG Serial.println(F("SetMedicDelayScreen"));
    #endif
    DrawScreen(SET_MEDIC_DELAY, "MEDIC DELAY", MAGENTA, WHITE, 3);
    DrawButton( 20,  50, 200, 55, BLACK,  "Up",   2, WHITE);
    DrawButton( 20, 210, 200, 55, BLACK,  "Down", 2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT", 2, BLACK);
    
    DrawTextLabel( 0,  140, MAGENTA, String(medicDelay), 4, BLACK, 3);
  }
}

////////////////////////////////////////////////////////////////////////////////

void SetHostile()
{
  DrawSetHostile();
  char const* Action = GetButtonPress();
  if      (Action == "Yes")  { hostile =  true; lastState = NONE; }
  else if (Action ==  "No")  { hostile = false; lastState = NONE; } 
  else if (Action == "EXIT") { EEPROM.write(eeHOSTILE, hostile); state = CONFIG1; }
}

////////////////////////////////////////////////////////////////////

void DrawSetHostile()
{
  if (teamID == 0)
  {
    if (lastState != state)
    {
      #ifdef DEBUG Serial.println(F("DrawSetHostileScreen"));
      #endif
      DrawScreen(SET_HOSTILE, "NOT VALID", MAGENTA, WHITE, 3);
      DrawButton( 70, 290, 100, 30, YELLOW, "EXIT", 2, BLACK);
      tft.setTextColor(BLACK);
      tft.setTextSize(3);
      tft.setCursor(5, 140);
      tft.println("Please select");
      tft.setCursor(5, 180);
      tft.println("a team first.");
    }
  }
  else
  if (lastState != state)
  {
    #ifdef DEBUG Serial.println(F("SetHostileScreen"));
    #endif
    DrawScreen(SET_HOSTILE, "HOSTILE ?", MAGENTA, WHITE, 3);
    DrawButton( 20,  80, 200, 55, BLACK,  "Yes", 2, WHITE);
    DrawButton( 20, 180, 200, 55, BLACK,  "No",  2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT", 2, BLACK);

    uint16_t tickPosition = 195-(100*hostile);                       // Place a Yellow 'tick' in the active button
    tft.fillRoundRect(180, tickPosition, 25, 25, 5, YELLOW);
  }
}

////////////////////////////////////////////////////////////////////////////////

void ClearScore()
{
    DrawClearScore();
 
  char const* Action = GetButtonPress();
  if      (Action == "Yes")  { EEPROM.write(eeMEDIC_COUNT, 0); medicCount = 0; tft.fillScreen(BLACK); state = CONFIG1; }
  else if (Action == "No")   state = CONFIG1;
}

////////////////////////////////////////////////////////////////////

void DrawClearScore()
{
  if (lastState != state)
  {
    #ifdef DEBUG Serial.println(F("ClearScoreScreen"));
    #endif
    DrawScreen(SET_TEAM, "ARE YOU SURE", MAGENTA, WHITE, 3);
    DrawButton( 20,  80, 200, 55, BLACK,  "No", 2, WHITE);
    DrawButton( 20, 180, 200, 55, BLACK,  "Yes",   2, WHITE);
  }
}

////////////////////////////////////////////////////////////////////////////////



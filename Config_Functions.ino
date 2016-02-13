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
 
  char* Action = GetButtonPress();
  if      (Action == "No Team")  { teamID = 0; lastState = NONE; }
  else if (Action == "Team 1")   { teamID = 1; lastState = NONE; }
  else if (Action == "Team 2")   { teamID = 2; lastState = NONE; }
  else if (Action == "Team 3")   { teamID = 3; lastState = NONE; }
  else if (Action == "EXIT")     { EEPROM.write(eeTEAM_ID, teamID); state = CONFIG; }
}

////////////////////////////////////////////////////////////////////

void DrawSetTeam()
{
  if (lastState != state)
  {
    if (deBug) Serial.println(F("SetTeamIDscreen"));
    DrawScreen(SET_TEAM, "CHOOSE TEAM", MAGENTA, WHITE, 3);
    DrawButton( 20,  40, 200, 55, BLACK,  "No Team", 2, WHITE);
    DrawButton( 20, 100, 200, 55, BLACK,  "Team 1",  2, WHITE);
    DrawButton( 20, 160, 200, 55, BLACK,  "Team 2",  2, WHITE);
    DrawButton( 20, 220, 200, 55, BLACK,  "Team 3",  2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT",    2, BLACK);
    if (deBug) PrintButtonArray();

    uint16_t tickPosition = (teamID*60)+55;                       // Place a Yellow 'tick' in the active button
    tft.fillRoundRect(180, tickPosition, 25, 25, 5, YELLOW);
  }
}

////////////////////////////////////////////////////////////////////

void SetMedicDelay()
{
  DrawMedicDelay();
  
  char* Action = GetButtonPress();
  if      (Action == "Up")      { medicDelay++; DrawTextLabel( 0,  140, MAGENTA, String(medicDelay), 4, BLACK, 3); }
  else if (Action == "Down")    { medicDelay--; DrawTextLabel( 0,  140, MAGENTA, String(medicDelay), 4, BLACK, 3); }
  else if (Action == "EXIT")    { EEPROM.write(eeMEDIC_DELAY, medicDelay); state = CONFIG; }
}

////////////////////////////////////////////////////////////////////

void DrawMedicDelay()
{
  if (lastState != state)
  {
    if (deBug) Serial.println(F("SetMedicDelayScreen"));
    DrawScreen(SET_MEDIC_DELAY, "MEDIC DELAY", MAGENTA, WHITE, 3);
    DrawButton( 20,  50, 200, 55, BLACK,  "Up",   2, WHITE);
    DrawButton( 20, 210, 200, 55, BLACK,  "Down", 2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT", 2, BLACK);
    if (deBug) PrintButtonArray();
    
    DrawTextLabel( 0,  140, MAGENTA, String(medicDelay), 4, BLACK, 3);
  }
}

////////////////////////////////////////////////////////////////////////////////

void SetHostile()
{
  DrawSetHostile();
  char* Action = GetButtonPress();
  if      (Action == "Yes")  { hostile =  TRUE; lastState = NONE; }
  else if (Action ==  "No")  { hostile = FALSE; lastState = NONE; } 
  else if (Action == "EXIT") { EEPROM.write(eeHOSTILE, hostile); state = CONFIG; }
}

////////////////////////////////////////////////////////////////////

void DrawSetHostile()
{
  if (teamID == 0)
  {
    if (lastState != state)
    {
       if (deBug) Serial.println(F("DrawSetHostileScreen"));
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
    if (deBug) Serial.println(F("SetHostileScreen"));
    DrawScreen(SET_HOSTILE, "HOSTILE ?", MAGENTA, WHITE, 3);
    DrawButton( 20,  80, 200, 55, BLACK,  "Yes", 2, WHITE);
    DrawButton( 20, 180, 200, 55, BLACK,  "No",  2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT", 2, BLACK);
    if (deBug) PrintButtonArray();

    uint16_t tickPosition = 195-(100*hostile);                       // Place a Yellow 'tick' in the active button
    tft.fillRoundRect(180, tickPosition, 25, 25, 5, YELLOW);
  }
}

////////////////////////////////////////////////////////////////////////////////

void ClearScore()
{
    DrawClearScore();
 
  char* Action = GetButtonPress();
  if      (Action == "Yes")  { EEPROM.write(eeMEDIC_COUNT, 0); medicCount = 0; tft.fillScreen(BLACK); state = CONFIG; }
  else if (Action == "No")   state = CONFIG;
}

////////////////////////////////////////////////////////////////////

void DrawClearScore()
{
  if (lastState != state)
  {
    if (deBug) Serial.println(F("ClearScoreScreen"));
    DrawScreen(SET_TEAM, "ARE YOU SURE", MAGENTA, WHITE, 3);
    DrawButton( 20,  80, 200, 55, BLACK,  "No", 2, WHITE);
    DrawButton( 20, 180, 200, 55, BLACK,  "Yes",   2, WHITE);
    if (deBug) PrintButtonArray();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Setup()
{
  DrawSetup();
  
  char* Action = GetButtonPress();
  if      (Action == "Change PIN")  state = CHANGE_PIN;
  else if (Action == "Tagger")      state = TAGGER;
  else if (Action == "EXIT")        state = CONFIG;
}

////////////////////////////////////////////////////////////////////

void DrawSetup()
{
  if (lastState != state)
  {
    if (deBug) Serial.println(F("Setup Screen"));
    DrawScreen(SETUP, "SET-UP", MAGENTA, WHITE, 3);
    DrawButton( 20,  50, 200, 55, BLACK,  "Change PIN",   2, WHITE);
    DrawButton( 20, 210, 200, 55, BLACK,  "Tagger", 2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT", 2, BLACK);
    if (deBug) PrintButtonArray();
  }
}

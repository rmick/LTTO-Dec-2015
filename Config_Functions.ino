//  void SetTeam()
//  void DrawSetTeam()
//  void SetMedicDelay()
//  void DrawMedicDelay()
//  void UpdateMedicDelay()
//  void SetHostile()
//  
//  void ClearScore()
//  void DrawClearScore()

void SetTeam()
{
  DrawSetTeam();
 
  char* Action = GetButtonPress();                                  //TODO: Why does this routine NOT call Update TeamID ??????
  if      (Action == "No Team")  { teamID = 0; lastState = NONE; }
  else if (Action == "Team 1")   { teamID = 1; lastState = NONE; }
  else if (Action == "Team 2")   { teamID = 2; lastState = NONE; }
  else if (Action == "Team 3")   { teamID = 3; lastState = NONE; }
  else if (Action == "EXIT")     { EEPROM.write(4, teamID); state = CONFIG; }
}

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

////////////////////////////////////////////////////////////////////////////////

void SetMedicDelay()
{
  DrawMedicDelay();
  
  char* Action = GetButtonPress();
  if      (Action == "Up")      { medicDelay++; UpdateMedicDelay(); }   // lastState = NONE; }
  else if (Action == "Down")    { medicDelay--; UpdateMedicDelay(); }   // lastState = NONE; }
  else if (Action == "EXIT")    { EEPROM.write(2, medicDelay); state = CONFIG; }
}

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
    
    //char&showDelay = String(MedicDelay);
    //DrawButton( 80, 130,  80, 55, YELLOW,showDelay, 4, BLACK);    //TODO: why is (char*) printing random characters???
    UpdateMedicDelay();
  }
}

void UpdateMedicDelay()
{
  {
    Serial.print(F("MedicDelay = "));
    Serial.println(medicDelay);
    tft.fillRect(40, 135, 160, 40, MAGENTA);
    int numWidth = CountDigits(medicDelay)*(5*4);
    tft.setCursor((240-numWidth)/2, 140);
    tft.setTextColor(BLACK);
    tft.setTextSize(4);
    tft.print(medicDelay);
  }
}
////////////////////////////////////////////////////////////////////////////////

void SetHostile()               // TODO: Send IR Tags every X seconds against other teams !
{
  DrawSetHostile();
  char* Action = GetButtonPress();
  if      (Action == "Yes")  { hostile =  TRUE; lastState = NONE; }
  else if (Action ==  "No")  { hostile = FALSE; lastState = NONE; } 
  else if (Action == "EXIT") { EEPROM.write(8, hostile); state = CONFIG; }
}

void DrawSetHostile()
{
  if (teamID == 0)
  {
    if (lastState != state)
    {
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
  if      (Action == "Yes")  { EEPROM.write(0, 0); numLives = 0; tft.fillScreen(BLACK); state = CONFIG; }
  else if (Action == "No")   state = CONFIG;
}

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


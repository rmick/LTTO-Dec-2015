//  void SetTeam()
//  void UpdateTeamID()
//  void DrawSetTeamID()

////////////////////////////////////////////////////////////////////////////////

void SetTeam()
{
  DrawSetTeam();
 
  char* Action = GetButtonPress();                                  //TODO: Why does this routine NOT call Update TeamID ??????
  if      (Action == "No Team")  { TeamID = 0; lastState = NONE; }
  else if (Action == "Team 1")   { TeamID = 1; lastState = NONE; }
  else if (Action == "Team 2")   { TeamID = 2; lastState = NONE; }
  else if (Action == "Team 3")   { TeamID = 3; lastState = NONE; }
  else if (Action == "EXIT")     { EEPROM.write(4, TeamID); state = CONFIG; }
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

    uint16_t TickPosition = (TeamID*60)+55;
    tft.fillRoundRect(180, TickPosition, 25, 25, 5, YELLOW);
  }
}

////////////////////////////////////////////////////////////////////////////////

void SetMedicDelay()
{
  DrawMedicDelay();
  
  char* Action = GetButtonPress();
  if      (Action == "Up")      { MedicDelay++;   lastState = NONE; }
  else if (Action == "Down")    { MedicDelay--;   lastState = NONE; }
  else if (Action == "EXIT")    { EEPROM.write(2, MedicDelay); state = CONFIG; }
  Serial.println(MedicDelay);
}

void DrawMedicDelay()
{
  if (lastState != state)
  {
    if (deBug) Serial.println(F("SetMedicDelayScreen"));
    DrawScreen(SET_MEDIC_DELAY, "MEDIC DELAY", MAGENTA, WHITE, 3);
    DrawButton( 20,  40, 200, 55, BLACK,  "Up",             2, WHITE);
    DrawButton( 80, 130,  80, 55, YELLOW,(char*)MedicDelay, 4, BLACK);    //TODO: why is (char*) printing random characters???
    DrawButton( 20, 220, 200, 55, BLACK,  "Down",           2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT",           2, BLACK);
    if (deBug) PrintButtonArray();
  }
}
////////////////////////////////////////////////////////////////////////////////

void SetHostile()
{
  state = CONFIG;
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
    DrawButton( 20,  40, 200, 55, BLACK,  "No", 2, WHITE);
    DrawButton( 20, 100, 200, 55, BLACK,  "Yes",   2, WHITE);
    if (deBug) PrintButtonArray();
  }
}





void SetTeamID()
{ 
  if (deBug) Serial.println(F("TeamID screen"));
  
  ButtonCount = 0;
  DrawButton( 20,  40, 200, 55, BLACK,   "No Team",  2, WHITE);
  DrawButton( 20, 100, 200, 55, BLACK,   "Team 1",   2, WHITE);
  DrawButton( 20, 160, 200, 55, BLACK,   "Team 2",   2, WHITE);
  DrawButton( 20, 220, 200, 55, BLACK,   "Team 3",   2, WHITE);
  DrawButton( 70, 290, 100, 30, YELLOW,  "Save",     2, BLACK);
  if (deBug) PrintButtonArray();
  
  uint16_t TickPosition = (TeamID*60)+55;
  tft.fillRoundRect(180, TickPosition, 25, 25, 5, YELLOW);

  //bool loopy = 0;
  //do
  //{
    char* TeamIDAction = GetButtonPress();
    if      (TeamIDAction == "No Team")  TeamID = 0; 
    else if (TeamIDAction == "Team 1")   TeamID = 1;
    else if (TeamIDAction == "Team 2")   TeamID = 2;
    else if (TeamIDAction == "Team 3")   TeamID = 3;
    else if (TeamIDAction == "Save")     DrawConfigScreen();
    tft.fillRoundRect(180, TickPosition, 25, 25, 5, BLACK);
    TickPosition = (TeamID*60)+55;
    tft.fillRoundRect(180, TickPosition, 25, 25, 5, YELLOW);
  //}
  //while(loopy == 0);
}

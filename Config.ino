//  void ConfigMode()
/// void DrawConfigScreen()
///


void ConfigMode()
{
  DrawConfigScreen();

  char* Action = GetButtonPress();
  if      (Action == "Set Team")              state = SET_TEAM;
  else if (Action == "Set Medic Delay")       state = SET_MEDIC_DELAY;
  else if (Action == "Set Hostile")           state = SET_HOSTILE;
  else if (Action == "Clear Score")           state = CLEAR_SCORE;
  else if (Action == "More...")               state = SETUP;
  else if (Action == "EXIT")                  state = MEDIC;

////////////////  Later on add MODES
////////////////  Simple Host
////////////////  Capture the flag
////////////////  Own the Zone
////////////////  Ammo supply base
////////////////  Use RFID tags to do recharges 

}

////////////////////////////////////////////////////////////////////////////////

void DrawConfigScreen()
{
  if (lastState != state)
  {
    #ifdef DEBUG Serial.println(F("DrawConfigScreen"));
    #endif
    DrawScreen(CONFIG, "CONFIG", MAGENTA, WHITE, 3);
    DrawButton( 20,  35, 200, 45, BLACK,  "Clear Score",      2, WHITE);
    DrawButton( 20,  85, 200, 45, BLACK,  "Set Medic Delay",  2, WHITE);
    DrawButton( 20, 135, 200, 45, BLACK,  "Set Hostile",      2, WHITE);
    DrawButton( 20, 185, 200, 45, BLACK,  "Set Team",         2, WHITE);
    DrawButton( 20, 235, 200, 45, BLACK,  "More...",          2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT",             2, BLACK);
    if (deBug) PrintButtonArray();
  }
}

//  void ConfigMode()
/// void DrawConfigScreen()
///


void ConfigMode()
{
  DrawConfigScreen();

  char* Action = GetButtonPress();
  if      (Action == "Set Team ID")           state = SET_TEAM;         // TODO: Set the Team ID for the Respawn signal (not sure if this is valid or not)
  else if (Action == "Set Medic Delay")       state = SET_MEDIC_DELAY;  // TODO: Set the length of the countdown timer
  else if (Action == "Set Hostile")           state = SET_HOSTILE;      // TODO: enable Hostile mode - sends opposing 2/3 teams MegaTags
  else if (Action == "Clear Score")           state = CLEAR_SCORE;
  else if (Action == "EXIT")                  state = PINPAD;

////////////////  Later on add MODES
////////////////  Tagger Mode
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
    if (deBug) Serial.println(F("DrawConfigScreen"));
    DrawScreen(CONFIG, "CONFIG", MAGENTA, WHITE, 3);
    DrawButton( 20,  40, 200, 55, BLACK,  "Clear Score",    2, WHITE);
    DrawButton( 20, 100, 200, 55, BLACK,  "Set Medic Delay",  2, WHITE);
    DrawButton( 20, 160, 200, 55, BLACK,  "Set Hostile",    2, WHITE);
    DrawButton( 20, 220, 200, 55, BLACK,  "Set Team ID",    2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT",           2, BLACK);
    if (deBug) PrintButtonArray();
  }
}

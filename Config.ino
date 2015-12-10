///
///
///


void ConfigMode()
{

  DrawConfigScreen();
////////////////In medic mode
////////////////Set Team ID
////////////////If Team ID set, enable Hostile mode - sends opposing 2/3 teams MegaTags
////////////////Set CountDown Timer Length
////////////////Reset Counter

  char* Action = GetButtonPress();
  if      (Action == "Team ID")    SetTeamID(); // TODO: Set the Team ID for the Respawn signal (not sure if this is valid or not)
  else if (Action == "Timer+")      ;// TODO: Set the length of the countdown timer
  else if (Action == "Hostile?")    ;// TODO: enable Hostile mode - sends opposing 2/3 teams MegaTags
  else if (Action == "Reset Score") 
  {
    EEPROM.write(0, 0);
    numLives = 0;
  }
  else if (Action == "EXIT") state = PinCode;
  


////////////////Later on add MODES
//////////////// Capture the flag
//////////////// Own the Zone
//////////////// Ammo supply base
//////////////// Use RFID tags to do recharges 



}

////////////////////////////////////////////////////////////////////////////////

void DrawConfigScreen()
{
  if (lastState != state)
    {
      
  DrawScreen(Config, "CONFIG", MAGENTA, WHITE);
  
  DrawButton( 20,  40, 200, 55, BLACK,  "Reset Score", 2, WHITE);
  DrawButton( 20, 100, 200, 55, BLACK,  "TimerValue",  2, WHITE);
  DrawButton( 20, 160, 200, 55, BLACK,  "Hostile?",    2, WHITE);
  DrawButton( 20, 220, 200, 55, BLACK,  "Team ID",     2, WHITE);
  DrawButton( 70, 290, 100, 30, YELLOW, "EXIT",        2, BLACK);
  if (deBug) PrintButtonArray();
  lastState = state;
  }
}

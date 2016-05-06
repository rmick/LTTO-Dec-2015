//  void ConfigScreen1()
//  void DrawConfigScreen1()
//  void ConfigScreen2()
//  void DrawConfig2


void ConfigScreen1()
{
  DrawConfigScreen1();

  char const* Action = GetButtonPress();
  if      (Action == "Set Team")              state = SET_TEAM;
  else if (Action == "Set Medic Delay")       state = SET_MEDIC_DELAY;
  else if (Action == "Set Hostile")           state = SET_HOSTILE;
  else if (Action == "Clear Score")           state = CLEAR_SCORE;
  else if (Action == "More...")               state = CONFIG2;
  else if (Action == "EXIT")                  state = MEDIC;

////////////////  Later on add MODES
////////////////  Simple Host
////////////////  Capture the flag
////////////////  Own the Zone
////////////////  Ammo supply base

}

////////////////////////////////////////////////////////////////////////////////

void DrawConfigScreen1()
{
  if (lastState != state)
  {
    #ifdef DEBUG
      Serial.println(F("DrawConfig-1"));
    #endif
    DrawScreen(CONFIG1, "CONFIG", MAGENTA, WHITE, 3);
    DrawButton( 20,  35, 200, 45, BLACK,  "Clear Score",      2, WHITE);
    DrawButton( 20,  85, 200, 45, BLACK,  "Set Medic Delay",  2, WHITE);
    DrawButton( 20, 135, 200, 45, BLACK,  "Set Hostile",      2, WHITE);
    DrawButton( 20, 185, 200, 45, BLACK,  "Set Team",         2, WHITE);
    DrawButton( 20, 235, 200, 45, BLACK,  "More...",          2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT",             2, BLACK);
  }
}

////////////////////////////////////////////////////////////////////

void ConfigScreen2()
{
  DrawConfigScreen2();
  
  char const* Action = GetButtonPress();
  if      (Action == "Change PIN")  state = CHANGE_PIN;
  else if (Action == "Tagger")      state = TAGGER;
  else if (Action == "Host Game")   state = HOST;
  else if (Action == "EXIT")        state = CONFIG1;
}

////////////////////////////////////////////////////////////////////

void DrawConfigScreen2()
{
  if (lastState != state)
  {
    #ifdef DEBUG
      Serial.println(F("DrawConfig-2"));
    #endif
    DrawScreen(CONFIG2, "CONFIG", MAGENTA, WHITE, 3);
    DrawButton( 20,  50, 200, 55, BLACK,  "Change PIN", 2, WHITE);
    DrawButton( 20, 210, 200, 55, BLACK,  "Tagger",     2, WHITE);
    DrawButton( 20, 120, 200, 55, BLACK,  "Host Game",  2, WHITE);
    DrawButton( 70, 290, 100, 30, YELLOW, "EXIT",       2, BLACK);
  }
}

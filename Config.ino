#include <Arduino.h>

//  void ConfigScreen1()
//  void DrawConfigScreen1()
//  void ConfigScreen2()
//  void DrawConfig2


void ConfigScreen1()
{
	DrawConfigScreen1();

	char const* Action = GetButtonPress();
	if      (strcmp(Action, "Set Team")			== 0)	state = SET_TEAM;
	else if (strcmp(Action, "Set Medic Delay")	== 0)	state = SET_MEDIC_DELAY;
	else if (strcmp(Action, "Set Hostile")		== 0)	state = SET_HOSTILE;
	else if (strcmp(Action, "Clear Score")		== 0)	state = CLEAR_SCORE;
	else if (strcmp(Action, "More...")			== 0)	state = CONFIG2;
	else if (strcmp(Action, "EXIT")				== 0)	state = MEDIC;

////////////////  Later on add MODES
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
  if      (strcmp(Action, "Change PIN")	== 0)	state = CHANGE_PIN;
  else if (strcmp(Action, "Tagger")		== 0)	state = TAGGER;
  else if (strcmp(Action, "Host Game")	== 0)	state = HOST;
  else if (strcmp(Action, "EXIT")		== 0)	state = CONFIG1;
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

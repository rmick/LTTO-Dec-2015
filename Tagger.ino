//  void TaggerMode()
//  void FireLaser(byte megaTag)
//  void SetShields()

//  DrawTaggerScreen

///////////////////////////////////////////////////////////////////////////////

bool Shields = False;

void TaggerMode()
{ 
  DrawTaggerScreen();
  
  char* Action = GetButtonPress();
  if      (Action == "Tag 1" && Shields == False)   FireLaser(B000000);
  else if (Action == "Tag 2" && Shields == False)   FireLaser(B0000001);
  else if (Action == "Tag 3" && Shields == False)   FireLaser(B0000010);
  else if (Action == "Tag 4" && Shields == False)   FireLaser(B0000011);
  else if (Action == "Shields")                     SetShields();
  else if (Action == "ReLoad")                      Reload();                    
  else if (Action == "EXIT")                        state = PINPAD;
}
///////////////////////////////////////////////////////////////////////////////

void FireLaser(byte megaTag)
{
  tft.fillScreen(YELLOW);
    SendIR('T', megaTag);
    lastState = NONE;
    DrawTaggerScreen();
    //TODO: Adjust AmmoCount
}
///////////////////////////////////////////////////////////////////////////////

void SetShields()
{
  Shields = !Shields;
  lastState = NONE;
  if (!Shields) DrawTaggerScreen();
  else if (Shields) DrawTaggerScreenShieldsUp();
                                                                  //TODO: Shields countdown timer.
}

///////////////////////////////////////////////////////////////////////////////

void Reload()
{
  // TODO: Reload ammo
}

///////////////////////////////////////////////////////////////////////////////

void DrawTaggerScreen()
{
  if (lastState != state)
  {
    if (deBug) Serial.println(F("DrawTaggerScreen"));
    DrawScreen(TAGGER, "TAG MODE", GREEN, BLUE, 3);
    //lastState = state;
    DrawButton(  5,  40,  100, 55, WHITE,  "Tag 1",   2, BLACK);
    DrawButton(  5, 100,  100, 55, WHITE,  "Tag 2",   2, BLACK);
    DrawButton(  5, 160,  100, 55, WHITE,  "Tag 3",   2, BLACK);
    DrawButton(  5, 220,  100, 55, WHITE,  "Tag 4",   2, BLACK);
    DrawButton(135,  40,  100, 55, BLACK,  "ReLoad",  2, WHITE);
    DrawButton(135, 220,  100, 55, RED,    "Shields", 2, GREEN);
    DrawButton( 70, 290,  100, 30, YELLOW, "EXIT",    2, BLACK);
    if (deBug) PrintButtonArray();
  }
}

///////////////////////////////////////////////////////////////////////////////

void DrawTaggerScreenShieldsUp()
{
  if (lastState != state)
  {
    if (deBug) Serial.println(F("DrawTaggerScreen-ShieldsUp"));
    DrawScreen(TAGGER, "TAG MODE", GREEN, BLUE, 3);
    lastState = state;
    DrawButton(  5,  40,  100, 55, GREEN,  "Tag 1",   2, BLACK);
    DrawButton(  5, 100,  100, 55, GREEN,  "Tag 2",   2, BLACK);
    DrawButton(  5, 160,  100, 55, GREEN,  "Tag 3",   2, BLACK);
    DrawButton(  5, 220,  100, 55, GREEN,  "Tag 4",   2, BLACK);
    DrawButton(135,  40,  100, 55, BLACK,  "ReLoad",  2, WHITE);
    DrawButton(135, 220,  100, 55, RED,    "Shields", 2, GREEN);
    DrawButton( 70, 290,  100, 30, YELLOW, "EXIT",    2, BLACK);
    if (deBug) PrintButtonArray();
  }
}

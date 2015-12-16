//  void TaggerMode()
//  void FireLaser(byte megaTag)
//  void SetShields()
//  void Reload()
//  void DrawTaggerScreen()
//  void DrawTaggerScreenShieldsUp()

///////////////////////////////////////////////////////////////////////////////

bool Shields = FALSE;

void TaggerMode()
{ 
  DrawTaggerScreen();
  
  char* Action = GetButtonPress();
  if      (Action == "Tag 1" && Shields == FALSE)   FireLaser(B0000000);
  else if (Action == "Tag 2" && Shields == FALSE)   FireLaser(B0000001);
  else if (Action == "Tag 3" && Shields == FALSE)   FireLaser(B0000010);
  else if (Action == "Tag 4" && Shields == FALSE)   FireLaser(B0000011);
  else if (Action == "Shields")                     SetShields();
  else if (Action == "ReLoad")                      Reload();                    
  else if (Action == "EXIT")                        state = PINPAD;

  if (Shields) UpdateShieldsTimer();
}

void UpdateShieldsTimer()
{
  //Start a timer
  //check if 1 second has expired and then update display and variable
}
///////////////////////////////////////////////////////////////////////////////

void FireLaser(byte megaTag)
{
  if (shotCount == 0) return;
  tft.fillScreen(YELLOW);
  SendIR('T', megaTag);
  shotCount = shotCount - (megaTag+1);
  if (shotCount == 255) shotCount = 0;
  lastState = NONE;
  DrawTaggerScreen();
}
///////////////////////////////////////////////////////////////////////////////

void SetShields()
{
//  if (remainingShields) ;    // Need to add some smarts here.
  Shields = !Shields;
  lastState = NONE;
  if (!Shields) DrawTaggerScreen();
  else if (Shields) DrawTaggerScreenShieldsUp();

                                                                  //TODO: Shields countdown timer.
}

///////////////////////////////////////////////////////////////////////////////

void Reload()
{
  // if reLoadAmount is 0 then unlimited reloads, else kep count.
  
  tft.fillScreen(BLACK);
  delay (500);
  shotCount = reloadAmount;
  reloadAmount--;
  EEPROM.write(eeRELOAD_AMOUNT, reloadAmount);
  lastState = NONE;
  DrawTaggerScreen();
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

    DrawTextLabel( 120,  110, CYAN, String(shotCount),    3, BLACK, 3);
    DrawTextLabel( 0,  170, CYAN, String(shieldsTimer), 3, BLACK, 3);
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
    //DrawButton(135,  40,  100, 55, BLACK,  "ReLoad",  2, WHITE);
    DrawButton(135, 220,  100, 55, RED,    "Shields", 2, GREEN);
    DrawButton( 70, 290,  100, 30, YELLOW, "EXIT",    2, BLACK);
    if (deBug) PrintButtonArray();

    //DrawTextLabel( 150,  110, 0, String(shotCount),    3, BLACK, 3);
    DrawTextLabel( 150,  170, 0, String(shieldsTimer), 3, BLACK, 3);
  }
}

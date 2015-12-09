//  TaggerMode
//  DrawTaggerScreen

///////////////////////////////////////////////////////////////////////////////

bool Shields;

void TaggerMode()
{ 
  char* Action = GetButtonPress();
  if      (Action == "Tag 1" && Shields == False)   SendIR('T', B0000000);
  else if (Action == "Tag 2" && Shields == False)   SendIR('T', B0000001);
  else if (Action == "Tag 3" && Shields == False)   SendIR('T', B0000010);
  else if (Action == "Tag 4" && Shields == False)   SendIR('T', B0000011);
  // TODO: Adjust ammo : tft.fillScreen(YELLOW);
  else if (Action == "Shields") Shields = !Shields;
  else if (Action == "ReLoad") ;                                 // TODO: Reload ammo
  else if (Action == "EXIT") state = PinCode;
}

///////////////////////////////////////////////////////////////////////////////

void DrawTaggerScreen()
{
  Serial.println(F("DrawTaggerScreen"));
  tft.fillScreen(GREEN);
  
  tft.setCursor(50, 10);
  tft.setTextColor(BLUE);
  tft.setTextSize(3);
  tft.println(F("TAG MODE"));

  ButtonCount= 0;
  DrawButton(  5,  40,  100, 55, WHITE,  "Tag 1",   2, BLACK);
  DrawButton(  5, 100,  100, 55, WHITE,  "Tag 2",   2, BLACK);
  DrawButton(  5, 160,  100, 55, WHITE,  "Tag 3",   2, BLACK);
  DrawButton(  5, 220,  100, 55, WHITE,  "Tag 4",   2, BLACK);
  DrawButton(135,  40,  100, 55, BLACK,  "ReLoad",  2, WHITE);
  DrawButton(135, 220,  100, 55, RED,    "Shields", 2, GREEN);
  DrawButton( 70, 290,  100, 30, YELLOW, "EXIT",    2, BLACK);
  if (deBug) PrintButtonArray();
}



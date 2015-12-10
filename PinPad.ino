// PinPadScreen
// DrawPinPadScreen

////////////////////////////////////////////////////////////////////////////////

 void PinPadScreen()
{   
   char* Action = GetButtonPress();
  if      (Action == "1")   Serial.println("!");
  else if (Action == "0")   state = Tagger;
  else if (Action == "Clr") state = Medic;
  else if (Action == "OK")  state = Config;

  //delay(200);

  //TODO: Action the actual 4digit PIN code
  //TODO: Make a change pincode screen and method
}

////////////////////////////////////////////////////////////////////////////////

void DrawPinPadScreen()
{ 
  tft.fillScreen(BLACK);
  tft.setCursor(65, 0);
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println(F("Pin Code?"));
  if (deBug) Serial.println(F("DrawPinPad"));

  ButtonCount = 0;
  DrawButton( 10,  20,  65,  65, BLUE,  "1",   4, BLACK);
  DrawButton( 85,  20,  65,  65, BLUE,  "2",   4, BLACK);
  DrawButton(160,  20,  65,  65, BLUE,  "3",   4, BLACK);
  DrawButton( 10,  95,  65,  65, BLUE,  "4",   4, BLACK);
  DrawButton( 85,  95,  65,  65, BLUE,  "5",   4, BLACK);
  DrawButton(160,  95,  65,  65, BLUE,  "6",   4, BLACK);
  DrawButton( 10, 170,  65,  65, BLUE,  "7",   4, BLACK);
  DrawButton( 85, 170,  65,  65, BLUE,  "8",   4, BLACK);
  DrawButton(160, 170,  65,  65, BLUE,  "9",   4, BLACK);
  DrawButton( 10, 245,  65,  65, RED,  "Clr",  3, WHITE);
  DrawButton( 85, 245,  65,  65, BLUE,  "0",   4, BLACK);
  DrawButton(160, 245,  65,  65, GREEN, "OK",  3, WHITE);
  if (deBug) PrintButtonArray();
}


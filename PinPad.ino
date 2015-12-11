// void PinPadMode()
// void DrawPinPadScreen()

////////////////////////////////////////////////////////////////////////////////

void PinPadMode()
{
  DrawPinPadScreen();
  
  char* Action = GetButtonPress();
  if      (Action == "1")   Serial.println("!");
  else if (Action == "0")   state = TAGGER;
  else if (Action == "Clr") state = MEDIC;
  else if (Action == "OK")  state = CONFIG;

  //delay(200);

  //TODO: Action the actual 4digit PIN code
  //TODO: Make a change pincode screen and method
}

////////////////////////////////////////////////////////////////////////////////

void DrawPinPadScreen()
{ 
  if (lastState != state)
  {
    if (deBug) Serial.println(F("DrawPinPadScreen"));
    DrawScreen(PINPAD, "Pin Code?", BLACK, YELLOW, 2);
    DrawButton( 10,  25,  65,  65, BLUE,  "1",   4, BLACK);
    DrawButton( 85,  25,  65,  65, BLUE,  "2",   4, BLACK);
    DrawButton(160,  25,  65,  65, BLUE,  "3",   4, BLACK);
    DrawButton( 10, 100,  65,  65, BLUE,  "4",   4, BLACK);
    DrawButton( 85, 100,  65,  65, BLUE,  "5",   4, BLACK);
    DrawButton(160, 100,  65,  65, BLUE,  "6",   4, BLACK);
    DrawButton( 10, 175,  65,  65, BLUE,  "7",   4, BLACK);
    DrawButton( 85, 175,  65,  65, BLUE,  "8",   4, BLACK);
    DrawButton(160, 175,  65,  65, BLUE,  "9",   4, BLACK);
    DrawButton( 10, 250,  65,  65, RED,  "Clr",  3, WHITE);
    DrawButton( 85, 250,  65,  65, BLUE,  "0",   4, BLACK);
    DrawButton(160, 250,  65,  65, GREEN, "OK",  3, WHITE);
    if (deBug) PrintButtonArray();
  }
}


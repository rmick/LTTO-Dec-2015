// void PinPadMode()
// void DrawPinPadScreen()

////////////////////////////////////////////////////////////////////////////////

void DrawAsterisk(uint8_t offset, uint8_t number)
{
  if (offset > 3) return;
  //DrawTextLabel( 150+(offset*15), 5, 0, String(number), 2, RED, 1);
  DrawTextLabel( 150+(offset*15), 5, 0, "*", 2, RED, 1);
}

////////////////////////////////////////////////////////////////////////////////

byte pinCodeEntered[4];

void PinPadMode()
{
  DrawPinPadScreen();
  
  char* numberPressed = GetButtonPress();
  static unsigned int digitCount = 0;
     
  if      (numberPressed == "1")  { pinCodeEntered[digitCount] = 1; DrawAsterisk(digitCount, 1); digitCount++; }
  else if (numberPressed == "2")  { pinCodeEntered[digitCount] = 2; DrawAsterisk(digitCount, 2); digitCount++; }
  else if (numberPressed == "3")  { pinCodeEntered[digitCount] = 3; DrawAsterisk(digitCount, 3); digitCount++; }
  else if (numberPressed == "4")  { pinCodeEntered[digitCount] = 4; DrawAsterisk(digitCount, 4); digitCount++; }
  else if (numberPressed == "5")  { pinCodeEntered[digitCount] = 5; DrawAsterisk(digitCount, 5); digitCount++; }
  else if (numberPressed == "6")  { pinCodeEntered[digitCount] = 6; DrawAsterisk(digitCount, 6); digitCount++; }
  else if (numberPressed == "7")  { pinCodeEntered[digitCount] = 7; DrawAsterisk(digitCount, 7); digitCount++; }
  else if (numberPressed == "8")  { pinCodeEntered[digitCount] = 8; DrawAsterisk(digitCount, 8); digitCount++; }
  else if (numberPressed == "9")  { pinCodeEntered[digitCount] = 9; DrawAsterisk(digitCount, 9); digitCount++; }
  else if (numberPressed == "0")  { pinCodeEntered[digitCount] = 0; DrawAsterisk(digitCount, 0); digitCount++; }
  else if (numberPressed =="Clr") { // Clear the display and reset counter
                                    for (uint8_t x = 0; x<4; x++)
                                    {
                                      DrawTextLabel( 150, 5, 0, "       ", 2, BLACK, 8);
                                      digitCount = 0;
                                    }
                                  }
  else if (numberPressed == "OK") { //Check if we have a match
                                    if (pinCodeEntered[0] == pinCode[0] && 
                                        pinCodeEntered[1] == pinCode[1] &&
                                        pinCodeEntered[2] == pinCode[2] &&
                                        pinCodeEntered[3] == pinCode[3] )   { digitCount = 0; state = CONFIG; }
                                    else 
                                    {
                                      DrawTextLabel( 150, 5, 0, "WRONG", 2, RED, 5);
                                      delay (500);
                                      DrawTextLabel( 150, 5, 0, "       ", 2, BLACK, 8);
                                      digitCount = 0;
                                    }
                                  }
  
  
  //else if (Action == "Clr") state = MEDIC;
  //else if (Action == "OK")  state = CONFIG;
  
  //TODO: Action the actual 4digit PIN code
  //TODO: Make a change pincode screen and method

}

////////////////////////////////////////////////////////////////////////////////

void DrawPinPadScreen()
{ 
  if (lastState != state)
  {
    if (deBug) Serial.println(F("DrawPinPadScreen"));
    DrawScreen(PINPAD, "Pin Code?      ", BLACK, YELLOW, 2);
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

/////////////////////////////////////////////////////////////////////



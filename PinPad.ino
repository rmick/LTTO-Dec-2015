
//  void ClearPin()
//  void PinPadMode()
//  void DrawPinPadScreen()
//  void DrawChangePinScreen()
//  void ChangePin()

////////////////////////////////////////////////////////////////////////////////
byte pinCodeEntered[4];
byte newPin[4];

void ClearPin()
{
  for (uint8_t x = 0; x<4; x++)
  {
    pinCodeEntered[x] = -1;
    newPin[x] = -1;
  }                                 
}

////////////////////////////////////////////////////////////////////////////////

void PinPadMode()
{
  static unsigned int digitCount = 0;
  static int timeOut;
  
  DrawPinPadScreen();

  timeOut ++;
  if (timeOut == 5000)
  {
    state = MEDIC;
    ClearPin;
  }
  
  char* buttonPressed = GetButtonPress();
  uint8_t numberPressed = atoi(buttonPressed);


  if (digitCount < 4)
    {
      for (byte index = 0; index <10; index++)
      {
        if (numberPressed == index)
        { 
          pinCodeEntered[digitCount] = index;
          DrawTextLabel( 150+(digitCount*15), 5, BLACK, "*", 2, YELLOW, 1);
          //DrawTextLabel( 150+(digitCount*15), 5, BLACK, String(index), 2, YELLOW, 1);
          digitCount++;
          timeOut = 0;
        }
      }
    } 
  if (buttonPressed =="Clr")
  { 
    // Clear the display and reset counter
    {
      DrawTextLabel( 150, 5, BLACK, "       ", 2, BLACK, 8);
      digitCount = 0;
      timeOut = 0;
    }
  }
  else if (buttonPressed == "OK")
    { 
      //Check if we have a match
      {
        #ifdef DEBUG Serial.print("\nEntered: ");
        for (int x= 0; x<4; x++)
        {
          Serial.print(pinCodeEntered[x]);
        }
          Serial.print("\nPinCode: ");
        for (int y= 0; y<4; y++)
        {
          Serial.print(pinCode[y]);
        }
        #endif
      }
                                            
      if (pinCodeEntered[0] == pinCode[0] && 
          pinCodeEntered[1] == pinCode[1] &&
          pinCodeEntered[2] == pinCode[2] &&
          pinCodeEntered[3] == pinCode[3] )
          {
            digitCount = 0;
            state = CONFIG;
            ClearPin();
          }
      else 
      {
        DrawTextLabel( 150, 5, BLACK, "WRONG", 2, RED, 5);
        delay (500);
        DrawTextLabel( 150, 5, BLACK, "       ", 2, BLACK, 8);
        digitCount = 0;
        timeOut = 0;
      }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DrawPinPadScreen()
{ 
  if (lastState != state)
  {
    #ifdef DEBUG Serial.println(F("DrawPinPadScreen"));
    #endif
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
    #ifdef DEBUG PrintButtonArray();
    #endif
  }
}

/////////////////////////////////////////////////////////////////////

void DrawChangePinScreen(byte runNumber)
{ 
  if (lastState != state)
  {
    #ifdef DEBUG Serial.println(F("DrawChangePinScreen"));
    #endif
    if      (runNumber == 1)
    {
      DrawScreen(PINPAD, "New Pin?      ", BLACK, YELLOW, 2);
    }
    else if (runNumber == 2)
    {
      DrawScreen(PINPAD, "Confirm?      ", BLACK, GREEN,  2);
      DrawTextLabel( 150, 5, BLACK, String(newPin[0]), 2, WHITE, 1);
      DrawTextLabel( 165, 5, BLACK, String(newPin[1]), 2, WHITE, 1);
      DrawTextLabel( 180, 5, BLACK, String(newPin[2]), 2, WHITE, 1);
      DrawTextLabel( 195, 5, BLACK, String(newPin[3]), 2, WHITE, 1);
    }
    DrawButton( 10,  25,  65,  65, BLUE,  "1",   4, BLACK);
    DrawButton( 85,  25,  65,  65, BLUE,  "2",   4, BLACK);
    DrawButton(160,  25,  65,  65, BLUE,  "3",   4, BLACK);
    DrawButton( 10, 100,  65,  65, BLUE,  "4",   4, BLACK);
    DrawButton( 85, 100,  65,  65, BLUE,  "5",   4, BLACK);
    DrawButton(160, 100,  65,  65, BLUE,  "6",   4, BLACK);
    DrawButton( 10, 175,  65,  65, BLUE,  "7",   4, BLACK);
    DrawButton( 85, 175,  65,  65, BLUE,  "8",   4, BLACK);
    DrawButton(160, 175,  65,  65, BLUE,  "9",   4, BLACK);
    DrawButton( 10, 250,  65,  65, RED,  "Clr",  2, WHITE);
    DrawButton( 85, 250,  65,  65, BLUE,  "0",   4, BLACK);
    DrawButton(160, 250,  65,  65, GREEN, "Save",2, WHITE);
    #ifdef DEBUG  PrintButtonArray();
    #endif
  }
}

/////////////////////////////////////////////////////////////////////

void ChangePin()
{
  static unsigned int digitCount = 0;
  static int timeOut;
  
  DrawChangePinScreen(1);

  timeOut ++;
  if (timeOut == 5000)
  {
    state = CONFIG;
    ClearPin;
  }
  
  char* buttonPressed = GetButtonPress();
  uint8_t numberPressed = atoi(buttonPressed);

  if (digitCount < 4)
    {
      for (byte index = 0; index <10; index++)
      {
        if (numberPressed == index)
        { 
          newPin[digitCount] = index;
          DrawTextLabel( 150+(digitCount*15), 5, BLACK, String(index), 2, YELLOW, 1);
          digitCount++;
          timeOut = 0;
        }
      }
    } 
  if (buttonPressed =="Clr")
  { 
    // Clear the display and reset counter
    {
      DrawTextLabel( 150, 5, BLACK, "       ", 2, BLACK, 8);
      digitCount = 0;
      timeOut = 0;
    }
  }
  else if (buttonPressed == "Save")
    { 
      digitCount = 0;
      state = CONFIRM_PIN;
    }                             
}     

/////////////////////////////////////////////////////////////////////

void ConfirmPin()
{
  static unsigned int digitCount = 0;
  static int timeOut;
  static byte confirmPin[4];
  
  DrawChangePinScreen(2);

  timeOut ++;
  if (timeOut == 5000)
  {
    state = CONFIG;
    ClearPin;
  }
  
  char* buttonPressed = GetButtonPress();
  uint8_t numberPressed = atoi(buttonPressed);

  if (digitCount < 4)
    {
      for (byte index = 0; index <10; index++)
      {
        if (numberPressed == index)
        { 
          confirmPin[digitCount] = index;
          if      (numberPressed == newPin[digitCount]) DrawTextLabel( 150+(digitCount*15), 5, BLACK, String(index), 2, GREEN, 1);
          else if (numberPressed != newPin[digitCount]) DrawTextLabel( 150+(digitCount*15), 5, BLACK, String(index), 2, RED,   1);
          digitCount++;
          timeOut = 0;
        }
      }
    } 
  if (buttonPressed =="Clr")
  { 
    // Clear the display and reset counter
    {
      DrawTextLabel( 150, 5, BLACK, "       ", 2, BLACK, 8);
      DrawTextLabel( 150, 5, BLACK, String(newPin[0]), 2, WHITE, 1);
      DrawTextLabel( 165, 5, BLACK, String(newPin[1]), 2, WHITE, 1);
      DrawTextLabel( 180, 5, BLACK, String(newPin[2]), 2, WHITE, 1);
      DrawTextLabel( 195, 5, BLACK, String(newPin[3]), 2, WHITE, 1);
      digitCount = 0;
      timeOut = 0;
    }
  }
  else if (buttonPressed == "Save")
    { 
      //Check for a match
      if (confirmPin[0] == newPin[0] && 
          confirmPin[1] == newPin[1] &&
          confirmPin[2] == newPin[2] &&
          confirmPin[3] == newPin[3] )
          {
            digitCount = 0;
            StoreNewPin();
            state = CONFIG;
            ClearPin();
          }
    }                             
}     

/////////////////////////////////////////////////////////////////////

void StoreNewPin()
{
  tft.fillScreen(WHITE); delay (1000);
  //Write new PIN to EEPROM.
  for (byte d = 0; d<4; d++)
  {
    EEPROM.write(eePIN_CODE+d, newPin[d]);
    pinCode[d] = newPin[d];
  }
}


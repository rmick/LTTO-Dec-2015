//  ReadMedicScreen
//  DrawMedicScreen
//  Recharge

//////////////////////////////////////////////////////////////////////////////

void ReadMedicScreen()
{
    //////////////////Read the Touchscreen
  //GetTouchInput();
  //if (touchGood)
    TSPoint p = ts.getPoint();
    // Sharing pins, so we need to reset the directions of the touchscreen pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > 10 && p.z <1000)
    {
      /////////////////Was the touch on the RedCross
      if (p.y > 400 && p.y <850)                   // y is from the bottom up
      {
        ReCharge(chargeDelay);
      }
      ////////////////Check for LowTouch
      else if (p.y <150)
      {
        if (deBug) Serial.println(F("Low Touch"));
        delay(1000);
        GetTouchInput();            //Check to see if the touch is still there
        if (touchGood && p.y <150)  
        {
          state = PinCode;
        }
      }
      ///////////////High Touch for a Test Tag
      else if (p.y > 900)
      {
        if (deBug) Serial.println(F("High Touch"));
        tft.fillScreen(YELLOW);
        SendIR('T', B0000001);
        DrawMedicScreen();
      }
    }
}


///////////////////////////////////////////////////////////////////////////////

void DrawMedicScreen()
{
  Serial.println(F("DrawMedic"));
  tft.fillScreen(CYAN);
  
  tft.setCursor(4, 10);
  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.println(F("MEDIC STATION"));
  tft.fillRect(40, 100, 160, 40, RED);
  tft.fillRect(100, 40, 40, 160, RED);

  tft.setTextColor(MAGENTA);
  tft.setCursor(10, 240);
  tft.setTextSize(4);
  tft.println(F("ReCharges"));

  if (numLives > 0)
  {
    tft.fillRect(0, 275, 240, 42, CYAN);
    int numWidth = CountDigits(numLives)*(5*4);            // Characters are 5 by 8 pixels, x 4 (TextSize)
    tft.setCursor((240-numWidth)/2, 283);
    tft.setTextSize(4);
    tft.setTextColor(MAGENTA);
    tft.println(numLives);
  }
}

///////////////////////////////////////////////////////////////////////

void ReCharge(int timer)
{
  /////////////////////////// Countdown Timer

  while (timer >0)
  {
    int numWidth = CountDigits(timer)*(5*4);
    tft.setCursor((240-numWidth)/2, 105);   
    //tft.setCursor(110, 105);
    tft.setTextColor(BLUE);
    tft.setTextSize(4);
    tft.print(timer);
    delay (1000);
    tft.fillRect(40, 100, 160, 40, RED);
    timer--;
  }
    
  /////////////////////////// Counter Update

  numLives++;
  EEPROM.write(0, numLives);

  /////////////////////////// Send the ReCharge Beacon

  tft.fillScreen(GREEN);
  for (int repeat = 1; repeat <=5; repeat++)
  {
    SendIR('B', B0000011);      // Send a beacon with the message 00011 (DEC 3)           // This is a non-team beacon
    if (deBug) Serial.println(F("Beacon Sent"));
  }
  DrawMedicScreen();
}


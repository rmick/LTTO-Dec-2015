// PinPadScreen
// DrawKeypad
// GetKeyPress



////////////////////////////////////////////////////////////////////////////////

void PinPadScreen()
{
  int keyNum;
  
  tft.fillScreen(BLACK);
  tft.setCursor(65, 0);
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println(F("Pin Code?"));
  if (deBug) Serial.println(F("DrawPinPad"));
  DrawKeypad();
  
  //////////////////Wait for Data Input
  
  while (state == PinCode)
  { 
    keyNum = GetKeyPress();
    if (deBug & keyNumNew)
    {
      Serial.print(F("The keypressed was "));
      Serial.println(keyNum);
    }
    
    // now do something with the number
  if (keyNumNew)
  {
    switch(keyNum)
    {
      case 255:
        // Ok button
        state = Config;
        break;
      case -1:
        // Clr button
        state = Medic;
        break;
      case 0:
        // 0 button
        state = Tagger;
        break;
    }
    keyNumNew = 0;
    delay(200);
  }  

   /////////////////If PIN correct proceed, else Clear and wait for input
   
  }
  
}




///////////////////////////////////////////////////////////////////////////////

void DrawKeypad()
{
  if (deBug) Serial.println(F("DrawKeyPad"));

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

/////////////////////////////////////////////////////////////////////////////////

int GetKeyPress()
{   
  int numPressed = 42;
  TSPoint p = ts.getPoint();
  // Sharing pins, so we need to reset the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > 10 && p.z <1000)                                      
   {
    byte column = 0; 
    byte row = 0;
    if (deBug)
    {
      Serial.print(F("Touch: "));
      Serial.print(p.x);
      Serial.print(F(", "));
      Serial.println(p.y);
    }
    //Check for 1 of 3 columns
    if (p.x > 700 && p.x < 800) column = 1;
    if (p.x > 400 && p.x < 550) column = 2;
    if (p.x > 200 && p.x < 350) column = 3;

    //Check for 1 of 4 rows
    if (p.y > 750 && p.y < 850) row = 1;
    if (p.y > 550 && p.y < 700) row = 2;
    if (p.y > 350 && p.y < 500) row = 3;
    if (p.y > 175 && p.y < 290) row = 4;

    // A big If then else statement to decide which number
    if (column == 1 && row == 1) numPressed = 1;
    if (column == 2 && row == 1) numPressed = 2; 
    if (column == 3 && row == 1) numPressed = 3;
    if (column == 1 && row == 2) numPressed = 4;
    if (column == 2 && row == 2) numPressed = 5;
    if (column == 3 && row == 2) numPressed = 6;
    if (column == 1 && row == 3) numPressed = 7;
    if (column == 2 && row == 3) numPressed = 8;
    if (column == 3 && row == 3) numPressed = 9;
    if (column == 2 && row == 4) numPressed = 0;
    if (column == 1 && row == 4) numPressed = -1;
    if (column == 3 && row == 4) numPressed = 255;
    keyNumNew = 1;
    return numPressed;
    }

}

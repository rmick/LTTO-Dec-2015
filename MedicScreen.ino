//  void MedicMode()
//  void DrawMedicScreen()
//  void ReCharge(int timer)
//  void ReCharge(int timer)

//////////////////////////////////////////////////////////////////////////////

void MedicMode()
{
  DrawMedicScreen();   
  static int count;
  char* Action = GetButtonPress();
  if      (Action == "Heal")   ReCharge(medicDelay);
  else if (Action == "EXIT")   state = PINPAD;

  count++;
 
  if (count == 1000 && hostile)
  {
    if (teamID == 1)  SendIR ('T', B0100011);
    if (teamID == 2)  SendIR ('T', B1000011);
    if (teamID == 3)  SendIR ('T', B1100011);
    count = 0;
  }
}


///////////////////////////////////////////////////////////////////////////////

void DrawMedicScreen()
{
  if (lastState != state)
  {
    if (deBug) Serial.println(F("DrawMedicScreen"));
    DrawScreen(MEDIC, "MEDIC STATION", CYAN, RED, 3);
    DrawButton  (40,   40, 160, 160, CYAN,  "Heal", 4, CYAN);      // Draw a huge 'hidden' Cyan button behid the Red Cross
    tft.fillRect(40,  100, 160,  40, RED);                         // Draw a Red Cross over the top.
    tft.fillRect(100,  40,  40, 160, RED);
    DrawButton  (100, 310,  40,   5, CYAN,  "EXIT", 1, CYAN);
    
    // Draw the Bottom part of the screen with the Recharge Info
    tft.setTextColor(MAGENTA);
    tft.setCursor(10, 240);
    tft.setTextSize(4);
    tft.println("ReCharges");
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
}

////////////////////////////////////////////////////////////////////////////////////////////

void ReCharge(int timer)
{
  // Countdown Timer
  while (timer >0)
  {
    int numWidth = CountDigits(timer)*(5*4);
    tft.setCursor((240-numWidth)/2, 105);
    tft.setTextColor(BLUE);
    tft.setTextSize(4);
    tft.print(timer);
    delay (1000);
    tft.fillRect(40, 100, 160, 40, RED);
    timer--;
  }
  SendBeacon();
  numLives++;
  EEPROM.write(0, numLives);
}

/////////////////////////////////////////////////////////////////////////////////////

void SendBeacon()         // Medic beacon is B0000011 (no team). Bits 2+3 alter for TeamID.
{
  int bitShiftTeamID = teamID << 2;
  Serial.println(bitShiftTeamID, BIN);
  int beaconSignal = bitShiftTeamID + 3;
  Serial.println(beaconSignal, BIN);
  tft.fillScreen(GREEN);
  for (int repeat = 1; repeat <=5; repeat++)
  {
    SendIR('B', beaconSignal);      // Send a beacon with the message 00011 (DEC 3)           // This is a non-team beacon
    if (deBug) Serial.println(F("Beacon Sent"));
  }
  lastState = NONE;
  DrawMedicScreen();
}


//  void MedicMode()
//  void DrawMedicScreen()
//  void ReCharge(int timer)
//  void ReCharge(int timer)

//////////////////////////////////////////////////////////////////////////////

void MedicMode()
{
  DrawMedicScreen();   
  static int count;
  char const* Action = GetButtonPress();
  if      (Action == "Heal")   ReCharge(medicDelay);
  else if (Action == "EXIT")   state = PINPAD;

  count++;
 
  if (count == 1000 && hostile)
  {
    if (teamID == 1)  { SendIR ('T', B1000011); SendIR ('T', B1100011); }    // Tag Teams 2 + 3         //  If teamID == 0 then there NO teams,
    if (teamID == 2)  { SendIR ('T', B0100011); SendIR ('T', B1100011); }    // Tag Teams 1 + 3         //  so there is no hostile mode possible
    if (teamID == 3)  { SendIR ('T', B0100011); SendIR ('T', B1000011); }    // Tag Teams 1 + 2
    count = 0;
  }
}


///////////////////////////////////////////////////////////////////////////////

void DrawMedicScreen()
{
  if (lastState != state)
  {
    #ifdef DEBUG Serial.println(F("DrawMedicScreen"));
    #endif
    DrawScreen(MEDIC, "MEDIC STATION", CYAN, RED, 3);
    DrawButton  (40,   40, 160, 160, CYAN,  "Heal", 4, CYAN);      // Draw a huge 'hidden' Cyan button behid the Red Cross
    tft.fillRect(40,  100, 160,  40, RED);                         // Draw a Red Cross over the top.
    tft.fillRect(100,  40,  40, 160, RED);
    DrawButton  (100, 310,  40,  10, CYAN,  "EXIT", 1, CYAN);
    
    // Draw the Bottom part of the screen with the Recharge Info
    DrawTextLabel  ( 0,  240, CYAN, "Activations", 3, MAGENTA, 0);
    DrawTextLabel  ( 0,  280, CYAN, String(medicCount), 4, MAGENTA, 0);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////

void ReCharge(int timer)
{
  // Countdown Timer
  while (timer >0)
  {
    DrawTextLabel  ( 0,  105, RED, String(timer), 4, YELLOW, 3);
    delay (1000);                                                    // TODO: Change to non blocking (better for Hostile mode)
    timer--;
  }
  medicCount++;
  EEPROM.write(eeMEDIC_COUNT, medicCount);
  SendBeacon();
}

/////////////////////////////////////////////////////////////////////////////////////

void SendBeacon()         // Medic beacon is B0000011 (no team). Bits 2+3 alter for TeamID.
{
//  TODO: Make a function to create the data
  // void AssembleByte(teamID, playerID, mega/beacon) 
  int bitShiftTeamID = teamID << 2;
  int bitShiftPlayerID = playerID << 5;
  //TODO: Do a XOR merge of TeamID and PlayerID
  int beaconSignal = bitShiftTeamID + B11;
  tft.fillScreen(GREEN);
  for (int repeat = 1; repeat <=5; repeat++)
  {
    SendIR('B', beaconSignal);
  }
  lastState = NONE;
  DrawMedicScreen();
}


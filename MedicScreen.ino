#include <Arduino.h>

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
  if      (strcmp(Action, "Heal")	== 0)   ReCharge(medicDelay);
  else if (strcmp(Action, "CONFIG")	== 0)   state = PINPAD;

  count++;
 
  if (count == 1000 && hostile)
  {
    if (teamID == 1)  { ltto.sendIR ('T', B1000011); ltto.sendIR ('T', B1100011); }    // Tag Teams 2 + 3         //  If teamID == 0 then there NO teams,
    if (teamID == 2)  { ltto.sendIR ('T', B0100011); ltto.sendIR ('T', B1100011); }    // Tag Teams 1 + 3         //  so there is no hostile mode possible
    if (teamID == 3)  { ltto.sendIR ('T', B0100011); ltto.sendIR ('T', B1000011); }    // Tag Teams 1 + 2
    count = 0;
  }
}


///////////////////////////////////////////////////////////////////////////////

void DrawMedicScreen()
{
  if (lastState != state)
  {
    #ifdef DEBUG
	  Serial.println(F("DrawMedicScreen"));
    #endif
    DrawScreen(MEDIC, "MEDIC STATION", BLACK, RED, 3);
    DrawButton  (40,   40, 160, 160, BLACK,  "Heal", 4, BLACK);      // Draw a huge 'hidden' button behind the Red Cross
    tft.fillRect(40,  100, 160,  40, RED);							// Draw a Red Cross over the top.
    tft.fillRect(100,  40,  40, 160, RED);
    DrawButton  (100, 300,  40,  10, BLACK,  "CONFIG", 1, CYAN);
    
    // Draw the Bottom part of the screen with the Recharge Info
    DrawTextLabel  ( 0,  230, BLACK, "Respawns",		 4, MAGENTA, 0);
    DrawTextLabel  ( 0,  270, BLACK, String(medicCount), 4, MAGENTA, 0);
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
  int beaconSignal = bitShiftTeamID + B11;
  tft.fillScreen(GREEN);
  for (int repeat = 1; repeat <=5; repeat++)
  {
    ltto.sendIR(BEACON, beaconSignal);				//TODO: Which of these methods actaully works as a Medic Beacon ?
	//ltto.sendZoneBeacon(SUPPLY_ZONE, teamID);
  }
  lastState = NONE;
  DrawMedicScreen();
}

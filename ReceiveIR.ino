

unsigned long pinChangeTime;
bool newPulse;
int pass;
unsigned long startSpaceTime;
unsigned long startMarkTime;
unsigned int markLength;
unsigned int spaceLength;
bool newMark;
bool newSpace;

bool startBit=0;
bool startBreak=0;
int packetIndex;
bool packetGood =0;
bool dataBitSet;

unsigned int IrDataPacket[12];

void GetIR()
{

  if (deBug) Serial.println(), Serial.print(F("GetIR-pass#")), Serial.print(pass), Serial.print(F("-> "));

  bool pinState = digitalRead(IR_RECEIVE_PIN);
 
  if (pinState)                                           // We have the start of a Break (inverted)
  {
    startSpaceTime = pinChangeTime;                       // Set the start time of the Break
    markLength = pinChangeTime - startMarkTime;           // Stop the break timer and store the value
    newPulse = 0;                                     // Set flag to false as we have processed it
    newMark = 1;
  }
  else                                                    // We have the start of a Mark (inverted)
  {
    startMarkTime = pinChangeTime;
    spaceLength = pinChangeTime - startSpaceTime;
    newPulse = 0;
    newSpace = 1;
  }

    pass++;                                               // Used for deBug to count how times we go thru this routine.

/*
State Machine

Check for a 3mS pulse and set Flag1
Check for 6mS break and set Flag2 + clear datapacket. If not 6mS abort
Check for 3 or 6mS pulse to set packet type and expected length. If not 3/6 abort
Wait 2mS for next pulse. If >2.5mS abort
Wait for 1ms or 2mS pulse. Store as a bit in the array. Inc array index
Repeat 5 or 7 times depending on packet expected
Abort if any of the above fail. Also start again if a new 3/6 packet arrives
If all data is good, store array and set flag for action
Have a beer.
*/


  if (markLength >2500 & markLength <3500 & startBit !=1)        // Found a start bit
  {
    startBit = 1;
    pass = 1;
    IrDataPacket[1] = 3;
    markLength = 0;
    //if (deBug) Serial.print("3s");
    return;
  }

  if (spaceLength >5500 & spaceLength <6500 & startBit)     // Found a start break
  {
    startBreak = 1;
    IrDataPacket[2] = 6;
    packetIndex = 3;
    spaceLength = 0;
    //if (deBug) Serial.print("6s");






    
    for (int i1 = 3; i1 <=10; i1++)                   //Clear datapacket (leaving bits 0-2 as set) - use 9 as 0/1 are data
      {
        IrDataPacket[i1]= 9;
      }
    return;
  }

  if (startBit ==1 && startBreak ==1)                         // We have a datapacket, now look for 3 or 6mS mark
  {
    if (markLength >2500 && markLength < 3500)
    {
      IrDataPacket[packetIndex] = 3;
      markLength = 0;
      if (deBug) Serial.print(F("TAG"));
      return;
    }
    if (markLength >5500 && markLength < 6500)
    {
      IrDataPacket[packetIndex] = 6;
      markLength = 0;
      if (deBug) Serial.print(F("BEACON"));
      return;
    }
    if (spaceLength >1500 && spaceLength < 2500)               // Check for a valid 2mS break and Inc packetIndex
    {
      packetIndex++;
      spaceLength = 0;
      //if (deBug) Serial.print("2mS Break");
      return;
    }                       
    if (markLength >500 && markLength < 1500)               // Here comes the data....
    {
      IrDataPacket[packetIndex] = 0;
      markLength = 0;
      //if (deBug) Serial.print("            Bit0. ");
      dataBitSet = 1;
    }
    if (markLength >1500 && markLength < 2500)
    {
      IrDataPacket[packetIndex] = 1;
      markLength = 0;
      //if (deBug) Serial.print("            Bit1. ");
      dataBitSet = 1;
    }
  }

  if (IrDataPacket[3] == 3 && packetIndex == 10)
  {
    packetGood = 1;
    startBit = 0;
    startBreak = 0;
    packetIndex = 0;
    if (deBug) Serial.println(), Serial.println(F("Good Packet 7bit Tag")), Serial.println(); 
    return;
  }

  if (IrDataPacket[3] == 6 && packetIndex == 8)
  {
    packetGood = 1;
    startBit = 0;
    startBreak = 0;
    packetIndex = 0;
    if (deBug) Serial.println(), Serial.println(F("Good Packet 5bit Beacon")), Serial.println();
    return;
  }

  if (dataBitSet)                                   // We received a valid data bit so exit before the bad data catch below
  {
    dataBitSet = 0;
    return;
  }

  
// Check for any bad data and abort                // If we get to here then there is something wrong
  
 

    if (deBug)
    {
      Serial.println(), Serial.print(F("Bad Data: "));          
      Serial.print(F("Space=")), Serial.print(spaceLength), Serial.print(F(", Mark=")), Serial.println(markLength);
    }
    startBit = 0;
    startBreak = 0;
    for (int e = 1; e <=10; e++)
      {
        IrDataPacket[e]= 8;
      }       

 // Do I need to check for a valid 3mS mark, in case it appears mid data stream?
}

//////////////////////////////////////////////////////////////////////////////

/*
void ISRpulse()
{
  pinChangeTime = micros();              // Store the time that the pin changes
  newPulse = 1;                          // Set flag to true so that GetIR() will be triggered from Void Loop
}
*/


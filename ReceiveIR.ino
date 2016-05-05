//  void ISRchange()
//  createIRmessage()
//  void PrintIR()
//  void clearIRarray()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

volatile byte countISR = 0;
uint8_t irTimeOut = 12;                      // This is the length of break to look for, to indicate end of packet.
volatile byte overflowISR = 0;       // Debug only
 
void ISRchange()
{
  ////---------------------------------------------------------------------------------------------------------
  //  Set up the variables
    volatile static int16_t pulseLength;
    volatile static uint16_t lastEdge;
    volatile static uint16_t pinChangeTime;
   

  ////---------------------------------------------------------------------------------------------------------
  //  Action the Interrupt
    overflowISR++;
    pinChangeTime = micros();                   // Store the time that the pin changes
    pulseLength = pinChangeTime - lastEdge;     // Measure the elapsed time since last lastEdge
  
  ////---------------------------------------------------------------------------------------------------------
  //  Look for short packets (noise) and discard
    if (pulseLength < 500 && pulseLength > 0)
    {
      overflowISR = 0;
      return;              // exit as the pulse is too short, so probably noise
    }

  ////---------------------------------------------------------------------------------------------------------
  // continue actioning the Interrupt
    receiveMilliTimer = irTimeOut;                       // Set the timer for a XXmS break - the expiry of which indicates an end of packet.
    lastEdge = pinChangeTime;                           // Reset the lastEdge to now
    volatile int8_t bitLength = (pulseLength+500)/1000;          // Round up and divide by 1,000 to give mS
    volatile int pinState = digitalRead(IR_RECEIVE_PIN);         // Check state of pin (High/Low) for mark or space.
    if (pinState == LOW) bitLength = 0 - bitLength;     // Set a Mark as Positive and a Break as Negative
    //if (PINB & 8); else   bitLength = 0 - bitLength;

  ////---------------------------------------------------------------------------------------------------------
  //  Look for a 3/6 Header
  
    if (bitLength == -6)                                // Look for a 6mS break
    {
      if (messageIR[countISR-1] == 3)                   // Check that the preceding bit was 3mS Header.
      {                                                 // :-) We have a header so start collecting data
        messageIR[1] = 3;                               // Store the 3mS into index[1] of the array
        countISR = 2;                                   // Set the counter to 2 (we filled [1] above, so 2 is next)
        //Serial.print(F("\nHeader."));        
      }
      else 
      {
        Serial.print("x");
        //Serial.print(F("\nRecIR:55 - X/-6 header error. X = "));
        //Serial.print(messageIR[countISR-1]);
        receiveMilliTimer = irTimeOut*2;               //Give it time for ClearIRarray to run.
        ClearIRarray();
        overflowISR = 0;
        return;
      }
    }
  
  ////---------------------------------------------------------------------------------------------------------
  //  Check for too many bits without a header (stops overflow of Array variables)
    if (countISR > (ARRAY_LENGTH-2) )
    {
      Serial.print("y");
      //Serial.println(F("\nRecIR:67 - Array Overlength Error Trap"));
      countISR = 0;
      overflowISR = 0;
      return;
    }

  ////---------------------------------------------------------------------------------------------------------
  //  Store the data to the message array
    if      (countISR  < 4)                   messageIR[countISR] = bitLength;         // Store the raw +3/-6/+X data to the first bits to keep it obvious
    else if (countISR >= 4 && bitLength > 0)  messageIR[countISR] = bitLength - 1;     // Store + data bits as 0/1 instead of the raw +1/+2 to make it simple
    else                                      messageIR[countISR] = bitLength;         // Leave the - spaces as raw 2mS
 
  ////---------------------------------------------------------------------------------------------------------
  // Check for re-entrant ISR routine calls.
    if (overflowISR >1)
    {
      Serial.print("z");
      //Serial.print(F("\nRecIR:79 - reEntrant error in ISR. countISR= "));
      //Serial.println(countISR);
    }
 
  ////---------------------------------------------------------------------------------------------------------
  //  increment the ISR counter and clear the overflow count
    countISR++;
    overflowISR = 0;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateIRmessage()                                                            // TODO: Currently not checking for valid -2mS breaks !!!!
{
  disableInterrupt(IR_RECEIVE_PIN);
  
  if (deBug)
  {
    if (countISR > 5)   Serial.print(F("Create."));
    if (countISR < 6) { Serial.print(F("\nRecIR:106 - CountISR <5 Error")); ClearIRarray(); enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE); return; }       // The message was incomplete
  }
  
  ////---------------------------------------------------------------------------------------------------------
  //  Set the message type via the number of bits received (and 1/9th bit of Packet/Checksum)
    if      (countISR > 20 && messageIR[3] == 3 && messageIR[5] == 0)  { receivedIRmessage.type = 'P'; Serial.print(F("packet."));   }       // Packet
    else if (countISR > 20 && messageIR[3] == 3 && messageIR[5] == 1)  { receivedIRmessage.type = 'C'; Serial.print(F("checksum.")); }       // Checksum
    else if (countISR > 18 && messageIR[3] == 3 && countISR < 21)      { receivedIRmessage.type = 'D'; Serial.print(F("data."));     }       // Data
    else if (countISR > 16 && messageIR[3] == 3 && countISR < 19)      { receivedIRmessage.type = 'T'; Serial.print(F("tag."));      }       // Tag
    else if (countISR > 12 && messageIR[3] == 6 && countISR < 15)      { receivedIRmessage.type = 'B'; Serial.print(F("beacon."));   }       // Beacon - only beacons have 3/6/6 header !!
    else    { Serial.print(F("incorrect packet length: ")); Serial.print(countISR); PrintIR(); ClearIRarray(); enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE); return; }
 
  ////---------------------------------------------------------------------------------------------------------
  // Set the message length based on the type
    byte messageLength = 0;
    if      (receivedIRmessage.type == 'T') messageLength = 17;   // Long Break [0] + 3 header [1,2,3] + break [4] + 7 bits,breaks [5,7,9,11,13,15,17]
    else if (receivedIRmessage.type == 'B') messageLength = 13;   // Long Break [0] + 3 header [1,2,3] + break [4] + 5 bits,breaks [5,7,9,11,13]
    else if (receivedIRmessage.type == 'P') messageLength = 21;   // Long Break [0] + 3 header [1,2,3] + break [4] + 9 bits,breaks [5,7,9,11,13,15,17,19,21]
    else if (receivedIRmessage.type == 'D') messageLength = 19;   // Long Break [0] + 3 header [1,2,3] + break [4] + 8 bits,breaks [5,7,9,11,13,15,17,19]
    else if (receivedIRmessage.type == 'C') messageLength = 21;   // Long Break [0] + 3 header [1,2,3] + break [4] + 9 bits,breaks [5,7,9,11,13,15,17,19,21]
    else    (receivedIRmessage.type == '_');
  
  ////---------------------------------------------------------------------------------------------------------
  //  Push the data into the dataPacket
    for (int i = 5; i<=messageLength; i+=2)
      {
        if (messageIR [i] == 42)
        {
          Serial.print(F("\nRecIR:134 - bad data '42' "));
          ClearIRarray();
          enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE);
          return;
        }
        receivedIRmessage.dataPacket = receivedIRmessage.dataPacket << 1;
        receivedIRmessage.dataPacket = receivedIRmessage.dataPacket + (messageIR [i]);
      }

  ////---------------------------------------------------------------------------------------------------------
  //  Tidy up and go home
  countISR = 0;                                                   // Reset the counter ready for the next packet.
  newIRmessageReady = TRUE;                                       // Set the flag to say there is a new message to decode.
  //if (deBug) PrintIR();
  enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE);
}

//////////////////////////////////////////////////////////////////////

void DecodeDataIR()
{
  PrintIR();
  ClearIRarray();
  // TODO:  So far this does nothing, we need to action the data for hosting.
}

//////////////////////////////////////////////////////////////////////

void PrintIR()
{
  bool extendedPrintIR = TRUE;
  
  disableInterrupt(IR_RECEIVE_PIN);

  if (extendedPrintIR == TRUE)
  {   
    Serial.print(F("\nPrintIR(ext): "));
    for (int i = 0; i<=ARRAY_LENGTH; i++)
    {
      if (messageIR[i] != 42)
      {
        Serial.print(messageIR [i]);
        Serial.print(F(", "));
      }
      
    }

  }

  else
  {
    Serial.print(F("\nPrintIR: "));
    Serial.print(receivedIRmessage.type);
    Serial.print(F(", "));
    Serial.print(receivedIRmessage.dataPacket, BIN);
    Serial.println();
  }
  
  if (receivedIRmessage.type == 'C') Serial.println(F("-------------------"));
  
  enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE);
}

////////////////////////////////////////////////////////////////////

void ClearIRarray()
{
  //#ifdef DEBUG 
  Serial.println(F("\nClearIRarray()"));
  //#endif
  for (int i = 0; i<=ARRAY_LENGTH; i++)
  {
    messageIR[i] = 42;                                          // This shows clear, as 0 is a valid data bit.
  }
  receivedIRmessage.dataPacket = -32768;
  newIRmessageReady = FALSE;
  countISR = 0;                                                 // This is here to clear things up when not in Tagger mode (i.e. not receiving)
}

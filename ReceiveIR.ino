//  void ISRchange()
//  void PrintIR()
//  void clearIRarray()

///////////////////////////////////////////////////////////////////////

byte countISR = 0;

void ISRchange()
{
  static int16_t pulseLength;
  static uint16_t lastEdge;
  static uint16_t pinChangeTime;
  static byte expectedMessageLength = 0;
  
  static byte overflowISR = 0;                  // TODO: Debug, remove it!
  uint16_t timerISR = 0;                        // TODO: Debug, remove it!
  static uint16_t elapsedtime = 0;              // TODO: Debug, remove it!
  static uint16_t runtime = micros();           // TODO: Debug, remove it!

  // Action the Interrupt........
  overflowISR++;
  pinChangeTime = micros();                   // Store the time that the pin changes
  pulseLength = pinChangeTime - lastEdge;     // Measure the elapsed time since last lastEdge
  if (pulseLength < 500 && pulseLength > 0)
  {
    //Serial.print("(S-");
    //Serial.print(pulseLength);
    //Serial.print("mS)");
    overflowISR = 0;
    return;              // exit as the pulse is too short, so probably noise
  }
  receiveMilliTimer = 20;
  lastEdge = pinChangeTime;                           // Reset the lastEdge to now
  int8_t bitLength = (pulseLength+500)/1000;
  if (PINB & 8); else   bitLength = 0 - bitLength;    //Set a Mark as Positive and a Break as Negative.


  // Find a Header............
  if (bitLength == -6)                        // Look for a 6mS break
  {
    if (messageIR[countISR-1] == 3)           // Check that the preceding bit was 3mS Header.
    {                                         // - we have a header so start collecting data
      if (deBug) Serial.print(F("(3/6)\t"));
      messageIR[1] = 3;
      countISR = 2;
      Serial.print(F("\nHeader."));
    }
    else 
    {
      ClearIRarray();
      countISR = 2;       // reset the count as this is a long break, even if the 3mS mark is missing.
      if (deBug)
      {
        //Serial.print("(");
        //Serial.print(messageIR[countISR]);
        //Serial.print("/6)");
      }
    }
  }
  
  // Set message length based on header type                      
  //              TODO: This does not work with Data packets - So we need a new routine to deal with this !!!
  //                    (a) In non-game mode use a different routine, as all packets will be P,D,C type only
  //                    (b) Use Timer0 and ISR routines to watch for long breaks (>6mS) as the end of a packet

  if (state == TAGGER)
  {
    if        (messageIR[3] == 3)   expectedMessageLength = 18;
    else if   (messageIR[3] == 6)   expectedMessageLength = 14;
  }
  
  // Check for a Long Break...............
  if (bitLength < -7 || bitLength > 7)                // This means we have an error, as max value is +/- 6
  { 
    //if (deBug) Serial.println("RecIR:77 - Long Break");     //TODO: Use the RxTimer in Tagger mode to trigger packets, such as text data.
    messageIR[0] = bitLength;
    countISR = 1;
    overflowISR = 0;
    return;
  }

  // Check for too many bits without a header......... (stops overflow of Array variables)
  if (countISR > (ARRAY_LENGTH-2) )
  {
    Serial.println(F("\nRecIR:86 - Array Overlength Error Trap"));
    countISR = 0;
  }

  //Store the data to the message array
  if      (countISR < 5 && bitLength > 0) messageIR[countISR] = bitLength;            //Store the raw 3/6/x data to the first bits to keep it obvious
  else if (countISR > 5 && bitLength > 0) messageIR[countISR] = bitLength - 1;    //Store 0/1 instead of the raw 1/2 bits for the rest to make it simple
  else                                    messageIR[countISR] = bitLength;        // This stops the -2ms breaks from being changed to -3mS !!
               
  //messageIRpulse[countISR] = pulseLength;
  countISR++;

    if (deBug)
    {
      //Serial.print(".");
    }
    
  // Look for the end of a message and process it.  
  if (countISR == expectedMessageLength) CreateIRmessage('T');
  if (countISR >  expectedMessageLength && expectedMessageLength > 10)        //TODO: this is debug. The >10 is because until the 363 stuff arrives the length = ZERO.
  {
    Serial.print(F("\n   My what a long message you have! RecIR:105   - "));
  }

  // Store the time it took to process the interupt routine 
  timerISR = micros() - pinChangeTime;
  //messageISRdelay[countISR] = timerISR;
  //messageISRelapsed [countISR] = micros();
   
  // Check for re-entrant ISR routine calls.
  if (overflowISR >1)
  {
    Serial.print(F("(R-"));
    Serial.print(countISR);
    Serial.print(F(")"));
  }
  overflowISR = 0;

  // End of ISR routine.
}

//////////////////////////////////////////////////////////////////////

void CreateIRmessage(char source)                                      // TODO: Currently not checking for valid -2mS breaks !!!!
{
  if (countISR > 5) Serial.print(F("Create."));
  switch (source)
  {
    case 'T':
      if      (messageIR[3] == 3)  { receivedIRmessage.type = 'T'; Serial.print(F("tag.")); }
      else if (messageIR[3] == 6)  { receivedIRmessage.type = 'B'; Serial.print(F("beacon.")); }
      else Serial.print(F("RecIR:134: WTF!"));
    break;

    case 'H':
      if (countISR <5)              // Noise filter.
      {
        rxTimerExpired = FALSE;
        return;       
      }
      
      if      (countISR > 21 && messageIR[5] == 0)  { receivedIRmessage.type = 'P'; Serial.print(F("packet.")); }
      else if (countISR < 21)                       { receivedIRmessage.type = 'D'; Serial.print(F("data.")); }
      else if (countISR > 21 && messageIR[5] == 1)  { receivedIRmessage.type = 'C'; Serial.print(F("checksum.")); }
      else    { Serial.print(F("unknown:")); Serial.print(countISR); Serial.print(F(".")); }        
    break;
    
  }
 
  //Set message length
  byte messageLength = 0;
  if      (receivedIRmessage.type == 'T') messageLength = 17;   // Long Break [0] + 3 header [1,2,3] + break [4] + 7 bits,breaks [5,7,9,11,13,15,17]
  else if (receivedIRmessage.type == 'B') messageLength = 13;   // Long Break [0] + 3 header [1,2,3] + break [4] + 5 bits,breaks [5,7,9,11,13]
  else if (receivedIRmessage.type == 'P') messageLength = 21;   // Long Break [0] + 3 header [1,2,3] + break [4] + 9 bits,breaks [5,7,9,11,13,15,17,19,21]
  else if (receivedIRmessage.type == 'D') messageLength = 19;   // Long Break [0] + 3 header [1,2,3] + break [4] + 8 bits,breaks [5,7,9,11,13,15,17,19]
  else if (receivedIRmessage.type == 'C') messageLength = 21;   // Long Break [0] + 3 header [1,2,3] + break [4] + 9 bits,breaks [5,7,9,11,13,15,17,19,21]
  else { messageLength = countISR; Serial.print(F("set.")); Serial.print(countISR); Serial.print(receivedIRmessage.type); Serial.print(F("."));}      //TODO: this is a trap all, for debug
  
  //Push the data into the dataPacket
  for (int i = 5; i<=messageLength; i+=2)
    {
      receivedIRmessage.dataPacket = receivedIRmessage.dataPacket << 1;
      receivedIRmessage.dataPacket = receivedIRmessage.dataPacket + (messageIR [i]);
    }
 
  countISR = 0;
  if (deBug) PrintIR();
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
  bool extendedPrintIR = FALSE;
  
  disableInterrupt(IR_RECEIVE_PIN);
  countISR = 0;

/*
  if (extendedPrintIR == TRUE)
  {
    Serial.println();
    
    Serial.print(F("\nPrinting bitLength data:\t"));
    for (int i = 0; i<=ARRAY_LENGTH; i++)
    {
      Serial.print(messageIR [i]);
      Serial.print(F("\t"));
    }
    
    Serial.print(F("\nPrinting pulse raw times:\t"));
    for (int i = 0; i<=ARRAY_LENGTH; i++)
    {
      Serial.print(messageIRpulse [i]);
      Serial.print(F("\t"));
    }
    
    Serial.print(F("\nPrinting total elasped times:\t"));  
    for (int i = 1; i<=ARRAY_LENGTH; i++)
    {
//      Serial.print(messageISRelapsed [i]);
      Serial.print(F("\t"));
    }
    Serial.print(F("\nPrinting ISR cpu times:\t\t"));  
    for (int i = 1; i<=ARRAY_LENGTH; i++)
    {
//      Serial.print(messageISRdelay [i]);
      Serial.print(F("\t"));
    }
  }
*/

  Serial.print("\nPrintIR: ");
  Serial.print(receivedIRmessage.type);
  Serial.print(", ");
  Serial.print(receivedIRmessage.dataPacket, BIN);
  Serial.println();

  if (receivedIRmessage.type == 'C') Serial.println(F("-------------------"));
  
  enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE);
}

////////////////////////////////////////////////////////////////////

void ClearIRarray()
{
  #ifdef DEBUG Serial.println(F("ClearIRarray()"));
  #endif
  for (int i = 0; i<=ARRAY_LENGTH; i++)
  {
    messageIR[i]          = 0;
    //messageIRpulse[i]     = 0;
    //messageISRdelay[i]    = 0;
    //messageISRelapsed[i]  = 0;
    
  }
  receivedIRmessage.type = '_';
  receivedIRmessage.dataPacket = 0;
  
}

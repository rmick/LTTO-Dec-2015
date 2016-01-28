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
    Serial.print("(S-");
    Serial.print(pulseLength);
    Serial.print("mS)");
    overflowISR = 0;
    return;              // exit as the pulse is too short, so probably noise
  }
  lastEdge = pinChangeTime;                   // Reset the lastEdge to now
  int8_t bitLength = (pulseLength+500)/1000;
  if (PINB & 8); else   bitLength = 0 - bitLength;    //Set a Mark as Positive and a Break as Negative.


  // Find a Header............
  if (bitLength == -6)                        // Look for a 6mS break
  {
    if (messageIR[countISR-1] == 3)           // Check that the preceding bit was 3mS Header.
    {                                         // - we have a header so start collecting data
      if (deBug) Serial.print("(3/6)");
      messageIR[1] = 3;
      countISR = 2;
    }
    else 
    {
      ClearIRarray();
      countISR = 2;       // reset the count as this is a long break, even if the 3mS mark is missing.
      if (deBug)
      {
        //return;
        Serial.print("(");
        Serial.print(messageIR[countISR]);
        Serial.print("/6)");
      }
    }
  }
  
///////////////  STOP this for now so that I can see if I am getting extra bits from noise !!
  // Set message length based on header type
  if        (messageIR[3] == 3)   expectedMessageLength = 18;
  else if   (messageIR[3] == 6)   expectedMessageLength = 14;
  //else if   (there is no header) expectedMessageLength = 2-3bytes  // TODO: how do we process packets without headers ?
  
  // Check for a Long Break...............
  if (bitLength < -7 || bitLength > 7)                // This means we have an error, as max value is +/- 6
  { 
    if (deBug) Serial.print(">");
    messageIR[0] = bitLength;
    countISR = 1;
    overflowISR = 0;
    return;
  }

  // Check for too many bits without a header......... (stops overflow of Array variables)
  if (countISR > (ARRAY_LENGTH-2) )
  {
    Serial.println("\nHouston we have a problem");
    countISR = 0;
  }

  //Store the data to the message array
  messageIR[countISR] = bitLength; 
  messageIRpulse[countISR] = pulseLength;
  countISR++;

    if (deBug)
    {
      Serial.print(".");
      //Serial.print(countISR);
    }
    
  // Look for the end of a message and process it.  
  if (countISR == expectedMessageLength) CreateIRmessage();

  // Store the time it took to process the interupt routine 
  timerISR = micros() - pinChangeTime;
  messageISRdelay[countISR] = timerISR;
  messageISRelapsed [countISR] = micros();
   
  // Check for re-entrant ISR routine calls.
  if (overflowISR >1)
  {
    Serial.print("(R-");
    Serial.print(countISR);
    Serial.print(")");
  }
  overflowISR = 0;

  // End of ISR routine.
}

//////////////////////////////////////////////////////////////////////

void CreateIRmessage()
{
  //if (deBug) Serial.println("\nCreating IR message");

  byte dataBit;
  if      (messageIR[3] == 3)   receivedIRmessage.type = 'T';
  else if (messageIR[3] == 6)   receivedIRmessage.type = 'B';
  else                          receivedIRmessage.type = 'e';

  for (int i = 5; i<=11; i=i+2)
  {
    if      (receivedIRmessage.byteMsb == 1) dataBit = 0;
    else if (receivedIRmessage.byteMsb == 2) dataBit = 1;
    fred receivedIRmessage.byteMsb = dataBit << 1;
    receivedIRmessage.byteMsb = receivedIRmessage.byteMsb + messageIR [i];
    
    if (deBug)
    {
      //Serial.print ("\t");
      //Serial.print (messageIR[i]);
      //Serial.print ("-");
      //Serial.print (receivedIRmessage.byteMsb);
    }
    
  }
  for (int i = 13; i<=17; i=i+2)
  {
    receivedIRmessage.byteLsb = receivedIRmessage.byteLsb<< 1;
    receivedIRmessage.byteLsb = receivedIRmessage.byteLsb + messageIR [i];
    if (deBug)
    {
      //Serial.print ("\t");
      //Serial.print (messageIR[i]);
      //Serial.print ("-");
      //Serial.print (receivedIRmessage.byteLsb);
    }
  }
//countISR = 0;                         TODO : TEMPORARY TO LOOK FOR OVERLENGTH PACKETS !!!!
  if (deBug) PrintIR();
}

//////////////////////////////////////////////////////////////////////

void PrintIR()
{
  bool extendedPrintIR = FALSE;
  
  disableInterrupt(IR_RECEIVE_PIN);
  countISR = 0;
  
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
      Serial.print(messageISRelapsed [i]);
      Serial.print(F("\t"));
    }
    Serial.print(F("\nPrinting ISR cpu times:\t\t"));  
    for (int i = 1; i<=ARRAY_LENGTH; i++)
    {
      Serial.print(messageISRdelay [i]);
      Serial.print(F("\t"));
    }
  }

  Serial.print("\nIRMessage: ");
  Serial.print(receivedIRmessage.type);
  Serial.print(", ");
  Serial.print(receivedIRmessage.byteMsb, BIN);
  Serial.print(", ");
  Serial.print(receivedIRmessage.byteLsb, BIN);
  Serial.println();
  
  ClearIRarray();
  enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE);
}

////////////////////////////////////////////////////////////////////

void ClearIRarray()
{
  for (int i = 0; i<=ARRAY_LENGTH; i++)
  {
    //messageIR[i] = 42;
    //messageISRpulse[i] = 42
    //messageISRdelay[i] = -42;
    
  }
  receivedIRmessage.type = 'n';
  receivedIRmessage.byteMsb = 0;
  receivedIRmessage.byteLsb = 0;
}

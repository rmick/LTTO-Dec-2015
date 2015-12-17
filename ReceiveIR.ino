uint16_t pinChangeTime;
uint16_t pulseLength;
byte countISR = 0;

void ISRchange()
{
  static uint16_t lastEdge;
  pinChangeTime = micros();                   // Store the current time that the pin changes
  pulseLength = pinChangeTime - lastEdge;     // Subtract the current time from the last time
  lastEdge = pinChangeTime;                   // Reset the timer
  //newIRpulse = TRUE;                          // Set flag to true so that GetIR() will be triggered from Void Loop
  GetIR();
  countISR++;
}

//////////////////////////////////////////////////////////////////////

byte messageIndex;
uint16_t delayTime;
bool pinState;

void GetIR()
{
  if (countISR > 99)
  {
    Serial.println(F("!!!!! Too slow Farmer Joe !!!!!"));
    ClearIRarray();
  }
  
  delayTime = micros() - pinChangeTime;
  //bool pinState = digitalRead(IR_RECEIVE_PIN);
  pinState = PINB & 8;
  
  char highLow;
  if (pinState == 1) highLow = 'm';  
  if (pinState == 0) highLow = 's'; 
  
  messageIR       [messageIndex] = (pulseLength+500)/1000;
  messageIRpin    [messageIndex] = highLow;
  messageIRdelay  [messageIndex] = delayTime;
  
  messageIndex++;
  newIRpulse = FALSE;
  countISR = 0;
  //if (pulseLength > 25000 || index > 35) DecodeIR();
  if (messageIndex > 45) DecodeIR();
}

void DecodeIR()
{
  Serial.print(F("Incoming IR message: "));       // The [0] item is the llong break, so start reading at 1!
  for (int i = 1; i<=50; i++)
  {
    Serial.print(messageIR [i]);
    Serial.print(F("-"));
    Serial.print(messageIRpin [i]);
    Serial.print(F(", "));
    //Serial.print(messageIRdelay [i]);
    //Serial.print(F(") ,"));
  }
  ClearIRarray();
}

void ClearIRarray()
{ 
  newIRmessage = FALSE;
  messageIndex = 0;
  Serial.println();
  for (int j = 0; j<=40; j++)
  {
    messageIR[j] = 0;
    messageIRpin[j] = ' ';
    messageIRdelay[j] = 0;
  }
}



/*
uint16_t pinChangeTime;
uint16_t pulseLength;
byte countISR = 0;

void ISRchange()
{
  static uint16_t lastEdge;
  pinChangeTime = micros();                   // Store the current time that the pin changes
  pulseLength = pinChangeTime - lastEdge;     // Subtract the current time from the last time
  lastEdge = pinChangeTime;                   // Reset the timer
  //newIRpulse = TRUE;                        // Set flag to true so that GetIR() will be triggered from Void Loop
  countISR++;                                 // Counter that should never get to 2, If it does we have dropped a Bit.
  GetIR();
  
}

//////////////////////////////////////////////////////////////////////

byte messageIndex;
uint16_t delayTime;
//bool pinState;

void GetIR()
{
  if (countISR > 1)
  {
    Serial.println(F("!!!!! Too slow Farmer Joe !!!!!"));
    ClearIRarray();
  }
  
  bool pinState = PINB & 8;               // faster method than " pinState = digitalRead(IR_RECEIVE_PIN); "
  delayTime = micros() - pinChangeTime;
  
  char highLow;
  if (pinState == 1) highLow = 'H';  
  if (pinState == 0) highLow = 'L'; 
  
  messageIR       [messageIndex] = (pulseLength+500)/1000;
  messageIRpin    [messageIndex] = highLow;
  messageIRdelay  [messageIndex] = delayTime;
  
  messageIndex++;
  newIRpulse = FALSE;
  countISR = 0;
  if (pulseLength >12000) DecodeIR(); //messageIndex = 0;
  //if (messageIndex = 17) DecodeIR();
}

void DecodeIR()
{
  Serial.print(F("Incoming IR message: "));       // The [0] item is the llong break, so start reading at 1!
  for (int i = 0; i<=30; i++)
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

////////////////////////////////////////////////////////////////

void ClearIRarray()
{ 
  //newIRmessage = FALSE;
  messageIndex = 0;
  Serial.println();
  for (int j = 0; j<=30; j++)
  {
    messageIR[j] = 0;
    messageIRpin[j] = ' ';
    messageIRdelay[j] = 0;
  }
}
*/


uint16_t pulseLength;
uint16_t lastEdge;

uint16_t pinChangeTime;
byte countISR = 0;

void ISRchange()
{
  pinChangeTime = micros();              // Store the time that the pin changes
  
  pulseLength = pinChangeTime - lastEdge;
  lastEdge = pinChangeTime;
  newIRpulse = TRUE;                          // Set flag to true so that GetIR() will be triggered from Void Loop
  countISR++;
}

//////////////////////////////////////////////////////////////////////

byte index;
uint16_t bitTime =0;

/*
void GetIR()
{
  messageIR[countISR] = pulseLength
}
*/


void GetIR()
{
  bool pinState = digitalRead(IR_RECEIVE_PIN);

  if (countISR > 1) Serial.println(F("!!!!! Too slow Farmer Joe !!!!!"));
  
  char highLow;
  if (pinState == 0) highLow = 'H';  
  if (pinState == 1) highLow = 'L'; 
  
  uint16_t newBit;
  newBit = pinChangeTime - bitTime;
  bitTime = pinChangeTime;
  uint16_t delayTime = micros() - pinChangeTime;
  messageIR    [index] = (newBit+500)/1000;
  messageIRpin [index] = highLow;
  messageIRdelay [index] = delayTime;
  index++;
  newIRpulse = FALSE;
  countISR = 0;
  //if (newBit > 25000 || index > 35) DecodeIR();
  if (index > 17) DecodeIR();
}

void DecodeIR()
{
  Serial.print(F("Incoming IR message: "));
  for (int i = 1; i<=40; i++)
  {
    Serial.print(messageIR [i]);
    Serial.print(F(""));
    Serial.print(messageIRpin [i]);
    Serial.print(F(", ("));
    Serial.print(messageIRdelay [i]);
    Serial.print(F(") ,"));
  }
  newIRmessage = FALSE;
  index = 0;
  Serial.println();
  for (int j = 0; j<=40; j++)
  {
    messageIR[j] = 0;
    messageIRpin[j] = ' ';
    messageIRdelay[j] = 0;
  }
}





uint16_t pulseLength;
uint16_t lastEdge;

uint16_t pinChangeTime;
byte countISR = 0;

void ISRchange()
{
  pinChangeTime = micros();              // Store the time that the pin changes
  pulseLength = pinChangeTime - lastEdge;
  lastEdge = pinChangeTime;
  countISR++;
  GetIRx();
}

//////////////////////////////////////////////////////////////////////

void GetIRx()
{
  messageIR[countISR] = (pulseLength+500)/1000;
  char pinState;
  if (PINB & 8) pinState = 'h'; else pinState = 'l';
  messageIRpin [countISR] = pinState;
  if (countISR > 17) DecodeIR();
  //else if (pulseLength > 12000) countISR = 0;
}

///////////////////////////////////////////////////////////////////////

void DecodeIR()
{
  Serial.print(F("Incoming IR message: "));
  for (int i = 2; i<=40; i++)
  {
    Serial.print(messageIR [i]);
    Serial.print(F(""));
    Serial.print(messageIRpin [i]);
    Serial.print(F(","));
  }
  Serial.println();
  for (int j = 0; j<=40; j++)
  {
    messageIR[j] = 0;
    messageIRpin[j] = ' ';
    messageIRdelay[j] = 0;
  }
  countISR = 0;
}


/*
//////////////////  OLD GET_IR()  ////////////////////////////

byte index;
uint16_t bitTime =0;

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
  if (newBit > 120000 || index > 17) DecodeIR();        // GOT IT - How to deal with rolloever of newBit length !!!
  //if (index > 17) DecodeIR();
}
*/


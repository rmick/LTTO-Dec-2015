


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
  GetIR();
}

//////////////////////////////////////////////////////////////////////

void GetIR()
{
  int8_t bitLength = (pulseLength+500)/1000;
  if (PINB & 8); else bitLength = 0 - bitLength;    //Set a Mark as Positive and a Break as Negative.
  messageIR[countISR] = bitLength;
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
    Serial.print(F(", "));
    messageIR[i] = 0;
  }
  Serial.println();
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


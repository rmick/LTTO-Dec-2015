uint16_t pinChangeTime;

void ISRchange()
{
  pinChangeTime = micros();              // Store the time that the pin changes
  newIRpulse = TRUE;                          // Set flag to true so that GetIR() will be triggered from Void Loop
}

//////////////////////////////////////////////////////////////////////

byte index;
uint16_t bitTime =0;

void GetIR()
{
  bool pinState = digitalRead(IR_RECEIVE_PIN);
  
  char highLow;
  if (pinState == 0) highLow = 'H';  
  if (pinState == 1) highLow = 'L'; 
  
  uint16_t newBit;
  if (bitTime == 0) bitTime = pinChangeTime;
  newBit = bitTime - pinChangeTime;
  bitTime = pinChangeTime;
  uint16_t delayTime = micros() - pinChangeTime;
  messageIR    [index] = newBit;
  messageIRpin [index] = highLow;
  messageIRdelay [index] = delayTime;
  index++;
  newIRpulse = FALSE;
  if (newBit > 25000 || index > 35) DecodeIR();
}

void DecodeIR()
{
  Serial.print(F("Incoming IR message: "));
  for (int i = 0; i<=40; i++)
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


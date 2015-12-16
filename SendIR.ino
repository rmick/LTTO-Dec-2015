// void SendIR(char type, unsigned int message)
// void PulseIR(int mSec)
// void DeBug(String data)
// void print_binary(int v, int num_places)


//////////////////////////////////////////////////////////////////////

void SendIR(char type, unsigned int message)
{
  disableInterrupt(IR_RECEIVE_PIN);

  irTime = micros();          ////////// for DeBug purposes only.
  int msgLength = 0;
  int interDelay = 25;

  if (deBug) Serial.print(F("Sending IR- "));
  if (deBug) Serial.print(type);
  if (deBug) Serial.print(F(":"));
  if (deBug) print_binary(message, 8);
  if (deBug) Serial.println();
  
  //Send Header
  switch (type)
  {
    // if Type = B then Beacon,   so header is 366
    // if Type = T then Tag,      so header is 363
    // if Type = D the data byte, so header is Null
    case 'D':
      msgLength = 9;
      interDelay = 25;
      // No header so go straight to message transmission
      break;
    case 'T':
      msgLength = 7;
      interDelay = 1000;
      PulseIR(3);
       DeBug(F("3mS mark  "));       ///////////
      delayMicroseconds (6000);
       DeBug(F(" 6mS break"));       ///////////
      PulseIR(3);
       DeBug(F("3mS mark  "));       ///////////
      break;
    case 'B':
      msgLength = 5;
       interDelay = 25;
      PulseIR(3);
       DeBug(F("3mS mark  "));       ///////////
      delayMicroseconds (6000);
       DeBug(F(" 6mS break"));       ///////////
      PulseIR(6);
       DeBug(F("6mS mark  "));       ///////////
      break;
  }

  //Send message
  for (int bitCount=msgLength-1; bitCount >=0; bitCount--)
     {
      delayMicroseconds (2000);
        DeBug(F(" 2mS break"));      //////////
      PulseIR(bitRead(message, bitCount)+1);
        DeBug(String(bitRead(message, bitCount))+F(" bit     "));      //////////
     }

  delay(interDelay);                                 
  if (deBug) Serial.println(F("IR Sent! "));

  enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE);
}

///////////////////////////////////////////////////////////////

void PulseIR(int mSec)
{
  unsigned long pulseStartTime = micros();
  unsigned long pulseLength = mSec*1000;
  unsigned long pulseEndTime = pulseStartTime + pulseLength - 24;
  while (pulseEndTime >micros() )
  {
    digitalWrite(IR_LED, HIGH);
    delayMicroseconds(12);
    digitalWrite(IR_LED, LOW);
    delayMicroseconds(12);
  }
}

/////////////////////////////////////////////////////////////

void DeBug(String data)
{
  return;                             //Comment out to Enable DeBug messages
  irTime = micros() - irTime;         // N.B. When DeBug runs it stops SendIR from talking to guns !!!!
  Serial.print(data + " - ");
  Serial.println(irTime);
  irTime = micros();
}

////////////////////////////////////////////////////////////

void print_binary(int v, int num_places)
{
  int mask=0, n;
  for (n=1; n<=num_places; n++)
  {
    mask = (mask << 1) | 0x0001;
  }
  v = v & mask;  // truncate v to specified number of places
  
  while(num_places)
  {
    if (v & (0x0001 << num_places-1))
    {
      Serial.print("1");
    }
    else
    {
      Serial.print("0");
    }
    
    --num_places;
    if(((num_places%4) == 0) && (num_places != 0))
    {
      Serial.print("_");
    }
  }
}


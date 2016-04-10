// void SendIR(char type, unsigned int message)
// void PulseIR(int mSec)
// void DeBug(String data)
// void print_binary(int v, int num_places)


//////////////////////////////////////////////////////////////////////

void SendIR(char type, uint16_t message)
{
  //disableInterrupt(IR_RECEIVE_PIN);

  irTime = micros();          ////////// for DeBug purposes only.
  int msgLength = 0;
  int interDelay = 25;

  #ifdef DEBUG
    Serial.print(F("\nSending IR- "));
    Serial.print(type);
    Serial.print(F(": "));
    PrintBinary(message, 10);
    Serial.println();
  #endif
  
  //Send Header
  switch (type)
  {
    // if Type = B then Beacon,   so header is 366
    // if Type = T then Tag,      so header is 363
    // if Type = D the data byte, so no header
    // if Type = P then Packet, no header and need to add 0 as the 9th bit
    // if Type = C then CheckSum, no header and need to add a 1 as the 9th bit
    case 'P':
      msgLength = 9;
      interDelay = 25;
      PulseIR(3);
      delayMicroseconds (6000);
      PulseIR(3);
      //Serial.print(F("\nPckt: ")); Serial.print(message); Serial.print(F("\t")); Serial.print(message,HEX); Serial.print(F("\t"));
      break;

    case 'D':
      msgLength = 8;
      interDelay = 25;
      PulseIR(3);
      delayMicroseconds (6000);
      PulseIR(3);
      //Serial.print(F("\nData: ")); Serial.print(message); Serial.print(F("\t")); Serial.print(message,HEX); Serial.print(F("\t"));
      break;

    case 'C':
      msgLength = 9;
      interDelay = 25;
      PulseIR(3);
      delayMicroseconds (6000);
      PulseIR(3);
      message = message | 256;
      //Serial.print(F("\nChk:  ")); Serial.print(message-256); Serial.print(F("\t")); Serial.print(message,HEX); Serial.print(F("\t"));
      break;
         
    case 'T':
      msgLength = 7;
      interDelay = 5;
      PulseIR(3);
      delayMicroseconds (6000);
      PulseIR(3);
      break;
    
    case 'B':
      msgLength = 5;
       interDelay = 25;
      PulseIR(3);
      delayMicroseconds (6000);
      PulseIR(6);
      break;
  }

  //Send message
  for (int bitCount=msgLength-1; bitCount >=0; bitCount--)
     {
      delayMicroseconds (2000);
      PulseIR(bitRead(message, bitCount)+1);        // the +1 is to convert 0/1 data into 1/2mS pulses.
     }

  delay(interDelay);                                 
  
  #ifdef DEBUG
    Serial.print(F("\nIR Sent! "));
  #endif

  //enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE);
}

///////////////////////////////////////////////////////////////

void PulseIR(int mSec)
{
  unsigned long pulseStartTime = micros();
  unsigned long pulseLength = mSec*1000;
  unsigned long pulseEndTime = pulseStartTime + pulseLength - 24;
  
  if (IR_LED != 11)
  {
    while (pulseEndTime >micros() )
    {
      digitalWrite(IR_LED, HIGH);
      delayMicroseconds(12);
      digitalWrite(IR_LED, LOW);
      delayMicroseconds(12);
    }
  }
  else
  {
    /*
    TCCR2A = 0x00;
    bitWrite(TCCR2A, COM2A0, 1);
    bitWrite(TCCR2A, COM2B1, 1);
    bitWrite(TCCR2A, WGM21, 1);
    bitWrite(TCCR2A, WGM20, 1);
  
    TCCR2B = 0x00;
    bitWrite(TCCR2B, WGM22, 1);
    bitWrite(TCCR2B, CS20, 1);  
  
    OCR2A = 209;
    OCR2B = 128;  //Doesn't matter
    */
    
    pinMode(IR_LED, OUTPUT);
    delayMicroseconds(pulseLength);
    pinMode(IR_LED, INPUT);
    
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

void PrintBinary(int v, int num_places)
{
//#ifdef DEBUG
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
//  #endif
}


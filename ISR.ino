//////////////////////////////////////////////////////////////////////////////////////////////////////////////

const byte  ARRAY_LENGTH = 28;
int8_t      messageIR [ARRAY_LENGTH];
static int16_t receiveMilliTimer = 32767;
const byte IR_TIME_OUT = 9;          //  This is the length of break to look for, to indicate end of packet.
                                      //  Longest packet break is 6mS. The Timer0 interrupt could trigger 1uS - 999uS after
                                      //  the ISR therefore need to allow a buffer of 1mS - which is a total of 7mSec.
                                      //  Then add a small fudge factor of ~30%

void ISRchange()
{
  #ifdef DEBUG
    digitalWrite(DE_BUG_TIMING_PIN, HIGH);
  #endif
  
  ///---------------------------------------------------------------------------------------------------------
  //  Set up the variables
    int16_t pulseLength;
    static uint16_t lastEdge;
    uint16_t pinChangeTime;
        
  ////---------------------------------------------------------------------------------------------------------
  //  Action the Interrupt
    pinChangeTime = micros();                   // Store the time that the pin changes
    pulseLength = pinChangeTime - lastEdge;     // Measure the elapsed time since last lastEdge
  
  ////---------------------------------------------------------------------------------------------------------
  //  Look for short pulses (noise) and discard
    if (pulseLength < 500 && pulseLength > 0)
    {
        shortPulseLengthError++;
        #ifdef DEBUG
          digitalWrite(DE_BUG_TIMING_PIN, LOW);
        #endif
        return;              // exit as the pulse is too short, so probably noise
    }

  ////---------------------------------------------------------------------------------------------------------
  // Pulse length is good. Continue actioning the Interrupt
    receiveMilliTimer = IR_TIME_OUT;                     // Set the timer for a XXmS break - the expiry of which indicates an end of packet.
    lastEdge = pinChangeTime;                            // Reset the lastEdge to now
    int8_t bitLength = (pulseLength+500)/1000;           // Round up and divide by 1,000 to give mS
    int pinState = digitalRead(IR_RECEIVE_PIN);          // Check state of pin (High/Low) for mark or space.
    if (pinState == LOW) bitLength = 0 - bitLength;      // Set a Mark as Positive and a Break as Negative
  
  ////---------------------------------------------------------------------------------------------------------
  //  Store the data to the message array
    if (countISR < ARRAY_LENGTH)
    {
      if      (countISR  < 4)                   messageIR[countISR] = bitLength;         // Store the raw +3/-6/+X data to the first bits to keep it obvious
      else if (countISR >= 4 && bitLength > 0)  messageIR[countISR] = bitLength - 1;     // Store + data bits as 0/1 instead of the raw +1/+2 to make it simple
      else                                      messageIR[countISR] = bitLength;         // Leave the - spaces as raw 2mS
    }
    else
    {
      arrayOverLengthError++;
      #ifdef DEBUG
        digitalWrite(DE_BUG_TIMING_PIN, LOW);
      #endif
      countISR = 0;
      return;
    }
  ////---------------------------------------------------------------------------------------------------------
  //  increment the ISR counter pack your bags.
    countISR++;
    #ifdef DEBUG
      digitalWrite(DE_BUG_TIMING_PIN, LOW);
    #endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

SIGNAL(TIMER0_COMPA_vect)
{
  
  ////---------------------------------------------------------------------------------------------------------
  //  Action the 1mS timer. Look for End of Packet >6mS Break. 
    receiveMilliTimer--;
  
    if (receiveMilliTimer == 0)
    {
      #ifdef DEBUG
        digitalWrite(DE_BUG_TIMING_PIN, HIGH);
      #endif
      if (( decodedIRmessage.newMessage = FALSE) ) decodedIRmessage.messageOverwritten++;
      
      receiveMilliTimer = 32767;
      irPacketLength = countISR;
      countISR = 0;
      CreateIRmessage();
      #ifdef DEBUG
        digitalWrite(DE_BUG_TIMING_PIN, LOW);
      #endif
    }

  ////---------------------------------------------------------------------------------------------------------
  //  Prevent rollover into the 0 trigger value
    if (receiveMilliTimer == 100) receiveMilliTimer = 32767;      // Prevents rollover into the 25mS zone
}


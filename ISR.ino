
int16_t receiveMilliTimer = 32768;

SIGNAL(TIMER0_COMPA_vect)
{
  
  receiveMilliTimer--;

  if (receiveMilliTimer == 0)
  {
    rxTimerExpired = TRUE;
    receiveMilliTimer = 32767;
  }

  if (receiveMilliTimer == 100) receiveMilliTimer = 32767;      // Prevents rollover into the 25mS zone
  
  if (receiveMilliTimer < 0)
  {
    receiveMilliTimer = 32768;
    Serial.print(F("\nTimer0 RX Rollover Error - ISR:14"));
  }
}


//////////////////////TIMER 1 INTERUPT///////////////////////////

/*
  // initiliase timer1
  noInterrupts();
  TCCR1A = 0;               // clear the TCCR1A register (known starting point)
  TCCR1B = 0;               // clear the TCCR1A register (known starting point)

  // set Timer1counter to the correct value for the interrupt interval
  //timer1counter = 64911; //preload timer 65536-16MHz/256/100Hz
  //timer1counter = 64286; //preload timer 65536-16MHz/256/50Hz
  //timer1counter = 34286; //preload timer 65536-16MHz/256/2Hz
  timer1counter = 1; 

  TCNT1 = timer1counter;                              // preload timer
  TCCR1B |= (1 << CS12);                              // 256 prescaler
  //TCCR1B |= (1 << CS12); TCCR1B |= (1 << CS10);     // 1024 prescaler
  TIMSK1 |= (1 << TOIE1);     // enable timer overflow interupt     
  interrupts();

}


ISR(TIMER1_OVF_vect)          // interrrupt service routine
  {
    // if (deBug) Serial.println(" Tick");
    //digitalWrite(13, !digitalRead(13));   // Interferes with the IR LED on the same Pin!
    TCNT1 = timer1counter;    //re-preload the timer
  }
*/


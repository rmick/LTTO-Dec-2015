void setup()
{
  Serial.begin(250000);
  pinMode (IR_LED, OUTPUT);
  pinMode (IR_RECEIVE_PIN, INPUT_PULLUP);
  enableInterrupt (IR_RECEIVE_PIN, ISRchange, CHANGE);
  Serial.println(F("\nHere we go boys...."));

  #ifdef DEBUG
    pinMode (DE_BUG_TIMING_PIN, OUTPUT);         // This is my DeBug timing pin, fed to the Logic Analyser
    digitalWrite(DE_BUG_TIMING_PIN, LOW);
  #endif
  
  ////---------------------------------------------------------------------------------------------------------
  //  Set Timer0 interrupt
  
  // Timer0 is used for millis(), so this routine piggybacks on that by using a mid-time interupt.
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  
  
  ////---------------------------------------------------------------------------------------------------------
  //  Set up the LCD screen
  
  tft.reset();
  uint16_t identifier = tft.readID();
  Serial.print(F("LCD driver : "));
  Serial.println(identifier);
  if (identifier == 0x0) identifier = 0x7789;
  tft.begin(identifier);

  if (identifier == 0x7789) tft.setRotation(2);
  else tft.setRotation(0);

  tft.fillScreen(BLACK);            // It fails first time, so do it here before we start the program
  


  ////---------------------------------------------------------------------------------------------------------
  //  Initialises the EEPROM on first upload/run.
  
  if (maxReloads == 255)   { maxReloads =    0;  EEPROM.write(eeMAX_RELOADS, 0);  }
  if (medicDelay == 255)   { medicDelay =   10;  EEPROM.write(eeMEDIC_DELAY, 10); }
  if (medicCount == 255)   { medicCount =    0;  EEPROM.write(eeMEDIC_COUNT,  0); }
  if (shieldsTimer == 255) { shieldsTimer = 30;  EEPROM.write(eeSHIELDS_TIMER, 15); }  
  if (reloadAmount > 100)  { reloadAmount = 15;  EEPROM.write(eeRELOAD_AMOUNT, 15); tagCount = reloadAmount; }
  if (teamID == 255)       { teamID = 0;         EEPROM.write(eeTEAM_ID, 0); }
  if (playerID == 255)     { playerID = 1;       EEPROM.write(eePLAYER_ID, 1); }
  if (pinCode[0] == 255)   { pinCode[0] = 1;     EEPROM.write(eePIN_CODE,   1);
                             pinCode[1] = 2,     EEPROM.write(eePIN_CODE+1, 2);
                             pinCode[2] = 3;     EEPROM.write(eePIN_CODE+2, 3);
                             pinCode[3] = 4;     EEPROM.write(eePIN_CODE+3, 4);  }



  ////////////////////// TIMER 2 38kHz Oscilator/////////////////////

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
    // OCR2B = 128;  //Doesn't matter
    */
/*
  ////////////////////// TIMER 2 INTERUPT ///////////////////////////
  // OC2A is Pin 11 on a Uno
  // OC2A is Pin 10 on a Mega
  TCCR2A = _BV (COM2A0) | _BV(WGM21);  // CTC, toggle OC2A on Compare Match
  TCCR2B = _BV (CS20);   // No prescaler
  OCR2A =  209;          // compare A register value (210 * clock speed)
                        //  = 13.125 nS , so frequency is 1 / (2 * 13.125) = 38095

*/

}

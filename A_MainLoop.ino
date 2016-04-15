void loop()
{ 
  ////////////////////////
  if      (state == MEDIC)            MedicMode();
  else if (state == PINPAD)           PinPadMode();
  else if (state == TAGGER)           TaggerMode();
  else if (state == CONFIG1)          ConfigScreen1();
  else if (state == SET_TEAM)         SetTeam();
  else if (state == SET_HOSTILE)      SetHostile();
  else if (state == CLEAR_SCORE)      ClearScore();
  else if (state == SET_MEDIC_DELAY)  SetMedicDelay();
  else if (state == CONFIG2)          ConfigScreen2();
  else if (state == CHANGE_PIN)       ChangePin();
  else if (state == CONFIRM_PIN)      ConfirmPin();
  else if (state == GAME_OVER)        GameOver();
  else if (state == SCORES)           DisplayScores();
  else if (state == HOST)             HostMode();
  //////////////////////////////////

  if (state == HOST)
  {
    if (rxTimer0 <=0) CreateIRmessage();
  }
  
  if (receivedIRmessage.type != '_')
  {
    //if (state != TAGGER) 
    if      (state == TAGGER)   DecodeIR();
    else if (state == HOST)     DecodeIR();
    else                        ClearIRarray();     // Clears IR data when not in Tagger/Host Mode.
  }

  // Read the Serial port for a keypress              This is testing stuff and can be deleted later
  if (Serial.available() !=0)
    {
      char keyIn = Serial.read();
      switch (keyIn)
        {
        case 'h':
          state = HOST;
          break;
        case 't':
          state = TAGGER;
          break;
        case 'n':
          AnnounceCustomGame();
          break;
        case 'b':
          Serial.println();
          break;
        }
    }
}


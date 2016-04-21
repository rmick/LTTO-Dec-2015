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

  if (rxTimerExpired)
    {
      rxTimerExpired = FALSE;
      if      (state == HOST)
      {
        CreateIRmessage('H');
      }
      else if (state == TAGGER)
      {
        CreateIRmessage('T');
      }
      rxTimerExpired = FALSE;
    }


/*
  if (state == HOST)
  {
    if (rxTimerExpired)
    {
      rxTimerExpired = FALSE;
      CreateIRmessage('H');
      if (rxTimerExpired) DecodeDataIR();
    }
  }
*/

  
  if (receivedIRmessage.type != '_')
  {
    if      (state == TAGGER)   DecodeTagIR();
    else if (state == HOST)     DecodeDataIR();
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
        case 'b':
          Serial.println();
          Serial.println(F("_____________________________"));
          Serial.println();
          break;
        case 'd' :
          DecodeTagIR();
          break;
        case 'j' :
          RequestJoinGame();
          break;
        case 'x':
          FireLaser();
          break;
        }
    }
}


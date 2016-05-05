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

  if (newIRmessageReady)
  {
      if      (receivedIRmessage.type == 'T')  DecodeTagIR();
      else if (receivedIRmessage.type == 'B')  DecodeTagIR();
      else if (receivedIRmessage.type == 'P')  DecodeDataIR();
      else if (receivedIRmessage.type == 'D')  DecodeDataIR();
      else if (receivedIRmessage.type == 'C')  DecodeDataIR();
      else
      {
        Serial.print(F("\nMainLoop:28 - invalid message.type = "));
        Serial.print(receivedIRmessage.type);
      }
      newIRmessageReady = FALSE;
  }

////////////////////////////////// DeBug access /////////////////////////////////////////

  if (Serial.available() !=0)
    {
      char keyIn = Serial.read();
      switch (keyIn)
        {
        case 'h':
          state = HOST;
          break;
        case 'a' :
          AnnounceCustomGame();
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
        case 'p' :
          PrintIR();
          break;
        }
    }
}


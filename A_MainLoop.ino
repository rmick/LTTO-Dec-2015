void loop()
{ 
  uint32_t entryTime = micros();
  
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

  
   ////---------------------------------------------------------------------------------------------------------
  //  This is where the rubber hits the road. Decode the messages.

 uint8_t         taggerID = 0;
    
    if (DecodeIR() )
    {  
        switch (decodedIRmessage.type)
        {
            case 'T':
//              ProcessRxTag(decodedIRmessage.TeamID, decodedIRmessage.PlayerID, decodedIRmessage.ShotStrength);
              Serial.print(F("\n\Tag - "));
              Serial.print(decodedIRmessage.rawDataPacket, BIN);
              break;
            
            case 'B':
              BeaconFlash(true);
              break;

            case 'P':
              //Serial.print(F("\n\t\t------------------\n\t\tPacket - 0x"));
              //Serial.print(decodedIRmessage.PacketByte, HEX);
              ProcessRxPacket();
              break;

            case 'D':
              //Serial.print(F("\n\t\t\tData - 0x"));
              //Serial.print(decodedIRmessage.DataByte, HEX);
              ProcessRxDataByte();
              break;

            case 'C':
              //Serial.print(F("\n\t\tCheckSum - 0x"));
              //Serial.print(decodedIRmessage.rawDataPacket & B11111111, HEX); 
              ProcessRxCheckSum();
              break;   
        }
        decodedIRmessage.newMessage = false;
 
    }   
    BeaconFlash(false);


////////////////////////////////// DeBug Terminal access /////////////////////////////////////////

  if (Serial.available() !=0)
    {
      char keyIn = Serial.read();
      switch (keyIn)
        {   
        case 'b':
          Serial.println();
          Serial.println(F("_____________________________"));
          Serial.println();
          break;
        case 'd' :
          DecodeIR();
          break;
        case 'j' :
          RequestJoinGame();
          break;
        case 's' :
          SendText();
          break;  
        case 'p' :
          PrintIR('X');
          break;
        case 'x' :
          Serial.print(F("\nTag Sent :-)"));
          SendIR('T',0);
          break;
        case 't' :
          state = TAGGER;
          break;
        case 'h' :
          state = HOST;
          break;
        case 'g' :
          debugStartHost = true;
          assignToTeam = 1;
          assignToPlayer = 1;
          break;
        case 'c' :
          debugStartHost = false;
          StartCountDown();
          break;  
        case 'm' :
          state = MEDIC;
          break;
          
          
        #ifdef DEBUG
        case 'q' :
          shortPulseLengthError = 0;
          arrayOverLengthError = 0;
          badMessage_CountISRshortPacket = 0;
          badMessage_non3_6Header = 0;
          badMessage_InvalidPacketType = 0;
          break;
        case 'z' :
          Serial.print(F("\n----------------"));
          Serial.print(F("\nShortPulse: "));
          Serial.print(shortPulseLengthError);
          Serial.print(F("\tArrayOverRun: "));
          Serial.print(arrayOverLengthError);
          Serial.print(F("\nShortPacketLength: "));
          Serial.print(badMessage_CountISRshortPacket);
          Serial.print(F("\tInvalidPacketType: "));
          Serial.print(badMessage_InvalidPacketType);
          Serial.print(F("\tBadMessage_non3_6Header: "));
          Serial.print(badMessage_non3_6Header);
          Serial.print(F("\n----------------"));
          break;
        #endif 
        }
    }
    //Serial.print(F("\nLoopTime = "));
    //1\Serial.print( micros() - entryTime );
}


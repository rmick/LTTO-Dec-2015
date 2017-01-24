#include <Arduino.h>

void loop()
{
	////---------------------------------------------------------------------------------------------------------
	//  State Machine
	//#ifdef DEBUG
		static byte ActivityLED = 0;
		ActivityLED++;
		if (ActivityLED == 127)	digitalWrite(13, LOW);
		if (ActivityLED == 255)	digitalWrite(13, HIGH);
	//#endif

	switch (state)
	{
		case MEDIC:
			MedicMode();
			GetData();			//Calls ltto.available for Beacon display/debug code
			break;
		case PINPAD:
			PinPadMode();
			break;
		case TAGGER:
			TaggerMode();
			break;
		case CONFIG1:
			ConfigScreen1();
			break;
		case SET_TEAM:
			SetTeam();
			break;
		case SET_HOSTILE:
			SetHostile();
			break;
		case CLEAR_SCORE:
			ClearScore();
			break;
		case SET_MEDIC_DELAY:
			SetMedicDelay();
			break;
		case CONFIG2:
			ConfigScreen2();
			break;
		case CHANGE_PIN:
			ChangePin();
			break;
		case CONFIRM_PIN:
			ConfirmPin();
			break;
		case GAME_OVER:
			GameOver();
			break;
		case SCORES:
			DisplayScores();
			break;
		case HOST:
			HostMode();
			break;
		case ANNOUNCE_GAME:
			AnnounceGame();
			break;
		case DEFAULT:
			MedicMode();
			break;
	}

  ////---------------------------------------------------------------------------------------------------------
  //  General Code

    BeaconFlash(false);			//Turns off beacon after TimeOut


////////////////////////////////// DeBug Terminal access /////////////////////////////////////////

#ifdef TERMINAL_INTERFACE
    if (Serial.available() !=0)
    {
        char keyIn = Serial.read();
        switch (keyIn)
        {
            case 'b' :
                Serial.println();
                Serial.println(F("_____________________________"));
                Serial.println();
                break;
            case 'p' :
                ltto.printIR('X');
                break;
            case 'x' :
                Serial.print(F("\nTag Sent :-)"));
                ltto.sendIR('T',0);
              break;
            case 't' :
                state = TAGGER;
                break;
			case 'h' :
                state = HOST;
                break;
			case 'm' :
                state = MEDIC;
                break;
			case 's' :
				Serial.print(state);
				break;
			case 'o' :
				digitalWrite(13, LOW);
				break;
			case 'a' :
				SetisAdvertiseGameDisabled(true);
				break;
    
            //#ifdef LOCALDEBUG
                case 'q' :
					//TODO Create this class			 -   ltto.clearErrors();
					/*shortPulseLengthError = 0;
                    arrayOverLengthError = 0;
                    badMessage_CountISRshortPacket = 0;
                    badMessage_non3_6Header = 0;
                    badMessage_InvalidPacketType = 0;*/
                    break;
                case 'c' :
					ltto.readErrors();
					/*Serial.print(F("\n----------------"));
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
                    Serial.print(F("\n----------------"));*/
                    break;
           // #endif
        }
    }
#endif
}

////---------------------------------------------------------------------------------------------------------
//  void GetData()

void GetData()
{
#ifdef TEST
	if (ltto.available())										// available() checks if there is a message to receive. (1 = true, 0 = false)
	{
		switch (ltto.readMessageType() )                        // readMessageType() returns the type of Message (TAG, BEACON, PACKET, DATA, CHECKSUM)
		{
		case TAG:
			Serial.print(F("\nTag - "));
			Serial.print(F("Team #: "));
			Serial.print(ltto.readTeamID());					// readTeamID() returns the teamID (0 = None, 1 = team1, 2 = team2, 3 = team3)
			Serial.print(F("\tPlayer #: "));
			Serial.print(ltto.readPlayerID());					// readPlayerID() returns the playerID (Player # 1 through 8)
			Serial.print(F("\tShotStrength: "));
			Serial.print(ltto.readShotStrength());				// readShotStrength() returns the shotStrength (value 1 through 4, where 1 is a normal tag - non mega)
			break;

		case BEACON:
			BeaconFlash(true);

			switch (ltto.readBeaconType())
			{
				case (IFF_BEACON):
					Serial.print(F("\n\tIFF Beacon - "));				// readBeaconType() returns the beacon type. IFF_BEACON is an 'Identify Friend or Foe' beacon, sent by all taggers twice a second.
					Serial.print(F("TeamID #: "));						// It includes a TeamID, in order to work out if the sending tagger is on the same team as the receiver.
					Serial.print(ltto.readTeamID());
					break;
				case (HIT_RESPONSE_BEACON):
					Serial.print(F("\n\t Hit Response Beacon - "));		// readBeaconType() returns the beacon type. HIT_RESPONSE_BEACON is sent once when a tagger is hit.
					Serial.print(F("TeamID #: "));						// It returns the TeamID and the strength of the tag received.
					Serial.print(ltto.readTeamID());
					Serial.print(F("\tTagStrength: "));
					Serial.print(ltto.readShotStrength());
					break;
				case (ZONE_BEACON):
					Serial.print(F("\n\t  Zone Beacon"));				 // GetBeaconType() returns the beacon type. ZONE_BEACON is not currently used in LTTO taggers.
					break;
				case (CONTESTED_ZONE):
					Serial.print(F("\n\t   Contested Zone"));			 // readBeaconType() returns the beacon type. CONTESTED_ZONE is used in some Zone based games
					break;
				case (SUPPLY_ZONE):
					Serial.print(F("\n\t    Supply Zone Beacon - "));
					if (ltto.readTeamID() == 0)                         // If teamID = 0 then it is a Neutral zone (works for any team)
					{
						Serial.print(F("Neutral"));
					}
					else
					{
						Serial.print(F("TeamID #: "));                  // If teamID is 1,2 or 3 - then the supply base only works for that team
						Serial.print(ltto.readTeamID());
					}
					break;
			}
			break;

		case PACKET:													// A Packet byte a Header that announces what follows. Used to Host Games and send special messages
			Serial.print(F("\n\t\t------------------"));
			Serial.print(F("\n\t\tPacket1 - 0x"));
			Serial.print(ltto.readPacketByte(), HEX);
			Serial.print(F(" - "));
			Serial.print(ltto.readPacketName());
			break;

		case DATA:														// A Data byte contains information related to the Packet type. There are usually multiple data bytes in a message
			Serial.print(F("\n\t\t\tData - 0x"));
			Serial.print(ltto.readDataByte(), HEX);
			break;

		case CHECKSUM:                                                          // The CheckSum byte is the 8bit sum of the Packet byte + all the data bytes
			Serial.print(F("\n\t\tCheckSum - "));
			if (ltto.readCheckSumOK() == true)  Serial.print(F("OK."));			// If readCheckSumOK = 1 (true), then checkSum is correct.
			else if (ltto.readCheckSumOK() == false) Serial.print(F("BAD !"));  // If readCheckSumOK = 0 (false), there is a data error, and the entire message should be discarded
			Serial.print(F("\n\t\t------------------\n"));
			break;

		case BAD_MESSAGE:
			Serial.print(F("\n\t\t\tBad IR data - "));
			Serial.print(ltto.readDataType()  );
			Serial.println(ltto.readDataByte() );
			break;
		}

		if (ltto.readMessageOverwrittenCount() > 0)								// readMessageOverwrittenCount() returns a count of how many messages have been missed. Must be ZERO !
		{
			Serial.print(F("\n\nSorry but the main loop is too slow - "));
			Serial.print(ltto.readMessageOverwrittenCount());
			Serial.println(F(" message/s missed\n"));
			ltto.clearMessageOverwrittenCount();
		}
	}
#endif
}

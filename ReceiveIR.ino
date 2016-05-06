///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateIRmessage()                              //  TODO: Currently not checking for valid -2mS breaks !!!!
{
  if (irPacketLength < 6)                           //  The message was incomplete.
    { 
      decodedIRmessage.type = 's';  
      decodedIRmessage.newMessage = TRUE;        
      return; 
    }       

  if (messageIR[1] == 3 && messageIR[2] == -6);     //  We have a good header.
  else 
  {
    decodedIRmessage.type = 'h';  
    decodedIRmessage.newMessage = TRUE;
    return;
  }
  
  ////---------------------------------------------------------------------------------------------------------
  //  Set the message type via the number of bits received (and 1/9th bit of Packet/Checksum)
    if      (irPacketLength > 20 && messageIR[3] == 3 && messageIR[5] == 0)    decodedIRmessage.type = 'P';          // Packet
    else if (irPacketLength > 20 && messageIR[3] == 3 && messageIR[5] == 1)    decodedIRmessage.type = 'C';          // Checksum
    else if (irPacketLength > 18 && messageIR[3] == 3 && irPacketLength < 21)  decodedIRmessage.type = 'D';          // Data
    else if (irPacketLength > 16 && messageIR[3] == 3 && irPacketLength < 19)  decodedIRmessage.type = 'T';          // Tag
    else if (irPacketLength > 12 && messageIR[3] == 6 && irPacketLength < 15)  decodedIRmessage.type = 'B';          // Beacon - only beacons have 3/6/6 header !!
    else    {
              decodedIRmessage.type = 'x';  
              decodedIRmessage.newMessage = TRUE;      
              return; 
             }
 
  ////---------------------------------------------------------------------------------------------------------
  // Set the message length based on the type
    byte messageLength = 0;
    if      (decodedIRmessage.type == 'T') messageLength = 17;           // Long Break [0] + 3 header [1,2,3] + break [4] + 7 bits,breaks [5,7,9,11,13,15,17]
    else if (decodedIRmessage.type == 'B') messageLength = 13;           // Long Break [0] + 3 header [1,2,3] + break [4] + 5 bits,breaks [5,7,9,11,13]
    else if (decodedIRmessage.type == 'P') messageLength = 21;           // Long Break [0] + 3 header [1,2,3] + break [4] + 9 bits,breaks [5,7,9,11,13,15,17,19,21]
    else if (decodedIRmessage.type == 'D') messageLength = 19;           // Long Break [0] + 3 header [1,2,3] + break [4] + 8 bits,breaks [5,7,9,11,13,15,17,19]
    else if (decodedIRmessage.type == 'C') messageLength = 21;           // Long Break [0] + 3 header [1,2,3] + break [4] + 9 bits,breaks [5,7,9,11,13,15,17,19,21]
  
  ////---------------------------------------------------------------------------------------------------------
  //  Push the data into the dataPacket
    for (int i = 5; i<=messageLength; i+=2)
      {
        decodedIRmessage.rawDataPacket = decodedIRmessage.rawDataPacket << 1;
        decodedIRmessage.rawDataPacket = decodedIRmessage.rawDataPacket + (messageIR [i]);
      }

  ////---------------------------------------------------------------------------------------------------------
  //  Tidy up and go home

    decodedIRmessage.newMessage = TRUE;                                   // Set the flag to say there is a new message to decode.
    digitalWrite(DE_BUG_TIMING_PIN, LOW);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DecodeIR()
{
  if (decodedIRmessage.newMessage == FALSE) return FALSE;
  
  if (decodedIRmessage.type == 'T')
  {
    // Find TeamID of tagger
    decodedIRmessage.TeamID = decodedIRmessage.rawDataPacket & B01100000;             // TeamID = 1 thru 3  (0 = NoTeams)
    decodedIRmessage.TeamID = decodedIRmessage.TeamID >> 5;
    // Find PlayerID
    decodedIRmessage.PlayerID = (decodedIRmessage.rawDataPacket & B00011100);         // PlayerID = 1 thru 8
    decodedIRmessage.PlayerID = (decodedIRmessage.PlayerID >> 2) + 1;
    // Find tag Power
    decodedIRmessage.ShotStrength = (decodedIRmessage.rawDataPacket & B00000011)+1;      // Tag strength = 1 to 4
    
  }


     //TODO: Check for a bad 3/6 Tag packet and then flag as a near miss !!    
     
  else if (decodedIRmessage.type == 'B')
  { 
    // Find TeamID of Beacon
    decodedIRmessage.TeamID = decodedIRmessage.rawDataPacket & B11000;                // TeamID = 1 thru 3  (0 = NoTeams)
    decodedIRmessage.TeamID = decodedIRmessage.TeamID >> 3;
    // Check the TagBeacon flag
    decodedIRmessage.TagReceivedBeacon = (decodedIRmessage.rawDataPacket & B00100);   // Sets flag for whether the beacon was sent because of receiving a tag.
    decodedIRmessage.TagReceivedBeacon = (decodedIRmessage.TagReceivedBeacon >> 2);
    // Find tag Power
    decodedIRmessage.ShotStrength = (decodedIRmessage.rawDataPacket & B00011)+1;      // Tag strength = 1 to 4  - MUST be Zero if TagBeacon flag above is FALSE.

    //TODO:  Implement Zone Beacons and LTAR beacons   www.wiki.lazerswarm.com/wiki/Beacon_Signature.html
    //  (if TeamID = 0 in a hosted game, then it is a Medic Beacon)
  }
  
  else if (decodedIRmessage.type == 'P')
  {
    byteCount = 0;
    decodedIRmessage.PacketByte = decodedIRmessage.rawDataPacket & B11111111;
  }

  else if (decodedIRmessage.type == 'D')
  {
    byteCount++;
    decodedIRmessage.DataByte = decodedIRmessage.rawDataPacket & B11111111;
  }

  else if (decodedIRmessage.type == 'C')
  {
   decodedIRmessage.CheckSumRxByte = decodedIRmessage.rawDataPacket & B11111111;
  }

  else if (decodedIRmessage.type == 's')   badMessage_CountISRshortPacket++;
  else if (decodedIRmessage.type == 'x')   badMessage_InvalidPacketType++;
  else if (decodedIRmessage.type == 'h')   badMessage_non3_6Header++;

  decodedIRmessage.newMessage = FALSE;
  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t CheckSumRx = 0;

bool gameIDmatch = FALSE; 


void ProcessRxPacket()
{
    Serial.print(F("\nProcessRxPacket"));
    //Serial.print(decodedIRmessage.rawDataPacket);
    CheckSumRx = 0;
    CheckSumRx = CheckSumRx + decodedIRmessage.PacketByte;
    gameIDmatch = FALSE;
    
    switch (decodedIRmessage.PacketByte)
    {
        case 0x10:
          decodedIRmessage.PacketName = "RequestJoinGame";
           Serial.print(F("\nRx'd RequestJoinGame"));
          break;
        case 0x11:
          decodedIRmessage.PacketName = "AckPlayerAssign";
          Serial.print(F("\nAckPlayerAssign"));
          break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ProcessRxDataByte()
{
    CheckSumRx = CheckSumRx + decodedIRmessage.rawDataPacket;
    
    if (decodedIRmessage.PacketName == "RequestJoinGame")
     {
          Serial.print(F("\nDataByte"));
          if (byteCount == 1 && decodedIRmessage.DataByte == hostedGameID)
          {
            gameIDmatch = TRUE;
            Serial.print(F("gameID matched"));
          }
          if (byteCount == 2)
          {
            taggerID = decodedIRmessage.DataByte;
            Serial.print(F("TaggerID = "));
            Serial.print(taggerID, HEX);
          }
          if (byteCount == 3)                                                     ; // This is Team Request, do it later ! 
          
     }
    
    
    switch (decodedIRmessage.rawDataPacket)
    {
        //case 
    }
    
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ProcessRxCheckSum()
{
    //TODO: Move this stuff to the HOSTING module !
    // All this should do is set a flag based on 
    //if(CheckSumRx == decodedIRmessage.CheckSumRxByte)
    
    
    if (decodedIRmessage.PacketName == "RequestJoinGame" && CheckSumRx == decodedIRmessage.CheckSumRxByte && gameIDmatch == TRUE && taggerID != 0)
    {
      Serial.print (F("\nWe have a valid join request"));
      decodedIRmessage.PacketName = "null";
      AssignPlayer();
      


      //TODO; Acknowledge Player Assignment
    }
    if (decodedIRmessage.PacketName == "AckPlayerAssign" && CheckSumRx == decodedIRmessage.CheckSumRxByte && gameIDmatch == TRUE && taggerID != 0)
    {
      //TODO; Acknowledge Player Assignment
    }
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintIR(char mode)
{
  if (mode == 'X')
  {   
    Serial.print(F("\nPrintIR(ext): "));
    for (int i = 0; i<= ARRAY_LENGTH; i++)
    {
      if (messageIR[i] != 42)
      {
        Serial.print(messageIR [i]);
        Serial.print(F(", "));
      }      
    }
  }

  else if (mode == 'S')
  {
    Serial.print(F("\nPrintIR: "));
    Serial.print(decodedIRmessage.type);
    Serial.print(F(", "));
    Serial.print(decodedIRmessage.rawDataPacket, BIN);
    Serial.println();
  }
  
  if (decodedIRmessage.type == 'C') Serial.println(F("-------------------"));
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////


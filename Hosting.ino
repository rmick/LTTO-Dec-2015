#include <Arduino.h>

////--------------------------------------------------
//  Hosting variables

uint8_t	taggerToJoinID			= 0;
byte	assignToTeam			= 1;
byte	assignToPlayer			= 1;
byte	assignToTeamAndPlayer	= 8;		// Team 1, Player 1, as per defaults above.
byte	currentSelectedTeam		= 0;

bool	gameIDmatch				= false; 
byte	numberOfTeams			= 2;
char	teamTags				= 'N';
char	slowTags				= 'N';
bool	toggleTeamsMode			= true;
byte	countDownTime			= 10;



const byte arraySize			= 10;
int dataPacket[arraySize];					// an array to store a Packet, XX dataBytes and CheckSum: Max length = 15?
byte dataPacketArrayIndex		= 0;


////--------------------------------------------------
//  Host 2TeamRespawn Roonka Game (captured packets from LTTO host)
byte hostedGameType			= 0x0C;
byte hostedGameID			= 0x42;
byte hostedGameTime			= 0x15;
byte hostedTagsAvailable	= 0x99;
byte hostedReloadsAvailable = 0xFF;
byte hostedShieldTime		= 0x60;
byte hostedMegaTags			= 0x15;
byte hostedPackedFlags1		= 0xA8;
byte hostedPackedFlags2		= 0x32;
byte name1					= 0x32;
byte name2					= 0x54;
byte name3					= 0x52;
byte name4					= 0x53;
byte checkSum				= 0x00;

//////////////////////////////////////////////////////////////

void HostMode()
{
    DrawHostMode();

    char const* Action = GetButtonPress();
	if (Action == "Start")
	{
		assignToPlayer = 1;
		state = ANNOUNCE_GAME;
	}
	else if (Action == "# of Teams : ")
	{
		numberOfTeams++;
		if (numberOfTeams >  3) numberOfTeams = 0;
		if (numberOfTeams == 1) numberOfTeams = 2;
		DrawTextLabel(195, 55, BLACK, String(numberOfTeams), 3, WHITE, 1);
		if (numberOfTeams == 0)
		{
			assignToTeam = 0;
			assignToTeamAndPlayer = 1;
		}
		else
		{
			assignToTeam = 1;
			assignToTeamAndPlayer = 8;
		}
	}
	else if (Action == "TeamTags ? : ")
	{
		if		(teamTags == 'Y') teamTags = 'N';
		else if (teamTags == 'N') teamTags = 'Y';
		DrawTextLabel(195, 115, BLACK, String(teamTags), 3, WHITE, 1);
	}
	else if (Action == "SlowTags ? : ")
	{
		if		(slowTags == 'Y') slowTags = 'N';
		else if (slowTags == 'N') slowTags = 'Y';
		DrawTextLabel(195, 175, BLACK, String(slowTags), 3, WHITE, 1);
	}
    else if (Action == "EXIT")         state = CONFIG2;
}


///////////////////////////////////////////////////////////////////////////////

void DrawHostMode()
{
	if (lastState != state)
	{
		#ifdef DEBUG
		  Serial.println(F("HostMode Screen"));
		#endif
		DrawScreen(HOST, "HOST GAME", MAGENTA, WHITE, 3);
		DrawButton	 ( 20,  40, 200, 55, BLACK, "# of Teams : ",		2, WHITE	);
		DrawTextLabel(195,  55,			 BLACK,  String(numberOfTeams),	3, WHITE, 1	);
		DrawButton	 ( 20, 100, 200, 55, BLACK,  "TeamTags ? : ",		2, WHITE	);
		DrawTextLabel(195, 115,			 BLACK,	 String(teamTags),		3, WHITE, 1	);
		DrawButton	 ( 20, 160, 200, 55, BLACK,  "SlowTags ? : ",		2, WHITE	);
		DrawTextLabel(195, 175,			 BLACK,  String(slowTags),		3, WHITE, 1	);
		DrawButton	 ( 20, 220, 200, 55, BLACK,  "Start",				2, WHITE	);  
		DrawButton	 ( 70, 290, 100, 30, YELLOW, "EXIT",				2,BLACK		);
	}
}

///////////////////////////////////////////////////////////////////////////////
unsigned long hostTimer = millis();

void AnnounceGame()
{
	DrawAnnounceGame();

	char const* Action = GetButtonPress();
	if (Action == "Set Team : ")
	{
		if (numberOfTeams == 0) return;	//ignore as there are no teams
		assignToTeam++;
		if (assignToTeam > numberOfTeams) assignToTeam = 1;
		DrawTextLabel(195, 55, BLACK, String(assignToTeam), 3, WHITE, 1);
	}
	else if (Action == "ToggleTeams?  ")
	{
		if (toggleTeamsMode)
		{
			toggleTeamsMode = false;
			DrawTextLabel(195, 115, BLACK, "N", 3, WHITE, 1);
		}
		else
		{
			toggleTeamsMode = true;
			DrawTextLabel(195, 115, BLACK, "Y", 3, WHITE, 1);
		}
	}
	else if (Action == "Play Game")
	{
		StartCountDown();
	}
	else if (Action == "EXIT") state = HOST;


	// Start the hosting activity
	AdvertiseGame();
	if (ltto.available() )
	{
		switch (ltto.readMessageType() )                        // readMessageType() returns the type of Message (TAG, BEACON, PACKET, DATA, CHECKSUM)
		{
		case PACKET:
			dataPacketArrayIndex = 0;
			if (ltto.readPacketName() == "REQUEST_JOIN_GAME")	dataPacket[dataPacketArrayIndex] = REQUEST_JOIN_GAME;
			if (ltto.readPacketName() == "ACK_PLAYER_ASSIGN")	dataPacket[dataPacketArrayIndex] = ACK_PLAYER_ASSIGN;
			break;

		case DATA:
			dataPacket[++dataPacketArrayIndex] = ltto.readDataByte();
			break;

		case CHECKSUM:
			if (ltto.readCheckSumOK() )
			{
				if (dataPacket[0] == REQUEST_JOIN_GAME) ProcessJoinRequest();
				if (dataPacket[0] == ACK_PLAYER_ASSIGN)	AddPlayerToList();
			}
			dataPacketArrayIndex = 0;
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void DrawAnnounceGame()
{
	if (lastState != state)
	{
		#ifdef DEBUG
			Serial.println(F("AnnounceGame Screen"));
		#endif
		DrawScreen(HOST, "JOIN TAGGERS", MAGENTA, WHITE, 3);
		if (numberOfTeams != 0)
		{
			DrawButton(20,  40, 200, 55, BLACK, "Set Team : ",	  2, WHITE);
			DrawButton(20, 100, 200, 55, BLACK, "ToggleTeams?  ", 2, WHITE);
			DrawTextLabel(195, 55, BLACK, String(assignToTeam), 3, WHITE, 1);
			if (toggleTeamsMode) DrawTextLabel(195, 115, BLACK, "Y", 3, WHITE, 1);
			else				 DrawTextLabel(195, 115, BLACK, "N", 3, WHITE, 1);
		}
		DrawButton(20, 220, 200, 55, BLACK, "Play Game", 2, WHITE);
		DrawButton(70, 290, 100, 30, YELLOW, "EXIT",	2, BLACK);
	}
}

///////////////////////////////////////////////////////////////////////////////

void ProcessJoinRequest()
{
	
		//Serial.print("\nRespnd to JoinRequest");
		if (dataPacket[1] == hostedGameID)
		{
			taggerToJoinID = dataPacket[2];
			byte teamRequest = (dataPacket[3] && B000011);
			//if ( teamRequest != 0) assignToTeam = teamRequest;		//else assignToTeam = whatever it already is.
			AssignPlayer();
		}
		return;
		for (byte x = 0; x < arraySize; x++)
		{
			dataPacket[x] = 0;
		}
}

///////////////////////////////////////////////////////////////////////////////

void AssignPlayer()                              // This is the reply to:    0x010 - Request Join Game
{
	Serial.print(F("\nAssignTeam - "));
	Serial.print(assignToTeam);
	Serial.print(F(" : Player - "));
	Serial.println(assignToPlayer);
	DrawTextLabel(195, 55, BLACK, String(assignToTeam), 3, WHITE, 1);

	AssignToPlayerAndTeam();

	//TODO: Set up a callback to the main loop between each data byte.
	delay(100);
	ltto.sendIR(PACKET,		ASSIGN_PLAYER);
	loop();
	ltto.sendIR(DATA,		hostedGameID);
	loop();
	ltto.sendIR(DATA,		taggerToJoinID);
	loop();
	ltto.sendIR(DATA,		assignToTeamAndPlayer);
	loop();
	ltto.sendIR(CHECKSUM,	0x00);

	SelectNextPlayerAndTeam();
}

void AddPlayerToList()
{
	//TODO:	Check for ACK_PLAYER_ASSIGN from Tagger in response to ASSIGN_PLAYER)
	//TODO:	Now add it to the list of known taggers for Countdown flags
}

void AssignToPlayerAndTeam()
{
	assignToTeamAndPlayer = assignToTeam << 3;                                //  TeamID is 1 based
	assignToTeamAndPlayer = assignToTeamAndPlayer + (assignToPlayer - 1);       //  PlayerID is 0 based
	//ltto.printBinary(assignToTeamAndPlayer, 8);
}

void SelectNextPlayerAndTeam()
{
	if (toggleTeamsMode)
	{
		//Serial.print(F("\nToggleTeamsMode = Y  - "));
		//Serial.print(assignToTeam);
		assignToTeam++;
		
		if (assignToTeam > numberOfTeams)
		{
			if (assignToPlayer == 8) CheckIfGameIsFull();
			else
			{
				assignToPlayer++;
				assignToTeam = 1;
			}
		}
		//Serial.print(assignToTeam);
		DrawTextLabel(195, 55, BLACK, String(assignToTeam), 3, WHITE, 1);
		CheckIfGameIsFull();
	}
	else
	{
		//Serial.print(F("ToggleTeamsMode = N")); 
		assignToPlayer++;
		if (assignToPlayer > 8)
		{
			assignToPlayer = 1;
			assignToTeam++;
		}
		CheckIfGameIsFull();
		DrawTextLabel(195, 55, BLACK, String(assignToTeam), 3, WHITE, 1);
	}
}

void CheckIfGameIsFull()
{
	bool fullSteamAhead = false;
	if (numberOfTeams == 0 && assignToPlayer > 24)	fullSteamAhead = true;
	if (toggleTeamsMode)
	{
		if (numberOfTeams != 0 && (assignToTeam > numberOfTeams) && assignToPlayer == 8)
			fullSteamAhead = true;
	}
	else
	{
		if (numberOfTeams != 0 && (assignToTeam > numberOfTeams) && assignToPlayer == 8)
			fullSteamAhead = true;
	}
	if (fullSteamAhead)
	{
		delay(1500);
		StartCountDown();
	}
}

/*
  ////--------------------------------------------------
  //  Host 2Teams Game (captured packets from LTTO host)
  byte hostedGameType           = 0x03;
  byte hostedGameID             = 0xED;
  byte hostedGameTime           = 0x10;
  byte hostedTagsAvailable      = 0x99;
  byte hostedReloadsAvailable   = 0xFF;
  byte hostedShieldTime         = 0x15;
  byte hostedMegaTags           = 0x10;
  byte hostedPackedFlags1       = 0xCC;
  byte hostedPackedFlags2       = 0x02;
*/

 


///////////////////////////////////////////////////////////////////////////////

void AdvertiseGame()
{
	if (millis() - hostTimer > 1500)
	{
		//Serial.println(F("\n\tAnnounce 2 Teams Game"));
		ltto.sendIR(PACKET, hostedGameType);
		ltto.sendIR(DATA, hostedGameID);
		ltto.sendIR(DATA, hostedGameTime);
		ltto.sendIR(DATA, hostedTagsAvailable);
		ltto.sendIR(DATA, hostedReloadsAvailable);
		ltto.sendIR(DATA, hostedShieldTime);
		ltto.sendIR(DATA, hostedMegaTags);
		ltto.sendIR(DATA, hostedPackedFlags1);
		ltto.sendIR(DATA, hostedPackedFlags2);
		ltto.sendIR(DATA, name1);
		ltto.sendIR(DATA, name2);
		ltto.sendIR(DATA, name3);
		ltto.sendIR(DATA, name4);
		ltto.sendIR(CHECKSUM, checkSum);

		hostTimer = millis();
	}
}

///////////////////////////////////////////////////////////////////////////////

void StartCountDown()
{
	#ifdef DEBUG
		Serial.println(F("CountDown Screen"));
	#endif
	DrawScreen(HOST, "COUNT DOWN", RED, WHITE, 3);
	while (countDownTime > 0)
	{
		Serial.print(F("\nCountDown : "));
		Serial.print(countDownTime);
		ltto.sendIR(PACKET,	0x00);
		ltto.sendIR(DATA,		hostedGameID);
		ltto.sendIR(DATA,		ConvertDecToBCD(countDownTime) );
		ltto.sendIR(DATA,		0x08);                          //  TODO: This is packed byte of valid Team1 Player ID's  - Needs to be real data eventually.
		ltto.sendIR(DATA,		0x08);                          //  TODO: This is packed byte of valid Team1 Player ID's  - Needs to be real data eventually.
		ltto.sendIR(DATA,		0x00);                          //  TODO: This is packed byte of valid Team1 Player ID's  - Needs to be real data eventually.
		ltto.sendIR(CHECKSUM, 0x00);
		DrawTextLabel(0, 140, RED,String(countDownTime), 8, WHITE, 2);
		countDownTime--;
		delay (600);
	}
	// Resest the Respawn count
	EEPROM.write(eeMEDIC_COUNT, 0);
	medicCount = 0;
	state = MEDIC;
}

///////////////////////////////////////////////////////////////////////////////

byte ConvertDecToBCD(byte dec)
{
  if (dec == 0xff) return dec;
  return (byte) (((dec/10) << 4) | (dec %10) );
}

byte ConvertBCDtoDec(byte bcd)
{
  if (bcd == 0xff) return bcd;
  return (byte) (((bcd >> 4) & 0xF) *10) + (bcd & 0xF);
}

///////////////////////////////////////////////////////////////////////////////

void SendText()
{
  ltto.sendIR(PACKET, 0x80);
  ltto.sendIR(DATA, 0x48);
  ltto.sendIR(DATA, 0x45);
  ltto.sendIR(DATA, 0x4C);
  ltto.sendIR(DATA, 0x4C);
  ltto.sendIR(DATA, 0x4F);
  ltto.sendIR(CHECKSUM, 0xF4);
}

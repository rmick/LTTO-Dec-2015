#include <Arduino.h>

//  void TaggerMode()
//  void FireLaser(byte megaTag)
//  void SetShields()
//  void Reload()
//  void DrawTaggerScreen()
//  void DrawTaggerScreenShieldsUp()
//  void DecodeTagIR()
//  void GameOver()

byte tagPower = 1;

///////////////////////////////////////////////////////////////////////////////

void TaggerMode()
{ 
	DrawTaggerScreen();
  
	char const* Action = GetButtonPress();
	if      (strcmp(Action, "Team")		== 0)	{ teamID++;   if (teamID == 4)    teamID = 0;   DrawTextLabel(50, 90,  GREY, String(teamID),   2, WHITE, 1); }
	else if (strcmp(Action, "Player")	== 0)	{ playerID++; if (playerID == 9)  playerID = 1; DrawTextLabel(50, 175, GREY, String(playerID), 2, WHITE, 1); }
	else if (strcmp(Action, "TagPower")	== 0)	{ tagPower++; if (tagPower == 5)  tagPower = 1; DrawTextLabel(50, 260, GREY, String(tagPower), 2, WHITE, 1); }
	else if (strcmp(Action, "Fire")		== 0)	FireLaser();
	else if (strcmp(Action, "Shields")	== 0)	SetShields();
	else if (strcmp(Action, "ReLoad")	== 0)	Reload();
	else if (strcmp(Action, "Scores")	== 0)	state = SCORES;                    
	else if (strcmp(Action, "EXIT")		== 0)	state = CONFIG1;

	if (shieldsUp) UpdateShieldsTimer();

	CheckIR();
}

///////////////////////////////////////////////////////////////////////////////
uint32_t shieldsRunningTime;

void UpdateShieldsTimer()
{
	if ((shieldsRunningTime + 1000) < millis() )		// TODO: Allow for rollover
	{
		Serial.print(F(" -- "));
		shieldsTimer--;
		shieldsRunningTime = millis();
		DrawTextLabel(165, 260, GREY, String(shieldsTimer), 3, RED, 2);
		if (shieldsTimer == 0)
		{
			Serial.print(F("\nZero"));
			shieldsUp = false;
			lastState = NONE;
			DrawTaggerScreen();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void FireLaser()
{  
	if (tagCount == 0) return;          //TODO: Make a click noise
	if (tagCount < tagPower) return;    //TODO: Make a failed noise
	ltto.sendTag(teamID, playerID, tagPower);
	tagCount = tagCount - (tagPower);
	if (tagCount >= 250 && tagCount <= 255) tagCount = 0;
	lastState = NONE;
	DrawTaggerScreen();
}
///////////////////////////////////////////////////////////////////////////////

void SetShields()
{
	if (shieldsTimer == 0) return;
	else
	{
		shieldsUp = !shieldsUp;
		lastState = NONE;
		if (!shieldsUp) DrawTaggerScreen();
		else if (shieldsUp)
		{
			shieldsRunningTime = millis();
			DrawTaggerScreenShieldsUp();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void Reload()
{
  static uint8_t remainingReloads = maxReloads;

  if (remainingReloads >0 || maxReloads == 0)			 // if maxReloads is 0 then unlimited reloads, else keep count.
  {
    tft.fillScreen(BLACK);
    delay (500);
    tagCount = reloadAmount;
    if (remainingReloads > 0)  remainingReloads--;
    lastState = NONE;
    DrawTaggerScreen();
  }
}

///////////////////////////////////////////////////////////////////////////////

void DrawTaggerScreen()
{
  if (lastState != state)
  {
    #ifdef DEBUG
      Serial.println(F("\nDrawTaggerScreen"));
    #endif
    DrawScreen(TAGGER, "TAG MODE", GREY, BLUE, 3);
    lastState = state;
    DrawButton(  5,  30,  100, 55, WHITE,  "Team",		2, BLACK);
    DrawButton(  5, 115,  100, 55, WHITE,  "Player",	2, BLACK);
    DrawButton(  5, 200,  100, 55, WHITE,  "TagPower",	2, BLACK);
    DrawButton(135,  30,  100, 55, BLACK,  "ReLoad",	2, WHITE);
    DrawButton(135, 125,  100, 55, YELLOW, "Fire",		2, YELLOW);
    DrawButton(135, 200,  100, 55, RED,    "Shields",	2, GREEN);
    DrawButton(  5, 290,   50, 30, YELLOW, "Scores",	1, BLACK);
    DrawButton( 70, 290,  100, 30, YELLOW, "EXIT",		2, BLACK);

    DrawTextLabel(  50, 90,  GREY,  String(teamID),       2, WHITE, 1);
    DrawTextLabel(  50, 175, GREY,  String(playerID),     2, WHITE, 1);
    DrawTextLabel(  50, 260, GREY,  String(tagPower),     2, WHITE, 1);
    DrawTextLabel( 165,  88, GREY,  String(tagCount),     3, WHITE, 2);
	DrawTextLabel( 165, 130, YELLOW,"Tag",				  2, BLACK, 0);
    DrawTextLabel( 170, 155, YELLOW,String(playerHealth), 3, BLACK, 2);
    DrawTextLabel( 165, 260, GREY,  String(shieldsTimer), 3, RED,   2);
  }
}
///////////////////////////////////////////////////////////////////////////////

void DrawTaggerScreenShieldsUp()
{
  if (lastState != state)
  {
    #ifdef DEBUG
      Serial.println(F("DrawTaggerScreen-ShieldsUp"));
    #endif
    DrawScreen(TAGGER, "TAG MODE", GREY, BLUE, 3);
    lastState = state;
    DrawButton(135, 200,  100, 55, RED,    "Shields",  2, GREEN);
    DrawTextLabel( 165, 260, GREY,  String(shieldsTimer), 3, RED,   2);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void CheckIR()
{
	if (ltto.available())										// available() checks if there is a message to receive. (1 = true, 0 = false)
	{
		switch (ltto.readMessageType())                        // readMessageType() returns the type of Message (TAG, BEACON, PACKET, DATA, CHECKSUM)
		{
		case TAG:
			if (teamID == ltto.readTeamID() && friendlyFire == false && teamID != 0)
			{
				ShowFriendlyFire();
				return;
			}
			if (shieldsUp == true)
			{
				TakeHitShieldsUp();
				return;
			}
			else
			{
				TakeHit();
				UpdateScoreGrid();
				CheckIfDead();
			}
			break;

		case BEACON:
			BeaconFlash(true);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TakeHitShieldsUp()
{
	DrawButton(135, 200, 100, 55, RED, "Shields Blocked", 2, GREEN);
	delay(500);
	lastState = NONE;
	DrawTaggerScreenShieldsUp();
#ifdef DEBUG
	Serial.print(F("\nShields Blocked A Shot"));
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UpdateScoreGrid()
{
	byte currentHitIndex;
	currentHitIndex = ltto.readPlayerID();
	if (teamID != 0)  currentHitIndex = currentHitIndex + (teamID * 8);
	scoreGrid[currentHitIndex] = scoreGrid[currentHitIndex] + ltto.readShotStrength();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CheckIfDead()
{
	if (playerHealth == 0)
	{
		state = GAME_OVER;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TakeHit()
{
	tft.fillScreen(RED);
	lastState = NONE;
	DrawTaggerScreen();
	playerHealth = playerHealth - ltto.readShotStrength();
	if (playerHealth <= 255 && playerHealth >= 250) playerHealth = 0;
	DrawTextLabel(170, 155, YELLOW, String(playerHealth), 3, BLACK, 2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ShowFriendlyFire()
{
	DrawButton(135, 200, 100, 55, RED, "Friendly Fire", 2, GREEN);
	delay(500);
	lastState = NONE;
	DrawTaggerScreen();
#ifdef DEBUG
	Serial.print(F("\nFriendlyFire"));
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayScores()
{
  static bool scoresActive;
  if (!scoresActive)
  {
    tft.fillScreen(BLACK);
    DrawButton( 70, 290,  100, 30, YELLOW, "EXIT", 2, BLACK);
    scoresActive = true;
    tft.setTextColor(YELLOW);
    tft.setTextSize(1);
    for (uint8_t teamPlayerCount = 1; teamPlayerCount <25; teamPlayerCount++)
    {
      tft.setCursor(60, (teamPlayerCount*11)+5);
      byte displayPlayerID = teamPlayerCount%8; if (displayPlayerID == 0) displayPlayerID = 8;
      tft.print("Team "); tft.print(((teamPlayerCount-1)/8)+1); tft.print(", Player "); tft.print(displayPlayerID); tft.print(": ");
      tft.println(scoreGrid[teamPlayerCount]);
    }
  }

  char const* Action = GetButtonPress();
  if (strcmp(Action, "EXIT")	== 0)  { scoresActive = false; state = TAGGER; lastState = NONE; DrawTaggerScreen(); }

  
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GameOver()
{
  tft.fillScreen(BLACK);
  DrawTextLabel ( 0, 100, BLACK, "Game", 6, RED, 0);
  DrawTextLabel ( 0, 180, BLACK, "Over", 6, RED, 0);
}


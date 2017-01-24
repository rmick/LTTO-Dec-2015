#include <Arduino.h>

int8_t	pinCodeEntered[4] = {-1, -1, -1, -1};
int8_t	newPin[4] = {-1, -1, -1, -1};
int		timeOut = 0;
byte	digitCount = 0;
#define PINCODE 'p'
#define NEWPIN  'n'
#define CONFIRM 'c'

void PinPadMode()
{
	DrawPinPadScreen("Enter PIN:      ");
	CheckTimeOut();
	ProcessPinButtonPress(PINCODE);
}

void ChangePin()
{
	DrawPinPadScreen("New PIN:     ");
	CheckTimeOut();
	ProcessPinButtonPress(NEWPIN);
}

void ConfirmPin()
{
	DrawPinPadScreen("Confirm PIN:     ");
	CheckTimeOut();
	ProcessPinButtonPress(CONFIRM);
}

bool CheckPinOK()
{
	if (pinCodeEntered[0] == pinCode[0] &&
		pinCodeEntered[1] == pinCode[1] &&
		pinCodeEntered[2] == pinCode[2] &&
		pinCodeEntered[3] == pinCode[3])
	{
		digitCount = 0;
		ClearPin();
		return true;
	}
	else
	{
		PinWrong();
		return false;
	}
}

bool CheckPinCodesMatch()
{
	Serial.print(F("Checking PIN codes - "));
	if (pinCodeEntered[0] == newPin[0] &&
		pinCodeEntered[1] == newPin[1] &&
		pinCodeEntered[2] == newPin[2] &&
		pinCodeEntered[3] == newPin[3])
	{
		Serial.print(F("codes match"));
		DrawTextLabel(160, 5, BLACK, "GOOD", 2, GREEN, 5);
		delay(500);
		return true;
	}
	else return false;
}

void ProcessPinButtonPress(char mode)
{
	char const* buttonPressed = GetButtonPress();

	if (strcmp(buttonPressed, "Clr") == 0)
	{
		DrawTextLabel(160, 5, BLACK, "       ", 2, BLACK, 8);
		digitCount = 0;
		timeOut = 0;
	}

	else if (strcmp(buttonPressed, "OK") == 0)
	{
		switch (mode)
			{
			case PINCODE:
				if (CheckPinOK() )	state = CONFIG1;
				else PinWrong();
				break;
			case NEWPIN:
				state = CONFIRM_PIN;
				break;
			case CONFIRM:
				if (CheckPinCodesMatch() )
				{
					StoreNewPin();
					state = CONFIG1;
					ClearPin();
				}
				else PinWrong();
				break;
			case DEFAULT:
				state = CONFIG1;
				break;
			}
	}

	else
		if (digitCount < 4)
		{
			uint8_t numberPressed = atoi(buttonPressed);
			if (numberPressed >= 0 && numberPressed <= 9)
			{
				switch (mode)
				{
				case PINCODE:
					DrawTextLabel(160 + (digitCount * 15), 5, BLACK, "*", 2, YELLOW, 1);
					pinCodeEntered[digitCount] = numberPressed;
					break;
				case NEWPIN:
					DrawTextLabel(160 + (digitCount * 15), 5, BLACK, buttonPressed, 2, YELLOW, 1);
					newPin[digitCount] = numberPressed;
					break;
				case CONFIRM:
					if		(numberPressed == newPin[digitCount]) DrawTextLabel(160 + (digitCount * 15), 5, BLACK, buttonPressed, 2, GREEN, 1);
					else if (numberPressed != newPin[digitCount]) DrawTextLabel(160 + (digitCount * 15), 5, BLACK, buttonPressed, 2, RED,   1);
					pinCodeEntered[digitCount] = numberPressed;
				}
				digitCount++;
				timeOut = 0;
			}
		}
}

/////////////////////////////////////////////////////////////////////

void StoreNewPin()
{
	tft.fillScreen(WHITE);
	//Write new PIN to EEPROM.
	for (byte d = 0; d<4; d++)
	{
		EEPROM.write(eePIN_CODE + d, newPin[d]);
		pinCode[d] = newPin[d];
	}
}

/////////////////////////////////////////////////////////////////////

void CheckTimeOut()
{
	timeOut++;
	if (timeOut >= 5000)
	{
		state = MEDIC;
		ClearPin();
	}
}

/////////////////////////////////////////////////////////////////////

void PinWrong()
{
	DrawTextLabel(160, 5, BLACK, "WRONG", 2, RED, 5);
	delay(500);
	DrawTextLabel(160, 5, BLACK, "       ", 2, BLACK, 8);
	digitCount = 0;
	timeOut = 0;
}

/////////////////////////////////////////////////////////////////////

void ClearPin()
{
	for (uint8_t x = 0; x<4; x++)
	{
		//pinCodeEntered[x] = -1;
		newPin[x] = -1;
		digitCount = 0;
	}
}

///////////////////////////////////////////////////////////////////

void DrawPinPadScreen(const char* titleText)
{
	if (lastState != state)
	{
	#ifdef DEBUG
		Serial.println(F("\nDrawPinPadScreen"));
	#endif
	// Convert String to const char*
	//char* titleTextChar;
	//titleText.toCharArray(titleTextChar, 20);
	DrawScreen(PINPAD, titleText, BLACK, YELLOW, 2);
	DrawButton( 10,  25, 65, 65, BLUE, "1",   4, BLACK);
	DrawButton( 85,  25, 65, 65, BLUE, "2",   4, BLACK);
	DrawButton(160,  25, 65, 65, BLUE, "3",   4, BLACK);
	DrawButton( 10, 100, 65, 65, BLUE, "4",   4, BLACK);
	DrawButton( 85, 100, 65, 65, BLUE, "5",   4, BLACK);
	DrawButton(160, 100, 65, 65, BLUE, "6",   4, BLACK);
	DrawButton( 10, 175, 65, 65, BLUE, "7",   4, BLACK);
	DrawButton( 85, 175, 65, 65, BLUE, "8",   4, BLACK);
	DrawButton(160, 175, 65, 65, BLUE, "9",   4, BLACK);
	DrawButton( 10, 250, 65, 65, RED,  "Clr", 3, WHITE);
	DrawButton( 85, 250, 65, 65, BLUE, "0",   4, BLACK);
	DrawButton(160, 250, 65, 65, GREEN,"OK",  3, WHITE);
	digitCount = 0;											//	Intialises it when entering PinPad Mode/s
	timeOut = 0;
	#ifdef DEBUG
		PrintButtonArray();
	#endif
	}
}

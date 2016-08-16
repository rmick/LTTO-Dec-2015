#include <Arduino.h>

void setup()
{
	Serial.begin(115200);
	delay(500);
	Serial.println(F("\nHere we go boys...."));
    ltto.begin(13,11);
  
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
}

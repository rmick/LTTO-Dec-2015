///
///
///

void SetUp()
{
  Serial.println(F("DrawSetUp"));
  tft.fillScreen(MAGENTA);
  
  tft.setCursor(65, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println(F("SET-UP"));

////////////////In medic mode
////////////////Set Team ID
////////////////If Team ID set, enable Hostile mode - sends opposing 2/3 teams MegaTags
////////////////Set CountDown Timer Length
////////////////Reset Counter



  //EEPROM.write(0, 0);
  //numLives = 0;


////////////////Later on add MODES
//////////////// Capture the flag
//////////////// Own the Zone
//////////////// Ammo supply base
//////////////// Use RFID tags to do recharges 

  delay (3000);
  state = PinCode;
}

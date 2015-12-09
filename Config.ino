///
///
///


void ConfigMode()
{


  delay (10000);
  state = PinCode;


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



}

//////////////////////////////////////////////////////////////////////////////////////////

void DrawConfigScreen()
{
  Serial.println(F("DrawSetUp"));
  tft.fillScreen(MAGENTA);
  
  tft.setCursor(65, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println(F("SET-UP"));

  ButtonCount= 0;
  DrawButton(  5,  40,  200, 55, BLACK,  "Team ID",     2, WHITE);
  DrawButton(  5,  100, 200, 55, BLACK,  "TimerValue",  2, WHITE);
  DrawButton(  5,  160, 200, 55, BLACK,  "Hostile?",    2, WHITE);
  DrawButton(  5,  220, 200, 55, BLACK,  "Reset Count", 2, WHITE);
  DrawButton( 70, 290,  100, 30, YELLOW, "EXIT",        2, BLACK);
}

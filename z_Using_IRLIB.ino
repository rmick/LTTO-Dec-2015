#include <Arduino.h>


/* Arduino based example of lazertag protocol.
 * Based on the sample code by Kevin Osborn & Nick Hollander
 *         Hook an IR LED up to Digital pin 3 (PWM) for Arduino Uno etc. (look in IRremote library for other platforms.
 *         Uses Ken Shirrif's IRremote library found at: https://github.com/shirriff/Arduino-IRremote
 */


/* lazertag protocol, originally reverse engineered by  Aaron Nabil and documented at 
 * http://web.archive.org/web/20090304155723/http://lasertagparts.com/ltto.htm
 *
 * 3ms On, 6ms Off, 3ms (preamble) followed by 7 bits spaced apart by 2ms. One is represented by 2ms, zero is 1ms
 * Tag Packet - In non hosted games, the player and team bits are zero, but the last two bits signify how powerful the blast is.
 */

/*

#include <IRremote.h>
const int irLED = 3;
const int button = 1;
const int irReceiver = 6;

IRsend irsend;

void setup()
  {
    Serial.begin(9600);
    pinMode(irLED,OUTPUT);
    pinMode(button,INPUT);
    pinMode(irReceiver,INPUT_PULLUP);
  }

int buttonState = 0;
unsigned int shootOne[] = {3000,6000,3000,2000,1000,2000,1000,2000,1000,2000,1000,2000,1000,2000,1000,2000,1000};
// 363 0000000
unsigned int shootTwo[] =   {3000,6000,3000,2000,1000,2000,1000,2000,1000,2000,1000,2000,1000,2000,1000,2000,2000};
unsigned int shootThree[] = {3000,6000,3000,2000,1000,2000,1000,2000,1000,2000,1000,2000,1000,2000,2000,2000,1000};
unsigned int shootFour[] =  {3000,6000,3000,2000,1000,2000,1000,2000,1000,2000,1000,2000,1000,2000,2000,2000,2000};
unsigned int beaconRespawn[] ={3000,6000,6000,2000,1000,2000,1000,2000,1000,2000,2000,2000,2000};
// 366 00011
unsigned int beaconLockOn[]={3000,6000,6000,2000,1000,2000,1000,2000,1000,2000,1000,2000,1000};
// 366 00000
unsigned int beaconZone[]={3000,6000,6000,2000,1000,2000,1000,2000,1000,2000,2000,2000,1000};
// 366 00001

void loop() {
  
  if (digitalRead(button) == LOW)
      {
      digitalWrite(10, HIGH);
      irsend.sendRaw(beaconRespawn,sizeof(beaconRespawn)/sizeof(int),38);
      Serial.println("RespawnBeacon");
      digitalWrite(10, LOW);
      delay(250);
      }
  
  if (Serial.available() !=0)
    {
    char c = Serial.read();


    
    if (c == '1' || buttonState == HIGH)
      {
      digitalWrite(5, HIGH);
      irsend.sendRaw(shootOne,sizeof(shootOne)/sizeof(int),38);
      Serial.println("shootOne");
      digitalWrite(5, LOW);
      }
    
    if (c == '4')
      {
      digitalWrite(5, HIGH);
      irsend.sendRaw(shootFour,sizeof(shootFour)/sizeof(int),38);
      digitalWrite(5, LOW);
      delay(100);
      digitalWrite(5, HIGH);
      Serial.println("shootFour");
      delay(100);
      digitalWrite(5, LOW);
      }    
  delay(100);
    }
}


*/

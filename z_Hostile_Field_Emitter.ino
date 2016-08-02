#include <Arduino.h>

/*

#include <IRremoteHFE.h>

/*

HOSTILE FIELD EMITTER (HFE)- A Lazer Tag Landmine

Created August 2013 by James Scott
Dublin Laser Combat Club
Dublin, Ireland

This code is compatible with the Hasbro LAZER TAG brand equipment, including the following taggers:
  Lazer Tag Team Ops (LTTO) Deluxe tagger
  IR2-X Drone tagger
  Tag Master Blaster (TMB) tagger
  Phoenix LTX tagger
  Lazer Tag Augmented Reality (LTAR) tagger, with or without iPhone or iPod attached

The Hostile Field Emitter (HFE) is a Lazer Tag landmine that constantly fires out a 360-degree signal which
  is intended to quickly tag out hostile players. The rate and intensity of fire within a 20-foot radius will act
  as a deterrent to enemy players approaching the area. Multiple HFE's can be used to defend a base or provide
  redirection of enemy players to a choke point.

The HFE is set up by a player shooting the mine after it is turned on.
Once set, the HFE replicates the setting player's fire signature and constantly sends this out at a specified interval.
The HFE can be set to include up to 3 megatags (additional hits) with each pulse, for a total of 4 damage points per pulse.

INSTRUCTIONS FOR USE
1. Turn on the power source to the HFE.
2. Fire any tagger that is already in the game at the IR receiver.
3. After 3 seconds, the HFE will begin to emit the received firing signal at the specified interval (see intDelay variable).
4. To reset the mine to a different team, turn it off and then turn it back on, following the same steps as above.

NOTES ABOUT TEAMS
In a non-hosted game with no teams (LTAG mode), or in a CUST game (hosted free-for-all), the mine
  will be hostile to all players, including the one who set the mine.
In all other games with teams, hosted or non-hosted, the mine will be hostile to all teams besides
  the team of the player that set the mine.
Because of a bug in the LTAR software, the LTAR tagger takes friendly fire from any other tagger that
  is set up under the same Team ID and Player ID. In a normal hosted game, this would not occur.
  However, with the HFE, the mine replicates the same Team and Player ID as the person who set it up. Therefore,
  if an LTAR is used to set the HFE, the HFE will be hostile to that player, but not to anyone else on that player's team.

TECHNICAL SETUP
0. Build the hardware setup with an Arduino board using the instructions below.
1. Install the Arduino software tool from www.arduino.cc
2. Open Windows Explorer and go to the Arduino folder (e.g., c:\Program Files\Arduino)
3. Place the IRRemote folder in the Arduino\Libraries folder. Note that I have modified the stock IRRemote code found online,
   so you will need to use the code I provided.
4. You can put this file (HostileFieldEmitter.ino) in any location.
5. Open HostileFieldEmitter.ino in the Arduino software.
6. Plug your Arduino board into the PC using a USB cable.
7. In Tools > Board, specify the Arduino board you are using.
8. In Tools > Serial Port, select the COM port that your Arduino is using (check Devices in Windows). This often defaults correctly.
9. Upload the program to the Arduino board using the Upload icon.
10. Unplug the Arduino board and attach a battery power source using the Vin pin and ensuring the battery source is grounded.
    I use a simple 4-AA battery box from Radio Shack.

ARDUINO HARDWARE WIRING
I am using the wiring provided by Alexander Faucher on https://github.com/afaucher/LazerTagHost

Here is AFaucher's parts list, I have used different, equivalent parts but the same wiring:

2x Resistor (100ohms, ¼ watt, 5%)
 * Vishay Metal Film Resistors – Through Hole
 * Mfg P/N: CCF07100RJKE36, Mouser P/N: 71-CCF07100RJKE36
 * $0.04 each
1x Capacitor (10uF, 16 volts)
 * TDK Multilayer Ceramic Capacitors (MLCC) – Leade
 * Mfg P/N: FK20X7R1C106K, Mouser P/N: 81—FK20X7R1C106K
 * $0.54 each
x1 17 Degree 40mW Infrared Emitters (I am using 40 degree)
 * Vishay Infrared Emitters
 * Mfg P/N: TSAL6200, Mouser P/N: 782-TSAL6200
 * $0.44 each
x1 38kHz Infrared Receiver (4.5-5.5V)
 * Vishay Infrared Receiver
 * Mfg P/N: TSOP4138, Mouser P/N: 782-TSOP4138
 * $1.16 each
x1 Arduino Uno
 * $25.00 each
x1 Breadboard
 * $2.00 each

Total: $29.22

Arduino Wiring:

Emitter
Power Out Ground → (short pin) IR Emitter (long pin) → 100ohm Resistor → Arduino Digital 3

Receiver
Arduino Digital 11 → (left pin, facing receiver) IR Receiver
Power Out Ground → (center pin) IR Receiver
Power Out Ground → 10uF Capacitor → (right pin, facing receiver) IR Receiver
Power Out 5v → 100ohm Resistor → (right pin, facing receiver) IR Receiver




int intDelay = 1000; // Delay between firing pulses in milliseconds, 1000 = 1 second
int intMega = 3; // number of additional tags to add to each firing pulse, also known as Megatags; must be between 0 and 3


const unsigned int RECV_PIN = 11;
const unsigned int BAUD_RATE = 9600;

IRsend irsend;
IRrecv irrecv(RECV_PIN);
decode_results results;

int intTeam = -1;
int intPlayer = -1;



void setup() {

  Serial.println(F("Program started."));
  Serial.begin(BAUD_RATE);

  pinMode(13, OUTPUT); // blink the built-in LED to show signal traffic
  irrecv.enableIRIn(); // Start the receiver

}



void dump(const decode_results* results) {

  int count = results->rawlen;
  float resraw[14];
  int resround[14];
  String reschar = "";

  // Check that result consists of 18 intervals (potential Lazer Tag fire signal)
  if (count == 18) {

    // Capture the raw interval results
    for (int i = 1; i < 15; i++) { // skip the first interval as it is random, and skip megatag indicators (last three numbers)
      resraw[i] = (results->rawbuf[i]*USECPERTICK); // retrieve the IR signature for each segment
      resround[i] = round(resraw[i] / 1000); // Round to account for  variation in actual observed intervals
      reschar = reschar + String(resround[i]); // compile a complete string of the signature
    }

    // Check that header conforms to Lazer Tag fire prefix (3000 -6000 3000)
    if (  resround[1] == 3
      &&  resround[2] == 6
      &&  resround[3] == 3
      ) {

        Serial.print(reschar) + " ";

        if (reschar     == "36321212121212") {intTeam = 0; intPlayer = 0;} // unhosted no teams (LTAG mode)

        else if(reschar == "36321212121222") {intTeam = 1; intPlayer = 0;} // unhosted LTX or LTAR with team setting
        else if(reschar == "36321212122212") {intTeam = 2; intPlayer = 0;} // unhosted LTX or LTAR with team setting
        else if(reschar == "36321212122222") {intTeam = 3; intPlayer = 0;} // unhosted LTX or LTAR with team setting

        // Note that hosted CUST games will fill up team/player IDs as needed even though there are no teams;
        //   each blaster recognizes this and records hits even if you are on the same "team" per the code signature
        else if(reschar == "36321222121212") {intTeam = 1; intPlayer = 1;}
        else if(reschar == "36321222121222") {intTeam = 1; intPlayer = 2;}
        else if(reschar == "36321222122212") {intTeam = 1; intPlayer = 3;}
        else if(reschar == "36321222122222") {intTeam = 1; intPlayer = 4;}
        else if(reschar == "36321222221212") {intTeam = 1; intPlayer = 5;}
        else if(reschar == "36321222222212") {intTeam = 1; intPlayer = 6;}
        else if(reschar == "36321222222222") {intTeam = 1; intPlayer = 7;}
        else if(reschar == "36321222221222") {intTeam = 1; intPlayer = 8;}

        else if(reschar == "36322212121212") {intTeam = 2; intPlayer = 1;}
        else if(reschar == "36322212121222") {intTeam = 2; intPlayer = 2;}
        else if(reschar == "36322212122212") {intTeam = 2; intPlayer = 3;}
        else if(reschar == "36322212122222") {intTeam = 2; intPlayer = 4;}
        else if(reschar == "36322212221212") {intTeam = 2; intPlayer = 5;}
        else if(reschar == "36322212222212") {intTeam = 2; intPlayer = 6;}
        else if(reschar == "36322212222222") {intTeam = 2; intPlayer = 7;}
        else if(reschar == "36322212221222") {intTeam = 2; intPlayer = 8;}

        else if(reschar == "36322222121212") {intTeam = 3; intPlayer = 1;}
        else if(reschar == "36322222121222") {intTeam = 3; intPlayer = 2;}
        else if(reschar == "36322222122212") {intTeam = 3; intPlayer = 3;}
        else if(reschar == "36322222122222") {intTeam = 3; intPlayer = 4;}
        else if(reschar == "36322222221212") {intTeam = 3; intPlayer = 5;}
        else if(reschar == "36322222222212") {intTeam = 3; intPlayer = 6;}
        else if(reschar == "36322222222222") {intTeam = 3; intPlayer = 7;}
        else if(reschar == "36322222221222") {intTeam = 3; intPlayer = 8;}

        }

    }

  }



void loop() {

  // Wait for a tag signal from any player to set up the HFE

  Serial.println(F("Ready to receive firing code."));

  do {
    if (irrecv.decode(&results)) {
      dump(&results);
      //Serial.println(intPlayer);
      irrecv.resume();
    }
  } while (intPlayer == -1);

  Serial.println("Hostile Field Emitter activated!");
  Serial.println(intTeam);
  Serial.println(intPlayer);
  delay(3000); // wait 3 seconds to allow player to exit the HFE's range


  // Fire tag signal at specified interval

  do {
    irsend.sendLazerTagFire(intTeam, intPlayer, intMega);
    Serial.println("HFE pulse generated!");
    Serial.println(intTeam);
    Serial.println(intPlayer);
    Serial.println(intMega);
    delay(intDelay);
  } while (1 == 1);



  // Below are the firing signals used by my custom sendLazerTagFire method

  //  irsend.sendLazerTag(int team, int player, int mega);
  irsend.sendLazerTagFire(0, 0, intMega); //  Non-hosted, no teams
  delay(intDelay);
  irsend.sendLazerTagFire(1, 0, intMega); //  Non-hosted, Team 1 fires
  delay(intDelay);
  irsend.sendLazerTagFire(2, 0, intMega); //  Non-hosted, Team 2 fires
  delay(intDelay);
  irsend.sendLazerTagFire(3, 0, intMega); //  Non-hosted, Team 3 fires
  delay(intDelay);

  // Loop through teams 1-3, players 1-8 on each team
  for (intTeam=1; intTeam<=3; intTeam++) {
    for (intPlayer=1; intPlayer<=8; intPlayer++) {
      irsend.sendLazerTagFire(intTeam, intPlayer, intMega);
      delay(intDelay);
      Serial.println(intTeam + " " + intPlayer);
    }
  }



}


*/

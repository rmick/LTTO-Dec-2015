#include <Arduino.h>

//
//
/*
Announce Game
Sent By: Host
Signature Type: Packet
Signature Count: 10-14 (including checksum)

The Announce Game packet is sent by the host at 1.5 second intervals (from start to start) until it receives a Request Join Game packet from a joining tagger.



- Packet Type (9 bits) - FIRST bit must be 0
    0x002 - Announce Custom Lazer Tag Game
    0x003 - Announce Custom Lazer Tag (2 Teams) Game
    0x004 - Announce Custom Lazer Tag (3 Teams) Game
    0x005 - Announce Hide and Seek Game
    0x006 - Announce Hunt the Prey Game
    0x007 - Announce Kings (2 Teams) Game
    0x008 - Announce Kings (3 Teams) Game
    0x009 - Announce Own the Zone Game
    0x00A - Announce Own the Zone (2 Teams) Game
    0x00B - Announce Own the Zone (3 Teams) Game
    0x00C - Announce Special Game
 
- Game ID (8 bits)

- Game length in minutes (8 bits) BCD

- Tags (8 bits) BCD

- Reloads (8 bits) BCD, 0xFF for unlimited

- Shields (8 bits) BCD

- Mega-Tags (8 bits) BCD, 0xFF for unlimited

- Flags 1 (8 bits)
    Neutralize players when tagged 10 times (1 bits)  ************ This is the one for Medic Stations to work *******
    Limited reloads (1 bits)
    Limited mega-tags (1 bits)
    Team Tags (1 bits)
    Medic Mode (1 bits)
    Slow Tags (1 bits)
          Maximum of one tag every other second
    Hunt the Prey (1 bits)
    Hunt direction (1 bits)
          0 - Team 1 hunts first
          1 - Team 2 hunts first)

- Flags 2 (8 bits)
    Contested zones (1 bits)
    Zones can be associated with teams (1 bits)
          "Number of teams" must be greater than 1.
          If this is set to 0, zones associated with teams will not affect players.
    Neutralize players for 15 seconds when tagged (1 bits)
    Supply zones unneutralize players (1 bits)
    Supply zones refill tags (1 bits)
    Hostile zones (1 bits)
          "Supply zones unneutralize players" must be 0.
          "Supply zones refill tags" must be 0.
    Number of teams (2 bits)
    
- Game Name (32 bits) only for packet type 0x00C

- Checksum (9 bits) - FIRST bit must be 1

 * 
 */



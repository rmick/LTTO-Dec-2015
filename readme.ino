#include <Arduino.h>

/*

This is the Start of a Medic Station to work with LTTO/LTX/LTAR (Hasbro/Nerf) laser tag systems

It uses a standard Arduino Uno board and a TFT Touchscreen shield (from Smoke and Wires in New Zealand).
(http://www.smokeandwires.co.nz/electronic-modules/1031463-24-inch-colour-tft-touchscreen-shield-for-arduino.html)

I am a self taught programmer, who has been using Arduinos since early 2015.
I am still learning, so many areas of the code are fairly horrid, but it is slowly evolving.

Credits

BaldWisdom.com
My first code was copied and pasted from his github
https://github.com/osbock/Baldwisdom/blob/master/duino_tag/duino_tag.ino
and includes these credits.
    * Arduino based example of lazertag protocol.
    *     Copyright 2013 Kevin Osborn, Nick Hollander
    *      Free for reuse,just give us a credit (Creative Commons Attribution)
    *         Hook an IR LED up to Digital pin 3 (PWM) for Arduino Uno etc. (look in IRremote library for other platforms.
    *         Uses Ken Shirrif's IRremote library found at: https://github.com/shirriff/Arduino-IRremote
    ----------------------------------
    * lazertag protocol, originally reverse engineered by  Aaron Nabil and documented at 
    * http://web.archive.org/web/20090304155723/http://lasertagparts.com/ltto.htm
    *
    * 3ms On, 6ms Off, 3ms (preamble) followed by 7 bits spaced apart by 2ms. One is represented by 2ms, zero is 1ms
    * In regular lazertag mode, the player and team bits are zero, but the last two bits signify how powerful the blast is.
    * for this example, we show a shot that does one damage and one that does 4 (max)


https://www.baldengineer.com/arduino-f-macro.html
Another Bald person saved the day when I started to run out SRAM in my Uno and got the usual random lockups.
I was just about to upgrade to Mega boards when I found this blog on the F macro,
and 10 minutes later had recovered 40% of my SRAM and the project was back on track.


Ken Sherriff - for his ubiqitous IR library.
Although I no longer use it in the project, it taught me most of what I needed to know to create the current code.
I ditched the library for a number of reasons.
  1)  I needed to use different Pins for the IR_RECEIVE and IR_SEND, as the pins in the library were used for the LCD touch screen.
  2)  I really wanted duplex send/receive, whereas in Ken's library sending turns off receive. Having said that,
      I currently disable interupts durin the sending, due to the dev hardware having the IR LED next to the receiver,
      which meant that the gun triggered it's own receiver. The interupts can be left on once proper hardware is built.


Tag Ferret - the inventor of the LTTO system, whose various blogs and websites provided much needed background information


Lazerswarm - makers of a nice LTTO hosting system, whose WIKI was invaluable for this project.


Dejan Deletic - a great software engineer who answered numerous questions and always pointed me in the right direction.

*/

/*
 *  Connect an IR LED up to Digital pin 3 (PWM) for Arduino Uno
 *  Uses Ken Shirrif's IRremote library found at: https://github.com/shirriff/Arduino-IRremote
 *  NB. When using the RAW command, the last parameter is in kHz, not Hz - so use 38, instead of 38,000
 
 *  Lazer Tag protocol reverse engineered by  Aaron Nabil and documented at 
 *  http://web.archive.org/web/20090304155723/lasertagparts.com/ltto.htm
 *  3ms On, 6ms Off, 3ms (preamble) followed by 7/9 bits spaced apart by 2ms. One is represented by 2ms, zero is 1ms
 *  In non-hosted games the Player and Team ID bits are all zero.
 *  Based on the sample code by Kevin Osborn & Nick Hollander
 */




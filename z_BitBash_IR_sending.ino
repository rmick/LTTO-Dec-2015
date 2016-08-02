#include <Arduino.h>

/*

Adventures In Silicon LTTO


//PINS
int trigger = 2;  //digital pin 2 but also known as interrupt 0
int IR_RECV = 0;  //Pin to receive TTL received IR pulses on
int IRLED = 11;    //Drives the sending IR LED
int magazine = 0;  //reads if magazine inserted - active low - held high by 26k ohm resistor
int rumble_motor = 0;

int carrier1 = 0;
int carrier2 = 0;


unsigned long time_delay_usec_elapsed =0;
unsigned long time_delay_usec_start = 0;
int temp_flash = 1;  

boolean shot_firing = false;    //used as a bool for telling whether we are currently in the process of firing a shot or not


void setup()
{
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  pinMode(IR_RECV, INPUT);
  pinMode(IRLED, OUTPUT);
  pinMode(rumble_motor, OUTPUT);
  pinMode(carrier1, OUTPUT);
  pinMode(carrier2, OUTPUT);
  
  //set up interrupts
//  attachInterrupt(0, fire_shot, FALLING);    //attaches the fire_shot() function to the interrupt 0 which is on pin 2 
//  attachInterrupt(1, receive_shot, FALLING);  //detects signal from photo diodes on PIN 3

  //initialise pins
  digitalWrite(IRLED, LOW);
  digitalWrite(13, HIGH);
  digitalWrite(rumble_motor, 0);

//Setup 38Khz carrier wave
  pinMode(3, INPUT);  //Don't need it
  pinMode(11, OUTPUT);
//Normal mode
  TCCR2A = 0x00;
  bitWrite(TCCR2A, COM2A0, 1);
  bitWrite(TCCR2A, COM2B1, 1);
  bitWrite(TCCR2A, WGM21, 1);
  bitWrite(TCCR2A, WGM20, 1);

  TCCR2B = 0x00;
  bitWrite(TCCR2B, WGM22, 1);
  bitWrite(TCCR2B, CS20, 1);  

  OCR2A = 209;
  OCR2B = 128;  //Doesn't matter

  

 
 


// analogWrite(rumble_motor, 128); 
 
}

void loop()
{

 pinMode(11, INPUT);
 digitalWrite(11, HIGH);
 
 if(digitalRead(2) == LOW) //trigger pulled
 {
 
      //NEW APPROACH - turn off interrupts and send FIRING signals direct
      
      noInterrupts();
 
 
      //on 3ms
      pinMode(11, OUTPUT);
   
      bitWrite(TCCR2B, CS20, 1);
      delayMicroseconds(3000);

      //off 6ms
      pinMode(11, INPUT);
      digitalWrite(11, HIGH);
      delayMicroseconds(6000);

      //on 3ms
      pinMode(11, OUTPUT);
      delayMicroseconds(3000);
//1 
      //off 2ms
      pinMode(11, INPUT);      
      digitalWrite(11, HIGH);
      delayMicroseconds(2000);
            
      //on 1ms
      pinMode(11, OUTPUT);
      delayMicroseconds(1000);
//2      
      //off 2ms
      pinMode(11, INPUT);      
      digitalWrite(11, HIGH);
      delayMicroseconds(2000);
            
      //on 1ms
      pinMode(11, OUTPUT);
      delayMicroseconds(1000);

//3      
      //off 2ms
      pinMode(11, INPUT);      
      digitalWrite(11, HIGH);
      delayMicroseconds(2000);
            
      //on 1ms
      pinMode(11, OUTPUT);
      delayMicroseconds(1000);
//4      
      //off 2ms
      pinMode(11, INPUT);      
      digitalWrite(11, HIGH);
      delayMicroseconds(2000);
            
      //on 1ms
      pinMode(11, OUTPUT);
      delayMicroseconds(1000);
//5      
      //off 2ms
      pinMode(11, INPUT);      
      digitalWrite(11, HIGH);
      delayMicroseconds(2000);
            
      //on 1ms
      pinMode(11, OUTPUT);
      delayMicroseconds(1000);
//6      
      //off 2ms
      pinMode(11, INPUT);      
      digitalWrite(11, HIGH);
      delayMicroseconds(2000);
            
      //on 1ms
      pinMode(11, OUTPUT);
      delayMicroseconds(1000);
//7      
      //off 2ms
      pinMode(11, INPUT);      
      digitalWrite(11, HIGH);
      delayMicroseconds(2000);
            
      //on 1ms
      pinMode(11, OUTPUT);
      delayMicroseconds(1000);

      
      pinMode(11, INPUT);
      digitalWrite(11, HIGH);
        
      interrupts();
      delay(500);
    
      
 }
 

    

}


 */

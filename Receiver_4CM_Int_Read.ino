/*
Beaver's Hobby Channel presents Idiotic Arduino Project #3
Read RC receiver up to 4 channels and display it on 16x2 LCD scren.
This code only works with Arduino Mega.
Pins:
Channel 1 -  53
Channel 2 -  52
Channel 3 -  51
Channel 4 -  50
 
LCD Portion forked Fork from LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 
 
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned long timer[5];
byte last_channel[4];
int input[4];


void setup() {
  lcd.begin(16, 2);

  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT0); // Mega pin 53
  PCMSK0 |= (1 << PCINT1); // Mega pin 52
  PCMSK0 |= (1 << PCINT2); // Mega pin 51
  PCMSK0 |= (1 << PCINT3); // MEga pin 50

}

void loop() {
  
  print();
} 


ISR(PCINT0_vect)
{
  timer[0] = micros();

  // CH1
  if(last_channel[0] == 0 && PINB & B00000001){
    last_channel[0] = 1;
    timer[1] = timer[0];
  }
  else if(last_channel[0] == 1 && !(PINB & B00000001)){
    last_channel[0] = 0;
    input[0] = timer[0] - timer[1];
  }

  // CH2
  if(last_channel[1] == 0 && PINB & B00000010){
    last_channel[1] = 1;
    timer[2] = timer[0];
  }
  else if(last_channel[1] == 1 && !(PINB & B00000010)){
    last_channel[1] = 0;
    input[1] = timer[0] - timer[2];
  }

  // CH3
  if(last_channel[2] == 0 && PINB & B00000100){
    last_channel[2] = 1;
    timer[3] = timer[0];
  }
  else if(last_channel[2] == 1 && !(PINB & B00000100)){
    last_channel[2] = 0;
    input[2] = timer[0] - timer[3];
  }

  // CH1
  if(last_channel[3] == 0 && PINB & B00001000){
    last_channel[3] = 1;
    timer[4] = timer[0];
  }
  else if(last_channel[3] == 1 && !(PINB & B00001000)){
    last_channel[3] = 0;
    input[3] = timer[0] - timer[4];
  }
 
}


void print() {
  lcd.setCursor(0, 0);
  lcd.print("C1:");
  lcd.print(input[0]);
  lcd.print(" "); // padding for 3 digits readout
  lcd.setCursor(9, 0);
  lcd.print("C2:");
  lcd.print(input[1]);
  lcd.print(" "); // padding for 3 digits readout
  
  lcd.setCursor(0, 1);
  lcd.print("C3:");
  lcd.print(input[2]);
  lcd.print(" "); // padding for 3 digits readout
  lcd.setCursor(9, 1);
  lcd.print("C4:");
  lcd.print(input[3]);
  lcd.print(" "); // padding for 3 digits readout
}

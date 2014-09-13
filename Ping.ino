/*this is a recoding of the ping function as the previous function
contained issues with being unable to get the data accurately and
also it would be really hard to completely recode from scratch.
This code will use the Ping library taken from an online source
as the basis to simplify the program code.

  Author:
  Christopher Chun-Hung Ho
  z5019205
  
  History:
  v1.2  13/9/14 Another edit to use the NewPing library that is written
                online. The sensors work, will need to specifically
                calibrate it during the test runs.
  v1.1  11/9/14 Rewrote ping test code
  v1.0  8/9/14  Wrote code to test ping function.
*/

#include <NewPing.h>
/*http://playground.arduino.cc/Code/NewPing
This is a ping library written by Tim Eckel to use the
HC-S04 ultrasound sensor. The library can return a median
of pings as well as time in microseconds*/


//Constant
#define SOS 0.34029                //Speed of Sound in mm/us

//Distance pin
const int Trigger = A1;        //Triggerger pin as we ran out out digital pins
const int LRec = 30;           //Left US receiver
const int CRec = 32;           //Centre US receiver
const int RRec = 34;           //Right US receiver
const int Ltrig = 31;           //Left US receiver
const int Ctrig = 33;           //Centre US receiver
const int Rtrig = 35;           //Right US receiver


//vairables
float Left;
float Right;
float Centre;


NewPing leftPing(Ltrig, LRec, 125);
NewPing cenPing(Ctrig, CRec, 125);
NewPing rightPing(Rtrig, RRec, 125);


int aval;
//-----------------------------------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------------------------------
//setup function
void setup(){
  
  //set up serial
  Serial.begin(9600);
  delay(500);
  Serial.println("loaded");
  
  
  //Set up output pins
  pinMode(Trigger, OUTPUT);
  pinMode(Ltrig, OUTPUT);
  pinMode(Ctrig, OUTPUT);
  pinMode(Rtrig, OUTPUT);
  //set up input pins
  pinMode(LRec, INPUT);
  pinMode(CRec, INPUT);
  pinMode(RRec, INPUT);
  
}

//loop function
void loop (){                                      //to test the ping function.
  
  aval = micros();
  
  Left = leftPing.ping_median() * SOS;
  Centre = cenPing.ping_median() * SOS;
  Right = rightPing.ping_median() * SOS;
  
  Serial.print(micros()-aval);
  Serial.print(":");
  Serial.print(Left);
  Serial.print("-");
  Serial.print(Centre);
  Serial.print("-");
  Serial.println(Right);
  
  delay(1000);
  
}

/*this is a recoding of the ping function as the previous function
contained issues with being unable to get the data accurately and
also it would be really hard to completely recode from scratch.
This code will use the Ping library taken from an online source
as the basis to simplify the program code.

  Author:
  Christopher Chun-Hung Ho
  z5019205
  
  History:
  
  v1.1  11/9/14 Rewrote ping test code
  v1.0  8/9/14  Wrote code to test ping function.
*/

#include <NewPing.h>


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

float Ping(int trig, int echo);


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
void loop (){                                      //loop has been simplified as it may change to manual control
  /*
  timer = micros();
  
  Left = Ping(LRec);
  Centre = Ping(CRec);
  Right = Ping(RRec);
  
  Serial.print(micros()-timer);
  Serial.print(":");
  Serial.print(Left);
  Serial.print("-");
  Serial.print(Centre);
  Serial.print("-");
  Serial.println(Right);*/
  
  aval = micros();
  
  Left = Ping(Ltrig, LRec);
  Centre = Ping(Ctrig, CRec);
  Right = Ping(Rtrig, RRec);
  
  Serial.print(micros()-aval);
  Serial.print(":");
  Serial.print(Left);
  Serial.print("-");
  Serial.print(Centre);
  Serial.print("-");
  Serial.println(Right);
  
  delay(1000);
  
}


float Ping(int trig, int echo){
  float distance = 0;                       //declare internal variables
  float average = 0;
  int timer[5];
  int temp;
  for(int i = 0; i < 5; i++){
    timer[i] = ;             //calculate ping difference
    average = average + float(timer[i]);                    //adds data to a total
  }
  average /= 5;                             //averages data
  distance = average * SOS;                 //convert to distance
  return distance;                          //return distance in mm
}

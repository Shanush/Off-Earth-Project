/* 
  Description:
  This program was written based off the system analysis written by the
  two other members of my team.
  The program is a fully automated system with a complete set of coded
  instructions for the drill.
  This version however will have some lines of code that are specific for
  debugging which may or may not be found in the final piece of code.
  Version 1.3 is a code for which the defined values may be wrong as we
  have not done testing and do not have the exact values for which the
  vehicle can run at, the code is purely theoretical at the moment.
  
  Author:
  Christopher Chun-Hung Ho
  z5019205
  
  History:
  
  v1.4  10/9/14 added manual control to the code.
  v1.3  5/9/14  initial implementation
		changed ClampControl to boolean
  v1.2  3/9/14  changed the format of the layout to Arduino style
  v1.1  28/8/14	outline the main parts
  v1.0  21/8/14 Write out the basics of the code
*/


//-----------------------------------------------------------------------------------------------------
//Define
//-----------------------------------------------------------------------------------------------------
//Time delay
#define ROTA_TIME 700              //time to rotate ball
#define READ_DELAY 100             //time between read of analogue pins
//Ball location
#define DISPLACEMENT 500.0         //displacement of sensor and ball
#define SPEED 150.0                //define speed of vehicle as mm/ms
//Power Level definition
#define VFS 700                    //voltage minimum recieve to stop forward
#define VBS 400                    //voltage minimum recieve to stop backward movement
                                   //this voltage will also stop clamping
//Constant
#define SOS 0.34029                //Speed of Sound in mm/us



//-----------------------------------------------------------------------------------------------------
//Const pins
//-----------------------------------------------------------------------------------------------------

/*Interrupt is on pin 2, this pin is referred to as interupt 0 and will not be
  used within the control of the vehicle as it is an interrupt pin to shutdown
  the machine after it has been initiated.
*/
//drill pin
const int driPin = 9;          //drill pin define
const int dForPin = 8;         //move drill forward
const int dBakPin = 7;         //move drill backward

//clamp pin
const int cloPin = 4;          //opens the clamp
const int clcPin = 3;          //closes the clamp
const int rota = 5;           //rotate ball motor

//Distance pin
const int Trigger = A1;        //Triggerger pin as we ran out out digital pins
const int LRec = A2;           //Left US receiver
const int CRec = A3;           //Centre US receiver
const int RRec = A4;           //Right US receiver

//Motor Pins
const int motPin1 = 11;        //motor pin 1 for logic gate
const int motPin2 = 10;        //motor pin 2 for logic gate
const int motEnable = 12;      //enable both motors.

//Analogue Pins
const int FBStp = 0;           //forward backward check

//manual overide
const int Override = 0;


//-----------------------------------------------------------------------------------------------------
//Declaring
//-----------------------------------------------------------------------------------------------------
//Variables
int aval;                     //read value of analogue pin
int movTime;                  //time to move.
int state;                    //state of the device

//Functions                   Pretty much self explanatory, their use is written in the Functions Section
void clampCtrl(boolean dir, int timer);
void drillCtrl();
void rotateCtrl();
void manualCtrl();
void shutdown();
void locate();
void forward();
float Ping(int echo);



//-----------------------------------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------------------------------
//setup function
void setup(){

  Serial.begin(9600);                              //initialise serial communications
  
  //Set up output pins
  pinMode(driPin, OUTPUT);
  pinMode(dForPin, OUTPUT);
  pinMode(dBakPin, OUTPUT);
  pinMode(cloPin, OUTPUT);
  pinMode(clcPin, OUTPUT);
  pinMode(rota, OUTPUT);
  pinMode(Trigger, OUTPUT);
  pinMode(motPin1, OUTPUT);
  pinMode(motPin2, OUTPUT);
  pinMode(motEnable, OUTPUT);
  //set up input pins
  pinMode(LRec, INPUT);
  pinMode(CRec, INPUT);
  pinMode(RRec, INPUT);
  
  
  attachInterrupt(Override, manualCtrl, CHANGE);   //switch to manual control the vehicle
  
  noInterrupts();                                  //disable interrupts as we wait for start signal
  
  Serial.println("Initialisation Complete");
  Serial.println("Input any key to begin");
  
  //Wait for start signal
  while(Serial.available()==0){                    //wait until it receives start signal
  }
  
  interrupts();                                    //enable the interrupt
  
  locate();                                        //make sure facing the ball
  
  forward();                                       //move to the ball.
  
  clampCtrl(true, 0);                              //clamp the ball, close clamps
}

//loop function
void loop (){                                      //loop has been simplified as it may change to manual control
  drillCtrl();                                     //Run the ball drilling function.
  //clampCtrl(false, 5);                               loosen calmp a bit (maybe).
  rotateCtrl();                                    //Run ball rotating process.
  //clampCtrl(true, 0);                               retighten the clamp.
}



//-----------------------------------------------------------------------------------------------------
// subroutines
//-----------------------------------------------------------------------------------------------------

float Ping(int echo){
  
  float distance = 0;                       //declare internal variables
  float average = 0;
  float timer[5];
  
  for(int i = 0; i < 5; i++){
    
    digitalWrite(Trigger, LOW);
    delayMicroseconds(2);                   //clean Triggerger signal
    digitalWrite(Trigger, HIGH);            
    delayMicroseconds(10);                  //Triggerger signal for 10us to make sure the device knows to Triggerger
    digitalWrite(Trigger, LOW);             //stop Triggerger
    
    aval = micros();                        //read time since device began functioning
    
    while(digitalRead(echo)==HIGH){         //pause until no return ping heard
    }
    
    timer[i] = micros() - aval;             //calculate ping difference
    average += timer[i];                    //adds data to a total
  }
  
  average /= 5;                             //averages data
  distance = average * SOS;                 //convert to distance
  
  return distance;                          //return distance in mm
}

void forward(){
  
  int timer;                                  //timer that will check how long motors run for
  float distance;
  
  /*motor only works with on and off so it must be able to run the motor so that it stops in time for the ball.
  need to know location of sensor and distance from the edge the ball will be loacted.
  */
  
  distance = Ping(CRec);                     //find distance using centre sensor
  
  timer = (distance - DISPLACEMENT) / SPEED; //calculate time it needs to run at
  
  
  digitalWrite(motPin1, HIGH);               //Start all motors
  digitalWrite(motPin2, HIGH);
  digitalWrite(motEnable, HIGH);
  
  delay(timer);                              //delay to stop using precalculated time
  
  digitalWrite(motEnable, LOW);              //Stop all motors]
}

void locate(){
  
  //declare local variables
  float Left = 0;
  float Right = 0;
  int SpeRight = 0;
  int SpeLeft = 0;
  
  Left = Ping(LRec);                                        //get current readings of distance
  Right = Ping(RRec);

  //loop until facing the ball properly
  do
  {
    //drive logic 
    if(Left > Right){                                       //decide which way to rotate
    
      digitalWrite(motPin1, LOW);                           //rotate counter clockwise with logic (0|1)
      digitalWrite(motPin2, HIGH);
      digitalWrite(motEnable, HIGH);
    } else {
      
      digitalWrite(motPin1, HIGH);                          //rotate clockwise with logic (1|0)
      digitalWrite(motPin2, LOW);
      digitalWrite(motEnable, HIGH);
    }      
    
    Left = Ping(LRec);                                      //get new left values
    Right = Ping(RRec);                                     //get new right values
  } while(Left > (Right + 5) || Left < (Right - 5));        //variance of +-5mm as the sensors come with an accuracy of 2%.
  
  digitalWrite(motEnable, LOW);                             //Stop all motors
}


void clampCtrl(boolean dir, int timer){
  
  if (dir == true){                        //decide whether to open or close (1 to close, 0 to open).
  
    digitalWrite (clcPin, HIGH);           //Start closing clamps
    /*method to detect closed*/
    digitalWrite (clcPin, LOW);            //Stop closing clamps
  
  } else {
    
    digitalWrite (cloPin, HIGH);           //Start opening clamps
    
    if (timer > 0){
      
      delay(timer);                        //movement time given
      
    } else {
      
      while (analogRead(FBStp) < VBS){     //check that the switch has been hit
        delay (READ_DELAY);                //time between checking for whether the switch has been hit
      }
      
    }
    
    digitalWrite (cloPin, LOW);            //Stop opening clamps 
  }
}

void drillCtrl(){
  
  digitalWrite(driPin, HIGH);               //drill on
  delay(100);                               //give drill time to pick up speed
  digitalWrite(dForPin, HIGH);              //drill forward
  
  while (analogRead (FBStp) <= VFS){        //wait until the stop switch has been hit
    delay (READ_DELAY);                     //time between checking for whether the switch has been hit
  }
  
  digitalWrite (dForPin, LOW);              //stop forward movement of the drill
  delay (50);                               //pause for moment
  digitalWrite (dBakPin, HIGH);             //move drill backwards
  
  aval = analogRead(FBStp);                 //read the value of the voltage
  
  while (aval > VFS || aval < VBS){         //check that the switch has been hit
    delay (READ_DELAY);                     //time between checking for whether the switch has been hit
    aval = analogRead(FBStp);               //read the value of the voltage
  }
  
  digitalWrite (dBakPin, LOW);              //stop backward movement
  digitalWrite (driPin, LOW);               //Turn off drill
}

void rotateCtrl(){
  
  digitalWrite (rota, HIGH);                //Turn the motor to turn the ball on
  delay (ROTA_TIME);                        //rotate the ball for how many milliseconds.
  digitalWrite (rota, LOW);                 //Stop turning the ball
}

//Manual Control of the device
void manualCtrl(){
  char command;
  
  //turn off all running devices
  digitalWrite(driPin, LOW);
  digitalWrite(dForPin, LOW);
  digitalWrite(dBakPin, LOW);
  digitalWrite(cloPin, LOW);
  digitalWrite(clcPin, LOW);
  digitalWrite(rota, LOW);
  digitalWrite(Trigger, LOW);
  digitalWrite(motEnable, LOW);
  
  //turn off interrupts
  noInterrupts();
  
  Serial.println("Entered manual control mode");
  
  //load command in
  command = Serial.read();
  
  while(command!='r'){
    
    switch (command){
      
      case 'w':
        
        Serial.println("Moving Forward");                                //acknowledge command received
        
        digitalWrite(motPin1, HIGH);                                     //set motor control
        digitalWrite(motPin2, HIGH);
        digitalWrite(motEnable, HIGH);                                   //enable motor
        
        while(Serial.read()!='s'){                                       //wait for stop signal
        }
        
        digitalWrite(motEnable, LOW);                                    //disable motor
        break;
        
      case 'a':
        
        Serial.println("Rotating CounterClockwise");                     //acknowledge command received
        
        digitalWrite(motPin1, LOW);                                      //set motor values
        digitalWrite(motPin2, HIGH);
        digitalWrite(motEnable, HIGH);                                   //enable motor
        
        while(Serial.read()!='s'){                                       //wait for stop signal
        }
        
        digitalWrite(motEnable, LOW);                                    //stop motor
        break;
        
      case 'd':
      
        Serial.println("Rotating Clockwise");                            //acknowledge command received
        
        digitalWrite(motPin1, HIGH);                                     //set the motors to correct position
        digitalWrite(motPin2, LOW);
        digitalWrite(motEnable, HIGH);                                   //enable motors
        
        while(Serial.read()!='s'){                                       //wait for stop signal
        }
        
        digitalWrite(motEnable, LOW);                                    //stop motor
        break;
        
      case 's':
        
        Serial.println("Moving Backward");                               //show command received
        
        digitalWrite(motPin1, LOW);                                      //set the motors to correct position
        digitalWrite(motPin2, LOW);
        digitalWrite(motEnable, HIGH);                                   //enable motors
        
        while(Serial.read()!='s'){                                       //wait for stop signal
        }
        
        digitalWrite(motEnable, LOW);                                    //stop motor
        break;
        
      case 'u':
        
        digitalWrite(driPin, HIGH);                                      //turn drill on
        
        Serial.println("drill turned on");                               //acknowledge command
        
        break;
        
      case 'i':
        
        digitalWrite(driPin, LOW);                                       //turn drill off
        
        Serial.println("Drill turned off");                              //acknowledge command
        
        break;
        
      case 'o':
        
        Serial.println("Drill moving forward");                          //acknowledge command
        
        digitalWrite(dForPin, HIGH);                                     //move drill forward
        
        while(analogRead(FBStp) <= VFS && Serial.read() !=' s'){         //wait until the stop switch has been hit or command to stop is given
          delay(10);                                                     //delay between reading
        }
        
        digitalWrite(dForPin, LOW);                                      //stop moving forward
        break;
        
      case 'p':
        
        Serial.println("Drill moving backward");                         //acknowledge command
        
        digitalWrite(dBakPin, HIGH);                                     //move drill back
        
        aval = analogRead(FBStp);                                        //read value to check whether the drill has moved back enough
        
        while((aval > VFS || aval < VBS) && Serial.read() !=' s'){       //wait until the stop switch has been hit or command to stop is given
          delay(10);                                                     //delay between reading
          aval = analogRead(FBStp);
        }
        
        digitalWrite(dBakPin, LOW);                                      //stop moving backward
        break;
        
      case 'j':
        
        Serial.println("Opening clamps");                                //acknowledge command
        
        digitalWrite(cloPin, HIGH);                                      //start opening the clamps
        //condition to stop opening
        digitalWrite(cloPin, LOW);                                       //stop opening the clamps
        break;
        
      case 'k':
        
        Serial.println("closing clamps");                                //acknowledge command
        
        digitalWrite(clcPin, HIGH);                                      //start closing the clamps
        //condition to stop clamp closing
        digitalWrite(clcPin, LOW);                                       //stop closing the clamps
        break;
        
      case 'l':
        
        Serial.println("Rotating ball");                                 //acknowledge command
        
        digitalWrite(rota, HIGH);                                        //start rotating the ball
        
        delay(ROTA_TIME);                                                //delay to rotate the ball
        
        digitalWrite(rota, LOW);                                         //stop rotating the ball
        break;
        
      case 'z':
        
        Serial.println("Are you sure you wish to shutdown? (Y/anything else to cancel)"); //acknowledge input and give options
        
        while(Serial.available() == 0){                                  //wait until receive input
        }
        
        if(Serial.read() == 'Y'){                                        //check for yes to shutdown
          shutdown();
        }
        
        break;
      
      default:
        
        break;                                                           //do nothing
        
    }
    
    while(Serial.available()==0){                                        //wait for next command
    }
    
    command = Serial.read();                                             //read command
    
  }
  
  switch(state){
    case 0:
      
      break;
    case 1:
      
      break;
  }
  
  interrupts();
  
  return;
} 

//reset the machine and stop movement
void shutdown(){
  
  aval = analogRead(FBStp);                 //read the value of the voltage
  
  if(aval > VFS || aval < VBS){             //check if drill is all the way out, if not, pull out
  
    digitalWrite(driPin, HIGH);             //drill on
    delay(100);                             //give drill time to pick up speed
    digitalWrite(dBakPin, HIGH);            //move drill backwards
    
    aval = analogRead(FBStp);               //read the value of the voltage
    
    while(aval > VFS || aval < VBS){        //check that the switch has been hit
      delay(READ_DELAY);                    //time between checking for whether the switch has been hit
      aval = analogRead(FBStp);             //read the value of the voltage
    }
    
    digitalWrite(dBakPin, LOW);             //stop backward movement
    digitalWrite(driPin, LOW);              //Turn off drill
  }
  
  clampCtrl(false, 0);                      //open clamps
  
  while(0!=1){                              //loops infinitely to stop the machine
  }
}



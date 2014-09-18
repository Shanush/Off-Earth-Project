/* 
  Description:
  Demonstration of vehicle movement.
  
  Author:
  Christopher Chun-Hung Ho
  z5019205
  
  History:
  
  v1.1  18/9/14 Added manual control to it.
  v1.0  13/9/14 Demo Created
*/

//temporary
#define FORWARD 1
#define LEFT 2
#define RIGHT 3
#define STOP 4

const int motPin1 = 11;        //motor pin 1 for logic gate
const int motPin2 = 10;        //motor pin 2 for logic gate

void manualCtrl();
void moveCtrl(int dir);

//manual overide
const int Override = 0;


void setup(){

  pinMode(motPin1, OUTPUT);
  pinMode(motPin2, OUTPUT);
  
  attachInterrupt(Override, manualCtrl, CHANGE);   //switch to manual control the vehicle
  
  noInterrupts();                                  //disable interrupts as we wait for start signal
  
  Serial.println("Initialisation Complete");
  Serial.println("Input any key to begin");
  
  //Wait for start signal
  while(Serial.available()==0){                    //wait until it receives start signal
  }
  
  interrupts();                                    //enable the interrupt
  

  delay(1000);
  
  digitalWrite(motPin1, HIGH);
  digitalWrite(motPin2, HIGH);
  delay(4000);
  digitalWrite(motPin1, HIGH);
  digitalWrite(motPin2, LOW);
  delay(4000);
  digitalWrite(motPin1, LOW);
  digitalWrite(motPin2, HIGH);
  delay(4000);
  digitalWrite(motPin1, LOW);
  digitalWrite(motPin2, LOW);
}

void loop(){
}

void moveCtrl(int dir){
	switch(dir){
		case FORWARD:
			digitalWrite(motPin1, HIGH);               //Start all motors
  		digitalWrite(motPin2, HIGH);
  		break;
  	case LEFT:
  		digitalWrite(motPin1, LOW);               //Start all motors
  		digitalWrite(motPin2, HIGH);
  		break;
  	case RIGHT:
  		digitalWrite(motPin1, HIGH);               //Start all motors
  		digitalWrite(motPin2, LOW);
  		break;
  	case STOP:
  		digitalWrite(motPin1, LOW);               //Start all motors
  		digitalWrite(motPin2, LOW);
  		break;
  }
}

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
  moveCtrl(STOP);
  
  //turn off interrupts
  noInterrupts();
  
  Serial.println("Entered manual control mode");
  
  //load command in
  command = Serial.read();
  
  while(command!='r'){
    
    switch (command){
      
      case 'w':
        
        Serial.println("Moving Forward");                                //acknowledge command received
        
        moveCtrl(FORWARD);                                    					 //set motor control
        
        while(Serial.read()!='s'){                                       //wait for stop signal
        }
        
        moveCtrl(STOP);                                    							 //stop motor
        break;
        
      case 'a':
        
        Serial.println("Rotating CounterClockwise");                     //acknowledge command received
        
        moveCtrl(LEFT);                                      						 //set motor values
        
        while(Serial.read()!='s'){                                       //wait for stop signal
        }
        
        moveCtrl(STOP);                                    							 //stop motor
        break;
        
      case 'd':
      
        Serial.println("Rotating Clockwise");                            //acknowledge command received
        
        moveCtrl(RIGHT);                                      					 //set motor values
        
        while(Serial.read()!='s'){                                       //wait for stop signal
        }
        
        moveCtrl(STOP);                                    							 //stop motor
        break;
        
      
      default:
        
        break;                                                           //do nothing
        
    }
    
    while(Serial.available()==0){                                        //wait for next command
    }
    
    command = Serial.read();                                             //read command
    
  }
  
  interrupts();
  
  return;
} 

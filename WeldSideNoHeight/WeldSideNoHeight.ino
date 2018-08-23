
#include <AccelStepper.h>

// DEFINING PINS ON THE MEGA
int XmotorPull = 8;    // Pull Pin on the X-Axis Motor
int XmotorDir = 9;     // Direction Pin on the X-Axis Motor
int ZmotorPull = 10;   // Pull Pin on the Z-Axis Motor
int ZmotorDir = 11;    // Direction Pin on the Z-Axis Motor

int SwXMax = 26;  // Pin for endstop switch X Axis Max Extention
int SwXMin = 22;  // Pin for endstop switch X Axis Min Extention
int SwZMax = 30;  // Pin for endstop switch Z Axis Max Extention

int SwXMaxState =0;  // Holds state of endstop switches
int SwXMinState =0;  // Holds state of endstop switches
int SwZMaxState =0;  // Holds state of endstop switches

//DEFINING MOTORS
AccelStepper XStep(AccelStepper::FULL2WIRE, XmotorPull, XmotorDir);
AccelStepper ZStep(AccelStepper::FULL2WIRE, ZmotorPull, ZmotorDir);

// GLOVBAL MOTOR SPEEDS
int XMaxSpeed = 100000;
int ZMaxSpeed = 10000;


//COURSE FINE MOTOR MOVMENT
int XStepCount=200;
int ZStepCount=200;

int XTarget =0;  // where we want the X axis to be
int ZTarget =0;  // where we want the Z axis to be

int oscFreq = 5000;     // how fast to move oscilation
int oscDist = 500;   // distance to move on each oscilation
int Osc =0;          // wether to osilate of not
int dir =0;          // direction to oscilate 


void setup(){
  
  pinMode(SwXMax, INPUT);  // Define endstop switches as inputs
  pinMode(SwXMin, INPUT);
  pinMode(SwZMax, INPUT);


  XStep.setCurrentPosition(0);
  ZStep.setCurrentPosition(0);
  XStep.setMaxSpeed(XMaxSpeed);
  XStep.setAcceleration(10000);
  ZStep.setMaxSpeed(ZMaxSpeed);
  ZStep.setAcceleration(15000);

  
  
  Serial.begin(115200);  // Start talking to RasberryPi
  delay(500);
  
}

void loop(){
    
  checkEndStop();  // check stop switches
  Oscilate();
  XStep.run();
  ZStep.run();
  
  // move the steppers twards the goal

 
}



void checkEndStop(){
  SwXMaxState = digitalRead(SwXMax);  // get readings from endstop switches
  SwXMinState = digitalRead(SwXMin);
  SwZMaxState = digitalRead(SwZMax);
  
  // X Max switch is triggered
  if(SwXMaxState == HIGH){
     SwXMaxState = digitalRead(SwXMax);
       if(SwXMaxState == HIGH){
         XStep.move(200);
       }   
  }
   else if(SwXMinState == HIGH){
     SwXMinState = digitalRead(SwXMin);
       if(SwXMinState == HIGH){
         XStep.move(-200);
       }   
  }
  
  if(SwZMaxState == HIGH){
     SwZMaxState = digitalRead(SwZMax);
       if(SwZMaxState == HIGH){
         ZStep.move(-200);
       }   
  }
  
}


void Oscilate(){
  int Xgoal = XStep.targetPosition();
  int Xstate = XStep.currentPosition();

  
 

  if(Xstate == Xgoal && Osc ==1){
  
   
    if(dir == 0){
      XStep.moveTo(Xstate +oscDist);
      dir =1;
     }
     else{
       XStep.moveTo(Xstate -oscDist);
       dir =0;
     }
  }
}


void serialEvent(){

  int cmd = Serial.read();
  
  switch (cmd) {
    
    
    case 'A':
      if(XStep.targetPosition() < XStep.currentPosition()){
         XStep.moveTo(XStep.currentPosition());     
      }
      XStep.moveTo(XStep.targetPosition() + XStepCount);
    break;
    
    
    case 'D':
      if(XStep.targetPosition() > XStep.currentPosition()){
         XStep.moveTo(XStep.currentPosition());
      }
      XStep.moveTo(XStep.targetPosition() - XStepCount);
    break;
    
    
    case 'W':
      if(ZStep.targetPosition() < ZStep.currentPosition()){
         ZStep.moveTo(ZStep.currentPosition());
      }
      ZStep.moveTo(ZStep.targetPosition() + ZStepCount);
    break;
    
    
    case 'S':
      if(ZStep.targetPosition() > ZStep.currentPosition()){
           ZStep.moveTo(ZStep.currentPosition());
        }
      ZStep.moveTo(ZStep.targetPosition() - ZStepCount);
    break;
    
    
    case 'X':
      if(XStepCount ==200){
        XStepCount = 100;
        break;
      }
      XStepCount = 200;
    break;
    
    
    case 'Z':
      if(ZStepCount == 200){
        ZStepCount = 100;
        break;
      }
      ZStepCount = 200;
    break;
    
    
    case 'G':{
      int input = Serial.parseInt();
      while(Serial.peek() == 'G'){
        input = Serial.parseInt();
      }
      long val = (1024-input);
      //I CHANGED THIS to 100 was 70 
      val = val *100; 
      if( val < 5000){
        val = 5000;
      }
      XStep.setAcceleration(val);
    }
    break;
    
    
    case 'H':{
      int input = Serial.parseInt();
      while(Serial.peek() == 'H'){
        input = Serial.parseInt();
      }
      long val = (1024 -input) *2.0;
       if( val < 200){
         Osc =0;
      }
      else{
        Osc =1;
        oscDist = val;
      }
    }
    break;
    
  return;
  }
  
}
  


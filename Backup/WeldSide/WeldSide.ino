
#include <AccelStepper.h>

// DEFINING PINS ON THE MEGA
int XmotorPull = 8;    // Pull Pin on the X-Axis Motor
int XmotorDir = 9;     // Direction Pin on the X-Axis Motor
int ZmotorPull = 10;   // Pull Pin on the Z-Axis Motor
int ZmotorDir = 11;    // Direction Pin on the Z-Axis Motor

int Sonic1 = 7;  // Pin to activate ultrasonic sensor 1
int Sonic2 = 6;  // Pin to activate ultrasonic sensor 2

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

// GLOBAL OFFSET VALUES
int sonicSensorOffset = 500;       // number of mm the sensor is offest from the welder head
int desiredWelderOffset = 10;      // number of mm we want the welder above the meterial.    

// HOLDER FOR RANGE READING 
int current_range_reading = -1;     // hold the place of a Usonic range reading
int advReading = 0;

//COURSE FINE MOTOR MOVMENT
int XStepCount=200;
int ZStepCount=200;

int XTarget =0;  // where we want the X axis to be
int ZTarget =0;  // where we want the Z axis to be

int heightFreq = 0;  // how often to check height
int oscFreq = 5000;     // how fast to move oscilation
int oscDist = 500;   // distance to move on each oscilation
int Osc =0;          // wether to osilate of not
int dir =0;          // direction to oscilate 


void setup(){
  
  pinMode(Sonic1, OUTPUT);  // set the trigger pin for the ultrasonic to an output on the mega.
  pinMode(Sonic2, OUTPUT);  // set the trigger pin for the ultrasonic to an output on the mega.

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
  Serial2.begin(9600);   // Start Talking to  ultrasonic sensor 1
  Serial3.begin(9600);   // Start Talking to  ultrasonic sensor 2
  
  delay(500);
  
}

void loop(){
    
  checkEndStop();  // check stop switches

  heightControl();

  Oscilate();

  XStep.run();
  ZStep.run();
  
  // move the steppers twards the goal

 
}
void alternateloop(){
  checkEndStop();
  Oscilate();
  RemoteCommand();
  XStep.run();
  return;
}


void serialEvent() {
    RemoteCommand();   // Get input from the remote
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


void RemoteCommand(){
  if (Serial.available() > 0) {
    int cmd = Serial.read();
    
    switch (cmd) {
      case 'A':
        if(XStep.targetPosition() < XStep.currentPosition()){
           //XStep.setCurrentPosition(0);
           XStep.moveTo(XStep.currentPosition());
           
        }
        XStep.moveTo(XStep.targetPosition() + XStepCount);
      break;
      case 'D':
        if(XStep.targetPosition() > XStep.currentPosition()){
           //XStep.setCurrentPosition(0);
           XStep.moveTo(XStep.currentPosition());
        }
        XStep.moveTo(XStep.targetPosition() - XStepCount);
      break;
      case 'W':
        if(ZStep.targetPosition() < ZStep.currentPosition()){
           //ZStep.setCurrentPosition(0);
           ZStep.moveTo(ZStep.currentPosition());
        }
        ZStep.moveTo(ZStep.targetPosition() + ZStepCount);
        sonicSensorOffset ++;
      break;
      case 'S':
        if(ZStep.targetPosition() > ZStep.currentPosition()){
             //ZStep.setCurrentPosition(0);
             ZStep.moveTo(ZStep.currentPosition());
          }
        ZStep.moveTo(ZStep.targetPosition() - ZStepCount);
        sonicSensorOffset --;
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
        int zz = Serial.parseInt();
        while(Serial.peek() == 'G'){
          zz = Serial.parseInt();
        }
        long val = (1024-zz);
        
        val = val *70;
        //val = val * 0.4;
        //val = pow(val,2);
        
        if( val < 5000){
          val = 5000;
        }
        XStep.setAcceleration(val);
      }
      break;
      case 'H':{
        int yy = Serial.parseInt();
        while(Serial.peek() == 'H'){
          yy = Serial.parseInt();
        }
        long val = (1024 -yy) *2.0;
         if( val < 200){
           Osc =0;
        }
        else{
          Osc =1;
          oscDist = val;
        }
      }
      break;
    //default: 
    return;
    }
  
  }
  
}
  
void heightControl(){
  heightFreq++;
  if(heightFreq == 2000){
    heightFreq =0;
    int Zgoal = ZStep.targetPosition();
    int Zstate = ZStep.currentPosition();
  
    if(Zstate == Zgoal){
      advReading = AdvSonicReading() - sonicSensorOffset;
      Serial.println(advReading);
      if(advReading > -250 && advReading < 250){
        if(advReading > 2){
           ZStep.moveTo(ZStep.targetPosition() - (200 * 0.5 * advReading));
        }
        if(advReading < -2){
           ZStep.moveTo(ZStep.targetPosition() + (200 * 0.5* advReading * -1));
        }
      }
      else{
        Serial.println("Height Adjust Error");
      }
    }
  }
}

double sum =0;
double AdvSonicReading(){
     sum =0;
     int j=0;
     for(int i = 0;i<7;i++){
       int one = getSonicReadingOne();
       int two = getSonicReadingTwo();
       if(one != -1){
         sum += one;
         j++;
       }
       if(two != -1){
         sum+=two;
         j++;
       }
     }
     if(j >5){
       return round(sum/j);
     }
     return -9999;
}
 


// Function that gathers one reading from Ultrasonic 1
int getSonicReadingOne(){
   digitalWrite(Sonic1, HIGH);
   //delay(50);
   waitForSensor();
     if (Serial2.available() > 5){  // one complete range, 6 characters long, is stored in the RX buffer.
          int inByte = Serial2.read(); // Examine the first stored character and decide what to do.
          if( inByte == 'R' ){ // "R" character 
            int thousands = (Serial2.read() - '0') * 1000; // Take and convert each range digit to human-readable integer format.
            int hundreds = (Serial2.read() - '0') * 100;
            int tens = (Serial2.read() - '0') * 10;
            int units = (Serial2.read() - '0') * 1;
            int cr = Serial2.read(); // clear out the buffer
            // Assemble the digits into the range integer.
            current_range_reading = thousands + hundreds + tens + units;
            
            if(current_range_reading == 300){         //This is the minimum reading for the HRXL MaxSonar, not the actual distance
              Serial.println("S1 too close!");
              digitalWrite(Sonic1, LOW);
              return -1;
            }
            else if(current_range_reading == 1000){   //This is the max reading for the HRXL MaxSonar, not the actual distance
              Serial.println("S1 too far!");
              digitalWrite(Sonic1, LOW);
              return -1;
            }
            else{
              digitalWrite(Sonic1, LOW);
              return current_range_reading;
            }
          }
     }
     else{
       digitalWrite(Sonic1, LOW);
       Serial.println("S1 Reading failed");
       return -1;
     }  
}

// Function that gathers one reading from Ultrasonic 2
int getSonicReadingTwo(){
   digitalWrite(Sonic2, HIGH);
   //delay(50);
   waitForSensor();
     if (Serial3.available() > 5){  // one complete range, 6 characters long, is stored in the RX buffer.
          int inByte = Serial3.read(); // Examine the first stored character and decide what to do.
          if( inByte == 'R' ){ // "R" character 
            int thousands = (Serial3.read() - '0') * 1000; // Take and convert each range digit to human-readable integer format.
            int hundreds = (Serial3.read() - '0') * 100;
            int tens = (Serial3.read() - '0') * 10;
            int units = (Serial3.read() - '0') * 1;
            int cr = Serial3.read(); // clear out the buffer
            // Assemble the digits into the range integer.
            current_range_reading = thousands + hundreds + tens + units;
            
            if(current_range_reading == 300){         //This is the minimum reading for the HRXL MaxSonar, not the actual distance
              Serial.println("S2 too close!");
              digitalWrite(Sonic2, LOW);
              return -1;
            }
            else if(current_range_reading == 5000){   //This is the max reading for the HRXL MaxSonar, not the actual distance
              Serial.println("S2 too far!");
              digitalWrite(Sonic2, LOW);
              return -1;
            }
            else{
              digitalWrite(Sonic2, LOW);
              return current_range_reading;
            }
          }
     }
     else{
       digitalWrite(Sonic2, LOW);
       Serial.println("S2 Reading failed");
       return -1;
     }
     
     
}
long startTime =0;
void waitForSensor(){
  startTime = millis();
  while((millis() - startTime) < 70){
      alternateloop();
   }
}

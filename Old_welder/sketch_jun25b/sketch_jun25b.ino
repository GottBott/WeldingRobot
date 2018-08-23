

int XmotorPull = 8;    // Pull Pin on the X-Axis Motor
int XmotorDir = 9;     // Direction Pin on the X-Axis Motor
int ZmotorPull = 10;   // Pull Pin on the Z-Axis Motor
int ZmotorDir = 11;    // Direction Pin on the Z-Axis Motor

// WILLL NEED TO CHANGE
int ZStepsPer_mm = 200;  // number of steps of the motor for 1 mm of travel in the Z Axis
int XStepsPer_mm = 200;  // number of steps of the motor for 1 mm of travel in the X Axis

int Sonic1 = 6;  // Pin to activate ultrasonic sensor 1
int Sonic2 = 7;  // Pin to activate ultrasonic sensor 2

int SwXMax = 22;  // Pin for switch ....TO DO$$$$$$$
int SwXMin = 24;
int SwZMax = 26;
int SwZMin = 28;

int current_range_reading = -1;     // hold the place of a Usonic range reading 
int sonicSensorOffset = 600;       // number of mm the sensor is offest from the welder head
int desiredWelderOffset = 10;      // number of mm we want the welder above the meterial.    


// Funcrion that initilizes hardware components 
void setup() {
  
  pinMode(XmotorPull, OUTPUT);  // Setup Pins as outputs for motor control
  pinMode(XmotorDir, OUTPUT);
  pinMode(ZmotorPull, OUTPUT);  
  pinMode(ZmotorDir, OUTPUT);
  
  pinMode(Sonic1, OUTPUT);  // set the trigger pin for the ultrasonic to an output on the mega.
  pinMode(Sonic2, OUTPUT);  // set the trigger pin for the ultrasonic to an output on the mega.
  
  digitalWrite(Sonic1, LOW); // so sonics dont pulse unless pin is brought high
  digitalWrite(Sonic2, LOW);
  
  pinMode(SwXMax, INPUT); 
  pinMode(SwXMin, INPUT); 
  pinMode(SwZMax, INPUT); 
  pinMode(SwZMin, INPUT); 
  
  Serial.begin(115200);  // Start talking to RasberryPi
  Serial1.begin(9600);   // Start Talking to  ultrasonic sensor 1
  Serial2.begin(9600);   // Start Talking to  ultrasonic sensor 2
  
  delay(500);            // Allow enough time for conections to establish

}

// Function that is called on repete by the micro
void loop() { 
  //MoveZAxis(AdvSonicReading() - sonicSensorOffset); // how far is the welder head from the meterial

 //delay(1000);
 //Serial.println(AdvSonicReading());
 //int x = AdvSonicReading();
// Serial.println(x);
// MoveZAxis(x - sonicSensorOffset);

//MoveXAxis(3);
for(int i =0; i<300;i++){
  MoveOneStep("X",1);
}
delay(20);
for(int i =0; i<300;i++){
  MoveOneStep("X",-1);
}
delay(20);



}



// Function gathers Sonic readings and takes the adverdge
int AdvSonicReading(){

  while(1){  
    int count = 0;
    int one = getSonicReadingOne();
    int two = getSonicReadingTwo();
    int three = getSonicReadingOne();
    int four = getSonicReadingTwo();
    int sum = (one+ two + three + four) /4;
    if(sum * 1.25 < one || sum * .75 > one){
      sum = 0;
    }
    else{
      return sum;
    }
  
  }
}


// Function that gathers one reading from Ultrasonic 1
int getSonicReadingOne(){
   digitalWrite(Sonic1, HIGH);
   while(1){
     if (Serial1.available() > 5){  // one complete range, 6 characters long, is stored in the RX buffer.
          int inByte = Serial1.read(); // Examine the first stored character and decide what to do.
          if( inByte == 'R' ){ // "R" character 
            int thousands = (Serial1.read() - '0') * 1000; // Take and convert each range digit to human-readable integer format.
            int hundreds = (Serial1.read() - '0') * 100;
            int tens = (Serial1.read() - '0') * 10;
            int units = (Serial1.read() - '0') * 1;
            int cr = Serial1.read(); // clear out the buffer
            // Assemble the digits into the range integer.
            current_range_reading = thousands + hundreds + tens + units;
            
            if(current_range_reading == 300){         //This is the minimum reading for the HRXL MaxSonar, not the actual distance
              Serial.println("S1 too close!");
            }
            else if(current_range_reading == 5000){   //This is the max reading for the HRXL MaxSonar, not the actual distance
              Serial.println("S1 too far!");
            }
            else{
              Serial.print("S1 Range (mm): ");
              Serial.println(current_range_reading); // write out the result and return it
              digitalWrite(Sonic1, LOW);
              return current_range_reading;
            }
          }
     }
   }
  
}

// Function that gathers one reading from Ultrasonic 2
int getSonicReadingTwo(){
   digitalWrite(Sonic2, HIGH);
   while(1){
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
              Serial.println("S2 too close!");
            }
            else if(current_range_reading == 5000){   //This is the max reading for the HRXL MaxSonar, not the actual distance
              Serial.println("S2 too far!");
            }
            else{
              Serial.print("S2 Range (mm): ");    
              Serial.println(current_range_reading);    // write out the result and return it
              digitalWrite(Sonic2, LOW);
              return current_range_reading;
            }
          }
     }
   }
  
}

// Function that will move the Z plane up or down 1 mm
void MoveZAxis(int distance){
  int dir =1;
  if(distance < 0){    // if negitive input set direction to -1
    dir = -1;
  }
  
  for(int i = 0; i< ZStepsPer_mm * abs(distance); i++){    // move the right number of steps based on input and steppsPer_mm
    MoveOneStep("Z", dir);
  }
}

void MoveXAxis(int distance){
  int dir =1;
  if(distance < 0){    // if negitive input set direction to -1
    dir = -1;
  }
  
  for(int i = 0; i< XStepsPer_mm * abs(distance); i++){    // move the right number of steps based on input and steppsPer_mm
    MoveOneStep("X", dir);
  }
}


// Function that takes in an axis and direction and moves the appropiate motor one step
// axis = 'X' or 'Z' dir = 1 for clockwise -1 for counterclockwise
void MoveOneStep(String axis, int dir){
  
  // Check if we are dealing with the X axis
  if(axis == "X"){
    digitalWrite(XmotorDir, LOW);    // Set the direction to Clockwise
    if(dir==1){
      digitalWrite(XmotorDir, HIGH);  // change direction if not Clockwise
    }
    digitalWrite(XmotorPull, HIGH);  // pulse the pull pin
    delay(1);
    digitalWrite(XmotorPull, LOW);
  }
  
  // If not X axis then must be Z axis
  else{
    digitalWrite(ZmotorDir, LOW);  
    if(dir==1){
      digitalWrite(ZmotorDir, HIGH);  
    }
    digitalWrite(ZmotorPull, HIGH);
    delay(1);
    digitalWrite(ZmotorPull, LOW);  
  }
} 


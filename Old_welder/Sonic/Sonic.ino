
int Sonic1 = 7;  // Pin to activate ultrasonic sensor 1
int Sonic2 = 6;  // Pin to activate ultrasonic sensor 2


int current_range_reading = -1;     // hold the place of a Usonic range reading 
int sonicSensorOffset = 600;       // number of mm the sensor is offest from the welder head
int desiredWelderOffset = 10;      // number of mm we want the welder above the meterial.    


// Funcrion that initilizes hardware components 
void setup() {
  

  
  pinMode(Sonic1, OUTPUT);  // set the trigger pin for the ultrasonic to an output on the mega.
  pinMode(Sonic2, OUTPUT);  // set the trigger pin for the ultrasonic to an output on the mega.
  
  digitalWrite(Sonic1, LOW); // so sonics dont pulse unless pin is brought high
  digitalWrite(Sonic2, LOW);
 
  
  Serial.begin(115200);  // Start talking to RasberryPi
  Serial2.begin(9600);   // Start Talking to  ultrasonic sensor 1
  //Serial3.begin(9600);   // Start Talking to  ultrasonic sensor 2
  
  delay(500);            // Allow enough time for conections to establish

}

// Function that is called on repete by the micro
void loop() {
delay(500); 
Serial.println(getSonicReadingOne());
delay(500);
//Serial.println(getSonicReadingTwo());
}






// Function that gathers one reading from Ultrasonic 1
int getSonicReadingOne(){
   digitalWrite(Sonic1, HIGH);
   while(1){
     Serial.println("struck 1");
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
     Serial.println("struck 2");
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



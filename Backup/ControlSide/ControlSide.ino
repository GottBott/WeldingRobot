

#include <Encoder.h>


Encoder knobLeft(50, 52);   // left rotery encoder
Encoder knobRight(51, 53);  // right rotary encoder
int oscFreqPin = 1;    // select the input pin for the potentiometer
int oscDistPin = 2;    // select the input pin for the potentiometer

long positionLeft  = -999;
long positionRight = -999;
int oscFreq = -999;
int oscDist = -999;

void setup() {
  Serial.begin(115200);    // talk to rasberry pi
}




void loop() {
  
  long newLeft, newRight;
  int newOscFreq, newOscDist;
  
  newOscFreq = analogRead(oscFreqPin);    // read the value from the sensor
  newOscDist = analogRead(oscDistPin);    // read the value from the sensor
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  
  // if detect a change on one of the encoders print to serial
 if ((newLeft - positionLeft) > 3){
     Serial.println('D');
     positionLeft = newLeft;
 }
 else if((positionLeft - newLeft) > 3){
     Serial.println('A');
     positionLeft = newLeft;
 }

 
 if ((newRight - positionRight) > 3){
     Serial.println('W');
     positionRight = newRight;
 }
 else if((positionRight - newRight) > 3){
     Serial.println('S');
     positionRight = newRight;
 }
 if (abs(newOscFreq - oscFreq) > 100){
     Serial.println('G');
     Serial.println(newOscFreq);
     oscFreq = newOscFreq;
 }
 if (abs(newOscDist - oscDist) > 100){
     Serial.println('H');
     Serial.println(newOscDist);
     oscDist = newOscDist;
 }
 
 


}

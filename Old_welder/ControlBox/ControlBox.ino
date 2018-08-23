

#include <Encoder.h>


Encoder knobLeft(50, 52);
Encoder knobRight(51, 53);


void setup() {
  Serial.begin(115200);
  Serial.println("TwoKnobs Encoder Test:");
}

long positionLeft  = -999;
long positionRight = -999;

void loop() {
  long newLeft, newRight;
  newLeft = knobLeft.read();
  newRight = knobRight.read();
 
 if (newLeft > positionLeft){
   for(int i =0; i< newLeft - positionLeft; i++){
     Serial.println('D');
   }
 }
 else if(newLeft < positionLeft){
   for(int i =0; i< positionLeft - newLeft; i++){
     Serial.println('A');
   }
 }
 
 if (newRight > positionRight){
   for(int i =0; i< newRight - positionRight; i++){
     Serial.println('W');
   }
 }
 else if(newRight < positionRight){
   for(int i =0; i< positionRight - newRight; i++){
     Serial.println('S');
   }
 }
    positionLeft = newLeft;
    positionRight = newRight;


}

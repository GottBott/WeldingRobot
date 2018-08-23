// Quickstop.pde
// -*- mode: C++ -*-
//
// Check stop handling.
// Calls stop() while the stepper is travelling at full speed, causing
// the stepper to stop as quickly as possible, within the constraints of the
// current acceleration.
//
// Copyright (C) 2012 Mike McCauley
// $Id:  $
#include <AccelStepper.h>


// Define a stepper and the pins it will use
AccelStepper stepper(1,8,9); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepper1(1,10,11);
void setup()
{  
  stepper.setMaxSpeed(10000);
  stepper.setAcceleration(100000);
  stepper1.setMaxSpeed(5000);
  stepper1.setAcceleration(3000);
 
}
void loop()
{    

      stepper.moveTo(1000);
    while (stepper.currentPosition() != 1000) // Full speed up to 300
      stepper.run();
    stepper.stop(); // Stop as fast as possible: sets new target
    stepper.runToPosition(); 
    // Now stopped after quickstop
    // Now go backwards
    stepper.moveTo(-1000);
    while (stepper.currentPosition() != 0) // Full speed basck to 0
      stepper.run();
    stepper.stop(); // Stop as fast as possible: sets new target
    stepper.runToPosition(); 
    // Now stopped after quickstop 
 
 

  
}

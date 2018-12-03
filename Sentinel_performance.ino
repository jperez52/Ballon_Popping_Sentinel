
#include <AccelStepper.h>

// Declare some pins
int pul1 = 2; // motor 1
int dir1 = 3; // motor 1
int pul2 = 4; // motor 2
int dir2 = 5; // motor 2
const int pwm = 7 ;     //initializing pin 7 as laser output

// Global variables
long start = 0; // home position for both motors
long maxX = 60000; // end stop for motor 2
long maxY = -26000; // end stop for motor 1
long steps = 500;  // number of steps to attempt to move at one time
long current1; // motor 1 current position
long current2; // motor 2 current position
int incomingByte = 0; // serial read variable



//initialize steppers
AccelStepper stepper1(1, pul1, dir1); 
AccelStepper stepper2(1, pul2, dir2); 

// Declare all the Motor Control Functions
void M1_C(); // C--> Clock Wise 
void M1_A(); // A--> Anti Clock Wise
void M2_C(); // M*_# where * = 1-5 (Motor Number) and # = C - A (Direction)
void M2_A();
void M1_stop();
void M2_stop();
void laser();


void setup() {


  delay(50);
  Serial.begin(115200);

  stepper1.setMaxSpeed(2000);  // set motor max speed
  stepper1.setAcceleration(1000);  // set motor max acceleration
  stepper2.setMaxSpeed(2000);
  stepper2.setAcceleration(1000);

  stepper1.setCurrentPosition(start);   //saving start position as zero
  stepper2.setCurrentPosition(start);   //saving start position as zero

  analogWrite(pwm,0); // turn off laser
  
}

void loop() { 
  if(Serial.available() > 0) {
            incomingByte = Serial.read(); // read the incoming byte:
            switch (incomingByte) {
              case '0':
               M2_C();
               delay(40);
               break;
              case '1':
               M2_A();
               delay(40);
               break;
              case '2':
               M2_stop();
               delay(40);
               break;
              case '3':
               M1_C();
               delay(40);
               break;
              case '4':
               M1_A();
               delay(40);
               break;
              case '5':
               M1_stop();
               delay(40);
               break;
              case '6': // whem both stop conditions have been met
               laser();
               delay(40);
               break;

        }

     }

}

// Define Functions for motor drivers
void M1_C(){
    analogWrite(pwm,0); // turn off laser
    current1 = stepper1.currentPosition();
    current1 = current1 + steps;
    if ( current1 > start && current1 < maxY) { //set limits
        stepper1.moveTo(current1); 
        stepper1.run();
    }
}

void M1_A(){
    analogWrite(pwm,0); // turn off laser
    current1 = stepper1.currentPosition();
    current1 = current1 - steps;
    if ( current1 > start && current1 < maxY) {
        stepper1.moveTo(current1); 
        stepper1.run();
    }
    
}

void M2_C(){
    analogWrite(pwm,0); // turn off laser
    current2 = stepper2.currentPosition();
    current2 = current2 + steps;
    if ( current2 > start && current2 < maxX) {
        stepper2.moveTo(current2);
        stepper2.run();
    }
}

void M2_A(){
    analogWrite(pwm,0); // turn off laser
    current2 = stepper2.currentPosition();
    current2 = current2 - steps;
    if ( current2 > start && current2 < maxX) {
        stepper2.moveTo(current2);
        stepper2.run();
    }
}

void M1_stop(){
    stepper1.stop();   //stoping motor 1 - not really necessary, but useful for matlab logic
}


void M2_stop(){
    stepper2.stop();   //stoping motor 2
}

void laser(){
    analogWrite(pwm,100); // turn on laser shoot until balloon pops and centriod moves/disappears
}


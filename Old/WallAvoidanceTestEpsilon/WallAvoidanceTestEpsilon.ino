#include <Stepper.h> // STTEPPER LIBRARY
#include <Math.h>

#define STEPS 200 // DON'T MESS IWTH THIS...DETERMINED BY THE MOTORS WE'RE USING

//#define WALL_CORRECT_THRESHOLD 1
//#define WALL_CORRECT_MULTIPLIER 5


#define PING_PIN_RIGHT 2
#define PING_PING_LEFT 4

#define IR_PIN_LEFT A1
#define IR_PIN_RIGHT A0
#define IREMITTER 3

//#define PING_SENSOR_POLL_RATE 7

#define MOTOR_SPEED 100 // KEEP SPEED 100

#define MOVE_FORWARD_STEPS 7

Stepper motorL(STEPS, 7,6,5,8);
Stepper motorR(STEPS, 9,12,11,10);

int turnCount = 0;


//int PingFrequencyCount = 0;
//int initialLeftDistance, initialRightDistance;


double wall_distance_front, wall_distance_left, wall_distance_right;

void setup() {
  Serial.begin(9600);
  //POWER LED
  delay(500);
  pinMode(IREMITTER, OUTPUT);  // IR emitter LED on digital pin 2
  digitalWrite(IREMITTER, LOW);
  motorL.setSpeed(MOTOR_SPEED);
  motorR.setSpeed(MOTOR_SPEED);
}

void loop(){
   if(!(turnCount%6==0)){
   float irL = readIR(true, 7); // true for left 
   float irR = readIR(false, 7);
   
   if(irL>1.9&&irR>1.0){
     wall_distance_left = getPingSensorCM(PING_PING_LEFT);
     wall_distance_right = getPingSensorCM(PING_PIN_RIGHT);
     delay(300);
     if(wall_distance_left>15){
       for(int i = 0; i < 106; i++){
         motorR.step(1);
         motorL.step(-1);
       }
       
     }else if(wall_distance_right>20){
       for(int i = 0; i < 106; i++){
         motorR.step(-1);
         motorL.step(1);
       }
       
     }else{
       for(int i = 0; i < 212; i++){
         motorR.step(1);
         motorL.step(-1);
       }
       
     }
   }
   if(irL>1.9){
     motorL.step(13);
     //motorR.step(5);
   }
   if(irR>1.0){
     motorR.step(13);
    // motorL.step(5);
   }
   
     for(int i =0; i <10;i++){
       motorR.step(1);
       motorL.step(1);
     }
     motorR.step(1);
     for(int i =0; i <10;i++){
       motorR.step(1);
       motorL.step(1);
     }
     motorR.step(1);
     for(int i =0; i <10;i++){
       motorR.step(1);
       motorL.step(1);
     }
     //motorR.step(1);
     for(int i =0; i <10;i++){
       motorR.step(1);
       motorL.step(1);
     }
     motorR.step(1);
     for(int i =0; i <10;i++){
       motorR.step(1);
       motorL.step(1);
     }
     motorR.step(1);
     
     //delay(300);
   }else{
     delay(1000);
     wall_distance_left = getPingSensorCM(PING_PING_LEFT);
     wall_distance_right = getPingSensorCM(PING_PIN_RIGHT);
     if(wall_distance_left>15){
       for(int i = 0; i < 106; i++){
         motorR.step(1);
         motorL.step(-1);
       }
       
     }else if(wall_distance_right>20){
       for(int i = 0; i < 106; i++){
         motorR.step(-1);
         motorL.step(1);
       }
       
     }else{
       for(int i = 0; i < 212; i++){
         motorR.step(1);
         motorL.step(-1);
       }
     }
   }
    turnCount++;
  
}


double microsecondsToCentimeters(double microseconds) {
  return (microseconds/2.0)*0.034;
}

void setToInput(int x){
  pinMode(x, INPUT);
}

void setToOutput(int x){
  pinMode(x, OUTPUT);
}

int getPingSensorCM(int pingPin) {
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);

  return cm;
}

double analyzedSensorValue(int pin, const int numberOfReadings){

  double nums[numberOfReadings];
  
  int sizeOfArray = numberOfReadings;
  
  for(int i = 0; i < numberOfReadings; i++){
    nums[i] = getPingSensorCM(pin);
    Serial.println(nums[i]);
  }
  Serial.println("-------NEXT-------");
  double meanSum = 0; // Sum of the numbers in the original array
  for(int i = 0; i < sizeOfArray ; i++){
    meanSum += nums[i];
  }
  double mean = meanSum/sizeOfArray ; // Mean of numbers in the original array
  double squaresSum = 0; // Sum of the (x-mean)^2
  for(int i = 0; i < sizeOfArray ; i++){
    squaresSum += pow(nums[i]-mean, 2);
  }
  double dev = sqrt((squaresSum)/(sizeOfArray-1)); //STD deviation of origional array
  double minV = mean-dev; // min cutoff of original
  double maxV = mean+dev; // max cutoff of original

  int count = 0; // counts how many values of the orignial pass the first cutoff
  double sum = 0.0; // sum of passing values
  for(int i = 0; i < sizeOfArray; i++){
    if(nums[i] > minV && nums[i] < maxV){
      sum += nums[i];
      count += 1.0;
    }
  }
  if(count == 0){//If all readings were identical
    return nums[0];
  }
  else{
    return sum/count; // At least one of the readings was different
  }
}  

float readIR(boolean leftIR, int times){ ////////////////////////// READ IR /////////////////////////////////////
  int value[times];//This was 10 in the original code....?
  for(int x = 0; x < times; x++){     
    digitalWrite(IREMITTER,LOW);           // turning the IR LEDs off to read the IR coming from the ambient
    delay(1); // minimum delay necessary to read values
    float ambientIR; // THE TYPE OF THIS WAS INT, I CHANGED IT TO FLOAT
    if(leftIR){// If we're reading the left IR
      ambientIR = analogRead(IR_PIN_LEFT);  // storing IR coming from the ambient
    }
    else{
      ambientIR = analogRead(IR_PIN_RIGHT);
    }
    digitalWrite(IREMITTER, HIGH);          // turning the IR LEDs on to read the IR coming from the obstacle
    delay(1); // minimum delay necessary to read values
    float obstacleIR; // THE TYPE OF THIS WAS INT, I CHANGED IT TO FLOAT
    if(leftIR){//If we're reading the left IR
      obstacleIR = analogRead(IR_PIN_LEFT);  // storing IR coming from the obstacle
    }
    else{
      obstacleIR = analogRead(IR_PIN_RIGHT);
    }
    value[x] = obstacleIR-ambientIR;   // calculating changes in IR values and storing it for future average
  }
  float distance;
  for(int x=0; x < times; x++){        // calculating the average based on the "accuracy"
    distance += value[x];
  }
  return(float)(distance/times);            // return the final value
}

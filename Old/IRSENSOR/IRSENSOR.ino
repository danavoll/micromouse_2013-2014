// Simple Proximity Sensor using Infrared
// Description: Measure the distance to an obstacle using infrared light emitted by IR LED and
//   read the value with a IR photodiode. The accuracy is not perfect, but works great
//   with minor projects.
// Author: Ricardo Ouvina
// Date: 01/10/2012
// Version: 1.0

#define IR_PIN_LEFT A1
#define IR_PIN_RIGHT A0
#define IREMITTER 3 // Digital Pin 3 for output

void setup(){
  Serial.begin(9600);         // initializing Serial monitor
  pinMode(IREMITTER,OUTPUT);  // IR emitter LED on digital pin 2
  digitalWrite(IREMITTER,LOW);// setup IR LED as off
}

void loop(){
    float irL = readIR(true, 15); // true for left 
    float irR = readIR(false, 15); // false for left
    delay(250);
  
    Serial.print("IR - L: "); // Prints the IR values
    Serial.print(irL);
    Serial.print("  R: ");
    Serial.println(irR);      
    
    
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

/*
Author: Joseph Materski
Version: 2.0
Does not include steppers or harware specifics
 Algorithm based on "An Efficient Algorithm for Robot Maze-Solving" by
 Hongshe Dang, Jinguo Song, Qin Guo
 */
int floodArray[16][16]; //2 dimensions for flood
unsigned char wallArray[16][16]; //2 dimensionsanol array for walls
int value = 14, l = 0, x = 0, y = 0;
int sensorL, sensorR, sensorF;
int absDir, relDir;

//setup method
void setup(){
  Serial.begin(9600); //initialize baud rate
  //* initialize stepper pins
}

//looop method
void loop(){
  sensorL = analogRead(2); //read value from analog pin 2(left ir)
  sensorR = analogRead(3); //read value from analog pin 3 (right ir)
  sensorF = analogRead(4); //read value from analog pin 4 (front ir)
  //* initialize sensor pins
  initializeGrid();
  updateWall();
  flood(x, y);
  printGrid();
  decide();
}

//initializeGrid method
void initializeGrid(){
  if (l==0){
    for( int r = 0; r<8; r++){ //autofills top left and bottom  right corner
      for(int c = 0; c<8; c++){
        floodArray [r][c]= value;
        floodArray [15-r][15-c] = value;
        value = value - 1;
      }
      value = value +7;
    }
    value = 14;
    for( int r = 15; r>7; r--){ //autofills top right and bottom left corner
      for(int c = 0; c<8; c++){
        floodArray [r][c] = value;
        floodArray [15-r][15-c] = value;
        value = value - 1;
      }
      value = value +7;
    }
    l=1;
  }
}

//printGrid method
void printGrid(){
  Serial.println();
  for(int i = 16; i>0;i--){ //probably not correct
    Serial.println();
    for(int j = 0; j<16; j++){
      if (floodArray[j][i] > 9){
        Serial.print(floodArray[j][i]);
        Serial.print(" ");
      } 
      else{
        Serial.print("0");
        Serial.print(floodArray[j][i]);
        Serial.print(" ");
      }
    }
  }
}

//updatedWall method
void updateWall(){
  leftWall();
  rightWall();
  frontWall();
}

//leftWall method
void leftWall(){
  if(sensorL<100){
    switch(relDir){ //does wall addition relative to direction
    case 0:
      wallArray[x][y] |= 0x8;//binary logic
      if (x>0){ //prevents nullPointer
        wallArray[x-1][y] |= 0x2; //updates right wall of adjacent cube
      }
      break;
    case 1:
      wallArray[x][y] |= 0x1;
      if (y<15){
        wallArray[x][y+1] |= 0x4;
      }
      break;
    case 2:
      wallArray[x][y] |= 0x2;
      if (x<15){
        wallArray[x+1][y] |= 0x8;
      }
      break;
    case 3:
      wallArray[x][y] |= 0x4;
      if (y>0){
        wallArray[x][y-1] |= 0x1;
      }
      break;
    }
  }
}

//rightWall method
void rightWall(){
  if(sensorR<100){
    switch(relDir){ //does wall addition relative to direction
    case 0:
      wallArray[x][y] |= 0x2;//binary logic
      if (x<15){ //prevents nullPointer
        wallArray[x+1][y] |= 0x8; //updates right wall of adjacent cube
      }
      break;
    case 1:
      wallArray[x][y] |= 0x4;
      if (y>0){
        wallArray[x][y-1] |= 0x1;
      }
      break;
    case 2:
      wallArray[x][y] |= 0x8;
      if (x>0){
        wallArray[x+1][y] |= 0x2;
      }
      break;
    case 3:
      wallArray[x][y] |= 0x1;
      if (y<15){
        wallArray[x][y+1] |= 0x4;
      }
      break;
    }   
  }
}

//frontWall method
void frontWall(){
  if(sensorR<100){
    switch(relDir){ //does wall addition relative to direction
    case 0:
      wallArray[x][y] |= 0x1;
      if (y<15){
        wallArray[x][y+1] |= 0x4;
      }
      break;

    case 1:
      wallArray[x][y] |= 0x2;//binary logic
      if (x<15){ //prevents nullPointer
        wallArray[x+1][y] |= 0x8; //updates right wall of adjacent cube
      }
      break;

    case 2:
      wallArray[x][y] |= 0x4;
      if (y>0){
        wallArray[x][y-1] |= 0x1;
      }
      break;

    case 3:
      wallArray[x][y] |= 0x8;
      if (x>0){
        wallArray[x+1][y] |= 0x2;
      }
      break;
    }   
  }
}

//flood method
void flood(int x1, int y1){ //takes in a x and y coordinate
  switch(wallArray[x1][y1]){ //does a switch of the wall array, stored as bits but written as a hex
    case(0x0): //no walls, 0x0 = 0000
    if (floodArray[x1][y1] == min(floodArray[x1-1][y1],min(floodArray[x1+1][y1],min(floodArray[x1][y1-1],floodArray[x1][y1+1]))) + 1){ //takes the minimum of the sorrounding boxes and checks if it is 1 more
    }
    else{
      floodArray[x1][y1] ==  min(floodArray[x1-1][y1],min(floodArray[x1+1][y1],min(floodArray[x1][y1-1],floodArray[x1][y1+1]))) + 1; //otherwise it will set it to 1 more and call a flood on surrounding boxes
      flood(x1,y1+1);
      flood(x1-1,y1);
      flood(x1+1,y1);
      flood(x1,y1-1);
    }
    break;
    case(0x1): //U, 0x1=0001
    if (floodArray[x1][y1] == min(floodArray[x1-1][y1],min(floodArray[x1+1][y1],floodArray[x1][y1-1])) + 1){ //this time it doesnt check the one above because there is a wall there
    }
    else{
      floodArray[x1][y1] = min(floodArray[x1-1][y1],min(floodArray[x1+1][y1],floodArray[x1][y1-1])) + 1; //again not checking or calling the flood on the one above it
      flood(x1-1,y1);
      flood(x1+1,y1);
      flood(x1,y1-1);
    } 
    break;
    case(0x2): //R
    if (floodArray[x1][y1] == min(floodArray[x1-1][y1],min(floodArray[x1][y1-1],floodArray[x1][y1+1])) + 1){
    }
    else{
      floodArray[x1][y1] =  min(floodArray[x1-1][y1],min(floodArray[x1][y1-1],floodArray[x1][y1+1])) + 1;
      flood(x1,y1+1);
      flood(x1-1,y1);
      flood(x1,y1-1);
    }

    break;
    case(0x3): //UR
    if (floodArray[x1][y1] == min(floodArray[x1-1][y1],floodArray[x1][y1-1]) + 1){
    }
    else{
      floodArray[x1][y1] =  min(floodArray[x1-1][y1],floodArray[x1][y1-1]) + 1;
      flood(x1-1,y1);
      flood(x1,y1-1);
    }
    break;
    case(0x4): //D
    if (floodArray[x1][y1] == min(floodArray[x1-1][y1],min(floodArray[x1+1][y1],floodArray[x1][y1+1])) + 1){
    }
    else{
      floodArray[x1][y1] =  min(floodArray[x1-1][y1],min(floodArray[x1+1][y1],floodArray[x1][y1+1])) + 1;
      flood(x1,y1+1);
      flood(x1-1,y1);
      flood(x1+1,y1);
    }
    break;
    case(0x5): //UD
    if (floodArray[x1][y1] == min(floodArray[x1-1][y1],floodArray[x1+1][y1]) + 1){
    }
    else{
      floodArray[x1][y1] =  min(floodArray[x1-1][y1],floodArray[x1+1][y1]) + 1;
      flood(x1-1,y1);
      flood(x1+1,y1);
    }
    break;
    case(0x6): //RD
    if (floodArray[x1][y1] == min(floodArray[x1-1][y1],floodArray[x1][y1+1]) + 1){
    }
    else{
      floodArray[x1][y1] =  min(floodArray[x1-1][y1],floodArray[x1][y1+1]) + 1;
      flood(x1,y1+1);
      flood(x1-1,y1);
    }
    break;
    case(0x7): //URD
    if (floodArray[x1][y1] == floodArray[x1-1][y1] + 1){
    }
    else{
      floodArray[x1][y1] = floodArray[x1-1][y1] + 1;
      flood(x1-1,y1);
    }
    break;
    case(0x8): //L
    if (floodArray[x1][y1] == min(floodArray[x1+1][y1],min(floodArray[x1][y1-1],floodArray[x1][y1+1])) + 1){
    }
    else{
      floodArray[x1][y1] =  min(floodArray[x1+1][y1],min(floodArray[x1][y1-1],floodArray[x1][y1+1])) + 1;
      flood(x1,y1+1);
      flood(x1,y1-1);
      flood(x1+1,y1);
    }
    break;
    case(0x9): //LU
    if (floodArray[x1][y1] == min(floodArray[x1+1][y1],floodArray[x1][y1-1]) + 1){
    }
    else{
      floodArray[x1][y1] =  min(floodArray[x1+1][y1],floodArray[x1][y1-1]) + 1;
      flood(x1,y1-1);
      flood(x1+1,y1);
    }
    break;
    case(0xA): //LR
    if (floodArray[x1][y1] == min(floodArray[x1][y1-1],floodArray[x1][y1+1]) + 1){
    }
    else{
      floodArray[x1][y1] = min(floodArray[x1][y1-1],floodArray[x1][y1+1]) + 1;
      flood(x1,y1+1);
      flood(x1,y1-1);
    }
    break;
    case(0xB): //LUR
    if (floodArray[x1][y1] == floodArray[x1][y1-1] + 1){
    }
    else{
      floodArray[x1][y1] = floodArray[x1][y1-1] + 1;
      flood(x1,y1-1);
    }
    break;
    case(0xC): //LD
    if (floodArray[x1][y1] == min(floodArray[x1+1][y1],floodArray[x1][y1+1]) + 1){
    }
    else{
      floodArray[x1][y1] =  min(floodArray[x1+1][y1],floodArray[x1][y1+1]) + 1;
      flood(x1,y1+1);
      flood(x1+1,y1);
    }
    break;
    case(0xD): //LUD
    if (floodArray[x1][y1] == floodArray[x1+1][y1] + 1){
    }
    else{
      floodArray[x1][y1] = floodArray[x1+1][y1] + 1;
      flood(x1+1,y1);
    }
      break;
    case(0xE): //LRD
    if (floodArray[x1][y1] == floodArray[x1][y1+1] + 1){
    }
    else{
      floodArray[x1][y1] = floodArray[x1][y1+1] + 1;
      flood(x1,y1+1);
    }
    break;

  }
}

//decide method
void decide(){
  //* figur out which adjacent wall is lowest
  //* going to be ass large aas flood, can't do both at once because flood is a recursion
  //this switch will check where walls are, and send that to findLowest
  //if there is a wall, it will send a value of 1000 so it will never be read as the lowest
  switch(wallArray[x][y]){
     case 0x0:
       findLowest(floodArray[x-1][y],floodArray[x][y+1],floodArray[x+1][y], floodArray[x][y-1]);
     break;
     case 0x1:
       findLowest(floodArray[x-1][y],1000,floodArray[x+1][y], floodArray[x][y-1]);
     break;
     case 0x2:
       findLowest(floodArray[x-1][y],floodArray[x][y+1],1000, floodArray[x][y-1]);
     break;
     case 0x3:
       findLowest(floodArray[x-1][y],1000,1000, floodArray[x][y-1]);
     break;
     case 0x4:
       findLowest(floodArray[x-1][y],floodArray[x][y+1],floodArray[x+1][y], 1000);
     break;
     case 0x5:
       findLowest(floodArray[x-1][y],1000,floodArray[x+1][y], 1000);
     break;
     case 0x6:
       findLowest(floodArray[x-1][y],floodArray[x][y+1],1000, 1000);
     break;
     case 0x7:
       findLowest(floodArray[x-1][y],1000,1000, 1000);
     break;
     case 0x8:
       findLowest(1000,floodArray[x][y+1],floodArray[x+1][y], floodArray[x][y-1]);
     break;
     case 0x9:
       findLowest(1000,1000,floodArray[x+1][y], floodArray[x][y-1]);
     break;
     case 0xA:
       findLowest(1000,floodArray[x][y+1],1000, floodArray[x][y-1]);
     break;
     case 0xB:
       findLowest(1000,1000,1000, floodArray[x][y-1]);
     break;
     case 0xC:
       findLowest(1000,floodArray[x][y+1],floodArray[x+1][y], 1000);
     break;
     case 0xD:
       findLowest(1000,1000,floodArray[x+1][y], 1000);
     break;
     case 0xE:
       findLowest(1000,floodArray[x][y+1],1000, 1000);
     break;
     case 0xF:
       Serial.print("Damn!");
     break;
     
  }
}

//findLowest method
//This method figures out which direction is lowest, and then figures out wich
//way to turn relative to the mouse
void findLowest(int left,int up,int right,int down){
  int lowest;
  int lowDir;
  switch(relDir){
    case 0:
       lowest = up;
       lowDir = 0;
       if (left<lowest){ lowest=left; lowDir = 3; }
       if (right<lowest){ lowest=right; lowDir = 1;}
       if (down<lowest){ lowest=down; lowDir = 2; }
       switch(lowDir){
         case 3:
           turnLeft();
         break;
         case 0:
           forward();
         break;
         case 1:
           turnRight();
         break;
         case 2:
           turnAround();
         break;
       }
    break;
    case 1:
       lowest = right;
       lowDir = 1;
       if (left<lowest){ lowest=left; lowDir = 3; }
       if (up<lowest){ lowest=up; lowDir = 0;}
       if (down<lowest){ lowest=down; lowDir = 2; }
       switch(lowDir){
         case 3:
           turnAround();
         break;
         case 0:
           turnLeft();
         break;
         case 1:
           forward();
         break;
         case 2:
           turnRight();
         break;
       }
    break;
    case 2:
       lowest = down;
       lowDir = 2;
       if (left<lowest){ lowest=left; lowDir = 3; }
       if (right<lowest){ lowest=right; lowDir = 1;}
       if (up<lowest){ lowest=up; lowDir = 0; }
       switch(lowDir){
         case 3:
           turnRight();
         break;
         case 0:
           turnAround();
         break;
         case 1:
           turnLeft();
         break;
         case 2:
           forward();
         break;
       }
    break;
    case 3:
       lowest = left;
       lowDir = 3;
       if (up<lowest){ lowest=up; lowDir = 0; }
       if (right<lowest){ lowest=right; lowDir = 1;}
       if (down<lowest){ lowest=down; lowDir = 2; }
       switch(lowDir){
         case 3:
           forward();
         break;
         case 0:
           turnRight();
         break;
         case 1:
           turnAround();
         break;
         case 2:
           turnLeft();
         break;
       }
    break;
   
  }
}

//forward method
void forward(){
  switch(relDir){ //switch case of relDir to see what coordinate to incriment
    case 0:
      y++;
    break;
    case 1:
      x++;
    break;
    case 2:
      y--;
    break;
    case 3:
      x--;
    break;
  }
  //*move forward
}
//left method
void turnLeft(){
  //*turn left
  relDir = relDir + 3; //updates relative direction
  relDir = relDir%4; //keeps between 0 and 3
  forward(); //moves forward after turn
}

//righ method
void turnRight(){
  //*turn right
  relDir = relDir + 1;
  relDir = relDir%4; 
  forward();
}

//turnAround method
void turnAround(){
  //*turn around
  relDir = relDir + 2;
  relDir = relDir%4;
  forward();
}




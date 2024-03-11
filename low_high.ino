/// ssp: 3210

#include<Servo.h>

#define sameMarge 15
#define servcount 4
#define ldrCount 4

Servo servos[servcount];
const int servoPins[servcount] = { 8, 9, 10, 11 };
const int ldrPins[ldrCount] = { A0, A1, A2, A3 };
int arrayInArray[servcount][2];

// the heights the motors will be set to
int heights[3] = { 0, 45, 90 };

void setup() {
  Serial.begin(9600);
  Serial.println("Hello, World!");

  // activate the servo's and make all the positions 0 
  for (int i = 0; i < servcount; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(0);
  }
  // set all the ldr's to input
  for (int i = 0; i < ldrCount; i++) {
    pinMode(ldrPins[i], INPUT);
  }
}

void loop() {
  // read the data from the LDR's
  for (int i = 0; i < ldrCount; i++) {
    arrayInArray[i][0] = i;
    arrayInArray[i][1] = analogRead(ldrPins[i]);
  }
  // if we want to have more specific height values
  // quicksort(arrayInArray, 0, servcount - 1);

  // but for now we are going to use a more primative way
  int i[2];
  highestElementIndex(arrayInArray, i);
  
  // a bit of debuging output
  printarray();
  printTheChosenOne(i);

  // set the motors to the right position 
  setMotors(i);
  Serial.println(); // clear distinction between blocks
  
  // don't overheat the arduino
  delay(500);
}

// nice formatting for printing the two chosen indices
void printTheChosenOne(int i[2]) {
  Serial.print((String)i[0] + ", " + (String)i[1]);
}

// nice formatting for printing the debug information
void printarray() {
  for (auto a: arrayInArray) {
    Serial.print((String)a[0] + " " + (String)a[1]);
  }
}

// set the motors
void setMotors(int i[2]) {
  // little macro for making the code more readable
  #define opposite(i) (3 - i)
  
  // check the difference betweeen the two top indices
  bool closeEnough = arrayInArray[i[0]][1] - arrayInArray[i[1]][1] < sameMarge;
  Serial.println(closeEnough);
  
  if (closeEnough) { // point it vertical or horizontal
    servos[i[0]].write(heights[0]);
    servos[i[1]].write(heights[0]);
   
    servos[opposite(i[0])].write(heights[2]);
    servos[opposite(i[1])].write(heights[2]);
  } else { // point it at diagonal
    // set the motor closest to the light to the slowest stand
    servos[i[0]].write(heights[0]);
  
    switch (i[0]) {
      // if 0 or 3 is the highest, 1 and 2 need to be in the middle
      case 0:
      case 3:
      servos[1].write(heights[1]);
      servos[2].write(heights[1]);
      break;
      // sameMut in reversed
      case 1:
      case 2:
      servos[0].write(heights[1]);
      servos[3].write(heights[1]);
      break;
    }
 
    servos[opposite(i[0])].write(heights[2]);
  }
}

// return the 2 highest elements indices in an array
void highestElementIndex(int arr[ldrCount][2], int outputArray[2]) {
  int highestI[2] = {-1, -1};
  int highestV[2] = {0, 0};
  
  for (int i = 0; i < ldrCount; i++) {
    if (arr[i][1] > highestV[0]) {
      highestV[1] = highestV[1];
      highestV[0] = arr[i][1];
      
      highestI[1] = highestI[0];
      highestI[0] = i;
    } else if (arr[i][1] > highestV[1]) {
      highestV[1] = arr[i][1];
      highestI[1] = i;
    }
  }

  outputArray[0] = highestI[0];
  outputArray[1] = highestI[1];
}

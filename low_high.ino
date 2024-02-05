#include<Servo.h>

const int servcount = 4;
Servo servos[servcount];
const int servopins[servcount] = {8, 9, 10, 11};

const int ldr_count = 4;
const int ldr_pins[ldr_count] = {A0, A1, A2, A3};
int array_in_array[servcount][2];

// the heights the motors will be set to
int heights[3] = {
  0, 90, 180
};

// which LDR is oppesite to which LDR ?
const int opposites[servcount/2][2] {
  {0, 3},
  {1, 2},
};

void setup() {
  Serial.begin(9600);
  Serial.println("Hello, World!");

  // activate the servo's and make all the positions 0 
  for (int i = 0; i < servcount; i++) {
    servos[i].attach(servopins[i]);
    servos[i].write(0);
  }
  // set all the ldr's to input
  for (int i = 0; i < ldr_count; i++) {
    pinMode(ldr_pins[i], INPUT);
  }
}

void loop() {
  // read the data from the LDR's
  for (int i = 0; i < ldr_count; i++) {
    array_in_array[i][0] = i;
    array_in_array[i][1] = analogRead(ldr_pins[i]);
  }
  // if we want to have more specific height values
  // quicksort(array_in_array, 0, servcount - 1);

  // but for now we are going to use a more primative way
  int i = highest_element_index(array_in_array);
  Serial.println(i);
  
  // a bit of debuging output
  printarray();
  Serial.println();

  // set the motors to the right position 
  set_motors(i);
  
  // don't overheat the arduino
  delay(1000);
}

// nice formatting for printing the debug information
void printarray() {
  for (auto a: array_in_array) {
    Serial.print(a[0]);
    Serial.print(" ");
    Serial.println(a[1]);
  }
}

void set_motors(int i) {
  // set the motor closest to the light to the slowest stand
  Serial.print(i);
  Serial.print(": ");
  Serial.println(heights[0]);
  servos[i].write(heights[0]);

  switch (i) {
    // if 0 or 3 is the highest, 1 and 2 need to be in the middle
    case 0:
    case 3:
    servos[1].write(heights[1]);
    servos[2].write(heights[1]);
    Serial.print("1: ");
    Serial.println(heights[1]);
    Serial.print("2: ");
    Serial.println(heights[1]);
    break;
    // same but in reversed
    case 1:
    case 2:
    Serial.print("0: ");
    Serial.println(heights[1]);
    Serial.print("3: ");
    Serial.println(heights[1]);
    servos[0].write(heights[1]);
    servos[3].write(heights[1]);
    break;
  }

  // find the oppisite motor and set it to the highest stand
  int opposite_i = 0;
  
  for (auto o: opposites) {
    if (i == o[0]) {
      opposite_i = o[1];
    }
    else if (i == o[1]) {
      opposite_i = o[0];
    }
  }
  
  Serial.print(opposite_i);
  Serial.print(": ");
  Serial.println(heights[2]);
  
  servos[opposite_i].write(heights[2]);
}

// the function name sais enough
int highest_element_index(int arr[ldr_count][2]) {
  int highest_i = 0;
  int highest_v = arr[0][1];

  for (int i = 0; i < ldr_count; i++) {
    if (arr[i][1] > highest_v) {
      highest_i = arr[i][0];
      highest_v = arr[i][1];
    }
  }

  return highest_i;
}

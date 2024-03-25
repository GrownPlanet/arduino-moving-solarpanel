#include<Servo.h>

#define same_marge 15
#define servcount 4
#define ldr_count 4

#define buzzer_pin 7

Servo servos[servcount];
const int servopins[servcount] = { 8, 9, 10, 11 };
const int ldr_pins[ldr_count] = { A0, A1, A2, A3 };
int array_in_array[servcount][2];

// the heights the motors will be set to
int heights[3] = { 0, 45, 90 };

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
  int i[2];
  highest_element_index(array_in_array, i);
  
  // a bit of debuging output
  printarray();
  print_the_chosen_one(i);

  // set the motors to the right position 
  set_motors(i);
  Serial.println(); // clear distinction between blocks

  // *soundeffects*
  if (Serial.available() > 0) {
    String input = Serial.readString();
    buzzer(input);
  }
  
  // don't overheat the arduino
  delay(500);
}

// nice formatting for printing the two chosen indices
void print_the_chosen_one(int i[2]) {
  Serial.print(i[0]);
  Serial.print(", ");
  Serial.println(i[1]);
}

// nice formatting for printing the debug information
void printarray() {
  for (auto a: array_in_array) {
    Serial.print(a[0]);
    Serial.print(" ");
    Serial.println(a[1]);
  }
}

// set the motors
void set_motors(int i[2]) {
  // little macro for making the code more readable
  #define opposite(i) (3 - i)
  
  // check the difference betweeen the two top indices
  bool close_enough = array_in_array[i[0]][1] - array_in_array[i[1]][1] < same_marge;
  Serial.println(close_enough);
  
  if (close_enough) { // point it vertical or horizontal
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
      // same but in reversed
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
void highest_element_index(int arr[ldr_count][2], int output_array[2]) {
  int highest_i[2] = {-1, -1};
  int highest_v[2] = {0, 0};
  
  for (int i = 0; i < ldr_count; i++) {
    if (arr[i][1] > highest_v[0]) {
      highest_v[1] = highest_v[1];
      highest_v[0] = arr[i][1];
      
      highest_i[1] = highest_i[0];
      highest_i[0] = i;
    } else if (arr[i][1] > highest_v[1]) {
      highest_v[1] = arr[i][1];
      highest_i[1] = i;
    }
  }

  output_array[0] = highest_i[0];
  output_array[1] = highest_i[1];
}

void buzzer(String input) {
  if (input.equals("TOOT\n")) {
    tone(buzzer_pin, 400);
    delay(500);
    noTone(buzzer_pin);
    delay(100);
    tone(buzzer_pin, 400);
    delay(700);
    noTone(buzzer_pin);
  } else if (input.equals("never\n")) {
    never();
  }
}

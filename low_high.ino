#include<Servo.h>

const int servcount = 4;
Servo servos[servcount];
const int servopins[servcount] = {8, 9, 10, 11};

const int senservcount = 4;
const int lichtsenspins[senservcount] = {A0, A1, A2, A3};
int array_in_array[servcount][2];

int heights[3] = {
  0, 90, 180
};
const int opposites[servcount][2] {
  {0, 3},
  {1, 2},
  {2, 1},
  {3, 0},
};

void setup() {
  Serial.begin(9600);
  Serial.println("Hello, World!");

  for (int i = 0; i < servcount; i++) {
    servos[i].attach(servopins[i]);
    servos[i].write(0);
  }
  for (int i = 0; i < senservcount; i++) {
    pinMode(lichtsenspins[i], INPUT);
  }
}

void loop() {
  return;
  for (int i = 0; i < senservcount; i++) {
    array_in_array[i][0] = i;
    array_in_array[i][1] = analogRead(lichtsenspins[i]);
  }
  // if we want to have more specific height values
  // quicksort(array_in_array, 0, servcount - 1);

  int i = highest_element_i(array_in_array);
  Serial.println(i);
  // int i = 1;

  servos[i].write(heights[0]);

  switch (i) {
    case 0:
    case 3:
    servos[1].write(heights[1]);
    servos[2].write(heights[1]);
    break;
    case 1:
    case 2:
    servos[0].write(heights[1]);
    servos[3].write(heights[1]);
    break;
  }

  int opposite_i = 0;
  for (auto v: opposites) {
    if (i == opposites[i][0]) {
      opposite_i = opposites[i][1];
    }
  }
  servos[opposite_i ].write(heights[2]);

  printarray();

  delay(1000);
  Serial.println();
}

void printarray() {
  for (auto a: array_in_array) {
    Serial.print(a[0]);
    Serial.print(" ");
    Serial.println(a[1]);
  }
}


int highest_element_i(int arr[senservcount][2]) {
  int highest_i = 0;
  int highest_v = arr[0][1];

  for (int i = 0; i < senservcount; i++) {
    if (arr[i][1] > highest_v) {
      highest_i = arr[i][0];
      highest_v = arr[i][1];
    }
  }

  return highest_i;
}

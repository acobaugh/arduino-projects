#include <Servo.h>

// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
const int Servo_pin = 9;

int servo_pos = 0;
int x_pos = 0;

Servo myservo;

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  myservo.attach(9);
  Serial.begin(9600);
  while (! Serial);
}

void loop() {
  x_pos = analogRead(X_pin);
  servo_pos =  map((int)x_pos,0,1023,0,179);
  /*Serial.print("servo = ");
  Serial.print(servo_pos);
  Serial.print(" x = ");
  Serial.print(x_pos);
  Serial.print("\n");*/
  myservo.write(servo_pos);
  //delay(100);
}

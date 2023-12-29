#include <LiquidCrystal.h>

const int encoderPin = 2;
const float wheelCircumference = 15.17; 
const int pulsePerTurn = 20; 
volatile int encoderPos = 0;
volatile boolean lastA;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int in1 = 3;
int in2 = 12;
int in3 = 11;
int in4 = 13;

void setup() {
  Serial.begin(9600);
  pinMode(encoderPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPin), updateEncoder, CHANGE);
  lastA = digitalRead(encoderPin);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("Distance: ");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop() {
  Serial.print("Distance = ");
	int iterations = 5;
	Serial.println(" cm");
	delay(500);
  float distance = (((float)encoderPos / pulsePerTurn) * wheelCircumference) / 2; // Calculate distance covered
  lcd.setCursor(9, 1);
  lcd.print(distance);
  if (distance>=800){ 
    motorStop(); // The Robot Car Stops
  }
}


void updateEncoder() {
  boolean currentA = digitalRead(encoderPin);

  if (lastA != currentA) {
    encoderPos++;
  }
  lastA = currentA;
}

void allMotorStart() {
  analogWrite(in1, 70);
  digitalWrite(in2, LOW);
  analogWrite(in3, 70);
  digitalWrite(in4, LOW);
}

void leftMotorStart(int speed) {
  analogWrite(in1, speed);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(150);
}

void rightMotorStart(int speed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(in3, speed);
  digitalWrite(in4, LOW);
  delay(150);
}

void motorStop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

#include <Wire.h>
#include <LiquidCrystal.h>
#include <NewPing.h>

#define TRIGGER_PIN 3
#define ECHO_PIN 2
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
unsigned int distance; // Define distance variable globally

#define in1 10
#define in2 12
#define in3 11
#define in4 13

void setup() {
  Wire.begin();
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Distance:");

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  delay(100);
}

void loop() {
  UltraSonic();
  if (distance ==0 || distance >20) {
    motorStart();
  }
  else if (distance < 20){
    motorStop();
  }
}

void UltraSonic() {
  delay(50);
  distance = sonar.ping_cm(); // Assign the global distance variable
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Display distance on LCD
  lcd.setCursor(0, 1);
  lcd.print("       "); // Clear the previous distance value
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");
}
void motorStop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}



void leftMotorStart() {
  analogWrite(in1, 150);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 150);
}

void rightMotorStart() {
  analogWrite(in1, 0);
  analogWrite(in2, 150);
  analogWrite(in3, 150);
  analogWrite(in4, 0);
}

void motorStart() {
  analogWrite(in1, 80);
  analogWrite(in2, 0);
  analogWrite(in3, 80);
  analogWrite(in4, 0);
}

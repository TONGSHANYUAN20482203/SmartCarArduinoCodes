#include <LiquidCrystal.h>



const int encoderPin = 2;
const float wheelCircumference = 15.17; // in some unit (e.g., inches, cm)
const int pulsePerTurn = 20;


volatile int encoderPos = 0;
volatile boolean lastA;


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int outputA = A1;
int outputB = A2;

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

  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);

  
}

void loop() {
  int sensorA = analogRead(outputA);
  int sensorB = analogRead(outputB);
  int thresholdA = 500; //threshold for sensorA
  int thresholdB = 500; //threshold for sensor B
  int turnSpeed = 150; // Adjust this value to control the turning speed
  if (sensorA >= thresholdA && sensorB >= thresholdB) {
    allMotorStart(); //starts all moter
  } else if (sensorA < thresholdA) {
    leftMotorStart(turnSpeed);//turn right
  } else if (sensorB < thresholdB) {
    rightMotorStart(turnSpeed);//turn left
  }
  else if (sensorA <= thresholdA && sensorB <= thresholdB) {
    motorStop();// stops all motor
   }
}

void updateEncoder() {
  boolean currentA = digitalRead(encoderPin);

  if (lastA != currentA) {
    encoderPos++;
    // Increment distance calculation here if needed
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

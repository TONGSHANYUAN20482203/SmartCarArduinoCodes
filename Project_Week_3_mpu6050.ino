// Include necessary libraries
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
// Create an MPU6050 object
Adafruit_MPU6050 mpu;
// Define motor control pins
int in1 = 10;
int in2 = 12;
int in3 = 11;
int in4 = 13;
// Line following sensor pins
int outputA = A1;// IR sensor output in
int outputB = A0;
const int encoderPin = 2;// encoder pin
volatile int encoderPos = 0;
volatile bool lastA;
const int pulsePerTurn = 48; // encoder pulse each 360 degrees turn
const float wheelCircumference = 21.36;
// LCD object for display
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
bool beforeRamp = true; //Arduino smart car before ramp
bool onRamp = false;// indicator for Arduino smart car when onRamp or not onRamp
void setup() {
  // Initialize MPU6050
  Wire.begin();
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Angle:");
  lcd.setCursor(0, 1);
  lcd.print("Distance:");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}
void loop() {
  int sensorA = analogRead(outputA);//IR sensor output
  int sensorB = analogRead(outputB);
  int distanceFlag = 1;
  int thresholdA = 300;// IR sensor threshold
  int thresholdB = 300;
  if (beforeRamp) {// if function for car before climbing ramp
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    float angle = atan2(a.acceleration.y, sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.z, 2))) * 180 / PI;// roll angle equation
    lcd.setCursor(9, 0);
    lcd.print(angle);
    delay(200);
    if (!onRamp) {// when the car is not onRamp, when beforeRamp or complete rotation function
      if (a.acceleration.y < 1.5 && a.acceleration.y > -1.0) { //compare the acceleration on y-axis
        lineFollow(sensorA, sensorB, thresholdA, thresholdB);
      } else if (a.acceleration.y >= 1.5) {
        motorSpeedUp();// speed for car to climb the ramp
        onRamp = true;// after the car climp up the ramp, it's onRamp
      } else if (a.acceleration.y <= -1.0) {
        lineFollowBefore(sensorA, sensorB, thresholdA, thresholdB);// switch from MPU6050 function to IR sensor function
        beforeRamp = false;// the car already went off the ramp and proceed to line following
      }
    } else {
      if (a.acceleration.y < 1.5) {
        motorStop();// stop all moter
        delay(4000);
        motorTurn();// the car turn 360 degrees
        delay(420);
        motorStop();// stop all motor
        delay(1000);
        onRamp = false;// the car is already onRamp, then complete the rotation function
      }
    }
  } else { // switch to IR sensor function
    attachInterrupt(digitalPinToInterrupt(encoderPin), updateEncoder, CHANGE);
    float distance = ((encoderPos / pulsePerTurn) * wheelCircumference) / 2;
    lcd.setCursor(9, 1);
    lcd.print(distance);
    if (distance >= 90 && distanceFlag == 1) {// when distance larger than 90cm, distanceFlag=0 to stop for a moment
      motorStop();
      delay(2000);
      distanceFlag = 0;
    } else {// procceding to IR sensor function
      lineFollow(sensorA, sensorB, thresholdA, thresholdB);
    }
  }
}
void lineFollowBefore(int sensorA, int sensorB, int thresholdA, int thresholdB) {
  if (sensorA > thresholdA && sensorB > thresholdB) {
    motorStartBefore();
  } else if (sensorA > thresholdA) {
    leftMotorStart();
  } else if (sensorB > thresholdB) {
    rightMotorStart();
  } else {
    motorStartBefore();
  }
}
void lineFollow(int sensorA, int sensorB, int thresholdA, int thresholdB) {
  if (sensorA <= thresholdA && sensorB <= thresholdB) {
    motorStart();
  } else if (sensorA > thresholdA) {
    leftMotorStart();
  } else if (sensorB > thresholdB) {
    rightMotorStart();
  }

  if (sensorA >= thresholdA && sensorB >= thresholdB) {
    motorStop();
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
void motorStart() {
  analogWrite(in1, 80);
  analogWrite(in2, 0);
  analogWrite(in3, 80);
  analogWrite(in4, 0);
}
void motorStartBefore() {
  analogWrite(in1, 80);
  analogWrite(in2, 0);
  analogWrite(in3, 80);
  analogWrite(in4, 0);
}
void motorTurn() {
  analogWrite(in1, 150);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  analogWrite(in4, 150);
  delay(1425);
}
void motorSpeedUp() {
  analogWrite(in1, 150);
  digitalWrite(in2, LOW);
  analogWrite(in3, 150);
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
void motorStop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

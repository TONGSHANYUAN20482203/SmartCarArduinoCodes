#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

SoftwareSerial Bluetooth(0,1);//RX, TX

const int IN2 = A4;
const int IN1 = 11;
const int IN3 = A5;
const int IN4 = 12;
const int enABle = 3;
 
//initialise the lcd pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Bluetooth.begin(9600);
  pinMode(enABle, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(enABle, 100);
  
  lcd.begin(16,2);
  lcd.print("Direction:");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Bluetooth.available() > 0){
    char data = Bluetooth.read();
    processCommand(data);
  }
  delay(100);//implement delays in the code to smoothen the steering process 
}


void forward(){
  lcd.setCursor(0,1);
  lcd.print("Going forward");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward(){
  lcd.setCursor(0,1);
  lcd.print("Reversing");
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN3, LOW);
}

void left(){
  lcd.setCursor(0,1);
  lcd.print("Turning left");
  analogWrite(enABle, 180);
  digitalWrite(IN1, HIGH);
	digitalWrite(IN2, LOW);
	digitalWrite(IN3, LOW);
	digitalWrite(IN4, HIGH);
}

void right(){
  lcd.setCursor(0,1);
  lcd.print("Turning right");
  analogWrite(enABle, 180);
  digitalWrite(IN1, LOW);
	digitalWrite(IN2, HIGH);
	digitalWrite(IN3, HIGH);
	digitalWrite(IN4, LOW);
}
void stop(){
  lcd.setCursor(0,1);
  lcd.print("Car stopped");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void processCommand(char data){
  switch (data){
      case 'F':
        forward();
        break;
      case 'G':
        backward();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case 'S':
        stop();
        break;
    }
}
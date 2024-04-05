#include <LiquidCrystal.h>

const int fronttrigSensorPin = 9;   // Front sensor connected to digital pin 9
const int frontechoSenorPin = 8;    // Front sensor connected to digital pin 8
const int reartrigSensorPin = 6;    // Rear sensor connected to digital pin 6
const int rearechoSensorPin = 7;    // Rear sensor connected to digital pin 7
const int greenLEDPin = 11;         // Green LED connected to digital pin 11
const int redLEDPin = 1;            // Red LED connected to digital pin 1
const int buzzerPin = 13;           // Buzzer connected to digital pin 13

LiquidCrystal lcd(12, 10, 5, 4, 3, 2);  // LCD pins (RS, EN, D4, D5, D6, D7)

void setup() {
  pinMode(fronttrigSensorPin, OUTPUT);
  pinMode(frontechoSenorPin, INPUT);
  pinMode(reartrigSensorPin, OUTPUT);
  pinMode(rearechoSensorPin, INPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  lcd.begin(16, 2);  // Initialize the LCD with 16 columns and 2 rows
}

void loop() {
  int frontDistance = measureDistance(fronttrigSensorPin, frontechoSenorPin);
  int rearDistance = measureDistance(reartrigSensorPin, rearechoSensorPin);
  
  displayDistance("Front:", frontDistance, "Rear:", rearDistance);
  //displayDistance();
  
  if (frontDistance <= 200 || rearDistance <= 200) {
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(2000);
  } else {
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);
    digitalWrite(buzzerPin, HIGH);
    delay(2000);
  }
}

int measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

void displayDistance(String label1, int frontdistance, String label2,int reardistance) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(label1);
  lcd.print(frontdistance< 200 ? "Suspect" : "Safe ");
  lcd.print(frontdistance);
  lcd.print("cm");
  lcd.setCursor(0, 1);
  lcd.print(label2);
  lcd.print(reardistance < 200 ? "Suspect" : "Safe ");
  lcd.print(reardistance);
}


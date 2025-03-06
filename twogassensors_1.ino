#include <LiquidCrystal.h>

// Initialize the LCD with the pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 10, 7, 4, 2);

// Pin definitions
const int gasSensor1Pin = A0;  // First gas sensor analog pin
const int gasSensor2Pin = A1;  // Second gas sensor analog pin
const int buzzerPin = 8;       // Buzzer digital pin
const int buttonPin = 9;       // Push button digital pin
const int redPin = 3;          // RGB LED red pin
const int greenPin = 5;        // RGB LED green pin
const int bluePin = 6;         // RGB LED blue pin

// Thresholds for gas concentration
const int gasThreshold1 = 300;  // Threshold for first gas sensor
const int gasThreshold2 = 400;  // Threshold for second gas sensor

// Variables
bool alarmAcknowledged = false;  // To track if the alarm is acknowledged

void setup() {
  // Set up the LCD
  lcd.begin(16, 2);
  lcd.print("Gas Monitoring");

  // Set up the buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // Set up the push button
  pinMode(buttonPin, INPUT_PULLUP);  // Enable internal pull-up resistor

  // Set up the RGB LED
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  setRGBColor(0, 255, 0);  // Start with green (safe)
}

void loop() {
  // Read the gas sensor values
  int gasValue1 = analogRead(gasSensor1Pin);
  int gasValue2 = analogRead(gasSensor2Pin);

  // Display the gas values on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Gas1: ");
  lcd.print(gasValue1);
  lcd.print("   ");  // Clear leftover characters

  lcd.setCursor(0, 1);
  lcd.print("Gas2: ");
  lcd.print(gasValue2);
  lcd.print("   ");

  // Check if either gas level exceeds the threshold
  if (gasValue1 > gasThreshold1 || gasValue2 > gasThreshold2) {
    if (!alarmAcknowledged) {
      lcd.setCursor(0, 0);
      lcd.print("Danger! Gas Leak!");
      digitalWrite(buzzerPin, HIGH);  // Activate the buzzer
      setRGBColor(255, 0, 0);        // Set RGB to red (danger)
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Gas Level: Safe");
    digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
    setRGBColor(0, 255, 0);       // Set RGB to green (safe)
    alarmAcknowledged = false;    // Reset alarm acknowledgment
  }

  // Check if the button is pressed to acknowledge the alarm
  if (digitalRead(buttonPin) == LOW) {  // Button is active LOW
    alarmAcknowledged = true;
    digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
    setRGBColor(0, 0, 255);       // Set RGB to blue (acknowledged)
    delay(500);  // Debounce delay
  }

  delay(500);  // Wait before the next reading
}

// Function to set the RGB LED color
void setRGBColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

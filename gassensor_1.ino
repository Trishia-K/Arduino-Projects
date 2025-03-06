#include <LiquidCrystal.h>

// Initialize the LCD library with the pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pin definitions
const int gasSensorPin = A0;  // Gas sensor analog pin
const int buzzerPin = 8;      // Buzzer digital pin

// Threshold for gas concentration
const int gasThreshold = 300;  // Adjust based on your sensor and environment

void setup() {
  // Set up the LCD
  lcd.begin(16, 2);
  lcd.print("Gas Level:");

  // Set up the buzzer
  pinMode(buzzerPin, OUTPUT);
  //digitalWrite(buzzerPin, LOW);
}

void loop() {
  // Read the gas sensor value
  int gasValue = analogRead(gasSensorPin);

  // Display the gas value on the LCD
  lcd.setCursor(0, 1);
  lcd.print("Value: ");
  lcd.print(gasValue);
  lcd.print("    ");  // Clear any leftover characters

  // Check if the gas level exceeds the threshold
  if (gasValue > gasThreshold) {
    lcd.setCursor(0, 0);
    lcd.print("Gas Leak oops!");
    tone(buzzerPin, 100); 
    delay(500);
    tone(buzzerPin, 200);
    delay(500);
    tone(buzzerPin, 300);
    
    //digitalWrite(buzzerPin, HIGH);  // Activate the buzzer
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Gas Level: Safe");
    noTone(buzzerPin);
    //digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
  }

  delay(500);  // Wait for a short time before the next reading
}
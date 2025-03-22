#include <Servo.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// Pin definitions
const int flameSensorPin = A0;
const int buzzerPin = A1;
const int servoPin = 9;
const int pumpPin = 13; // pin for pump
const int redPin = 7;  // Red pin of RGB LED
const int greenPin = 8; // Green pin of RGB LED
const int bluePin = A4;  // Blue pin of RGB LED 
const int buttonPin = 6; // Button pin

// Variables
int flameDetected = 0;
bool fireDetected = false;
bool buzzerOn = false; // Track buzzer state

// Initialize components
Servo sprinklerServo;
int Contrast = 130;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7
SoftwareSerial bluetooth(A2, A3); // RX, TX for Bluetooth

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("System Initialized!");

  // Initialize components
  pinMode(flameSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT); // No internal pull-up, since using external resistor

  sprinklerServo.attach(servoPin);
  sprinklerServo.write(90); // Set servo to default position (90°)
  analogWrite(10, Contrast);
  lcd.begin(16, 2);
  lcd.print("Fire Strike");
  delay(3000);
  lcd.clear();

  // Initialize Bluetooth communication
  bluetooth.begin(9600);
  bluetooth.println("Bluetooth Connected!");
  Serial.println("Bluetooth Connected!");
}

void loop() {
  // Read flame sensor value
  flameDetected = analogRead(flameSensorPin);
  Serial.println("Flame Sensor Value: " + String(flameDetected));

  if (flameDetected < 500) { // Fire detected
  fireDetected = true;
  // Manual button to turn off the buzzer
  //if (digitalRead(buttonPin) == LOW) { // Button pressed (LOW because of external pull-up)
    if (buzzerOn) { // Only act if the buzzer is currently on
      Serial.println("Button pressed. Turning off buzzer.");
      noTone(buzzerPin); // Turn off the buzzer
      buzzerOn = false; // Update buzzer state
      delay(500); // Simple debounce delay
    }
  //}
    
    Serial.println("Fire detected! Setting LED to red.");
    setColor(255, 0, 0); // Red color
    tone(buzzerPin, 1000); // Turn on the buzzer with a 1kHz tone
    buzzerOn = true; // Buzzer is on
    digitalWrite(pumpPin, HIGH); // Turn on the pump

    // Rotate servo to 0°, 90°, and 180° in sequence
    rotateServo();

    // Display fire status on LCD
    lcd.setCursor(0, 0);
    lcd.print("Fire Detected!");
    lcd.setCursor(0, 1);
    lcd.print("Sprinkler Active");

    // Send fire status to Serial Monitor
    Serial.println("Fire Detected! Sprinkler Active");

    // Send fire status to smartphone
    bluetooth.println("Fire Detected! Sprinkler Active");

  } else { // No fire
    fireDetected = false;
    Serial.println("No fire detected. Setting LED to green.");
    setColor(0, 255, 0); // Green color
    noTone(buzzerPin); // Turn off the buzzer
    buzzerOn = false; // Buzzer is off
    digitalWrite(pumpPin, LOW); // Turn off the pump

    // Set servo to default position (90°)
    sprinklerServo.write(90);

    // Display safe status on LCD
    lcd.setCursor(0, 0);
    lcd.print("Status: Safe");
    lcd.setCursor(0, 1);
    lcd.print("No Fire Detected");

    // Send safe status to Serial Monitor
    Serial.println("Status: Safe - No Fire Detected");

    // Send safe status to smartphone
    bluetooth.println("Status: Safe");
  }

  // Bluetooth control
  if (bluetooth.available()) {
    String bluetoothData = bluetooth.readStringUntil('\n');
    bluetoothData.trim();
    Serial.println("Bluetooth Command: " + bluetoothData);

    if (bluetoothData == "PUMP ON") {
      digitalWrite(pumpPin, HIGH);
      bluetooth.println("Pump turned ON");
      Serial.println("Pump turned ON via Bluetooth");
      setColor(255, 255, 0); // Yellow color for manual pump control
    } else if (bluetoothData == "PUMP OFF") {
      digitalWrite(pumpPin, LOW);
      bluetooth.println("Pump turned OFF");
      Serial.println("Pump turned OFF via Bluetooth");
      setColor(0, 255, 0); // Green color for safe state
    } else if (bluetoothData.startsWith("ANGLE")) {
      int angle = bluetoothData.substring(6).toInt();
      sprinklerServo.write(angle);
      bluetooth.println("Sprinkler angle set to: " + String(angle) + " deg");
      Serial.println("Sprinkler angle set to: " + String(angle) + " deg via Bluetooth");
      setColor(0, 0, 255); // Blue color for manual angle control
    }
  }

  delay(100); // Small delay for stability
}

// Function to set RGB LED color
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
  Serial.println("LED Color - R: " + String(red) + ", G: " + String(green) + ", B: " + String(blue));
}

// Function to rotate servo to 0°, 90°, and 180°
void rotateServo() {
  int angles[] = {0, 90, 180}; // Angles to rotate to
  for (int i = 0; i < 3; i++) {
    sprinklerServo.write(angles[i]); // Move servo to the current angle
    delay(1000); // Wait for 1 second at each angle
  }
}
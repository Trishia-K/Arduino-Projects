#include <Servo.h>
#include <SoftwareSerial.h>

// Pin Definitions
const int trigFront = 4;
const int echoFront = 5;
const int trigBack = 3;
const int echoBack = 2;
const int buzzer = 6;
const int redLED = 7;
const int greenLED = 8;
const int servoFrontPin = 10;
const int servoBackPin = 9;

// Bluetooth Module
SoftwareSerial bluetooth(11, 12); // RX, TX

// Servo Objects
Servo servoFront;
Servo servoBack;

// Variables
int frontDistance = 0;
int backDistance = 0;
int buzzerFrequency = 0;

void setup() {
  // Initialize Serial and Bluetooth
  Serial.begin(9600);
  bluetooth.begin(9600);

  // Initialize Pins
  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);
  pinMode(trigBack, OUTPUT);
  pinMode(echoBack, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Attach Servos
  servoFront.attach(servoFrontPin);
  servoBack.attach(servoBackPin);

  // Set Initial Servo Positions
  servoFront.write(90); // Center position
  servoBack.write(90);  // Center position
}

// Function to measure distance using ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}

void loop() {
  // Measure distances
  frontDistance = getDistance(trigFront, echoFront);
  backDistance = getDistance(trigBack, echoBack);

  // Print distances to Serial Monitor
  Serial.print("Front Distance: ");
  Serial.print(frontDistance);
  Serial.print(" cm, Back Distance: ");
  Serial.print(backDistance);
  Serial.println(" cm");

  // Front Sensor Logic
  if (frontDistance <= 50) { // Obstacle within 50 cm
    servoFront.write(0);      // Turn servo to scan left
    delay(500);
    servoFront.write(180);    // Turn servo to scan right
    delay(500);
    servoFront.write(90);     // Return to center

    // Buzzer and LED Feedback
    buzzerFrequency = map(frontDistance, 0, 100, 2000, 100); // Higher frequency as obstacle gets closer
    tone(buzzer, buzzerFrequency);
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);

    // Send Bluetooth Alert
    bluetooth.println("Obstacle detected in front!");
    Serial.println("Obstacle detected in front!"); // Serial output
  } else {
    noTone(buzzer);
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
  }

  // Back Sensor Logic
  if (backDistance <= 50) { // Obstacle within 50 cm
    tone(buzzer, 1000);     // Constant tone for back obstacle
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    servoBack.write(0);      // Turn servo to scan left
    delay(500);
    servoBack.write(180);    // Turn servo to scan right
    delay(500);
    servoFront.write(90);

    // Send Bluetooth Alert
    bluetooth.println("Obstacle detected behind!");
    Serial.println("Obstacle detected behind!"); // Serial output
  } else {
    noTone(buzzer);
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
  }

  delay(100); // Small delay for stability
}
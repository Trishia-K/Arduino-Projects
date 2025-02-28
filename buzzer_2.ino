#define LDR A0
#define RED_LED 11
#define GREEN_LED 13
#define Buzzer 12
#define Button 2
#define THRESHOLD 500

void setup(){
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop(){
  int lightlevel = analogRead(A1);
  Serial.print("Light level: ");
  Serial.println(lightlevel);

  if(lightlevel > 100){
    digitalWrite(11, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);

      if (digitalRead(2) == LOW){delay(100);
      if(digitalRead(2)== LOW){
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);
      }
     }
  } else{
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);}

    delay(1000);
}

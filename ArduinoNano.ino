int8_t trig1 = 2;
int8_t echo1 = 3;
int8_t trig2 = 5;
int8_t echo2 = 4;
int8_t buzzer = 6;
uint8_t distance1;
uint8_t distance2;
uint64_t time = 0;
bool x = true;
// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(2000000);
}
uint64_t time1 = millis();
String sRead;
// the loop function runs over and over again until power down or reset
void loop() {
  if (millis() - time1 > 200) {
    distance1 = senzorCall(trig1, echo1);
  }
  if (millis() - time1 > 400) {
    distance2 = senzorCall(trig2, echo2);

    if (distance1 < 40)
      Serial.print(distance1 / 10);
    else
      Serial.print(5);
    if (distance2 < 40)
      Serial.println(distance2 / 10);
    else
      Serial.println(5);

    time1 = millis();
  }
  if (sRead.toInt() == 0)
    if (distance1 < 10 || distance2 < 10)
      analogWrite(buzzer, 127);
    else if (distance1 < 20 || distance2 < 20)
      buzzerCall(250);
    else if (distance1 < 30 || distance2 < 30)
      buzzerCall(500);
    else if (distance1 < 40 || distance2 < 40)
      buzzerCall(750);
    else
     analogWrite(buzzer, 0);

  if (Serial.available() > 0) {
    sRead = Serial.readStringUntil('\n');
  }
  if (distance1 > 40 && distance2 > 40 || sRead.toInt() != 0)
    analogWrite(buzzer, sRead.toInt()*127);

}
int senzorCall(int trig, int echo) {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long timecount = pulseIn(echo, HIGH);
  return (timecount * 0.034) / 2;
}

void buzzerCall(int perioada) {
  if (x) {
    time = millis();
    x = false;
  }

  if (millis() - time < 50)
    analogWrite(buzzer, 127);
  else if (millis() - time < perioada)
    analogWrite(buzzer, 0);
  else
    x = true;
}

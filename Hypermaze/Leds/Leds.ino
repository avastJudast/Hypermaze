
void setup() {
  //LED pins
  pinMode(13, OUTPUT);//LED
  pinMode(12, OUTPUT);//LED
  pinMode(11, OUTPUT);//LED
  pinMode(10, OUTPUT);//PUMP
  //rat snesor is A0 bc its analog
  Serial.begin(9600);
}

void loop() {
  //to test the leds, we'll use the rat sensor as a proximity sensor
  Serial.println("Analog: " + String(analogRead(A0)));
  if (analogRead(A0) > 400) {
    digitalWrite(13, HIGH);
    digitalWrite(10, HIGH);
  }
  if (analogRead(A0) > 300) {
    digitalWrite(12, HIGH);
  }
  if (analogRead(A0) > 270) {
    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
  }
  delay(1000);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  delay(500);
}

int sensorPin = A5;    // select the input pin for the potentiometer
//int ditigtal = 13;
void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
}

void loop() {
  // read the value from the sensor:L
  Serial.println(analogRead(sensorPin));
  delay(100);
} 

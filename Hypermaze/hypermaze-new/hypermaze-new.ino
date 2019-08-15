//.ino use arduino compiler
//FOR SLAVE CONTROLLERS
#define LEDS 13
#define exitSensorOut 12
#define exitSensorIn 11
#define pump 10
//#define  9
#define highLow 8 //input from master
#define releaseDirection 7
#define exitSensorRaw 6
#define ratShared 5
#define ratSharedOut 4
#define closedSensor 3
#define motorStep 2
#define motorDir 1
#define ratSensor A0


bool mode;
int motorSpeed;
bool IHaveRat;
bool feedState;

//Functions
void feed() {
  digitalWrite(pump, HIGH);
  delay(50);//50 milliseconds == 100uL of sauce
  digitalWrite(pump, LOW);
}
void dispense() {
  int rando = random(0, 100);
  if (mode == true) { //EX 2
    if (feedState == LOW) { //high is 1 low is 0
      if (rando <= 15) {//15% chance for low
        feed();
      }
    } else {
      if (rando <= 85) { //85% chance for high
        feed();
      }
    }
  } else { //EX 1
    if (feedState == HIGH) {
      feed();
    } else {
      //dont feed
    }
  }
}

void Release() {
    digitalWrite(motorDir, digitalRead(releaseDirection)); //release south == LOW north == HIGH
    //take steps as to take it off the closedSensor
    Buffer();
    //now the conditional will work
    Serial.println("waiting for closed senseor = low");
    while (digitalRead(closedSensor) == HIGH);//moves to n/s position
    {
      Step();
    }
    Serial.println("done");
    Serial.println("waiting for rat to trigger exit sensor");
    while (digitalRead(exitSensorRaw) == HIGH || digitalRead(exitSensorIn)) {
    //wait for rat to leave
    }
  digitalWrite(exitSensorOut, HIGH);//will trigger the other sensor
  Serial.println("done");
  //take it off the sensor
  Buffer();
  while (digitalRead(closedSensor) == HIGH);//moves to goal position
  {
    Step();
  }
  digitalWrite(exitSensorOut, LOW);
}
/*
   how to operate motor
    //TURNS CLOCKWISE
  digitalWrite(motorDir, HIGH);
    //TURNS COUNTER CLOCKWISE
  digitalWrite(motorDir, LOW);
    //DRIVES THE MOTOR
  digitalWrite(motorStep, HIGH);
  delay(motorSpeed);//smalller == faster
  digitalWrite(motorStep, LOW);
*/
void Step() {
  digitalWrite(motorStep, HIGH);
  delay(motorSpeed);
  digitalWrite(motorStep, LOW);
}

void Buffer() {
  Serial.println("got to buffer");
  while (digitalRead(closedSensor) == LOW) {
    Step();
  }
  Serial.println("exiting buffer");
}
void Close() {
  Serial.println("got into close methed");
  Buffer();
  Serial.println("waiting for closed sensor");
  while (digitalRead(closedSensor) == HIGH) {
    Step();
  }
  Serial.println("done");
  Serial.println("hit n/s spot");
  Buffer();
  Serial.println("waiting for closed sensor (again)");
  while (digitalRead(closedSensor) == HIGH) {
    Step();
  }
  Serial.println("done");
}
void setup() {
  Serial.begin(9600);
  //initialise every pin to output just in case
  for (int i = 0; i <= 13; i++) {
    pinMode(i, OUTPUT);

  }
  //initialise input pins
  pinMode(ratSensor, INPUT);
  pinMode(closedSensor, INPUT);
  pinMode(highLow, INPUT);
  pinMode(exitSensorRaw, INPUT);
  pinMode(ratShared, INPUT);
  pinMode(exitSensorIn, INPUT);
  randomSeed(analogRead(A5));//needed for random numbers
  //initial behaviors
  motorSpeed = 15;//the smaller the number the faster the motor
  digitalWrite(motorDir, LOW);
  digitalWrite(ratShared, LOW);
  digitalWrite(exitSensorOut, LOW);
  if (digitalRead(highLow) == HIGH) {
    mode = true;//deterministic
  } else {
    mode = false;//probabilistic
  }
  IHaveRat = false;
  feedState = LOW;
//wait for rat to be placed
while(ratSensor < 400){}
Release();
}

void loop() {
  if (analogRead(ratSensor) > 400) { //if theres a rat
    digitalWrite(ratSharedOut, HIGH);//broadcast to everything else
    Serial.println("rat sensor detected rat");
    IHaveRat = true;
  }
  if (digitalRead(ratShared) == HIGH) { //if a rat was sensed anywhere
    //close the door
    Close();
    Serial.println("door should be closed");
    if (IHaveRat) { //if holding a rat
      Serial.println("go to dispense()");
      Serial.println("delay 15 seconds");
      dispense();//takes 50 milliseconds to feed
      delay(14950);//wait for a total 15 seconds
      Serial.println("done");
    }
    else { //no rat inside
      Serial.println("no rat inside, wait 15 seconds");
      delay(15000);//wait a total of 15 seconds
      Serial.println("done");
    }
    Release();//n/s behavior and then goal behavior
    Serial.println("leds should be on, rat is released.");
    digitalWrite(ratSharedOut, LOW);
    digitalWrite(exitSensorOut, LOW);
    Serial.println("door should be facing goal position");
    exit(0);
    feedState = digitalRead(highLow);
  }
  //while waiting in goal position for rat
  Serial.println("waiting for rat to trigger catch");
}

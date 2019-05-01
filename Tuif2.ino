#include<Bounce2.h>
#include<Servo.h>
#include<Wire.h>
//Pins
const int leftBtnPin = 50;
const int rightBtnPin = 49;
const int joystickBtnPin = 51;
const int encoderPinA = 52;
const int encoderPinB = 53;
const int servoPin = 3;
//Vars
int angle = 0;
int encoderPos = 0;
int encoderStepsPerRevolution = 16;
int verticalAim = 0;
int horizontalAim = 0;
int pos = 0;
boolean leftBtn = false;
boolean rightBtn = false;
boolean joystickBtn = false;
boolean encoderALast = LOW; //remembers the previous state of the encoder pin A
Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();
Bounce debouncer3 = Bounce();
boolean previousState1 = false;
boolean switchState1 = false;
boolean previousState2 = false;
boolean switchState2 = false;
boolean previousState3 = false;
boolean switchState3 = false;
Servo myservo;
const int MPU_addr=0x68; // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup() {
  // DI
  pinMode(leftBtnPin, INPUT_PULLUP);
  debouncer1.attach(leftBtnPin);
  debouncer1.interval(5);
  pinMode(rightBtnPin, INPUT_PULLUP);
  debouncer2.attach(rightBtnPin);
  debouncer2.interval(5);
  pinMode(joystickBtnPin, INPUT_PULLUP);
  debouncer3.attach(joystickBtnPin);
  debouncer3.interval(5);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  // AI
  pinMode(A0, INPUT); //x-Axis
  pinMode(A1, INPUT); //y-Axis
  // AO
  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  //Serial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // Update DIs
  debouncer1.update();
  debouncer2.update();
  debouncer3.update();
  boolean currentState1 = !debouncer1.read();
  boolean currentState2 = !debouncer2.read();
  boolean currentState3 = !debouncer3.read();
  if ((previousState1 == LOW) && (currentState1 == HIGH)) {
    leftBtn = HIGH;
  }
  else
  {
    leftBtn = LOW;
  }

  if ((previousState2 == LOW) && (currentState2 == HIGH)) {
    rightBtn = HIGH;
  }
  else
  {
    rightBtn = LOW;
  }

  if ((previousState3 == LOW) && (currentState3 == HIGH)) {
    joystickBtn = HIGH;
  }
  else
  {
    joystickBtn = LOW;
  }
  previousState1 = currentState1;
  previousState2 = currentState2;
  previousState3 = currentState3;

  boolean encoderA = digitalRead(encoderPinA);
  if ((encoderALast == HIGH) && (encoderA == LOW))
  {
    if (digitalRead(encoderPinB) == LOW)
    {
      encoderPos--;
    }
    else
    {
      encoderPos++;
    }
    //angle = (encoderPos % encoderStepsPerRevolution) * 360 / encoderStepsPerRevolution;
    angle = (((encoderPos % encoderStepsPerRevolution) * 360) + (encoderStepsPerRevolution/2) )/ encoderStepsPerRevolution;
  }
  encoderALast = encoderA;

  // Update AIs
  verticalAim = analogRead(A0);
  horizontalAim = analogRead(A1);

  //Update AO
  //myservo.write(angle);
  myservo.write(map(angle, 0,360,0,180));

  //Read BUS
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true); // request a total of 14 registers

  AcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  //Serial Message (verticalAim[-100,100], horizontalAim[-100,100], encoderPos[-32768,32767], angle[-360,360],leftBtn[0,1],rightBtn[0,1],joystickBtn[0,1])
  Serial.print(map(verticalAim, 0, 1023, -100, 100));
  Serial.print(",");
  Serial.print(map(horizontalAim, 0, 1023, -100, 100));
  Serial.print(",");
  Serial.print(encoderPos);
  Serial.print(",");
  Serial.print(angle);
  Serial.print(",");
  Serial.print(leftBtn);
  Serial.print(",");
  Serial.print(rightBtn);
  Serial.print(",");
  Serial.print(joystickBtn);
  Serial.print(",");
  Serial.print(AcX);
  Serial.print(",");
  Serial.print(AcY);
  Serial.print(",");
  Serial.print(AcZ);
  Serial.print(",");
  Serial.print(Tmp/340.00+36.53);
  Serial.print(",");
  Serial.print(GyX);
  Serial.print(",");
  Serial.print(GyY);
  Serial.print(",");
  Serial.println(GyZ);
  //delay(333);
  //delay(10);
}

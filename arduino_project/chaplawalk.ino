#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 150  // Minimum pulse length count (approx. 0°)
#define SERVOMAX 500  // Maximum pulse length count (approx. 180°)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz PWM frequency

class CustomServo
{
public:
  int pinNum = 0;
  int defaultPosition = 0;
  int curPos = 0;
  CustomServo(int pinNum, int defaultPosition)
  {
    this->defaultPosition = defaultPosition;
    this->pinNum = pinNum;
    this->curPos = defaultPosition;
  }

  void CapitalMoveServo(int where)
  {
    pwm.setPWM(this->pinNum, 0, where);
    this->curPos = where;
  }

  void MoveServo(int shift, int delayNew)
  {

    if (shift >= 0)
    {
      for (int i = this->curPos; i <= shift + this->curPos; i++)
      {
        pwm.setPWM(this->pinNum, 0, i);
        delay(delayNew);
      }
    }

    else
    {
      for (int i = this->curPos; i >= this->curPos + shift; i--)
      {
        pwm.setPWM(this->pinNum, 0, i);
        delay(delayNew);
      }
    }
    this->curPos += shift;
  }
};

// legs
CustomServo *servo0 = new CustomServo(0, (SERVOMAX + SERVOMIN) / 2 - 10);
CustomServo *servo1 = new CustomServo(1, (SERVOMAX + SERVOMIN) / 2 - 5);
CustomServo *servo2 = new CustomServo(2, (SERVOMAX + SERVOMIN) / 2);
CustomServo *servo3 = new CustomServo(3, (SERVOMAX + SERVOMIN) / 2 - 7);
CustomServo *servo4 = new CustomServo(4, (SERVOMAX + SERVOMIN) / 2 - 5);
CustomServo *servo11 = new CustomServo(11, (SERVOMAX + SERVOMIN) / 2 - 6);
CustomServo *servo12 = new CustomServo(12, (SERVOMAX + SERVOMIN) / 2 - 5);
CustomServo *servo13 = new CustomServo(13, (SERVOMAX + SERVOMIN) / 2);
CustomServo *servo14 = new CustomServo(14, (SERVOMAX + SERVOMIN) / 2 - 5);
CustomServo *servo15 = new CustomServo(15, (SERVOMAX + SERVOMIN) / 2 - 5);

// arms
CustomServo *servo5 = new CustomServo(5, (SERVOMAX + SERVOMIN) / 2);
CustomServo *servo6 = new CustomServo(6, (SERVOMAX + SERVOMIN) / 2 - 10);
CustomServo *servo7 = new CustomServo(7, (SERVOMAX + SERVOMIN) / 2);
CustomServo *servo8 = new CustomServo(8, (SERVOMAX + SERVOMIN) / 2);
CustomServo *servo9 = new CustomServo(9, (SERVOMAX + SERVOMIN) / 2);
CustomServo *servo10 = new CustomServo(10, (SERVOMAX + SERVOMIN) / 2);

void defaultPose()
{
  // legs
  servo0->CapitalMoveServo(servo0->defaultPosition);
  servo1->CapitalMoveServo(servo1->defaultPosition);
  servo2->CapitalMoveServo(servo2->defaultPosition);
  servo3->CapitalMoveServo(servo3->defaultPosition);
  servo4->CapitalMoveServo(servo4->defaultPosition);
  servo11->CapitalMoveServo(servo11->defaultPosition);
  servo12->CapitalMoveServo(servo12->defaultPosition);
  servo13->CapitalMoveServo(servo13->defaultPosition);
  servo14->CapitalMoveServo(servo14->defaultPosition);
  servo15->CapitalMoveServo(servo15->defaultPosition);

  // arms
  servo5->CapitalMoveServo(servo5->defaultPosition);
  servo6->CapitalMoveServo(servo6->defaultPosition);
  servo7->CapitalMoveServo(servo7->defaultPosition);
  servo8->CapitalMoveServo(servo8->defaultPosition);
  servo9->CapitalMoveServo(servo9->defaultPosition);
  servo10->CapitalMoveServo(servo10->defaultPosition);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Servo test!");

  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);

  Wire.setClock(400000);
  defaultPose();
  chaplaMove(1);
}

void chaplaMove(int num)
{
  for (int k = 0; k < num; k++)
  {
    delay(1001);
    Serial.println(1);
    servo0->MoveServo(-20, 20);
    servo15->MoveServo(-20, 20); // Stage 1

    delay(1001);
    Serial.println(2);
    servo3->MoveServo(40, 15); // Stage 2
    servo2->MoveServo(-30, 15);
    servo1->MoveServo(-30, 15);

    delay(1001);
    Serial.println(3);
    servo15->MoveServo(20, 20); // Stage 3
    servo0->MoveServo(20, 20);

    delay(1001);
    Serial.println(4);
    servo3->MoveServo(20, 15); // Stage 4
    servo12->MoveServo(-20, 15);

    delay(1001);
    Serial.println(5);
    servo1->MoveServo(20, 15); // Stage 5
    servo14->MoveServo(-20, 15);

    delay(1001);
    Serial.println(6);
    servo15->MoveServo(20, 20); // Stage 6
    servo0->MoveServo(20, 20);

    delay(1001);
    Serial.println(7);
    servo13->MoveServo(30, 15);
    servo12->MoveServo(-40, 15); // Stage 7
    servo14->MoveServo(30, 15);

    delay(1001);
    Serial.println(8);
    servo15->MoveServo(-20, 20); // Stage 8
    servo0->MoveServo(-20, 20);
    delay(1000);

    // Serial.println(9);
    // servo3->MoveServo(-60, 10); // Stage 9
    // servo12->MoveServo(60, 10);

    // servo2->MoveServo(30, 10);
    // servo13->MoveServo(-30, 10);
    // delay(1000);
    defaultPose();
  }
}

void loop()
{
  Serial.println("eetes");
  delay(1000);
}
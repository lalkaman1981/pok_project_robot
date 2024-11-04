#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // Minimum pulse length count (approx. 0°)
#define SERVOMAX  500 // Maximum pulse length count (approx. 180°)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz PWM frequency

class CustomServo {        
    public:
    int pinNum = 0;
    int defaultPosition = 0;
    int curPos = 0;
    CustomServo(int pinNum, int defaultPosition) {
        this->defaultPosition = defaultPosition;
        this->pinNum = pinNum;
        this->curPos = defaultPosition;
    }

    void CapitalMoveServo(int where) {
        pwm.setPWM(this->pinNum, 0, where);
        this->curPos = where;
    }

    void MoveServo(int shift, int delayNew) {

      if (shift >= 0) {
          for (int i = this->curPos; i <= shift + this->curPos; i++) {
              pwm.setPWM(this->pinNum, 0, i);
              delay(delayNew);
          }
      }

      else {
          for (int i = this->curPos; i >= this->curPos + shift; i--) {
              pwm.setPWM(this->pinNum, 0, i);
              delay(delayNew);
          }
      }
      this->curPos += shift;
    }
};

CustomServo* servo0 = new CustomServo(10, (SERVOMAX + SERVOMIN) / 2 - 12);
CustomServo* servo1 = new CustomServo(11, (SERVOMAX + SERVOMIN) / 2);

CustomServo* servo2 = new CustomServo(2, (SERVOMAX + SERVOMIN) / 2);
CustomServo* servo3 = new CustomServo(3, (SERVOMAX + SERVOMIN) / 2);
CustomServo* servo4 = new CustomServo(4, (SERVOMAX + SERVOMIN) / 2 + 5);
CustomServo* servo5 = new CustomServo(5, (SERVOMAX + SERVOMIN) / 2 + 5);
CustomServo* servo6 = new CustomServo(6, (SERVOMAX + SERVOMIN) / 2);
CustomServo* servo7 = new CustomServo(7, (SERVOMAX + SERVOMIN) / 2);

CustomServo* servo8 = new CustomServo(8, (SERVOMAX + SERVOMIN) / 2 + 8);
CustomServo* servo9 = new CustomServo(9, (SERVOMAX + SERVOMIN) / 2 - 5);

void defaultPose() {
  // pwm.setPWM(servo0->pinNum, 0, servo0->defaultPosition);
  // pwm.setPWM(servo1->pinNum, 0, servo1->defaultPosition);
  // pwm.setPWM(servo2->pinNum, 0, servo2->defaultPosition);
  // pwm.setPWM(servo3->pinNum, 0, servo3->defaultPosition);
  // pwm.setPWM(servo4->pinNum, 0, servo4->defaultPosition);
  // pwm.setPWM(servo5->pinNum, 0, servo5->defaultPosition);
  // pwm.setPWM(servo6->pinNum, 0, servo6->defaultPosition);
  // pwm.setPWM(servo7->pinNum, 0, servo7->defaultPosition);
  // pwm.setPWM(servo8->pinNum, 0, servo8->defaultPosition);
  // pwm.setPWM(servo9->pinNum, 0, servo9->defaultPosition);
  // servo0->MoveServo(0, 5);
  // servo1->MoveServo(0, 5);
  // servo2->MoveServo(0, 5);
  // servo3->MoveServo(0, 5);
  // servo4->MoveServo(0, 5);
  // servo5->MoveServo(0, 5);
  // servo6->MoveServo(0, 5);
  // servo7->MoveServo(0, 5);
  // servo8->MoveServo(0, 5);
  // servo9->MoveServo(0, 5);
  servo0->CapitalMoveServo(servo0->defaultPosition);
  servo1->CapitalMoveServo(servo1->defaultPosition);
  servo2->CapitalMoveServo(servo2->defaultPosition);
  servo3->CapitalMoveServo(servo3->defaultPosition);
  servo4->CapitalMoveServo(servo4->defaultPosition);
  servo5->CapitalMoveServo(servo5->defaultPosition);
  servo6->CapitalMoveServo(servo6->defaultPosition);
  servo7->CapitalMoveServo(servo7->defaultPosition);
  servo8->CapitalMoveServo(servo8->defaultPosition);
  servo9->CapitalMoveServo(servo9->defaultPosition);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Servo test!");

  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at 50 Hz
  
  Wire.setClock(400000);       // Set I2C speed to 400kHz      
  defaultPose();
  chaplaMove(3);
}

void chaplaMove(int num) {
  for (int k = 0; k < num; k++){
    delay(500);
    Serial.println(1);
    servo0->MoveServo(-20, 20);
    servo5->MoveServo(-20, 20); // Stage B

    delay(500);
    Serial.println(2);
    servo3->MoveServo(40, 15); // Stage C
    servo2->MoveServo(-30, 15);
    servo1->MoveServo(-30, 15);

    delay(500);
    Serial.println(3);
    servo5->MoveServo(20, 20); // Stage D
    servo0->MoveServo(20, 20);
    
    delay(500);
    Serial.println(4);
    servo3->MoveServo(20, 15); // Stage E
    servo8->MoveServo(-20, 15);
    
    delay(500);
    Serial.println(5);
    servo1->MoveServo(20, 15); // Stage E
    servo6->MoveServo(-20, 15);

    delay(500);
    Serial.println(6);
    servo5->MoveServo(20, 20); // Stage B
    servo0->MoveServo(20, 20);

    delay(500);
    Serial.println(7);
    servo7->MoveServo(30, 15);
    servo8->MoveServo(-40, 15); // Stage C
    servo6->MoveServo(30, 15);

    delay(500);
    Serial.println(8);
    servo5->MoveServo(-20, 20); // Stage B
    servo0->MoveServo(-20, 20);
    delay(1000);
    defaultPose();
  }
}

void loop() {
    Serial.println("eetes");
    // defaultPose();
    // chaplaMove(10);
    delay(1000);
    // defaultPose();
    // servo3->MoveServo(50, 15);
}

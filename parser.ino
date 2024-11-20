#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <vector>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150
#define SERVOMAX  500
#define SERVO_FREQ 50
#define NUM_SERVOS 17

class CustomServo {        
    public:
    int pinNum = 0;
    int defaultPosition = 0;
    int curPos = 0;
    bool reverse = false; //2345
    CustomServo(int pinNum, int defaultPosition, bool reverse = false) { //2345
        this->defaultPosition = defaultPosition;
        this->pinNum = pinNum;
        this->curPos = defaultPosition;
        this->reverse = false; //2345
    }

    void CapitalMoveServo(int where) {
        pwm.setPWM(this->pinNum, 0, where);
        this->curPos = where;
    }

    void MoveServo(int shift, int delayNew) {

          if (this->reverse == true) { //2345
              shift = -1 * shift;
          }

      int maxPos = shift + this->curPos;

      if (shift >= 0) {
          for (int i = this->curPos; i <= maxPos; i++) {
              pwm.setPWM(this->pinNum, 0, i);
              delay(delayNew);
          }
      }

      else {
          for (int i = this->curPos; i >= maxPos; i--) {
              pwm.setPWM(this->pinNum, 0, i);
              delay(delayNew);
          }
      }
      this->curPos += shift;
    }
};

void parseCommand(String command);
void moveServos();

CustomServo* servos[NUM_SERVOS];
int positions[NUM_SERVOS];
unsigned long moveTime = 500;
unsigned long delayTime = 500;
int activeServos = 0;

void defaultPose() {
  for (int i = 0; i < NUM_SERVOS; ++i) {
    if (servos[i] != nullptr) {
      servos[i]->CapitalMoveServo(servos[i]->defaultPosition);
    }
  }
}

void setup() {
    Serial.begin(9600);

    pwm.begin();
    pwm.setPWMFreq(SERVO_FREQ);
    
    Wire.setClock(400000);

    servos[9] = new CustomServo(10, (SERVOMAX + SERVOMIN) / 2 - 3);
    servos[10] = new CustomServo(11, (SERVOMAX + SERVOMIN) / 2 + 3);
    servos[11] = new CustomServo(2, (SERVOMAX + SERVOMIN) / 2 + 4);
    servos[12] = new CustomServo(3, (SERVOMAX + SERVOMIN) / 2 + 5);
    servos[13] = new CustomServo(4, (SERVOMAX + SERVOMIN) / 2 + 4);
    servos[1] = new CustomServo(5, (SERVOMAX + SERVOMIN) / 2 + 4);
    servos[2] = new CustomServo(6, (SERVOMAX + SERVOMIN) / 2 + 2);
    servos[3] = new CustomServo(7, (SERVOMAX + SERVOMIN) / 2 + 4);
    servos[4] = new CustomServo(8, (SERVOMAX + SERVOMIN) / 2 + 3);
    servos[5] = new CustomServo(9, (SERVOMAX + SERVOMIN) / 2 + 5);

    defaultPose();
}

void loop() {
  if (Serial.available()) {
      String command = Serial.readStringUntil('\n');
      Serial.println(command);
      if (command == "def") {
        defaultPose();
      } else {
        parseCommand(command);
        moveServos();
      }
      delay(delayTime);
  }
}

void parseCommand(String command) {

    moveTime = 500;
    delayTime = 500;

    for (int i = 0; i < NUM_SERVOS; i++) {
        positions[i] = -1;
    }

    int len = command.length();
    int tIndex = command.indexOf("T");

    if (tIndex != -1) {
        int tmp = 0;
        for (int i = tIndex; i <= len && command[i] != 'D'; ++i){
            tmp = i;
        }
        moveTime = command.substring(tIndex + 1, tmp).toInt();
    }

    int dIndex = command.indexOf("D");
    if (dIndex != -1) {
        delayTime = command.substring(dIndex + 1).toInt();
        Serial.println(delayTime);
    }

    std::vector<String> vec;
    String temp = "";

    int tempInd = 0;

    if (dIndex == tIndex) {
        tempInd = len;
    } else if (dIndex < tIndex){
        tempInd = (dIndex != -1) ? dIndex : tIndex;
    } else {
        tempInd = (tIndex != -1) ? tIndex : dIndex;
    }

    for (int j = 0; j < tempInd; ++j) {
        if (command[j] == '#') {
            if (temp != "") {
                vec.push_back(temp);
                temp = "";
            }
        } else {
            temp += command[j];
        }
    }

    for (String commandPart : vec) {
        byte motor = 0;
        temp = "";

        for (char ch : commandPart) {
            if (ch == 'P') {
                motor = temp.toInt();
                temp = "";
            } else {
                temp += ch;
            }
        }
        positions[motor] = temp.toInt();
    }
}

void moveServos() {
    for (int i = 0; i < NUM_SERVOS; ++i) {
        if (servos[i] != nullptr && positions[i] != -1) {
            int angle = map(positions[i], 0, 1000, 150, 500);
            CustomServo* servo = servos[i];
            servo->MoveServo(angle - servo->curPos, moveTime / NUM_SERVOS);
        }
    }
}

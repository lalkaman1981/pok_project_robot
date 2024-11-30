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
    bool reverse = false;
    double multiplier = 1.0; // cooc
    int offset = 0;
    CustomServo(int pinNum, int defaultPosition, int offset = 0, bool reverse = false, double multiplier = 1.0) { //cooc
        this->defaultPosition = defaultPosition;
        this->pinNum = pinNum;
        this->curPos = defaultPosition;
        this->reverse = reverse; //2345
        this->multiplier = multiplier; // cooc
        this->offset = offset;
    }

    void CapitalMoveServo(int where) {
        pwm.setPWM(this->pinNum, 0, where);
        this->curPos = where;
    }

    void MoveServo(int shift, int time) {

        if (shift == 0) return;

        int maxPos = shift + this->curPos;

        double delayNew = (time / abs(shift)) * 1000;

        if (shift >= 0) {
            for (int i = this->curPos; i <= maxPos; i++) {
                pwm.setPWM(this->pinNum, 0, i);
                delayMicroseconds(delayNew);
            }
        }

        else {
            for (int i = this->curPos; i >= maxPos; i--) {
                pwm.setPWM(this->pinNum, 0, i);
                delayMicroseconds(delayNew);
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
      servos[i]->CapitalMoveServo(servos[i]->defaultPosition + servos[i]->offset);
    }
  }
}

void setup() {
    Serial.begin(9600);

    pwm.begin();
    pwm.setPWMFreq(SERVO_FREQ);
    
    Wire.setClock(400000);
    bool reverse = false;

    servos[1] = new CustomServo(15, (SERVOMAX + SERVOMIN) / 2, -5);
    servos[2] = new CustomServo(14, (SERVOMAX + SERVOMIN) / 2, -5);
    servos[3] = new CustomServo(13, (SERVOMAX + SERVOMIN) / 2);
    servos[4] = new CustomServo(12, (SERVOMAX + SERVOMIN) / 2, -5, true);
    servos[5] = new CustomServo(11, (SERVOMAX + SERVOMIN) / 2, -4);
    //
    servos[6] = new CustomServo(10, (SERVOMAX + SERVOMIN) / 2);
    servos[7] = new CustomServo(9, (SERVOMAX + SERVOMIN) / 2);
    servos[8] = new CustomServo(8, (SERVOMAX + SERVOMIN) / 2);
    //
    servos[9] = new CustomServo(0, (SERVOMAX + SERVOMIN) / 2, -10, reverse);
    servos[10] = new CustomServo(1, (SERVOMAX + SERVOMIN) / 2, -5, reverse);
    servos[11] = new CustomServo(2, (SERVOMAX + SERVOMIN) / 2, reverse);
    servos[12] = new CustomServo(3, (SERVOMAX + SERVOMIN) / 2, -7, reverse);
    servos[13] = new CustomServo(4, (SERVOMAX + SERVOMIN) / 2, -5, reverse);

    //
    servos[14] = new CustomServo(5, (SERVOMAX + SERVOMIN) / 2, reverse);
    servos[15] = new CustomServo(6, (SERVOMAX + SERVOMIN) / 2, reverse);
    servos[16] = new CustomServo(7, (SERVOMAX + SERVOMIN) / 2, reverse);
    //
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
      CustomServo* servo = servos[i];
      if (servo->reverse) {
        positions[i] = (1000 - ((int) ((positions[i] + servo->offset) * servo->multiplier)));
      }
      int angle = map(((int) positions[i] * servo->multiplier), 0, 1000, 150, 500);
      Serial.print("Num servos: ");
      Serial.println(NUM_SERVOS);
      Serial.println(angle);
      Serial.println(i);
      Serial.println("Before:");
      Serial.println(servo->curPos);
      servo->MoveServo(angle - servo->curPos, moveTime / NUM_SERVOS);
      Serial.println("After:");
      Serial.println(servo->curPos);
    }
  }
  Serial.println("################");
  Serial.println("################");
}

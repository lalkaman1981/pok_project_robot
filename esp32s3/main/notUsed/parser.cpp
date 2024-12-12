#include "parser.hpp"

unsigned long moveTime = 0;
unsigned long delayTime = 0;
int activeServos = 0;

Servo* servos[NUM_SERVOS];
int positions[NUM_SERVOS];
int motorQueue[NUM_SERVOS];

PCA9685 *pca9685 = new PCA9685();

void process_line(const std::string& line) {
	if (servos[1] == nullptr) {
		setServos();
	}

	if (line == "def") {
		defPosition();
	} else {
		std::cout << line << ": sigma"<< std::endl;
		parse(line);
		moveServos();	
	}
}

void parse(const std::string& line) {
	moveTime = 0;
	delayTime = 0;

	for (int i = 0; i < NUM_SERVOS; i++) {
		positions[i] = -1;
		motorQueue[i] = -1;
    }

    int len = line.length();

    int tIndex = line.find('T');
    if (tIndex != std::string::npos) {
        int tmp = 0;
        for (int i = tIndex; i <= len && line[i] != 'D'; ++i){
            tmp = i;
        }
    	moveTime = std::stoi(line.substr(tIndex + 1, tmp)); 
    }

    int dIndex = line.find('D');
    if (tIndex != std::string::npos) {
		delayTime = std::stoi(line.substr(dIndex + 1));
    }

    std::vector<std::string> vec;
    std::string temp = "";

    int tempInd = 0;

    if (dIndex == tIndex) {
        tempInd = len;
    } else if (dIndex < tIndex){
        tempInd = (dIndex != -1) ? dIndex : tIndex;
    } else {
        tempInd = (tIndex != -1) ? tIndex : dIndex;
    }

    for (int j = 0; j < tempInd; ++j) {
        if (line[j] == '#') {
            if (temp != "") {
                vec.push_back(temp);
                temp = "";
            }
        } else {
            temp += line[j];
        }
    }

	int counter = 1;

    for (std::string commandPart : vec) {
        short motor = 0;
        temp = "";

        for (char ch : commandPart) {
            if (ch == 'P') {
                motor = std::stoi(temp);
                temp = "";
            } else {
                temp += ch;
            }
        }
        positions[motor] = std::stoi(temp);
        motorQueue[counter++] = motor;
    }
}

void moveServos() {
	
	double time = (double) moveTime / NUM_SERVOS;
	
  	for (int i = 1; i < NUM_SERVOS; ++i) {

		int servoNum = motorQueue[i];

		if (servoNum == -1) {
			break;	
		}

		Servo* servo = servos[servoNum];

		if (servo->reverse) {
	    	positions[servoNum] = (1000 - ((int) ((positions[i] + servo->offset) * servo->multiplier)));
	  	}

		int pos = 150 + (positions[servoNum]/1000) * (350);

		servo->setAngle(pos - servo->curPos, time, pca9685);

	}
}

void defPosition() {
	for (int i = 1; i < NUM_SERVOS; i++) {
		servos[i]->setAngle( servos[i]->defaultPosition - servos[i]->curPos + servos[i]->offset, 0,pca9685);
	}
}

void setServos() {
	bool reverse = false;
	int startPosition = 325;

    servos[1] = new Servo(15, startPosition, -5);
    servos[2] = new Servo(14, startPosition, -5, true);
    servos[3] = new Servo(13, startPosition, 0, true);
    servos[4] = new Servo(12, startPosition, -5, true);
    servos[5] = new Servo(11, startPosition, -4);
    //
    servos[6] = new Servo(10, startPosition);
    servos[7] = new Servo(9, startPosition);
    servos[8] = new Servo(8, startPosition);
    //
    servos[9] = new Servo(0, startPosition, -10, reverse);
    servos[10] = new Servo(1, startPosition, -5, reverse);
    servos[11] = new Servo(2, startPosition, 0, reverse);
    servos[12] = new Servo(3, startPosition, -7, reverse);
    servos[13] = new Servo(4, startPosition, -5, reverse);
    //
    servos[14] = new Servo(5, startPosition, 0, reverse);
    servos[15] = new Servo(6, startPosition, 0, reverse);
    servos[16] = new Servo(7, startPosition, 0, reverse);
}
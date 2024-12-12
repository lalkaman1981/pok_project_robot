#ifndef SERVO_HPP
#define SERVO_HPP

#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pca_connect.hpp"

class PCA9685; 

class Servo {
public:
    int pinNum;
    int defaultPosition;
    int curPos;
    bool reverse;
    double multiplier;
    int offset;

    Servo(int pinNum, int defaultPosition, int offset = 0, bool reverse = false, double multiplier = 1.0);

    void setAngle(int shift, int time, PCA9685* pca9685);
};

#endif

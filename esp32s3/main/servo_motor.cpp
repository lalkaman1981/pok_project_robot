#include "servo_motor.hpp"


Servo::Servo(int pinNum, int defaultPosition, int offset, bool reverse, double multiplier) 
    : pinNum(pinNum), defaultPosition(defaultPosition), curPos(defaultPosition),
      reverse(reverse), multiplier(multiplier), offset(offset) {}


void Servo::setAngle(int shift, int time, PCA9685 *pca9685) {
    if (shift == 0)
    	shift = 1;

    int maxPos = shift + this->curPos;
    int delayNew = (time != 0) ? ((double)time / abs(shift)) : 0;

    if (shift >= 0) {
        for (int i = this->curPos; i <= maxPos; i++) {
            pca9685->setServoAngle(this->pinNum, i);
            vTaskDelay(pdMS_TO_TICKS(delayNew));
        }
    } else {
        for (int i = this->curPos; i >= maxPos; i--) {
            pca9685->setServoAngle(this->pinNum, i);
            vTaskDelay(pdMS_TO_TICKS(delayNew));
        }
    }

    this->curPos += shift;
}

#include "JoyStick.h"

JoyStick::JoyStick() {
    UINT joyNums;
    joyNums = joyGetNumDevs();
    if (joyNums >= 1) {
        joyGetPosEx(JOYSTICKID1, &this->joyinfoex);
    }
    this->joyinfoex.dwSize = sizeof(JOYINFOEX);
    this->joyinfoex.dwFlags = JOY_RETURNALL;
}

JOYINFOEX JoyStick::getJoyInfoEx() {
    return this->joyinfoex;
}

void JoyStick::updateJoyInfoEx() {
    joyGetPosEx(JOYSTICKID1, &this->joyinfoex);
}

int JoyStick::getX() {
    updateJoyInfoEx();
    return this->joyinfoex.dwXpos;
}

int JoyStick::getY() {
    updateJoyInfoEx();
    return this->joyinfoex.dwYpos;
}

int JoyStick::getR() {
    updateJoyInfoEx();
    return this->joyinfoex.dwRpos;
}

int JoyStick::getU() {
    updateJoyInfoEx();
    return this->joyinfoex.dwUpos;
}

int JoyStick::getZ() {
    updateJoyInfoEx();
    return this->joyinfoex.dwZpos;
}



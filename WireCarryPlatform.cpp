#include "WireCarryPlatform.h"
#include <qdebug.h>

WireCarryPlatform::WireCarryPlatform() {
}

WireCarryPlatform::WireCarryPlatform(Axis shift, Axis left, Axis right){
	this->shift = shift;
	this->leftHand = left;
	this->rightHand = right;

	this->pos = 0.0;

	this->is_leftHandClamped = true;
	this->is_rightHandClamped = true;
	this->shiftDir = true;
	this->wireDir = true;

	this->leftHand.setPPU(1); this->leftHand.setVelParams(1000, 1000, 1000, 1000);
	this->rightHand.setPPU(1); this->rightHand.setVelParams(1000, 1000, 1000, 1000);
}

void WireCarryPlatform::leftHandClamp(bool is_clamp) {
	if (is_clamp && this->is_leftHandClamped == false) {
		this->is_leftHandClamped = true;
		this->leftHand.returnCmdZero();
	}
	else if (!is_clamp && this->is_leftHandClamped == true) {
		this->is_leftHandClamped = false;
		this->leftHand.relMove(100);  // adjustable
	}
}

void WireCarryPlatform::rightHandClamp(bool is_clamp) {
	if (is_clamp && this->is_rightHandClamped == false) {
		this->is_rightHandClamped = true;
		this->rightHand.returnCmdZero();
	}
	else if (!is_clamp && this->is_rightHandClamped == true) {
		this->is_rightHandClamped = false;
		this->rightHand.relMove(100);  // adjustable
	}
}

void WireCarryPlatform::shiftRunRel(double dis) {
	this->shift.relMove(dis);
}

void WireCarryPlatform::shiftRunConti(USHORT dir) {
	this->shift.contiMove(dir);
}

void WireCarryPlatform::shiftStop() {
	this->shift.stop();
}

Axis WireCarryPlatform::getShift() {
	return this->shift;
}

bool WireCarryPlatform::getShiftDir() {
	return this->shiftDir;
}

void WireCarryPlatform::setShiftDir(bool dir) {
	this->shiftDir = dir;
}

bool WireCarryPlatform::getWireDir() {
	return this->wireDir;
}

void WireCarryPlatform::setWireDir(bool dir) {
	this->wireDir = dir;
}

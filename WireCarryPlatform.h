#pragma once

#include "Axis.h"

class WireCarryPlatform {
private:
	Axis shift;
	Axis leftHand;
	Axis rightHand;

	double pos;

	bool is_leftHandClamped;
	bool is_rightHandClamped;

	bool shiftDir;
	bool wireDir;

public:
	WireCarryPlatform();
	WireCarryPlatform(Axis shift, Axis left, Axis right);

	void leftHandClamp(bool);
	void rightHandClamp(bool);

	void shiftRunRel(double);
	void shiftRunConti(USHORT);
	void shiftStop();

	Axis getShift();

	bool getShiftDir();
	void setShiftDir(bool);

	bool getWireDir();
	void setWireDir(bool);
};


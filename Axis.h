#pragma once

#include <Windows.h>
#include "AdvMotApi.h"
#include "AdvMotDrv.h"
#include "AdvMotPropID.h"

class Axis {
public:
	Axis(HAND devHand, USHORT num);
	Axis();
	~Axis();

	void relMove(DOUBLE distance);
	void contiMove(USHORT dir);
	void returnCmdZero();
	void stop();

	void setVelParams(DOUBLE i_initVel, DOUBLE i_runningVel, DOUBLE i_acc, DOUBLE i_dec, DOUBLE mode = 0.0);
	void setPPU(ULONG i_PPU, ULONG denominator);
	void setPPU(ULONG i_PPU);
	void setCmdPos(DOUBLE);

	void changeVelByRate(DOUBLE);
	void changeVel(DOUBLE);

	DOUBLE getPPU();
	DOUBLE getCmdPos();
	U16 getAxisStatus();
	U32 getMotionStatus();
	HAND getHand();

public:
	HAND hand = 0;
	USHORT axisID = 0;
	ULONG PPU = 10, PPUDenominator = 1;
	DOUBLE initVel = 10, runningVel = 10, acc = 10, dec = 10, velMode = 0;
	DOUBLE cmdPos = 0.0;
};


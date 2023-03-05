#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>
#include<MMSystem.h>

#pragma comment(lib, "Winmm.lib")

class JoyStick {
public:
	JoyStick();

private:
	JOYINFOEX joyinfoex;

public:
	JOYINFOEX getJoyInfoEx();
	void updateJoyInfoEx();

	int getX();
	int getY();
	int getR();
	int getU();
	int getZ();
	
	int getPOV();
	int getButton();
};


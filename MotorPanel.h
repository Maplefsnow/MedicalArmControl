#pragma once

#include <Windows.h>
#include <qwidget.h>
#include <qthread.h>
#include <functional>
#include "Axis.h"
#include "JoyStick.h"
#include "WireCarryPlatform.h"
#include "AdvMotApi.h"
#include "AdvMotDrv.h"
#include "RunAxisByJoyStickThread.h"
#include "WakeRunAxisByJoyStickThread.h"
#include "UpdateMotorPanel.h"

class MotorPanel : public QWidget {
	Q_OBJECT

public:
	MotorPanel(QWidget* parent);
	~MotorPanel();

	DWORD deviceNum = 0;
	HAND deviceHand = 0;

	short axisID = -1;
	Axis axis[8];

	JoyStick joyStick;
	WireCarryPlatform wireCarryPlatform;

	UpdateMotorPanel* updateUIThread;
	RunAxisByJoyStickThread* runAxisThread;
	WakeRunAxisByJoyStickThread* wakeRunAxisThread;

public:
	Q_INVOKABLE void updateAxisStatus();
	Q_INVOKABLE void runAxisByJoyStick();
	Q_INVOKABLE void wakeRunAxisByJoyStick();

	Q_INVOKABLE void platformDirMonitor();

	void initAllAxis();
	Axis getAxis(int);

public slots:
	void checkCard();

	void setAxisParams();
	void setAxisZero();

	void axisRunDis();
	void axisRun();
	void axisRunRev();
	void axisReturnCmdZero();
	void axisStop();

	void panelParamsUpdate(int);

	void platformShiftRun();
};


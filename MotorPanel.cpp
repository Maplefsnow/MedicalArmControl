#include "MotorPanel.h"
#include "MainWindow.h"

MotorPanel::MotorPanel(QWidget* parent) : QWidget(parent) {
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	this->updateUIThread = new UpdateMotorPanel(this);
	this->runAxisThread = new RunAxisByJoyStickThread(this);
	this->wakeRunAxisThread = new WakeRunAxisByJoyStickThread(this);

	this->updateUIThread->start();
	this->runAxisThread->start();
	this->wakeRunAxisThread->start();

	checkCard();
	initAllAxis();
}

MotorPanel::~MotorPanel() {
	this->updateUIThread->stop();
	this->updateUIThread->wait();

	this->runAxisThread->exit();

	this->wakeRunAxisThread->stop();
	this->wakeRunAxisThread->wait();
}

Q_INVOKABLE void MotorPanel::updateAxisStatus() {
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	if (mainWindow == nullptr) return;
	if (this->axisID == -1) return;

	DOUBLE pos = 0.0; U16 status = 0;
	
	Axis axis = this->axis[this->axisID];
	pos = axis.getCmdPos();
	status = axis.getAxisStatus();

	QString statusStr = "";

	switch (status) {
		case 0: statusStr = "Disabled"; break;
		case 1: statusStr = "Ready"; break;
		case 2: statusStr = "Stopping..."; break;
		case 3: statusStr = "ERROR! Stopping..."; break;
		case 4: case 5: case 6: case 7: statusStr = "Running..."; break;
		default: break;
	}

	mainWindow->ui.label_status->setText(statusStr);
	mainWindow->ui.label_cmdPos->setText(QString::number(pos, 'f', 4));
}

Q_INVOKABLE void MotorPanel::runAxisByJoyStick() {
	int x1 = this->joyStick.getX(), x2 = this->joyStick.getU();
	int y1 = this->joyStick.getY(), y2 = this->joyStick.getR();
	int z  = this->joyStick.getZ(), POV = this->joyStick.getPOV();

	if (this->axis[2].getHand() == 0) return;

	bool is_control = false;

	try {
		if (y2 <= 37000 && y2 >= 28000) {
			this->axis[2].stop();
		}
		else {
			is_control = true;
			if (y2 > 32767) {
				if (this->axis[2].getMotionStatus() == 1) {
					this->axis[2].contiMove(0);
				}
				// this->axis[2].changeVel((y2 - 32767) / 1000);
			}
			else {
				if (this->axis[2].getMotionStatus() == 1) {
					this->axis[2].contiMove(1);
				}
				// this->axis[2].changeVel((32767 - y2) / 1000);
			}
		}
	}
	catch (const char* msg) {
		qDebug() << msg;
	}

	try {
		if (z <= 37000 && z >= 28000) {
			this->axis[3].stop();
		}
		else {
			is_control = true;
			if (z > 32767) {
				if (this->axis[3].getMotionStatus() == 1) this->axis[3].contiMove(0);
				//this->axis[2].changeVelByRate((z - 32767) / 100);
			}
			else {
				if (this->axis[3].getMotionStatus() == 1) this->axis[3].contiMove(1);
				//this->axis[2].changeVelByRate((32767 - z) / 100);
			}
		}
	} catch (const char* msg) {
		qDebug() << msg;
	}

	try {
		if (y1 <= 37000 && y1 >= 28000) {
			this->axis[0].stop();
		}
		else {
			is_control = true;
			if (y1 > 32767) {
				if (this->axis[0].getMotionStatus() == 1) this->axis[0].contiMove(0);
				//this->axis[0].changeVelByRate((y1 - 32767) / 100);
			}
			else {
				if (this->axis[0].getMotionStatus() == 1) this->axis[0].contiMove(1);
				//this->axis[0].changeVelByRate((32767 - y1) / 100);
			}
		}
	}
	catch (const char* msg) {
		qDebug() << msg;
	}

	if (is_control == false) {
		this->runAxisThread->stop();
	}
}

Q_INVOKABLE void MotorPanel::wakeRunAxisByJoyStick() {
	int x1 = this->joyStick.getX(), x2 = this->joyStick.getU();
	int y1 = this->joyStick.getY(), y2 = this->joyStick.getR();
	int z  = this->joyStick.getZ();

	if (this->axis[2].getHand() == 0) return;

	if (y1 >= 37000 || y1 <= 28000 || 
		y2 >= 37000 || y2 <= 28000 ||
		x2 >= 37000 || x2 <= 28000 ||
		z >= 37000 || z <= 28000) {
		if(this->runAxisThread->getRunnable() == false) {
			this->runAxisThread->setRunnable(true);
		}
	}
}

void MotorPanel::initAllAxis() {
	try {
		for (int i = 0; i < 8; i++) {
			this->axis[i] = Axis::Axis(this->deviceHand, i);
		}
	}
	catch (const char* errMsg) {
		qDebug() << errMsg;
	}
}

Axis MotorPanel::getAxis(int axisID) {
	return this->axis[axisID];
}

void MotorPanel::checkCard() {
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	DEVLIST devList[5]; ULONG devNum;
	Acm_GetAvailableDevs(devList, 2, &devNum);
	DWORD deviceNum = devList[0].dwDeviceNum;
	this->deviceNum = deviceNum;

	Acm_DevOpen(this->deviceNum, &this->deviceHand);

	char deviceNumStr[10];
	sprintf(deviceNumStr, "0x%x", deviceNum);

	qDebug() << deviceNumStr;

	// mainWindow->ui.label_cardID->setText(QString::fromStdString(deviceNumStr));
}

//void MotorPanel::initAxis() {
//	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());
//
//	if (this->deviceNum == 0) {
//		mainWindow->ui.statusBar->showMessage("No device card detected, please retry!", 2000);
//		return;
//	}
//
//	USHORT axisID = mainWindow->ui.comboBox_axisID->currentIndex();
//	try {
//		this->axis[axisID] = Axis::Axis(this->deviceHand, axisID);
//		this->axisID = axisID;
//	}
//	catch (const char* msg) {
//		mainWindow->ui.statusBar->showMessage(msg, 2000);
//		return;
//	}
//
//	char successMsg[100];
//	sprintf(successMsg, "Axis %d init success!", axisID);
//	mainWindow->ui.statusBar->showMessage(successMsg, 2000);
//}

void MotorPanel::setAxisParams() {
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	if (this->deviceHand == 0) {
		mainWindow->ui.statusBar->showMessage("No device card detected, please retry!", 2000);
		return;
	}
	if (this->axis[this->axisID].hand == 0 || this->axisID == -1) {
		mainWindow->ui.statusBar->showMessage("No axis selected, please select an axis!", 2000);
		return;
	}

	try {
		this->axis[this->axisID].setPPU(mainWindow->ui.spinBox_PPU->value(), mainWindow->ui.spinBox_PPUDenominator->value());

		this->axis[this->axisID].setVelParams(mainWindow->ui.spinBox_initVel->value(),
			mainWindow->ui.spinBox_runningVel->value(),
			mainWindow->ui.spinBox_acc->value(),
			mainWindow->ui.spinBox_dec->value());
	}
	catch (const char* errMsg) {
		mainWindow->ui.statusBar->showMessage(errMsg, 2000);
		return;
	}

	mainWindow->ui.statusBar->showMessage("Params set success!", 2000);
}

void MotorPanel::setAxisZero() {
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	if (this->deviceHand == 0) {
		mainWindow->ui.statusBar->showMessage("No device card detected, please retry!", 2000);
		return;
	}
	if (this->axis[this->axisID].hand == 0) {
		mainWindow->ui.statusBar->showMessage("No axis selected, please select an axis!", 2000);
		return;
	}

	try {
		this->axis[this->axisID].setCmdPos(0.0);
	}
	catch (const char* errMsg) {
		mainWindow->ui.statusBar->showMessage(errMsg, 2000);
	}
}

void MotorPanel::axisRunDis() {
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	if (this->deviceHand == 0) {
		mainWindow->ui.statusBar->showMessage("No device card detected, please retry!", 2000);
		return;
	}
	if (this->axis[this->axisID].hand == 0) {
		mainWindow->ui.statusBar->showMessage("No axis selected, please select an axis!", 2000);
		return;
	}

	try {
		this->axis[this->axisID].relMove(mainWindow->ui.doubleSpinBox_runDis->value());
	}
	catch (const char* errMsg) {
		mainWindow->ui.statusBar->showMessage(errMsg, 2000);
	}
}

void MotorPanel::axisRun() {
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	if (this->deviceHand == 0) {
		mainWindow->ui.statusBar->showMessage("No device card detected, please retry!", 2000);
		return;
	}
	if (this->axis[this->axisID].hand == 0) {
		mainWindow->ui.statusBar->showMessage("No axis selected, please select an axis!", 2000);
		return;
	}

	try {
		this->axis[this->axisID].contiMove(0);
	}
	catch (const char* errMsg) {
		mainWindow->ui.statusBar->showMessage(errMsg, 2000);
	}
}

void MotorPanel::axisRunRev() {
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	if (this->deviceHand == 0) {
		mainWindow->ui.statusBar->showMessage("No device card detected, please retry!", 2000);
		return;
	}
	if (this->axis[this->axisID].hand == 0) {
		mainWindow->ui.statusBar->showMessage("No axis selected, please select an axis!", 2000);
		return;
	}

	try {
		this->axis[this->axisID].contiMove(1);
	}
	catch (const char* errMsg) {
		mainWindow->ui.statusBar->showMessage(errMsg, 2000);
	}
}

void MotorPanel::axisReturnCmdZero() {
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	if (this->deviceHand == 0) {
		mainWindow->ui.statusBar->showMessage("No device card detected, please retry!", 2000);
		return;
	}
	if (this->axis[this->axisID].hand == 0) {
		mainWindow->ui.statusBar->showMessage("No axis selected, please select an axis!", 2000);
		return;
	}

	try {
		this->axis[this->axisID].returnCmdZero();
	}
	catch (const char* errMsg) {
		mainWindow->ui.statusBar->showMessage(errMsg, 2000);
	}
}

void MotorPanel::axisStop() {
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	if (this->deviceHand == 0) {
		mainWindow->ui.statusBar->showMessage("No device card detected, please retry!", 2000);
		return;
	}
	if (this->axis[this->axisID].hand == 0) {
		mainWindow->ui.statusBar->showMessage("No axis selected, please select an axis!", 2000);
		return;
	}

	try {
		this->axis[this->axisID].stop();
	}
	catch (const char* errMsg) {
		mainWindow->ui.statusBar->showMessage(errMsg, 2000);
	}
}

void MotorPanel::panelParamsUpdate(int axisID) {
	MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	this->axisID = axisID - 1;

	mainWindow->ui.spinBox_PPU->setValue(this->axis[axisID].PPU);
	mainWindow->ui.spinBox_PPUDenominator->setValue(this->axis[axisID].PPUDenominator);
	mainWindow->ui.spinBox_acc->setValue(this->axis[axisID].acc);
	mainWindow->ui.spinBox_dec->setValue(this->axis[axisID].dec);
	mainWindow->ui.spinBox_initVel->setValue(this->axis[axisID].initVel);
	mainWindow->ui.spinBox_runningVel->setValue(this->axis[axisID].runningVel);
}

#include "RunAxisByJoyStickThread.h"

RunAxisByJoyStickThread::RunAxisByJoyStickThread(QWidget *panel) {
	this->panel = panel;
}

void RunAxisByJoyStickThread::stop() {
	this->is_runnable = false;
}

void RunAxisByJoyStickThread::setRunnable(bool var) {
	this->is_runnable = var;
}

bool RunAxisByJoyStickThread::getRunnable() {
	return this->is_runnable;
}

void RunAxisByJoyStickThread::run() {
	while (true) {
		if (this->is_runnable) {
			QMetaObject::invokeMethod(this->panel, "runAxisByJoyStick", Qt::AutoConnection);
			QThread::msleep(10);
		}
	}
}

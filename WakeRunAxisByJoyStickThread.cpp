#include "WakeRunAxisByJoyStickThread.h"

WakeRunAxisByJoyStickThread::WakeRunAxisByJoyStickThread(QWidget *panel) {
	this->panel = panel;
}

void WakeRunAxisByJoyStickThread::stop() {
	this->is_runnable = false;
}

void WakeRunAxisByJoyStickThread::run() {
	while (this->is_runnable) {
		QMetaObject::invokeMethod(this->panel, "wakeRunAxisByJoyStick", Qt::AutoConnection);
		QThread::msleep(1);
	}
}

#include "UpdateMotorPanel.h"

UpdateMotorPanel::UpdateMotorPanel(QWidget* panel) {
	this->panel = panel;
}

void UpdateMotorPanel::run() {
	while (this->is_runnable) {
		QMetaObject::invokeMethod(this->panel, "updateAxisStatus", Qt::AutoConnection);
		QThread::msleep(10);
	}
}

void UpdateMotorPanel::stop() {
	this->is_runnable = false;
}
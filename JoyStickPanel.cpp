#include "JoyStickPanel.h"
#include "MainWindow.h"
#include "MotorPanel.h"
#include <qdebug.h>

JoyStickPanel::JoyStickPanel(QWidget* parent) : QWidget(parent) {
    MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

	this->updateJoyStickInfoThread = new UpdateJoyStickPanel(this);

	this->updateJoyStickInfoThread->start();
}

JoyStickPanel::~JoyStickPanel() {
	this->updateJoyStickInfoThread->stop();
	this->updateJoyStickInfoThread->wait();
	this->updateJoyStickInfoThread = nullptr;
}

Q_INVOKABLE void JoyStickPanel::updateJoyStickUI() {
    MainWindow* mainWindow = dynamic_cast<MainWindow*>(this->topLevelWidget());

    int x1 = this->joyStick.getX(), x2 = this->joyStick.getU();
    int y1 = this->joyStick.getY(), y2 = this->joyStick.getR();

    mainWindow->ui.horizontalSlider_x1->setValue(x1 * 100 / 65535);
    mainWindow->ui.horizontalSlider_x2->setValue(x2 * 100 / 65535);
    mainWindow->ui.verticalSlider_y1->setValue(100 - y1 * 100 / 65535);
    mainWindow->ui.verticalSlider_y2->setValue(100 - y2 * 100 / 65535);
}



UpdateJoyStickPanel::UpdateJoyStickPanel(QWidget* panel) {
    this->panel = panel;
}

void UpdateJoyStickPanel::stop() {
    this->is_running = false;
}

void UpdateJoyStickPanel::run() {
    while (this->is_running) {
        QMetaObject::invokeMethod(this->panel, "updateJoyStickUI", Qt::AutoConnection);
        QThread::msleep(1);
    }
}
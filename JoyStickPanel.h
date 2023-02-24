#pragma once
#include <qwidget.h>
#include <qthread.h>
#include "JoyStick.h"
#include "Axis.h"

class UpdateJoyStickPanel : public QThread {
	Q_OBJECT

public:
	UpdateJoyStickPanel(QWidget*);
	QWidget* panel;
	void stop();

private:
	bool is_running = true;

protected:
	void run();
};

class JoyStickPanel : public QWidget {
	Q_OBJECT

public:
	UpdateJoyStickPanel* updateJoyStickInfoThread;

public:
	JoyStick joyStick;

public:
	JoyStickPanel(QWidget *parent);
	~JoyStickPanel();
	Q_INVOKABLE void updateJoyStickUI();
};


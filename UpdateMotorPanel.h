#pragma once
#include <qthread.h>
#include <qwidget.h>

class UpdateMotorPanel : public QThread {
	Q_OBJECT

public:
	UpdateMotorPanel(QWidget*);
	QWidget* panel;
	void stop();

private:
	bool is_runnable = true;

protected:
	void run();
};


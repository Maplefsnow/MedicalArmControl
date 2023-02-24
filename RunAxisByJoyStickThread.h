#pragma once
#include <qwidget.h>
#include <qthread.h>

class RunAxisByJoyStickThread  : public QThread {
	Q_OBJECT

public:
	RunAxisByJoyStickThread(QWidget *);
	QWidget* panel;

	void stop();
	void setRunnable(bool);
	bool getRunnable();

private:
	bool is_runnable = true;

protected:
	void run();
};

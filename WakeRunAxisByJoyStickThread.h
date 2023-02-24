#pragma once

#include <QThread>
#include <qwidget.h>

class WakeRunAxisByJoyStickThread  : public QThread {
	Q_OBJECT

public:
	WakeRunAxisByJoyStickThread(QWidget*);
	QWidget* panel;
	void stop();

private:
	bool is_runnable = true;

protected:
	void run();
};

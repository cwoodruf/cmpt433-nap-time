/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * Implementation of PeriodicThread class
 * reference:
 * http://thesmithfam.org/blog/2010/02/07/talking-to-qt-threads/
 */
#include "periodicthread.h"
#include <QProcess>
#include <QFileInfo>
#include <QDebug>
#include <QTimer>
#define PERIODICTHREADTOMAX 120

extern "C" {
	#include <stdio.h>
}

PeriodicThread::PeriodicThread()
{
}

int PeriodicThread::setAll(int to, QString t, QStringList args) 
{
	QFileInfo f;

	f = QFileInfo(t);
	error = QString();

	if (to <= 0) {
		error.append("towait should positive! ");
	}

	if (error.length()) {
		qDebug() << error;
	}

	taskname = t;
	taskargs = args;
	towait = to;
	stop = false;
	return 0;
}

/**
 * runs a task periodically sleeping towait seconds
 */
void PeriodicThread::run()
{
	if (towait < 0 || towait > PERIODICTHREADTOMAX) {
		qDebug() << "whacky timer value: " << towait;
		return;
	}
	if (stop) qDebug() << "stop is true??? will exit right away!";

	while (true) {
		if (stop) break;
		sleep(towait);
		QProcess::execute(taskname,taskargs);
	}
}


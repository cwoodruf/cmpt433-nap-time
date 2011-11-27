/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * Object that starts a task that runs periodically forever. 
 * Killing the thread should kill the task.
 * You can change the task and period dynamically
 */

#ifndef PERIODICTHREAD_H
#define PERIODICTHREAD_H

#include <QThread>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QTimer>

class PeriodicThread : public QThread
{
	Q_OBJECT

public:
	PeriodicThread();
	void run();
	int setAll(int to, QString t, QStringList args);
	void stopRun() { stop = true; };

private:
	bool stop;
	int towait;
	QString taskname;
	QStringList taskargs;
	QString error;

};
	
#endif /* PERIODICTHREAD_H */

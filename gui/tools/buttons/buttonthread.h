/**
 * cmpt433 assignment 4 Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * basically identical to Brian Fraser's example
 * found here:
 * http://www2.cs.sfu.ca/CourseCentral/433/bfraser/solutions/qtthread/buttonthread.zip
 */

#ifndef BUTTONTHREAD_H
#define BUTTONTHREAD_H

#include <QThread>

class ButtonThread : public QThread
{
	Q_OBJECT

public:
	ButtonThread();
	void run();

signals:
	void buttonPressed(int mask);

};
	
#endif /* BUTTONTHREAD_H */

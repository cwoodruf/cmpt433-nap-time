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
    void buttonsChanged(int btnMask);
};

#endif // BUTTONTHREAD_H

#include "buttonthread.h"
#include <QDebug>

extern "C" {
#include "buttondrv.h"
}

ButtonThread::ButtonThread()
{
}


void ButtonThread::run()
{
    if (ButtonDrv_init() != 0) {
        emit buttonsChanged(-1);
        return;
    }

    int i = 0;
    //while (!stopThreads) {
    while (true) {
        int btnMask;
        int ret = ButtonDrv_read(&btnMask);
        if (ret == 0) {
            emit buttonsChanged(btnMask);
        }
    }
    qDebug()<<"Thread ending.";

    ButtonDrv_cleanup();
}

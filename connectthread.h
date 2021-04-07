#ifndef CONNECTTHREAD_H
#define CONNECTTHREAD_H

#include <QThread>
#include <QObject>
#include "mainwindow.h"

class ConnectThread : public QThread
{
    Q_OBJECT

private:
    QString ipIEC104;
    uint16_t portIEC104;
    CS104_Connection con;   //соединение IEC104Master

public:
    ConnectThread(QString ip, uint16_t port, CS104_Connection con);

signals:
    void setTextStatus(QString);    //сигнал для записи статуса соединения

protected:
    void run() override;
};

#endif // CONNECTTHREAD_H

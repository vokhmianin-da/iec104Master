#ifndef CONNECTTHREAD_H
#define CONNECTTHREAD_H

#include <QThread>
#include <QObject>
#include <QVariant>
#include "mainwindow.h"

class MainWindow;


class ConnectThread : public QThread
{
    Q_OBJECT

private:
    QString ipIEC104;
    uint16_t portIEC104;
    CS104_Connection con;   //соединение IEC104Master
    MainWindow *ptrWindow;

public:
    ConnectThread(QString ip, uint16_t port, MainWindow *ptr);
    friend void connectionHandler (void* parameter, CS104_Connection connection, CS104_ConnectionEvent event); /* Connection event handler */
    friend bool asduReceivedHandler (void* parameter, int address, CS101_ASDU asdu);   // CS101_ASDUReceivedHandler implementation (For CS104 the address parameter has to be ignored)

signals:
    void setTextStatus(QString);    //сигнал для записи статуса соединения

private slots:
    void sendCommand(int row, int column, QVariant val);

protected:
    void run() override;
};

#endif // CONNECTTHREAD_H

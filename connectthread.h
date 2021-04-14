#ifndef CONNECTTHREAD_H
#define CONNECTTHREAD_H

#include <QThread>
#include <QObject>
#include <QVariant>
#include <QQueue>

#include "iec60870/cs104_connection.h"
#include "iec60870/hal_time.h"
#include "iec60870/hal_thread.h"


class ConnectThread : public QThread
{
    Q_OBJECT

private:
    QString ipIEC104;
    uint16_t portIEC104;
    CS104_Connection con;   //соединение IEC104Master
    QQueue <InformationObject> commandQueue;    //очередь команд для отправки по IEC104
    bool isRun = false; //флаг наличия соединения

public:
    ConnectThread(QString ip, uint16_t port);
    bool isConnect();
    static void connectionHandler (void* parameter, CS104_Connection connection, CS104_ConnectionEvent event); /* Connection event handler */
    static bool asduReceivedHandler (void* parameter, int address, CS101_ASDU asdu);   // CS101_ASDUReceivedHandler implementation (For CS104 the address parameter has to be ignored)

signals:
    void setTextStatus(QString);    //сигнал для записи статуса соединения
    void getIEC104Info(int addr, int value);    //сигнал о получении данных
    void closeConnection(); //сигнал о закрытии текущего соединения

private slots:
    void commandIOformation(int addr, QVariant val, IEC60870_5_TypeID commandType);
    void disconnect();

protected:
    void run() override;
    friend class MainWindow;
};

#endif // CONNECTTHREAD_H

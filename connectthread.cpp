#include "connectthread.h"

ConnectThread::ConnectThread(QString ip, uint16_t port, CS104_Connection connection): ipIEC104(ip), portIEC104(port), con(connection)
{

}

void ConnectThread::run()
{
    QString temp;
    temp = "Connecting to: %1:%2";
    temp = temp.arg(ipIEC104).arg(portIEC104);
    emit setTextStatus(temp);

//    con = CS104_Connection_create(ipIEC104.toStdString().c_str(), portIEC104);

//    CS104_Connection_setConnectionHandler(con, connectionHandler, NULL);
//    CS104_Connection_setASDUReceivedHandler(con, asduReceivedHandler, NULL);
    while (1)
    {
//        /*Управление кнопками*/
//        ui->pbConnect->setEnabled(false);
//        ui->pbDisconnect->setEnabled(true);

        if (CS104_Connection_connect(con))  //здесь посмотреть возможность повторного включения
        {
            emit setTextStatus("Connected!");
            CS104_Connection_sendStartDT(con);
            CS104_Connection_sendInterrogationCommand(con, CS101_COT_ACTIVATION, 1, IEC60870_QOI_STATION);  //общий опрос

            struct sCP56Time2a testTimestamp;
            CP56Time2a_createFromMsTimestamp(&testTimestamp, Hal_getTimeInMs());
            CS104_Connection_sendTestCommandWithTimestamp(con, 1, 0x4938, &testTimestamp);
            emit setTextStatus("Wait ...");
//            disconnect(this, SIGNAL(setTextStatus(QString)), sender(), SLOT(on_setTextStatus(QString)));
//            this->exit();
//            break;
        }
        else
        {
            emit setTextStatus("Connect failed!");
        }
    }
}

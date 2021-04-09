#include "connectthread.h"

ConnectThread::ConnectThread(QString ip, uint16_t port, MainWindow *ptr): ipIEC104(ip), portIEC104(port), ptrWindow(ptr)
{

}

void ConnectThread::sendCommand(int row, int column, QVariant val)
{
    if(column == 1) //если изменилось поле value
    {
        if(row == 0)    //Для BitString
        {
            InformationObject sc = (InformationObject)  Bitstring32Command_create(NULL, 1, val.toUInt());
            CS104_Connection_sendProcessCommandEx(con, CS101_COT_ACTIVATION, 1, sc);
            InformationObject_destroy(sc);
        }
        if(row == 1)    //Для Word
        {
            InformationObject sc = (InformationObject)  SetpointCommandScaled_create(NULL, 3, val.toInt(), false, 0);
            CS104_Connection_sendProcessCommandEx(con, CS101_COT_ACTIVATION, 1, sc);
            InformationObject_destroy(sc);
        }
    }
}

void ConnectThread::run()
{
    QString temp;
    temp = "Connecting to: %1:%2";
    temp = temp.arg(ipIEC104).arg(portIEC104);
    emit setTextStatus(temp);

    con = CS104_Connection_create(ipIEC104.toStdString().c_str(), portIEC104);

    CS104_Connection_setConnectionHandler(con, connectionHandler, ptrWindow);
    CS104_Connection_setASDUReceivedHandler(con, asduReceivedHandler, ptrWindow);
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

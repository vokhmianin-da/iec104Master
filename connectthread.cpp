#include "connectthread.h"

ConnectThread::ConnectThread(QString ip, uint16_t port): ipIEC104(ip), portIEC104(port)
{

}

void ConnectThread::sendCommand(int addr, QVariant val, IEC60870_5_TypeID commandType)
{
    InformationObject sc;
    switch(commandType)
    {
    case C_SC_NA_1: sc = (InformationObject)SingleCommand_create(NULL, addr, val.toBool(), false, 0);
        break;
    case C_DC_NA_1: sc = (InformationObject)DoubleCommand_create(NULL, addr, val.toInt(), false, 0);
        break;
    case C_SE_NB_1: sc = (InformationObject)SetpointCommandScaled_create(NULL, addr, val.toInt(), false, 0);
        break;
    case C_SE_NC_1: sc = (InformationObject)SetpointCommandShort_create(NULL, addr, val.toFloat(), false, 0);
        break;
    case C_BO_NA_1: sc = (InformationObject)Bitstring32Command_create(NULL, addr, val.toUInt());
        break;
    default: return;
    }

    CS104_Connection_sendProcessCommandEx(con, CS101_COT_ACTIVATION, 1, sc);
    InformationObject_destroy(sc);

}

void ConnectThread::disconnect()
{
//        CS104_Connection_destroy(con);
        emit closeConnection();
//        this->disconnect();
        this->exit();
//        this->deleteLater();
}

void ConnectThread::run()
{
    QString temp;
    temp = "Connecting to: %1:%2";
    temp = temp.arg(ipIEC104).arg(portIEC104);
    emit setTextStatus(temp);

    con = CS104_Connection_create(ipIEC104.toStdString().c_str(), portIEC104);

    CS104_Connection_setConnectionHandler(con, connectionHandler, this);
    CS104_Connection_setASDUReceivedHandler(con, asduReceivedHandler, this);
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

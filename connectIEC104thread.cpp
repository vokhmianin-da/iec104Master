#include "connectIEC104thread.h"

ConnectIEC104Thread::ConnectIEC104Thread(QString ip, uint16_t port): ipIEC104(ip), portIEC104(port)
{

}

bool ConnectIEC104Thread::isConnect()
{
    if(con)
    {
        if(isRun)
        {
            return true;
        }
    }
    else
        return false;
}

void ConnectIEC104Thread::commandIOformation(int addr, QVariant val, IEC60870_5_TypeID commandType)
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
    default: sc = nullptr;
    }

    if(sc != nullptr)
    {
        commandQueue.enqueue(sc);
    }

}

void ConnectIEC104Thread::disconnect()
{
        CS104_Connection_destroy(con);
        if (con) emit closeConnection();
//        this->disconnect();
        this->terminate();
        this->deleteLater();
}

void ConnectIEC104Thread::run()
{
    QString temp;
    temp = "Connecting to: %1:%2";
    temp = temp.arg(ipIEC104).arg(portIEC104);
    emit setTextStatus(temp);

    if(!con) con = CS104_Connection_create(ipIEC104.toStdString().c_str(), portIEC104);

    CS104_Connection_setConnectionHandler(con, connectionHandler, this);
    CS104_Connection_setASDUReceivedHandler(con, asduReceivedHandler, this);
    while (1)
    {
        if(!isRun)
        {
            if (CS104_Connection_connect(con))  //здесь посмотреть возможность повторного включения
            {
                if (isConnect()) emit setTextStatus("Connected!");
                CS104_Connection_sendStartDT(con);
                CS104_Connection_sendInterrogationCommand(con, CS101_COT_ACTIVATION, 1, IEC60870_QOI_STATION);  //общий опрос

                struct sCP56Time2a testTimestamp;
                CP56Time2a_createFromMsTimestamp(&testTimestamp, Hal_getTimeInMs());
                CS104_Connection_sendTestCommandWithTimestamp(con, 1, 0x4938, &testTimestamp);
                if (isConnect()) emit setTextStatus("Wait ...");
            }
            else
            {
                emit setTextStatus("Connect failed!");
            }
        }
        else
        {
            if(!commandQueue.isEmpty())
            {
               InformationObject sc = commandQueue.dequeue();
               CS104_Connection_sendProcessCommandEx(con, CS101_COT_ACTIVATION, 1, sc);
               InformationObject_destroy(sc);
            }
        }
    }
}

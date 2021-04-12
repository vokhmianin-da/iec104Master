#include "mainwindow.h"
#include "ui_mainwindow.h"


void ConnectThread::connectionHandler(void *parameter, CS104_Connection connection, CS104_ConnectionEvent event)
{
    ConnectThread *ptrConnectThread = static_cast<ConnectThread *>(parameter); //берем указатель на объект соединения

    switch (event) {
        case CS104_CONNECTION_OPENED:
            printf("Connection established\n");
            break;
        case CS104_CONNECTION_CLOSED:
            //emit ptrMainWindow->ui->pbConnect->clicked();
            break;
        case CS104_CONNECTION_STARTDT_CON_RECEIVED:
            printf("Received STARTDT_CON\n");
            break;
        case CS104_CONNECTION_STOPDT_CON_RECEIVED:
            printf("Received STOPDT_CON\n");
            break;
    }
}

bool ConnectThread::asduReceivedHandler(void *parameter, int address, CS101_ASDU asdu) //прием данных
{
    ConnectThread *ptrConnectThread = static_cast<ConnectThread *>(parameter); //берем указатель на объект соединения

        if (CS101_ASDU_getTypeID(asdu) == M_ME_TE_1) {  //число тип 35

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                MeasuredValueScaledWithCP56Time2a io =
                        (MeasuredValueScaledWithCP56Time2a) CS101_ASDU_getElement(asdu, i);

                if(ptrConnectThread->isConnect()) emit ptrConnectThread->getIEC104Info(InformationObject_getObjectAddress((InformationObject) io), MeasuredValueScaled_getValue((MeasuredValueScaled) io));

                MeasuredValueScaledWithCP56Time2a_destroy(io);
            }
        }
        else if (CS101_ASDU_getTypeID(asdu) == M_SP_NA_1) { //дискретное значение тип 1

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                SinglePointInformation io =
                        (SinglePointInformation) CS101_ASDU_getElement(asdu, i);

                if(ptrConnectThread->isConnect()) emit ptrConnectThread->getIEC104Info( InformationObject_getObjectAddress((InformationObject) io), MeasuredValueScaled_getValue((MeasuredValueScaled) io));


                SinglePointInformation_destroy(io);
            }
        }
        else if (CS101_ASDU_getTypeID(asdu) == C_TS_TA_1) {

        }
        else if (CS101_ASDU_getTypeID(asdu) == M_ME_TF_1) { //число с плавающей точкой тип 36

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                MeasuredValueScaledWithCP56Time2a io =
                        (MeasuredValueScaledWithCP56Time2a) CS101_ASDU_getElement(asdu, i);

                if(ptrConnectThread->isConnect()) emit ptrConnectThread->getIEC104Info(InformationObject_getObjectAddress((InformationObject) io), MeasuredValueScaled_getValue((MeasuredValueScaled) io));


                MeasuredValueScaledWithCP56Time2a_destroy(io);
            }
        }
        else if (CS101_ASDU_getTypeID(asdu) == M_BO_TB_1) { //строка тип 33

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                MeasuredValueScaledWithCP56Time2a io =
                        (MeasuredValueScaledWithCP56Time2a) CS101_ASDU_getElement(asdu, i);

                if(ptrConnectThread->isConnect()) emit ptrConnectThread->getIEC104Info(InformationObject_getObjectAddress((InformationObject) io), MeasuredValueScaled_getValue((MeasuredValueScaled) io));


                MeasuredValueScaledWithCP56Time2a_destroy(io);
            }
        }

        return true;
}

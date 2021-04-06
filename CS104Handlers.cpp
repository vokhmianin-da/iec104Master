#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::connectionHandler(void *parameter, CS104_Connection connection, CS104_ConnectionEvent event)
{
    switch (event) {
        case CS104_CONNECTION_OPENED:
            printf("Connection established\n");
            break;
        case CS104_CONNECTION_CLOSED:
            printf("Connection closed\n");
            break;
        case CS104_CONNECTION_STARTDT_CON_RECEIVED:
            printf("Received STARTDT_CON\n");
            break;
        case CS104_CONNECTION_STOPDT_CON_RECEIVED:
            printf("Received STOPDT_CON\n");
            break;
    }
}

bool MainWindow::asduReceivedHandler(void *parameter, int address, CS101_ASDU asdu) //прием данных
{
    printf("RECVD ASDU type: %s(%i) elements: %i\n",
                TypeID_toString(CS101_ASDU_getTypeID(asdu)),
                CS101_ASDU_getTypeID(asdu),
                CS101_ASDU_getNumberOfElements(asdu));

        if (CS101_ASDU_getTypeID(asdu) == M_ME_TE_1) {

            printf("  measured scaled values with CP56Time2a timestamp:\n");

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                MeasuredValueScaledWithCP56Time2a io =
                        (MeasuredValueScaledWithCP56Time2a) CS101_ASDU_getElement(asdu, i);

                printf("    IOA: %i value: %i\n",
                        InformationObject_getObjectAddress((InformationObject) io),
                        MeasuredValueScaled_getValue((MeasuredValueScaled) io)
                );

                MeasuredValueScaledWithCP56Time2a_destroy(io);
            }
        }
        else if (CS101_ASDU_getTypeID(asdu) == M_SP_NA_1) {
            printf("  single point information:\n");

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                SinglePointInformation io =
                        (SinglePointInformation) CS101_ASDU_getElement(asdu, i);

                printf("    IOA: %i value: %i\n",
                        InformationObject_getObjectAddress((InformationObject) io),
                        SinglePointInformation_getValue((SinglePointInformation) io)
                );

                SinglePointInformation_destroy(io);
            }
        }
        else if (CS101_ASDU_getTypeID(asdu) == C_TS_TA_1) {
            printf("  test command with timestamp\n");
        }

        return true;
}

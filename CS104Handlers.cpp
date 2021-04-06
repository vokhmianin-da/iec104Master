#include "mainwindow.h"
#include "ui_mainwindow.h"

extern Ui::MainWindow *ptr;

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
        if (CS101_ASDU_getTypeID(asdu) == M_ME_TE_1) {

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                MeasuredValueScaledWithCP56Time2a io =
                        (MeasuredValueScaledWithCP56Time2a) CS101_ASDU_getElement(asdu, i);

                if( InformationObject_getObjectAddress((InformationObject) io) == ptr->tableWidget->item(0, 0)->text().toInt())
                {
                    QString temp;
                    temp = QString::number(MeasuredValueScaled_getValue((MeasuredValueScaled) io));
                    ptr->tableWidget->setItem(0, 1, new QTableWidgetItem(temp));
                }
                if( InformationObject_getObjectAddress((InformationObject) io) == ptr->tableWidget->item(1, 0)->text().toInt())
                {
                    QString temp;
                    temp = QString::number(MeasuredValueScaled_getValue((MeasuredValueScaled) io));
                    ptr->tableWidget->setItem(1, 1, new QTableWidgetItem(temp));
                }
                MeasuredValueScaledWithCP56Time2a_destroy(io);
            }
        }
        else if (CS101_ASDU_getTypeID(asdu) == M_SP_NA_1) {

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                SinglePointInformation io =
                        (SinglePointInformation) CS101_ASDU_getElement(asdu, i);

                if( InformationObject_getObjectAddress((InformationObject) io) == ptr->tableWidget->item(0, 0)->text().toInt())
                {
                    QString temp;
                    temp = QString::number(MeasuredValueScaled_getValue((MeasuredValueScaled) io));
                    ptr->tableWidget->setItem(0, 1, new QTableWidgetItem(temp));
                }
                if( InformationObject_getObjectAddress((InformationObject) io) == ptr->tableWidget->item(1, 0)->text().toInt())
                {
                    QString temp;
                    temp = QString::number(MeasuredValueScaled_getValue((MeasuredValueScaled) io));
                    ptr->tableWidget->setItem(1, 1, new QTableWidgetItem(temp));
                }

                SinglePointInformation_destroy(io);
            }
        }
        else if (CS101_ASDU_getTypeID(asdu) == C_TS_TA_1) {

        }

        return true;
}

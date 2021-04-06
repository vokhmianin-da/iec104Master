#include "mainwindow.h"
#include "ui_mainwindow.h"

Ui::MainWindow *ptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptr = ui;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setConnectionIEC104Master(QString ip, uint16_t port)
{
    /*Управление кнопками*/
    ui->pbConnect->setEnabled(false);
    ui->pbDisconnect->setEnabled(true);

    QString temp;
    temp = "Connecting to: %1:%2";
    temp = temp.arg(ip).arg(port);
    ui->textEdit->append(temp);

    con = CS104_Connection_create(ip.toStdString().c_str(), port);

    CS104_Connection_setConnectionHandler(con, connectionHandler, NULL);
    CS104_Connection_setASDUReceivedHandler(con, asduReceivedHandler, NULL);

    if (CS104_Connection_connect(con))
    {
        ui->textEdit->append("Connected!");
        CS104_Connection_sendStartDT(con);
        CS104_Connection_sendInterrogationCommand(con, CS101_COT_ACTIVATION, 1, IEC60870_QOI_STATION);

        struct sCP56Time2a testTimestamp;
        CP56Time2a_createFromMsTimestamp(&testTimestamp, Hal_getTimeInMs());
        CS104_Connection_sendTestCommandWithTimestamp(con, 1, 0x4938, &testTimestamp);
        ui->textEdit->append("Wait ...\n");
    }
    else
        ui->textEdit->append("Connect failed!");

    //Thread_sleep(1000);

}




void MainWindow::on_pbConnect_clicked() //кнопка "Connect"
{
    setConnectionIEC104Master(ui->leIP->text(), ui->lePort->text().toUInt());
}

void MainWindow::on_pbDisconnect_clicked()  //кнопка "Disconnect"
{
    /*Управление кнопками*/
    ui->pbConnect->setEnabled(true);
    ui->pbDisconnect->setEnabled(false);

    /*Закрытие соединения*/
    CS104_Connection_destroy(con);
    ui->textEdit->append("exit");
}

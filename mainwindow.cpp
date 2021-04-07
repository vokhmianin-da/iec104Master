#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow *ptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ptr = this;
    ui->setupUi(this);    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setConnectionIEC104Master(QString ip, uint16_t port)
{
    do
    {
        /*Запись значений порта и IP в переменные объекта соединения*/
        portIEC104 = port;
        ipIEC104 = ip;
        /*Управление кнопками*/
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        /*Для многопоточности*/
//        con = CS104_Connection_create(ipIEC104.toStdString().c_str(), portIEC104);

//        CS104_Connection_setConnectionHandler(con, connectionHandler, NULL);
//        CS104_Connection_setASDUReceivedHandler(con, asduReceivedHandler, NULL);

//        ConnectThread *myThread = new ConnectThread(ipIEC104 = ip, portIEC104, con);
//        connect(myThread, SIGNAL(setTextStatus(QString)), this, SLOT(on_setTextStatus(QString)));

//        myThread->start();
///////////////////////////////////////////////////////////////////
        QString temp;
        temp = "Connecting to: %1:%2";
        temp = temp.arg(ip).arg(port);
        ui->textEdit->append(temp);

        con = CS104_Connection_create(ip.toStdString().c_str(), port);

        CS104_Connection_setConnectionHandler(con, connectionHandler, NULL);
        CS104_Connection_setASDUReceivedHandler(con, asduReceivedHandler, NULL);

        if (CS104_Connection_connect(con))  //здесь посмотреть возможность повторного включения
        {
            ui->textEdit->append("Connected!");
            CS104_Connection_sendStartDT(con);
            CS104_Connection_sendInterrogationCommand(con, CS101_COT_ACTIVATION, 1, IEC60870_QOI_STATION);  //общий опрос

            struct sCP56Time2a testTimestamp;
            CP56Time2a_createFromMsTimestamp(&testTimestamp, Hal_getTimeInMs());
            CS104_Connection_sendTestCommandWithTimestamp(con, 1, 0x4938, &testTimestamp);
            ui->textEdit->append("Wait ...");
            break;
        }
        else
        {
            ui->textEdit->append("Connect failed!");
        }
    } while (1);

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

void MainWindow::on_tableWidget_cellChanged(int row, int column)    //отправка команды по изменению значения в таблице
{
    if(column == 1) //если изменилось поле value
    {
        if(row == 0)    //Для BitString
        {

            InformationObject sc = (InformationObject)  Bitstring32Command_create(NULL, 1, ptr->ui->tableWidget->item(0, 1)->text().toUInt());
            CS104_Connection_sendProcessCommandEx(con, CS101_COT_ACTIVATION, 1, sc);
            InformationObject_destroy(sc);
        }
        if(row == 1)    //Для Word
        {

            InformationObject sc = (InformationObject)  SetpointCommandScaled_create(NULL, 3, ptr->ui->tableWidget->item(1, 1)->text().toInt(), false, 0);
            CS104_Connection_sendProcessCommandEx(con, CS101_COT_ACTIVATION, 1, sc);
            InformationObject_destroy(sc);
        }
    }
}

void MainWindow::on_setTextStatus(QString str)
{
    ui->textEdit->append(str);
}

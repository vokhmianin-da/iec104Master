#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setConnectionIEC104Master(QString ip, uint16_t port)
{
        /*Запись значений порта и IP в переменные объекта соединения*/
        portIEC104 = port;
        ipIEC104 = ip;
        /*Управление кнопками*/
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);

        ConnectThread *myThread = new ConnectThread(ipIEC104, portIEC104, this);
        connect(myThread, SIGNAL(setTextStatus(QString)), this, SLOT(on_setTextStatus(QString)));
        connect(this, SIGNAL(sendCom(int, int, QVariant)), myThread, SLOT(sendCommand(int, int, QVariant)));

        myThread->start();


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

//    /*Закрытие соединения*/
//    CS104_Connection_destroy(con);
//    ui->textEdit->append("exit");
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)    //отправка команды по изменению значения в таблице
{
    emit sendCom(row, column, ui->tableWidget->item(row, column)->text());
//    if(column == 1) //если изменилось поле value
//    {
//        if(row == 0)    //Для BitString
//        {
//            emit sendCom(row, column, ui->tableWidget->item(0, 1)->text().toUInt());
//        }
//        if(row == 1)    //Для Word
//        {
//            emit sendCom(row, column, ui->tableWidget->item(1, 1)->text().toInt());
//        }
//    }
}

void MainWindow::on_setTextStatus(QString str)
{
    ui->textEdit->append(str);
}

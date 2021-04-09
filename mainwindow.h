#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVariant>

#include "iec60870/cs104_connection.h"
#include "iec60870/hal_time.h"
#include "iec60870/hal_thread.h"

#include <stdio.h>
#include <stdlib.h>

#include "CS104Handlers.h"
#include "connectthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setConnectionIEC104Master(QString ip, uint16_t port);  //создание соединения от мастера

    friend void connectionHandler (void* parameter, CS104_Connection connection, CS104_ConnectionEvent event); /* Connection event handler */
    friend bool asduReceivedHandler (void* parameter, int address, CS101_ASDU asdu);   // CS101_ASDUReceivedHandler implementation (For CS104 the address parameter has to be ignored)


private slots:
    void on_pbConnect_clicked();

    void on_pbDisconnect_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_setTextStatus(QString str);

private:
    Ui::MainWindow *ui;
    QString ipIEC104;
    uint16_t portIEC104;

signals:
    void sendCom(int, int, QVariant);
};
#endif // MAINWINDOW_H

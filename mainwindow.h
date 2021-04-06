#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "iec60870/cs104_connection.h"
#include "iec60870/hal_time.h"
#include "iec60870/hal_thread.h"

#include <stdio.h>
#include <stdlib.h>

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

private slots:
    void on_pbConnect_clicked();

    void on_pbDisconnect_clicked();

private:
    Ui::MainWindow *ui;
    CS104_Connection con;   //соединение IEC104Master

    static void connectionHandler (void* parameter, CS104_Connection connection, CS104_ConnectionEvent event); /* Connection event handler */
    static bool asduReceivedHandler (void* parameter, int address, CS101_ASDU asdu);   // CS101_ASDUReceivedHandler implementation (For CS104 the address parameter has to be ignored)
};
#endif // MAINWINDOW_H

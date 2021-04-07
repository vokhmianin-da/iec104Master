#ifndef IEC104MANAGER_H
#define IEC104MANAGER_H

#include <QObject>
#include "iec60870/cs104_connection.h"
#include "iec60870/hal_time.h"
#include "iec60870/hal_thread.h"

class IEC104Manager : public QObject
{
    Q_OBJECT
public:
    explicit IEC104Manager(QObject *parent = nullptr);

signals:
    void pbConnectClicked();
private:
    CS104_Connection con;   //соединение IEC104Master
    QString ipIEC104;
    uint16_t portIEC104;

    static void connectionHandler (void* parameter, CS104_Connection connection, CS104_ConnectionEvent event); /* Connection event handler */
    static bool asduReceivedHandler (void* parameter, int address, CS101_ASDU asdu);   // CS101_ASDUReceivedHandler implementation (For CS104 the address parameter has to be ignored)
};

#endif // IEC104MANAGER_H

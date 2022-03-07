#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NetTester.h"
#include <QTcpSocket>

class NetTester : public QMainWindow
{
    Q_OBJECT

public:
    NetTester(QWidget *parent = Q_NULLPTR);
    void bindConnect();
    int sendTCP(QString IP,quint16 port, QString data);
    int listenTCP(int port = 12600);
    int Log(QString log);
    void on_buttonSendTCP_clicked();
    void on_buttonListenTCP_clicked();

signals:
    
    

private:
    Ui::NetTesterClass ui;
    QTcpSocket* m_socket;
    
};

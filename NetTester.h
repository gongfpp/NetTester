#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NetTester.h"
#include <QTcpSocket>
#include "QTcpServer"
class NetTester : public QMainWindow
{
    Q_OBJECT

public:
    NetTester(QWidget *parent = Q_NULLPTR);
    ~NetTester();
    

    void bindConnect();

    int sendTCP(QString IP,quint16 port, QString data);
    int sendTCP(QString IP, quint16 port, QString data, QString& error);
    int listenTCP(int port);
    //��������
    void serverNewConnection();
    void serverReadyRead();
    void serverDisconnected();
    //�ͻ��˲�
    void clientReadyRead();
    void clientConnected();
    void clientDisconnected();

    //��ť��
    void on_buttonSendTCP_clicked();
    void on_buttonListenTCP_clicked();
    void on_buttonTest_clicked();

    //��־
    void Log(QString log);

signals:
    
    

private:
    Ui::NetTesterClass ui;
    QTcpServer* m_tcpServer;
    QTcpSocket* m_clientSocket;
    QTcpSocket* m_serverSocket;
    QString TestText;
};

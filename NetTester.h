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

    //网络函数
    int sendTCP(QString IP,quint16 port, QString data);
    int sendTCP(QString IP, quint16 port, QString data, QString& error);
    int listenTCP(int port);

    //按钮槽
    void on_buttonConnect_clicked();
    void on_buttonSendTCP_clicked();
    void on_buttonListenTCP_clicked();
    void on_buttonMultiSendTCP_clicked();
    void on_buttonTest_clicked();


    //日志
    void LogInfo(QString log,int opCode = 0);


    //服务器槽
    void serverNewConnection();
    void serverReadyRead();
    void serverDisconnected();

    //客户端槽
    void clientReadyRead();
    void clientConnected();
    void clientDisconnected();

signals:
    
protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::NetTesterClass ui;
    QTcpServer* m_tcpServer;
    QTcpSocket* m_clientSocket;
    QTcpSocket* m_serverSocket;
    QString TestText;
};

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

    //���纯��
    int sendTCP(QString IP,quint16 port, QString data);
    int sendTCP(QString IP, quint16 port, QString data, QString& error);
    int listenTCP(int port);

    //��ť��
    void on_buttonConnect_clicked();
    void on_buttonSendTCP_clicked();
    void on_buttonListenTCP_clicked();
    void on_buttonMultiSendTCP_clicked();
    void on_buttonTest_clicked();


    //��־
    void LogInfo(QString log,int opCode = 0);


    //��������
    void serverNewConnection();
    void serverReadyRead();
    void serverDisconnected();

    //�ͻ��˲�
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

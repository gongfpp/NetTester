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
    
    QString GET_FEED_INFORMATION(QString PROSN_ZJ, QString TBR_SBBH, QMap<QString, QString> CHECK_ITEM);

    void bindConnect();

    int sendTCP(QString IP,quint16 port, QString data);
    int sendTCP(QString IP, quint16 port, QString data, QString& error);
    int listenTCP(int port);
    //��������
    int serverNewConnection();
    int serverReadyRead();
    int serverDisconnected();
    //�ͻ��˲�
    int clentReadyRead();

    int Log(QString log);
    //��ť��
    void on_buttonSendTCP_clicked();
    void on_buttonListenTCP_clicked();

signals:
    
    

private:
    Ui::NetTesterClass ui;
    QTcpServer* m_tcpServer;
    QTcpSocket* m_clientSocket;
    QTcpSocket* m_serverSocket;
};

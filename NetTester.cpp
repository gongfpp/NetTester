#include "NetTester.h"
#include"QHostAddress"
NetTester::NetTester(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    bindConnect();
    setWindowTitle("TCP - 客户端");

    // 创建通信的套接字对象
    m_socket = new QTcpSocket(this);

   

}


void NetTester::bindConnect()
{
    connect(ui.buttonListenTCP, &QPushButton::clicked, this, &NetTester::listenTCP);
    connect(ui.buttonSendTCP, &QPushButton::clicked, this, &NetTester::sendTCP);
    

    //// 检测服务器是否回复了数据
    //connect(m_socket, &QTcpSocket::readyRead, [=]()
    //    {
    //        // 接收服务器发送的数据
    //        QByteArray recvMsg = m_socket->readAll();
    //        ui->record->append("服务器Say: " + recvMsg);
    //    });

    //// 检测是否和服务器是否连接成功了
    //connect(m_socket, &QTcpSocket::connected, this, [=]()
    //    {
    //        ui->record->append("恭喜, 连接服务器成功!!!");
    //        m_status->setPixmap(QPixmap(":/connect.png").scaled(20, 20));
    //    });

    //// 检测服务器是否和客户端断开了连接
    //connect(m_socket, &QTcpSocket::disconnected, this, [=]()
    //    {
    //        ui->record->append("服务器已经断开了连接, ...");
    //        ui->connectServer->setEnabled(true);
    //        ui->disconnect->setEnabled(false);
    //    });
}

int NetTester::sendTCP(QString url, QString data) {
    QString IP = ui.editSendIP->text();
    unsigned short port = ui.editSendPort->text().toUShort();
    m_socket->connectToHost(QHostAddress(IP), port);

    //m_socket->write(ui.plainTextEditData->toPlainText().toLocal8Bit());



    return 0;
}

int NetTester::listenTCP(int port)
{
    
    return 0;
}

int NetTester::Log(QString log)
{
    ui.plainTextEditLog->appendPlainText(log);
    return 0;
}

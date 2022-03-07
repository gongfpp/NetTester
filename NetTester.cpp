#include "NetTester.h"
#include"QHostAddress"
NetTester::NetTester(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    bindConnect();
    setWindowTitle(QString("TCP - client").toUtf8());

    // 创建通信的套接字对象
    m_socket = new QTcpSocket(this);

   

}
void NetTester::bindConnect()
{
    connect(ui.buttonListenTCP, &QPushButton::clicked, this, &NetTester::on_buttonListenTCP_clicked);
    connect(ui.buttonSendTCP, &QPushButton::clicked, this, &NetTester::on_buttonSendTCP_clicked);
    

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
int NetTester::sendTCP(QString IP,quint16 port, QString data) {
    m_socket->connectToHost(QHostAddress(IP), port);
    int reCode = m_socket->write(ui.plainTextEditData->toPlainText().toLocal8Bit());
    m_socket->close();
    return reCode;
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
void NetTester::on_buttonSendTCP_clicked()
{
    Log("on_buttonSendTCP_clicked");
    QString IP = ui.editSendIP->text();
    unsigned short port = ui.editSendPort->text().toUShort();
    QString data = ui.plainTextEditData->toPlainText();

    int reCode = sendTCP(IP, port, data);
    Log(QString("SendTCP reCode:%1").arg(reCode));
}
void NetTester::on_buttonListenTCP_clicked()
{
    Log("on_buttonListenTCP_clicked");
}

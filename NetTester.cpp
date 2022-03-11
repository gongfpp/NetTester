#include "NetTester.h"
#include"QHostAddress"
#include "QtXml"
#include "QFile"
#pragma execution_character_set("utf-8")

//初始化
NetTester::NetTester(QWidget *parent)
    : QMainWindow(parent)
    , m_clientSocket(new QTcpSocket(this))
    , m_tcpServer(new QTcpServer(this))
    , m_serverSocket(new QTcpSocket(this))
{
    ui.setupUi(this);
    bindConnect();
    setWindowTitle(QString("TCP - Client").toUtf8());
    ui.groupBoxReceive->hide();
    ui.editSendIP->setText("127.0.0.1");
    ui.editSendPort->setText("7000");
    
    //测试文本丢txt里 读取到TestText  测试按钮发这个变量的数据
    //QFile file("test.txt",this);
    //file.open(QIODevice::OpenMode::enum_type::ReadOnly);
    //TestText = file.readAll();
    //Log(QString("TestText=%1").arg(TestText));
    
    //状态栏
    ui.statusBar->showMessage("软件初始化完成 TCP未链接");
}
NetTester::~NetTester()
{
    //m_clientSocket->deleteLater();
    //m_clientSocket->close();
    //m_serverSocket->close();
}

//信号槽绑定
void NetTester::bindConnect()
{
    //按钮信号槽
    connect(ui.buttonListenTCP, &QPushButton::clicked, this, &NetTester::on_buttonListenTCP_clicked);
    connect(ui.buttonSendTCP, &QPushButton::clicked, this, &NetTester::on_buttonSendTCP_clicked);
    connect(ui.buttonTest, &QPushButton::clicked, this, &NetTester::on_buttonTest_clicked);

    //客户端 tcp信号槽
    connect(m_clientSocket, &QTcpSocket::connected, this, &NetTester::clientConnected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &NetTester::clientReadyRead);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &NetTester::clientDisconnected);
    

}
//发送TCP数据包
int NetTester::sendTCP(QString IP, quint16 port, QString data) {
    if (m_clientSocket->peerAddress() != QHostAddress(IP)) {
        m_clientSocket->disconnectFromHost();
        m_clientSocket->connectToHost(QHostAddress(IP), port);
        //m_clientSocket->waitForConnected();
    }
    //Log(QString("peer:%1").arg(m_clientSocket->peerAddress().toString()));
    int reCode;
    if (m_clientSocket->peerAddress() != QHostAddress(IP)) {
        reCode = -1;
        return reCode;
    }
    //Log(QString("连接到主机 :%1").arg(m_clientSocket->peerName()));
    QByteArray transData = data.toLocal8Bit();
    //reCode = m_clientSocket->write(transData, qstrlen(transData));
    reCode = m_clientSocket->write(transData);
    
    return reCode;
}
//发送TCP数据包 
int NetTester::sendTCP(QString IP,quint16 port, QString data,QString& error) {
    if (m_clientSocket->peerAddress() != QHostAddress(IP)) {
        m_clientSocket->disconnectFromHost();
        m_clientSocket->connectToHost(QHostAddress(IP), port);
        //m_clientSocket->waitForConnected();
    }
    //Log(QString("peer:%1").arg(m_clientSocket->peerAddress().toString()));
    int reCode;
    if (m_clientSocket->peerAddress() != QHostAddress(IP)) {
        reCode = -1;
        error = QString("TCP链接失败 现在链接至:%1").arg(m_clientSocket->peerAddress().toString());
        return reCode;
    }
    //Log(QString("连接到主机 :%1").arg(m_clientSocket->peerName()));
    QByteArray transData = data.toLocal8Bit();
    //reCode = m_clientSocket->write(transData, qstrlen(transData));
    reCode = m_clientSocket->write(transData);
    //m_clientSocket->waitForBytesWritten();
    if (reCode == -1)
        error = m_clientSocket->errorString();
    else
        error = "发送成功 数据长度:"+QString::number(reCode);
    return reCode;
}
//TODO:封装服务器端监听TCP TODO
int NetTester::listenTCP(int port)
{
    m_tcpServer->listen(QHostAddress::Any, port);
    return 0;
}
//TODO:封装服务器端监听TCP 新连接建立 TODO
void NetTester::serverNewConnection()
{
    
}
//TODO:封装服务器端监听TCP 新连接建立 准备读取 TODO
void NetTester::serverReadyRead()
{

}
//TODO:封装服务器端监听TCP 断开链接 TODO
void NetTester::serverDisconnected()
{

}
//TODO:封装客户端监听TCP 准备读取
void NetTester::clientReadyRead()
{
    ui.statusBar->showMessage("从服务器端接收数据");
}
void NetTester::clientConnected()
{
    ui.statusBar->showMessage("客户端TCP链接已建立:"+m_clientSocket->peerAddress().toString());
}
void NetTester::clientDisconnected()
{
    ui.statusBar->showMessage("客户端TCP链接已断开");
}
//日志
void NetTester::Log(QString log)
{
    ui.plainTextEditLog->appendPlainText(log);
}
//按钮：发送TCP
void NetTester::on_buttonSendTCP_clicked()
{
    Log("准备发送数据");
    QString IP = ui.editSendIP->text();
    unsigned short port = ui.editSendPort->text().toUShort();
    QString data = ui.plainTextEditData->toPlainText();
    QString error;
    int reCode = sendTCP(IP, port, data,error);
    if (reCode == -1) {
        Log(QString("发送数据出错 reCode:%1 error:%2").arg(reCode).arg(error));
    }
    else {
        Log(QString("发送数据成功 数据长度:%1").arg(reCode));
    }
    
}
//按钮：监听TCP
void NetTester::on_buttonListenTCP_clicked()
{
    Log("准备监听TCP 此功能未实现");
    quint16 port = ui.editPort->text().toUInt();
    //Log(QString("port : %1").arg(port));
    if (port == 0) {
        Log("端口号不能为空");
        return;
    }
    int reCode = listenTCP(port);
    if(reCode == 0)
        ui.buttonListenTCP->setEnabled(false);

}
//按钮：测试
void NetTester::on_buttonTest_clicked()
{
    Log(QString("测试功能：循环发送TCP数据包10000次"));
    QString IP = ui.editSendIP->text();
    unsigned short port = ui.editSendPort->text().toUShort();
    QString data = TestText;
    QString error;
    int reCode;
    for (int i = 0; i < 10000; i++)
    {
        reCode = sendTCP(IP, port, data, error);
        Log(QString("测试次数:%1 reCode=%2 error=%3").arg(i).arg(reCode).arg(error));
    }
}

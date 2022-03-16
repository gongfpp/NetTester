#include "NetTester.h"
#include"QHostAddress"
#include "QtXml"
#include "QFile"
#include "qmessagebox.h"
#pragma execution_character_set("utf-8")

//TODO：可封装以下操作为类
//日志可分4个等级（即四个函数 info\debug\warning\error ）、一种enum表示附加操作，如清空等
//TCP通讯需要的步骤较多，处理复杂


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
    //ui.groupBoxReceive->hide();
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

//关闭软件 重写了关闭事件
void NetTester::closeEvent(QCloseEvent* event) {
    if (QMessageBox::question(this, "确定", "是否退出程序", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        m_tcpServer->close();
        m_clientSocket->close();
        m_serverSocket->close();

        m_tcpServer->deleteLater();
        m_clientSocket->deleteLater();
        m_serverSocket->deleteLater();

        event->accept();
    }
    else
    {
        event->ignore();
    }
}
//信号槽绑定
void NetTester::bindConnect()
{
    //按钮信号槽
    connect(ui.buttonConnect, &QPushButton::clicked, this, &NetTester::on_buttonConnect_clicked);
    connect(ui.buttonListenTCP, &QPushButton::clicked, this, &NetTester::on_buttonListenTCP_clicked);
    connect(ui.buttonSendTCP, &QPushButton::clicked, this, &NetTester::on_buttonSendTCP_clicked);
    connect(ui.buttonMultiSendTCP, &QPushButton::clicked, this, &NetTester::on_buttonMultiSendTCP_clicked);

    //客户端 tcp信号槽
    connect(m_clientSocket, &QTcpSocket::connected, this, &NetTester::clientConnected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &NetTester::clientReadyRead);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &NetTester::clientDisconnected);
    
    //测试
    connect(ui.buttonTest, &QPushButton::clicked, this, &NetTester::on_buttonTest_clicked);
}

//日志
void NetTester::LogInfo(QString log,int opCode)
{
    if (opCode == 0x1)
        ui.plainTextEditLog->clear();
    ui.plainTextEditLog->appendPlainText(log);
}
//按钮：客户端-建立链接
void NetTester::on_buttonConnect_clicked()
{
    QString IP = ui.editSendIP->text();
    quint16 port = ui.editSendPort->text().toUInt();
    if (m_clientSocket->peerAddress() != QHostAddress(IP)) {
        m_clientSocket->disconnectFromHost();
        LogInfo("建立链接中...");
        m_clientSocket->connectToHost(QHostAddress(IP), port);
        //m_clientSocket->waitForConnected();
        //LogInfo("链接建立完成", 1);
    }
    else {
        LogInfo("已有此链接");
    }
    
}
//按钮：客户端-发送数据
void NetTester::on_buttonSendTCP_clicked()
{
    //Log("准备发送数据");
    QString IP = ui.editSendIP->text();
    unsigned short port = ui.editSendPort->text().toUShort();
    QString data = ui.plainTextEditData->toPlainText();
    QString error;
    int reCode = sendTCP(IP, port, data,error);
    if (reCode == -1) {
        LogInfo(QString("发送数据出错 reCode:%1 error:%2").arg(reCode).arg(error));
    }
    else {
        LogInfo(QString("发送数据成功 数据长度:%1").arg(reCode));
    }
    
}
//按钮：客户端-发送1000次数据
void NetTester::on_buttonMultiSendTCP_clicked()
{
    QString IP = ui.editSendIP->text();
    unsigned short port = ui.editSendPort->text().toUShort();
    QString data = ui.plainTextEditData->toPlainText();
    QString error;
    int reCode;
    for (int i = 0; i < 1000; i++)
    {
        reCode = sendTCP(IP, port, data, error);
        LogInfo(QString("测试次数:%1 reCode=%2 error=%3").arg(i).arg(reCode).arg(error));
    }
    LogInfo(QString("测试功能：循环发送TCP数据包1000次"));

}
//按钮：服务器-监听TCP
void NetTester::on_buttonListenTCP_clicked()
{
    LogInfo("准备监听TCP 此功能未实现");
    quint16 port = ui.editPort->text().toUInt();
    //Log(QString("port : %1").arg(port));
    if (port == 0) {
        LogInfo("端口号不能为空");
        return;
    }
    int reCode = listenTCP(port);
    if(reCode == 0)
        ui.buttonListenTCP->setEnabled(false);

}

//按钮：测试
void NetTester::on_buttonTest_clicked()
{
    LogInfo("测试函数空的");
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
int NetTester::sendTCP(QString IP, quint16 port, QString data, QString& error) {
    
    //Log(QString("peer:%1").arg(m_clientSocket->peerAddress().toString()));
    //错误代码 目前没设计功能 暂时定非0为错
    int reCode;
    if (m_clientSocket->peerAddress().isNull()) {
        reCode = -1;
        error = QString("TCP发送失败 未连接至服务器 ");
        return reCode;
    }
    if (m_clientSocket->peerAddress() != QHostAddress(IP)) {
        reCode = -1;
        error = QString("TCP发送失败 IP地址不一致 目标地址:%2 现在链接至:%1 ").arg(m_clientSocket->peerAddress().toString().arg(IP));
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
        error = "发送成功 数据长度:" + QString::number(reCode);
    return reCode;
}

//TODO:槽-服务器端监听TCP TODO
int NetTester::listenTCP(int port)
{
    m_tcpServer->listen(QHostAddress::Any, port);
    return 0;
}
//TODO:槽-服务器端监听TCP 新连接建立 TODO
void NetTester::serverNewConnection()
{

}
//TODO:槽-服务器端监听TCP 新连接建立 准备读取 TODO
void NetTester::serverReadyRead()
{

}
//TODO:槽-服务器端监听TCP 断开链接 TODO
void NetTester::serverDisconnected()
{

}
//TODO:槽-客户端监听TCP 准备读取
void NetTester::clientReadyRead()
{
    ui.statusBar->showMessage("从服务器端接收数据");
}
void NetTester::clientConnected()
{
    ui.statusBar->showMessage("客户端TCP链接已建立:" + m_clientSocket->peerAddress().toString());
}
void NetTester::clientDisconnected()
{
    ui.statusBar->showMessage("客户端TCP链接已断开");
}
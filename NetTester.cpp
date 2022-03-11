#include "NetTester.h"
#include"QHostAddress"
#include "QtXml"
#include "QFile"
#pragma execution_character_set("utf-8")

//��ʼ��
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
    
    //�����ı���txt�� ��ȡ��TestText  ���԰�ť���������������
    //QFile file("test.txt",this);
    //file.open(QIODevice::OpenMode::enum_type::ReadOnly);
    //TestText = file.readAll();
    //Log(QString("TestText=%1").arg(TestText));
    
    //״̬��
    ui.statusBar->showMessage("�����ʼ����� TCPδ����");
}
NetTester::~NetTester()
{
    //m_clientSocket->deleteLater();
    //m_clientSocket->close();
    //m_serverSocket->close();
}

//�źŲ۰�
void NetTester::bindConnect()
{
    //��ť�źŲ�
    connect(ui.buttonListenTCP, &QPushButton::clicked, this, &NetTester::on_buttonListenTCP_clicked);
    connect(ui.buttonSendTCP, &QPushButton::clicked, this, &NetTester::on_buttonSendTCP_clicked);
    connect(ui.buttonTest, &QPushButton::clicked, this, &NetTester::on_buttonTest_clicked);

    //�ͻ��� tcp�źŲ�
    connect(m_clientSocket, &QTcpSocket::connected, this, &NetTester::clientConnected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &NetTester::clientReadyRead);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &NetTester::clientDisconnected);
    

}
//����TCP���ݰ�
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
    //Log(QString("���ӵ����� :%1").arg(m_clientSocket->peerName()));
    QByteArray transData = data.toLocal8Bit();
    //reCode = m_clientSocket->write(transData, qstrlen(transData));
    reCode = m_clientSocket->write(transData);
    
    return reCode;
}
//����TCP���ݰ� 
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
        error = QString("TCP����ʧ�� ����������:%1").arg(m_clientSocket->peerAddress().toString());
        return reCode;
    }
    //Log(QString("���ӵ����� :%1").arg(m_clientSocket->peerName()));
    QByteArray transData = data.toLocal8Bit();
    //reCode = m_clientSocket->write(transData, qstrlen(transData));
    reCode = m_clientSocket->write(transData);
    //m_clientSocket->waitForBytesWritten();
    if (reCode == -1)
        error = m_clientSocket->errorString();
    else
        error = "���ͳɹ� ���ݳ���:"+QString::number(reCode);
    return reCode;
}
//TODO:��װ�������˼���TCP TODO
int NetTester::listenTCP(int port)
{
    m_tcpServer->listen(QHostAddress::Any, port);
    return 0;
}
//TODO:��װ�������˼���TCP �����ӽ��� TODO
void NetTester::serverNewConnection()
{
    
}
//TODO:��װ�������˼���TCP �����ӽ��� ׼����ȡ TODO
void NetTester::serverReadyRead()
{

}
//TODO:��װ�������˼���TCP �Ͽ����� TODO
void NetTester::serverDisconnected()
{

}
//TODO:��װ�ͻ��˼���TCP ׼����ȡ
void NetTester::clientReadyRead()
{
    ui.statusBar->showMessage("�ӷ������˽�������");
}
void NetTester::clientConnected()
{
    ui.statusBar->showMessage("�ͻ���TCP�����ѽ���:"+m_clientSocket->peerAddress().toString());
}
void NetTester::clientDisconnected()
{
    ui.statusBar->showMessage("�ͻ���TCP�����ѶϿ�");
}
//��־
void NetTester::Log(QString log)
{
    ui.plainTextEditLog->appendPlainText(log);
}
//��ť������TCP
void NetTester::on_buttonSendTCP_clicked()
{
    Log("׼����������");
    QString IP = ui.editSendIP->text();
    unsigned short port = ui.editSendPort->text().toUShort();
    QString data = ui.plainTextEditData->toPlainText();
    QString error;
    int reCode = sendTCP(IP, port, data,error);
    if (reCode == -1) {
        Log(QString("�������ݳ��� reCode:%1 error:%2").arg(reCode).arg(error));
    }
    else {
        Log(QString("�������ݳɹ� ���ݳ���:%1").arg(reCode));
    }
    
}
//��ť������TCP
void NetTester::on_buttonListenTCP_clicked()
{
    Log("׼������TCP �˹���δʵ��");
    quint16 port = ui.editPort->text().toUInt();
    //Log(QString("port : %1").arg(port));
    if (port == 0) {
        Log("�˿ںŲ���Ϊ��");
        return;
    }
    int reCode = listenTCP(port);
    if(reCode == 0)
        ui.buttonListenTCP->setEnabled(false);

}
//��ť������
void NetTester::on_buttonTest_clicked()
{
    Log(QString("���Թ��ܣ�ѭ������TCP���ݰ�10000��"));
    QString IP = ui.editSendIP->text();
    unsigned short port = ui.editSendPort->text().toUShort();
    QString data = TestText;
    QString error;
    int reCode;
    for (int i = 0; i < 10000; i++)
    {
        reCode = sendTCP(IP, port, data, error);
        Log(QString("���Դ���:%1 reCode=%2 error=%3").arg(i).arg(reCode).arg(error));
    }
}

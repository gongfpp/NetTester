#include "NetTester.h"
#include"QHostAddress"
#include "QtXml"
#include "QFile"
#include "qmessagebox.h"
#pragma execution_character_set("utf-8")

//TODO���ɷ�װ���²���Ϊ��
//��־�ɷ�4���ȼ������ĸ����� info\debug\warning\error ����һ��enum��ʾ���Ӳ���������յ�
//TCPͨѶ��Ҫ�Ĳ���϶࣬������


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
    //ui.groupBoxReceive->hide();
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

//�ر���� ��д�˹ر��¼�
void NetTester::closeEvent(QCloseEvent* event) {
    if (QMessageBox::question(this, "ȷ��", "�Ƿ��˳�����", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
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
//�źŲ۰�
void NetTester::bindConnect()
{
    //��ť�źŲ�
    connect(ui.buttonConnect, &QPushButton::clicked, this, &NetTester::on_buttonConnect_clicked);
    connect(ui.buttonListenTCP, &QPushButton::clicked, this, &NetTester::on_buttonListenTCP_clicked);
    connect(ui.buttonSendTCP, &QPushButton::clicked, this, &NetTester::on_buttonSendTCP_clicked);
    connect(ui.buttonMultiSendTCP, &QPushButton::clicked, this, &NetTester::on_buttonMultiSendTCP_clicked);

    //�ͻ��� tcp�źŲ�
    connect(m_clientSocket, &QTcpSocket::connected, this, &NetTester::clientConnected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &NetTester::clientReadyRead);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &NetTester::clientDisconnected);
    
    //����
    connect(ui.buttonTest, &QPushButton::clicked, this, &NetTester::on_buttonTest_clicked);
}

//��־
void NetTester::LogInfo(QString log,int opCode)
{
    if (opCode == 0x1)
        ui.plainTextEditLog->clear();
    ui.plainTextEditLog->appendPlainText(log);
}
//��ť���ͻ���-��������
void NetTester::on_buttonConnect_clicked()
{
    QString IP = ui.editSendIP->text();
    quint16 port = ui.editSendPort->text().toUInt();
    if (m_clientSocket->peerAddress() != QHostAddress(IP)) {
        m_clientSocket->disconnectFromHost();
        LogInfo("����������...");
        m_clientSocket->connectToHost(QHostAddress(IP), port);
        //m_clientSocket->waitForConnected();
        //LogInfo("���ӽ������", 1);
    }
    else {
        LogInfo("���д�����");
    }
    
}
//��ť���ͻ���-��������
void NetTester::on_buttonSendTCP_clicked()
{
    //Log("׼����������");
    QString IP = ui.editSendIP->text();
    unsigned short port = ui.editSendPort->text().toUShort();
    QString data = ui.plainTextEditData->toPlainText();
    QString error;
    int reCode = sendTCP(IP, port, data,error);
    if (reCode == -1) {
        LogInfo(QString("�������ݳ��� reCode:%1 error:%2").arg(reCode).arg(error));
    }
    else {
        LogInfo(QString("�������ݳɹ� ���ݳ���:%1").arg(reCode));
    }
    
}
//��ť���ͻ���-����1000������
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
        LogInfo(QString("���Դ���:%1 reCode=%2 error=%3").arg(i).arg(reCode).arg(error));
    }
    LogInfo(QString("���Թ��ܣ�ѭ������TCP���ݰ�1000��"));

}
//��ť��������-����TCP
void NetTester::on_buttonListenTCP_clicked()
{
    LogInfo("׼������TCP �˹���δʵ��");
    quint16 port = ui.editPort->text().toUInt();
    //Log(QString("port : %1").arg(port));
    if (port == 0) {
        LogInfo("�˿ںŲ���Ϊ��");
        return;
    }
    int reCode = listenTCP(port);
    if(reCode == 0)
        ui.buttonListenTCP->setEnabled(false);

}

//��ť������
void NetTester::on_buttonTest_clicked()
{
    LogInfo("���Ժ����յ�");
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
int NetTester::sendTCP(QString IP, quint16 port, QString data, QString& error) {
    
    //Log(QString("peer:%1").arg(m_clientSocket->peerAddress().toString()));
    //������� Ŀǰû��ƹ��� ��ʱ����0Ϊ��
    int reCode;
    if (m_clientSocket->peerAddress().isNull()) {
        reCode = -1;
        error = QString("TCP����ʧ�� δ������������ ");
        return reCode;
    }
    if (m_clientSocket->peerAddress() != QHostAddress(IP)) {
        reCode = -1;
        error = QString("TCP����ʧ�� IP��ַ��һ�� Ŀ���ַ:%2 ����������:%1 ").arg(m_clientSocket->peerAddress().toString().arg(IP));
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
        error = "���ͳɹ� ���ݳ���:" + QString::number(reCode);
    return reCode;
}

//TODO:��-�������˼���TCP TODO
int NetTester::listenTCP(int port)
{
    m_tcpServer->listen(QHostAddress::Any, port);
    return 0;
}
//TODO:��-�������˼���TCP �����ӽ��� TODO
void NetTester::serverNewConnection()
{

}
//TODO:��-�������˼���TCP �����ӽ��� ׼����ȡ TODO
void NetTester::serverReadyRead()
{

}
//TODO:��-�������˼���TCP �Ͽ����� TODO
void NetTester::serverDisconnected()
{

}
//TODO:��-�ͻ��˼���TCP ׼����ȡ
void NetTester::clientReadyRead()
{
    ui.statusBar->showMessage("�ӷ������˽�������");
}
void NetTester::clientConnected()
{
    ui.statusBar->showMessage("�ͻ���TCP�����ѽ���:" + m_clientSocket->peerAddress().toString());
}
void NetTester::clientDisconnected()
{
    ui.statusBar->showMessage("�ͻ���TCP�����ѶϿ�");
}
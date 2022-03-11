#include "NetTester.h"
#include"QHostAddress"
#include "QtXml"
#include "QFile"
#pragma execution_character_set("utf-8")
QString NetTester::GET_FEED_INFORMATION(QString PROSN_ZJ, QString TBR_SBBH, QMap<QString, QString>CHECK_ITEM)
{
    QDomDocument domDoc;
    domDoc.appendChild(domDoc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

    QDomElement STD_IN = domDoc.createElement("STD_IN");

    QDomElement ObjectID = domDoc.createElement("ObjectID");

    ObjectID.appendChild(domDoc.createTextNode("GET_FEED_INFORMATION"));
    QDomElement DATA = domDoc.createElement("DATA");
    QDomElement PROSN_ZJNode = domDoc.createElement("PROSN_ZJ");
    PROSN_ZJNode.appendChild(domDoc.createTextNode(PROSN_ZJ));
    QDomElement TBR_SBBHNode = domDoc.createElement("TBR_SBBH");
    TBR_SBBHNode.appendChild(domDoc.createTextNode(TBR_SBBH));
    QDomElement CHECK_ITEMS = domDoc.createElement("CHECK_ITEMS");
    QStringList keys = CHECK_ITEM.keys();
    for (int i = 0; i < keys.count(); ++i)
    {
        //ԭ���� ��������bug�� valueֵ��xmlԪ��<>�ڲ���  ��Э�鼰��������
        //QDomElement CHECK_ITEMNode = domDoc.createElement("CHECK_ITEM");
        //QDomElement ITEM_NAME = domDoc.createElement("ITEM_NAME");
        //ITEM_NAME.appendChild(domDoc.createTextNode(keys[i]));
        //QDomElement ITEM_VALUE = domDoc.createElement(CHECK_ITEM[keys[i]]);
        //CHECK_ITEMNode.appendChild(ITEM_VALUE);
        //CHECK_ITEMNode.appendChild(ITEM_NAME);
        //CHECK_ITEMS.appendChild(CHECK_ITEMNode);


        QDomElement CHECK_ITEMNode = domDoc.createElement("CHECK_ITEM");
        QDomElement ITEM_NAME = domDoc.createElement("ITEM_NAME");
        ITEM_NAME.appendChild(domDoc.createTextNode(keys[i]));
        QDomElement ITEM_VALUE = domDoc.createElement("ITEM_VALUE");
        //��ʱ�򲻿��� ���п�ֵ���Է�MES�����ܿ�ֵ����ط��üӸ���ֵ�жϣ��յľ�NA
        if (CHECK_ITEM[keys[i]].isEmpty()) {
            ITEM_VALUE.appendChild(domDoc.createTextNode("NA"));
        }
        else {
            ITEM_VALUE.appendChild(domDoc.createTextNode(CHECK_ITEM[keys[i]]));
        }

        CHECK_ITEMNode.appendChild(ITEM_NAME);
        CHECK_ITEMNode.appendChild(ITEM_VALUE);
        CHECK_ITEMS.appendChild(CHECK_ITEMNode);
    }
    DATA.appendChild(PROSN_ZJNode);
    DATA.appendChild(TBR_SBBHNode);
    DATA.appendChild(CHECK_ITEMS);
    STD_IN.appendChild(ObjectID);
    STD_IN.appendChild(DATA);
    domDoc.appendChild(STD_IN);
    return domDoc.toString(-1);
}

NetTester::NetTester(QWidget *parent)
    : QMainWindow(parent)
    , m_clientSocket(new QTcpSocket(this))
    , m_tcpServer(new QTcpServer(this))
    , m_serverSocket(new QTcpSocket(this))
{
    ui.setupUi(this);
    bindConnect();
    setWindowTitle(QString("TCP - client").toUtf8());

    // ����ͨ�ŵ��׽��ֶ���  ��ʼ���б���
    //m_socket = new QTcpSocket(this);
    //�������������� ��ʼ���б���
    //m_tcpServer = new QTcpServer(this);
    QFile file("test.txt",this);
    file.open(QIODevice::OpenMode::enum_type::ReadOnly);
    bool b = file.isOpen();
    QString fileText = file.readAll();
    Log(QString("open:%1").arg(b));
    Log(fileText);
    int reCode = sendTCP("127.0.0.1", 7000, fileText);
    Log(QString("reCode = %1").arg(reCode));
}
void NetTester::bindConnect()
{
    //��ť
    connect(ui.buttonListenTCP, &QPushButton::clicked, this, &NetTester::on_buttonListenTCP_clicked);
    connect(ui.buttonSendTCP, &QPushButton::clicked, this, &NetTester::on_buttonSendTCP_clicked);
    
    //�ͻ���
    connect(m_clientSocket, &QIODevice::readyRead, this, &NetTester::clentReadyRead);
    //connect(m_clientSocket,&QAbstractSocket::error)

    //// ���������Ƿ�ظ�������
    //connect(m_socket, &QTcpSocket::readyRead, [=]()
    //    {
    //        // ���շ��������͵�����
    //        QByteArray recvMsg = m_socket->readAll();
    //        ui->record->append("������Say: " + recvMsg);
    //    });

    //// ����Ƿ�ͷ������Ƿ����ӳɹ���
    //connect(m_socket, &QTcpSocket::connected, this, [=]()
    //    {
    //        ui->record->append("��ϲ, ���ӷ������ɹ�!!!");
    //        m_status->setPixmap(QPixmap(":/connect.png").scaled(20, 20));
    //    });

    //// ���������Ƿ�Ϳͻ��˶Ͽ�������
    //connect(m_socket, &QTcpSocket::disconnected, this, [=]()
    //    {
    //        ui->record->append("�������Ѿ��Ͽ�������, ...");
    //        ui->connectServer->setEnabled(true);
    //        ui->disconnect->setEnabled(false);
    //    });
}
int NetTester::sendTCP(QString IP, quint16 port, QString data) {
    m_clientSocket->connectToHost(QHostAddress(IP), port);
    int reCode = m_clientSocket->write(ui.plainTextEditData->toPlainText().toLocal8Bit());
    QAbstractSocket::SocketError error = m_clientSocket->error();
    m_clientSocket->close();
    return reCode;
}
int NetTester::sendTCP(QString IP,quint16 port, QString data,QString& error) {
    m_clientSocket->connectToHost(QHostAddress(IP), port);
    int reCode = m_clientSocket->write(ui.plainTextEditData->toPlainText().toLocal8Bit());
    error =  m_clientSocket->error();
    m_clientSocket->close();
    return reCode;
}

int NetTester::listenTCP(int port)
{
    m_tcpServer->listen(QHostAddress::Any, port);
    return 0;
}

int NetTester::serverNewConnection()
{
    return 0;
}

int NetTester::serverReadyRead()
{

    return 0;
}

int NetTester::serverDisconnected()
{
    return 0;
}

int NetTester::clentReadyRead()
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
    QString error;
    int reCode = sendTCP(IP, port, data,error);
    Log(QString("SendTCP reCode:%1 error:%2").arg(reCode).arg(error));
}
void NetTester::on_buttonListenTCP_clicked()
{
    Log("on_buttonListenTCP_clicked");
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

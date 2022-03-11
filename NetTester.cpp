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
        //原代码 估计是有bug的 value值在xml元素<>内部了  与协议及常理不符合
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
        //有时候不开机 会有空值，对方MES不接受空值，这地方得加个空值判断，空的就NA
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

    // 创建通信的套接字对象  初始化列表内
    //m_socket = new QTcpSocket(this);
    //创建服务器对象 初始化列表内
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
    //按钮
    connect(ui.buttonListenTCP, &QPushButton::clicked, this, &NetTester::on_buttonListenTCP_clicked);
    connect(ui.buttonSendTCP, &QPushButton::clicked, this, &NetTester::on_buttonSendTCP_clicked);
    
    //客户端
    connect(m_clientSocket, &QIODevice::readyRead, this, &NetTester::clentReadyRead);
    //connect(m_clientSocket,&QAbstractSocket::error)

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
        Log("端口号不能为空");
        return;
    }
    int reCode = listenTCP(port);
    if(reCode == 0)
        ui.buttonListenTCP->setEnabled(false);

}

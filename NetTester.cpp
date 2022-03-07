#include "NetTester.h"
#include"QHostAddress"
NetTester::NetTester(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    bindConnect();
    setWindowTitle("TCP - �ͻ���");

    // ����ͨ�ŵ��׽��ֶ���
    m_socket = new QTcpSocket(this);

   

}


void NetTester::bindConnect()
{
    connect(ui.buttonListenTCP, &QPushButton::clicked, this, &NetTester::listenTCP);
    connect(ui.buttonSendTCP, &QPushButton::clicked, this, &NetTester::sendTCP);
    

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

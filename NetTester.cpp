#include "NetTester.h"
#include"QHostAddress"
NetTester::NetTester(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    bindConnect();
    setWindowTitle(QString("TCP - client").toUtf8());

    // ����ͨ�ŵ��׽��ֶ���
    m_socket = new QTcpSocket(this);

   

}
void NetTester::bindConnect()
{
    connect(ui.buttonListenTCP, &QPushButton::clicked, this, &NetTester::on_buttonListenTCP_clicked);
    connect(ui.buttonSendTCP, &QPushButton::clicked, this, &NetTester::on_buttonSendTCP_clicked);
    

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

#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->connectPushButton, &QPushButton::clicked, this, &MainWindow::connectClick);
    connect(ui->measurePushButton, &QPushButton::clicked, this, &MainWindow::measureClick);
    connect(ui->getFwVersionPushButtonsocket, &QPushButton::clicked, this, &MainWindow::getFwVersionClick);
    connect(ui->getMeasuremntsPushButton, &QPushButton::clicked, this, &MainWindow::getMeasurementsClick);
    connect(ui->getNamePushButton, &QPushButton::clicked, this, &MainWindow::getNameClick);

    console.reset(new Console);
    ui->verticalLayout->addWidget(console.get());

}

MainWindow::~MainWindow()
{
    delete ui;

    socket->disconnectFromService();
}

void MainWindow::setNewDebugOutput(QString text)
{
    console->putData(text);
}

void MainWindow::sendData(QByteArray data)
{
    if(!socket)
        return;
    qDebug();
    qDebug() << "send:" << data.toHex();
    socket->write(data);
}

void MainWindow::connectClick()
{
    QBluetoothAddress remoteAddress("00:13:43:7F:B3:D2");

    if(socket)
        socket->disconnectFromService();

     // Connect to service
    socket.reset(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol));
    qDebug() << "Create socket";
    socket->connectToService(remoteAddress, QBluetoothUuid(QBluetoothUuid::SerialPort));
    qDebug() << "ConnectToService done";

    connect(socket.get(), &QBluetoothSocket::readyRead, this, &MainWindow::readSocket);
    connect(socket.get(), &QBluetoothSocket::connected, this, &MainWindow::connected);
    connect(socket.get(), &QBluetoothSocket::disconnected, this, &MainWindow::disconnected);
    connect(socket.get(), QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &MainWindow::onSocketErrorOccurred);
}

void MainWindow::measureClick()
{
    currentCmd = Command::MEASURE;
    QByteArray data = protocolCoder.encode(currentCmd);
    sendData(data);
}

void MainWindow::getFwVersionClick()
{
    currentCmd = Command::GET_DEVICE_INFO;
    QByteArray data = protocolCoder.encode(currentCmd);
    sendData(data);
}

void MainWindow::readSocket()
{
    qDebug() << "readSocket";
    if (!socket)
        return;

    QByteArray data = socket->readAll();

    qDebug() << "recv:" << data.toHex();

    try {
        protocolCoder.decodeResponse(currentCmd, data);
    }
    catch(const std::string& e){
        qDebug() << QString::fromStdString(e);
    }

   // getMeasurementsClick();
}

void MainWindow::connected()
{
    qDebug() << "connected\n";
}

void MainWindow::disconnected()
{
    qDebug() << "disconnected\n";
}

void MainWindow::onSocketErrorOccurred()
{
    qDebug() << "onSocketErrorOccurred\n";
}

void MainWindow::getMeasurementsClick()
{
    currentCmd = Command::GET_NEW_MEASUREMENT;
    QByteArray data = protocolCoder.encode(currentCmd);
    sendData(data);
}

void MainWindow::getNameClick()
{
    currentCmd = Command::GET_NAME;
    QByteArray data = protocolCoder.encode(currentCmd);
    sendData(data);
}

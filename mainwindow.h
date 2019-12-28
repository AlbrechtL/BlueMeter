#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>

#include "console.h"
#include "protocol_coder.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setNewDebugOutput(QString text);

private:
    Ui::MainWindow *ui;

    std::unique_ptr<Console> console;
    std::unique_ptr<QBluetoothSocket> socket;
    ProtocolCoder protocolCoder;
    Command currentCmd;

    void sendData(QByteArray data);

public slots:
    void connectClick();
    void measureClick();
    void getFwVersionClick();
    void readSocket();
    void connected();
    void disconnected();
    void onSocketErrorOccurred();
    void getMeasurementsClick();
    void getNameClick();

};
#endif // MAINWINDOW_H

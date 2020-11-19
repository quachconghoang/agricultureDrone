#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMqttClient>
#include "SensorsMQTT.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMqttClient *m_client;
    SensorsMQTT m_sensors;

    int m_current_dev;
    QString m_current_sub;
    QString m_current_pub;

    void setupMQTT(QString hostName, qint16 port);

private slots:
    void updateLogStateChange();
    void updateServer(int serv_id);
    void currentDeviceChanged(int dev_id);
//    void brokerDisconnected();
//    void on_buttonPublish_clicked();
//    void on_buttonSubscribe_clicked();
//    void on_buttonPing_clicked();
};
#endif // MAINWINDOW_H

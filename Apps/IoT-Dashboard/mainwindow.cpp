#include "mainwindow.h"
#include "ToggleButton.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QUrl>
#include <QDateTime>
#include <QTimer>
#include <QtMqtt/QMqttClient>
#include <iostream>
#include <QStandardItemModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->showFullScreen();
//    qDebug()<<("Compiled with Qt Version %s\n", QT_VERSION_STR);
    m_sensors.initHosting();

    for (QString host : m_sensors.hostnames){
        ui->hostBox->addItem(host);
        if(QUrl(host+":1880").isValid()) ui->hostBox->setCurrentIndex(ui->hostBox->findText(host));
    }

    m_sensors.loadSensorParameters(ui->hostBox->currentIndex());

    QIcon deviceIco = QIcon(":/Images/Resources/machine.png");
//    deviceIco
    for (int i = 0; i < m_sensors.sensorNodes.size(); ++i) {
        SensorNode node = m_sensors.sensorNodes[i];
        ui->devicesBox->addItem(deviceIco, QString::fromStdString(node.topic_sensor));
    }
    m_current_dev = -1;
    ui->devicesBox->setCurrentIndex(m_current_dev);

    connect(ui->devicesBox, SIGNAL(currentIndexChanged(int)), this, SLOT(currentDeviceChanged(int)));
    setupMQTT(m_sensors.hostnames[ui->hostBox->currentIndex()],1883);

//    QTimer::singleShot(1000, this, [this]{
//        auto subscription = m_client->subscribe(QString::fromStdString("demo/test"), quint8(2));
//        if (!subscription) {
//            qDebug()<<"Could not subscribe. Is there a valid connection?";
//        }
//    });
    /*
    QWidget *widget = new QWidget;
    widget->setWindowFlags(Qt::FramelessWindowHint);
    QHBoxLayout layout;
    widget->setLayout(&layout);
    ToggleButton *toggleButton1 = new ToggleButton(10, 8);
    ToggleButton *toggleButton2 = new ToggleButton(10, 12);
    layout.addWidget(toggleButton1);
    layout.addWidget(toggleButton2);
    widget->show();
 */

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMQTT(QString hostName, qint16 port) {
    m_client = new QMqttClient(this);
    m_client->setHostname(hostName);
    m_client->setPort(port);

    //    connect(m_client, &QMqttClient::disconnected, this, &MainWindow::brokerDisconnected);
    connect(m_client, &QMqttClient::stateChanged, this, &MainWindow::updateLogStateChange);
    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        const QString content = QDateTime::currentDateTime().toString()
                                + QLatin1String(" Received Topic: ")
                                + topic.name()
                                + QLatin1String(" Message: ")
                                + message
                                + QLatin1Char('\n');
        qDebug() << content;
    });

    m_client->connectToHost();
}

void MainWindow::updateLogStateChange() {
    const QString content = QDateTime::currentDateTime().toString()
                            + QLatin1String(": State Change")
                            + QString::number(m_client->state())
                            + QLatin1Char('\n');
    qDebug()<< "updateLogStateChange: " <<content;
}

void MainWindow::currentDeviceChanged(int dev_id) {
    qDebug()<<dev_id;
    if(dev_id == m_current_dev) return;

    m_client->unsubscribe(m_current_sub);

    m_current_dev = dev_id;
    m_current_sub = QString::fromStdString(m_sensors.sensorNodes[m_current_dev].topic_sensor);
    m_current_pub = QString::fromStdString(m_sensors.sensorNodes[m_current_dev].topic_control);

    auto subscription = m_client->subscribe(m_current_sub, quint8(2));
    if (!subscription) qDebug()<<"Could not subscribe. Is there a valid connection?";
}


#include "mainwindow.h"
#include "ToggleButton.h"
#include "./ui_mainwindow.h"
#include <QGuiApplication>
#include <QScreen>
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
//    this->showFullScreen();
//    this->setWindowFlags(Qt::FramelessWindowHint);

    QScreen * screen = QApplication::screens().at(0);
    QSize screenSize = screen->availableSize();
    qreal dotsPerInch = screen->logicalDotsPerInch();
    Qt::ScreenOrientation orient = screen->orientation();
    qDebug() << screenSize.width() << "-" << screenSize.height() << "dpi" << dotsPerInch;
    qDebug() << "orient" << screen->orientation();
    Qt::ScreenOrientation x = Qt::LandscapeOrientation;

    m_sensors.initHosting();

    for (QString host : m_sensors.hostnames){
        ui->hostBox->addItem(host);
        if(QUrl(host+":1883").isValid()) ui->hostBox->setCurrentIndex(ui->hostBox->findText(host));
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

    setupMQTT(m_sensors.hostnames[ui->hostBox->currentIndex()],1883);

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
    connect(ui->devicesBox, SIGNAL(currentIndexChanged(int)), this, SLOT(currentDeviceChanged(int)));
    connect(ui->hostBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateServer(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMQTT(QString hostName, qint16 port) {
    m_client = new QMqttClient();
    m_client->setHostname(hostName);
    m_client->setPort(port);
    connect(m_client, &QMqttClient::stateChanged, this, &MainWindow::updateLogStateChange);
    connect(m_client, &QMqttClient::messageReceived, this,  &MainWindow::receiveMessage);

    m_client->connectToHost();
}

void MainWindow::updateLogStateChange() {
    const QString content = QDateTime::currentDateTime().toString()
                            + QLatin1String(": State Change")
                            + QString::number(m_client->state())
                            + QLatin1Char('\n');
    qDebug()<< "updateLogStateChange: " <<content;
}

void MainWindow::updateServer(int serv_id) {
    m_client->disconnectFromHost();
    setupMQTT(m_sensors.hostnames[serv_id],1883);
    ui->devicesBox->setCurrentIndex(-1);
}

void MainWindow::currentDeviceChanged(int dev_id) {
//    qDebug()<<dev_id;
    if(dev_id == m_current_dev) return;

    m_client->unsubscribe(m_current_sub);
    m_current_dev = dev_id;
    m_current_sub = QString::fromStdString(m_sensors.sensorNodes[m_current_dev].topic_sensor);
    m_current_pub = QString::fromStdString(m_sensors.sensorNodes[m_current_dev].topic_control);

    auto subscription = m_client->subscribe(m_current_sub, quint8(2));
    if (!subscription) qDebug()<<"Could not subscribe. Is there a valid connection?";
}

void MainWindow::receiveMessage(const QByteArray &message, const QMqttTopicName &topic) {
    qDebug() << "receive";
    const QString content = QDateTime::currentDateTime().toString()
                            + QLatin1String(" Received Topic: ")
                            + topic.name()
                            + QLatin1String(" Message: ")
                            + message
                            + QLatin1Char('\n');
    qDebug() << content;
    ui->devicesView->addItem(content);
}


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
#include "MushroomView.h"
#include <QSizePolicy>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QScreen * screen = QApplication::screens().at(0);
    QSize screenSize = screen->availableSize();
    qreal dotsPerInch = screen->logicalDotsPerInch();
    Qt::ScreenOrientation orient = screen->orientation();
    Qt::ScreenOrientation x = Qt::LandscapeOrientation;

    QSize defSize = ui->iot_1->size();
    QSize curSize = this->size();
    #ifdef Q_OS_ANDROID
    curSize = screenSize;
    #endif
    qDebug() << this->size();
    double scl = double (curSize.width())/double(defSize.width());
    mushroomWidget = new MushroomView(ui->iot_1,scl);

//    mushroomWidget->setMinimumWidth(curSize.width());
//    mushroomWidget->setMinimumHeight(defSize.height()*curSize.width() / defSize.width());

    m_sensors.initHosting();
    for (QString host : m_sensors.hostnames){
        ui->hostBox->addItem(host);
        if(QUrl(host+":1883").isValid()) ui->hostBox->setCurrentIndex(ui->hostBox->findText(host));
    }
    m_sensors.loadSensorParameters(ui->hostBox->currentIndex());

    QIcon deviceIco = QIcon(":/Images/Resources/machine.png");
    for (int i = 0; i < m_sensors.sensorNodes.size(); ++i) {
        SensorNode node = m_sensors.sensorNodes[i];
        ui->devicesBox->addItem(deviceIco, QString::fromStdString(node.topic_sensor));
//        qDebug() << QString::fromStdString(node.topic_sensor);
    }
    m_current_dev = -1;
    ui->devicesBox->setCurrentIndex(m_current_dev);

    setupMQTT(m_sensors.hostnames[ui->hostBox->currentIndex()],1883);

    connect(ui->devicesBox, SIGNAL(currentIndexChanged(int)), this, SLOT(currentDeviceChanged(int)));
    connect(ui->hostBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateServer(int)));
    connect(mushroomWidget, SIGNAL(pushUpdate(QString)), this,  SLOT(controlMessage(QString)));
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
//    connect(m_client, &QMqttClient::messageReceived, this,  &MainWindow::receiveMessage);
    connect(m_client, &QMqttClient::messageReceived, mushroomWidget,  &MushroomView::receiveMessage);
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
    qDebug()<<dev_id;
    if(dev_id == m_current_dev) return;

    m_client->unsubscribe(m_current_sub);
    m_current_dev = dev_id;
    m_current_sub = QString::fromStdString(m_sensors.sensorNodes[m_current_dev].topic_sensor);
    m_current_pub = QString::fromStdString(m_sensors.sensorNodes[m_current_dev].topic_control);

    auto subscription = m_client->subscribe(m_current_sub, quint8(2));
    if (!subscription) qDebug()<<"Could not subscribe. Is there a valid connection?";
}

void MainWindow::controlMessage(QString msg) {
    qDebug() << msg;
//    ui->devicesView->addItem(content);
}


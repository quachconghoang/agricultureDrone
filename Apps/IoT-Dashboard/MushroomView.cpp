#include "MushroomView.h"
#include "ui_MushroomView.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>

MushroomView::MushroomView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MushroomView)
{
    ui->setupUi(this);
    loadColormap();

    int num_gauges=4;
    mRoundGaugeViews.resize(num_gauges);
    mScenes.resize(num_gauges);
    mRoundGauges.resize(num_gauges);
    // Assign Views
    mRoundGaugeViews[0] = ui->graphicsView_0;
    mRoundGaugeViews[1] = ui->graphicsView_1;
    mRoundGaugeViews[2] = ui->graphicsView_2;
    mRoundGaugeViews[3] = ui->graphicsView_3;

    QRect baseRect = ui->graphicsView_0->rect();
    mColor_OuterRing = QColor::fromRgb(160,160,160);
    double testValue = 100;
    QColor stateColor = getColorForValue(testValue);

//Prepare scenes
    for (int i = 0; i < mRoundGaugeViews.size(); ++i) {
        QGraphicsView * _gaugeViews = mRoundGaugeViews[i];
        _gaugeViews->setDragMode(QGraphicsView::NoDrag);
        _gaugeViews->setRenderHint(QPainter::Antialiasing);
        mScenes[i] = new QGraphicsScene(0,0,baseRect.width(),baseRect.height());
        mRoundGauges[i] = new RoundGaugeGraphicsObject(QRectF(17, 37, 180, 180));
    }

    setupGauge(0,GAUGE_TEMP);
    setupGauge(1,GAUGE_MOISTURE);
    setupGauge(2,GAUGE_TEMP_WATER);

    mLedToggle = new ToggleButton(10, 8);
    ui->hL_2->insertWidget(1,mLedToggle);

}

MushroomView::~MushroomView()
{
    delete ui;
}

void MushroomView::setupGauge(int viewID, GAUGE_TYPE type) {
    QGraphicsScene * tempScene = mScenes[viewID];
    RoundGaugeGraphicsObject * tempGauge = mRoundGauges[viewID];
    mRoundGaugeViews[viewID]->setScene(tempScene);
    tempScene->addItem(tempGauge);

    tempGauge->setOuterRingColor(mColor_OuterRing);
    tempGauge->setGlowRingEnabled(true);


    QColor default_color;
    switch (type) {
        case GAUGE_MOISTURE:
            default_color = QColor::fromRgb(100,160,255);
            tempScene->addText("     Humidity (%):");
            break;
        case GAUGE_TEMP_WATER:
            default_color = QColor::fromRgb(255,255,128);
            tempScene->addText("     Water Temperature (°C):");
            break;
        default:
            default_color = QColor::fromRgb(255,128,128);
            tempScene->addText("     Temperature (°C)");
    }

    tempGauge->setGlowRingColor(default_color);
    tempGauge->setFontColor(default_color);
    tempGauge->setValueColor(default_color);

    tempGauge->setRange(0, 100);
    tempGauge->setValue(50);
}

void MushroomView::loadColormap() {
    QImage pixmapJet, pixmapWinter;
    pixmapJet.load(":/Colormap/Resources/colormap_jet.png");
    pixmapWinter.load(":/Colormap/Resources/colormap_winter.png");

    double res = 100;
    double _step = (pixmapJet.width()-1)/res;
    for (int i = 0; i < res; ++i) {
        int id = i*_step;
        QColor c_Jet = pixmapJet.pixelColor(id,0).toRgb();
        QColor c_Winter = pixmapWinter.pixelColor(id,0).toRgb();
        mMapJet.push_back(c_Jet);
        mMapWinter.push_back(c_Winter);
    }
}

QColor MushroomView::getColorForValue(double value, double _min, double _max, bool useJet) {
    if(value < _min) value = _min;
    if(value > _max) value = _max;

    double band = _max - _min;
    double value_norm = (value - _min)/band;
    int scale = mMapJet.size()-1;

    QColor rs;
    if(useJet){
        rs = mMapJet[int(scale*value_norm)];
    } else{
        rs = mMapWinter[int(scale*value_norm)];
    }
    return rs;
}

void MushroomView::receiveMessage(const QByteArray &message, const QMqttTopicName &topic) {
    qDebug() << "receive" << message;
    QJsonDocument doc = QJsonDocument::fromJson(message);
    QJsonValue t = doc["t"];
    QJsonValue h = doc["h"];
    QJsonValue tW = doc["tW"];
    qDebug() << t.toDouble() << h.toDouble() << tW.toDouble();

//    for(int i = 0; i < array.size(); i++){
//        QJsonObject t_obj = array[i].toObject();
//        hostnames.push_back(t_obj["hostname"].toString());
}

void MushroomView::setTitle(QString text) {

}

void MushroomView::setValueFromJSON(QString json) {

}

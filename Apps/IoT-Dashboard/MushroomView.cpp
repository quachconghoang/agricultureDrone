#include "MushroomView.h"
#include "ui_MushroomView.h"
#include <QJsonDocument>
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
    mRoundGaugeViews[0] = ui->graphicsView_0;
    mRoundGaugeViews[1] = ui->graphicsView_1;
    mRoundGaugeViews[2] = ui->graphicsView_2;
    mRoundGaugeViews[3] = ui->graphicsView_3;

    QRect baseRect = ui->graphicsView_0->rect();
    mColor_OuterRing = QColor::fromRgb(160,160,160);
    mColor_Value = QColor::fromRgb(128,128,128);
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

    mScene = new QGraphicsScene(0,0,baseRect.width(),baseRect.height());

    ui->graphicsView_0->setScene(mScene);
    mRGauge_Temp = new RoundGaugeGraphicsObject(QRectF(17, 37, 180, 180));
    mScene->addItem(mRGauge_Temp);
    mScene->addText("     Temperature (°C)");
    mRGauge_Temp->setOuterRingColor(mColor_OuterRing);
    mRGauge_Temp->setGlowRingEnabled(true);

    mRGauge_Temp->setGlowRingColor(stateColor);
    mRGauge_Temp->setFontColor(stateColor);
    mRGauge_Temp->setValue(testValue);
    mRGauge_Temp->setValueColor(mColor_Value);
    mRGauge_Temp->setRange(0, 100);
}

MushroomView::~MushroomView()
{
    delete ui;
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

void MushroomView::setTitle(QString text) {

}

void MushroomView::setValueFromJSON(QString json) {

}

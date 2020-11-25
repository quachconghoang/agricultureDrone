#ifndef MUSHROOMVIEW_H
#define MUSHROOMVIEW_H

#include <QWidget>
#include <QMqttClient>
#include <QGraphicsView>
#include "RoundGaugeGraphicsObject.h"
#include "ToggleButton.h"

namespace Ui {
class MushroomView;
}

class MushroomView : public QWidget
{
    Q_OBJECT

public:
    explicit MushroomView(QWidget *parent = nullptr);
    ~MushroomView();

    void setTitle(QString text);
    void setValueFromJSON(QString json);

private:
    Ui::MushroomView *ui;
    QGraphicsScene* mScene;

    RoundGaugeGraphicsObject* mRGauge_Temp;
    RoundGaugeGraphicsObject* mRGauge_TempWater;
    RoundGaugeGraphicsObject* mRGauge_Moisture;

    QVector<QGraphicsView*> mRoundGaugeViews;
    QVector<RoundGaugeGraphicsObject*> mRoundGauges;
    QVector<QGraphicsScene*> mScenes;

    QColor mColor_OuterRing;
    QColor mColor_Value;

    QVector<QColor> mMapJet;
    QVector<QColor> mMapWinter;

    ToggleButton * mLedToggle;

    void loadColormap();
    QColor getColorForValue(double value, double _min=0, double _max=100, bool useJet=true);

private slots:
//    void receiveMessage(const QByteArray &message, const QMqttTopicName &topic);

};

#endif // MUSHROOMVIEW_H

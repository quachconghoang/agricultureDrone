#ifndef MUSHROOMVIEW_H
#define MUSHROOMVIEW_H

#include <QWidget>
#include <QMqttClient>
#include "RoundGaugeGraphicsObject.h"

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

private slots:
//    void receiveMessage(const QByteArray &message, const QMqttTopicName &topic);

};

#endif // MUSHROOMVIEW_H

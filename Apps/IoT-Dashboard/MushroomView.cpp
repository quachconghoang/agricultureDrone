#include "MushroomView.h"
#include "ui_MushroomView.h"
#include <QJsonDocument>

MushroomView::MushroomView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MushroomView)
{
    ui->setupUi(this);
}

MushroomView::~MushroomView()
{
    delete ui;
}

void MushroomView::setValueFromJSON(QString json) {

}

#include "MushroomView.h"
#include "ui_MushroomView.h"
#include <QJsonDocument>

MushroomView::MushroomView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MushroomView)
{
    ui->setupUi(this);
//    ui->groupBox_0->setTitle("000");
//    ui->groupBox->setTitle("sensor number x");
}

MushroomView::~MushroomView()
{
    delete ui;
}

void MushroomView::setTitle(QString text) {

}

void MushroomView::setValueFromJSON(QString json) {

}

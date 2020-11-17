#include "MushroomView.h"
#include "ui_MushroomView.h"

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

#ifndef MUSHROOMVIEW_H
#define MUSHROOMVIEW_H

#include <QWidget>

namespace Ui {
class MushroomView;
}

class MushroomView : public QWidget
{
    Q_OBJECT

public:
    explicit MushroomView(QWidget *parent = nullptr);
    ~MushroomView();

    void setValueFromJSON(QString json);

private:
    Ui::MushroomView *ui;

private slots:

};

#endif // MUSHROOMVIEW_H

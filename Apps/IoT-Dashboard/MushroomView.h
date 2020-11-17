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

private:
    Ui::MushroomView *ui;
};

#endif // MUSHROOMVIEW_H

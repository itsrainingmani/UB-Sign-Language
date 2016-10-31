#ifndef LEAPGUI_H
#define LEAPGUI_H

#include <QWidget>

namespace Ui {
class LeapGui;
}

class LeapGui : public QWidget
{
    Q_OBJECT

public:
    explicit LeapGui(QWidget *parent = 0);
    ~LeapGui();

private:
    Ui::LeapGui *ui;
};

#endif // LEAPGUI_H

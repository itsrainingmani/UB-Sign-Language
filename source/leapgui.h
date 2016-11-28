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

private slots:
    void on_upLeft_clicked();

    void on_upLeft_toggled(bool checked);

private:
    Ui::LeapGui *ui;
    void visualizer();
};

#endif // LEAPGUI_H

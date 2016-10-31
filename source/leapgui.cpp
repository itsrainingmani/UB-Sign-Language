#include "leapgui.h"
#include "ui_leapgui.h"

LeapGui::LeapGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeapGui)
{
    ui->setupUi(this);
}

LeapGui::~LeapGui()
{
    delete ui;
}

#include "leapgui.h"
#include "ui_leapgui.h"
#include<QtConcurrent/QtConcurrentRun>

void aFunction(){
    system("Visualizer");
}
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

void LeapGui::on_upLeft_clicked()
{

    extern void aFunction();
    QFuture<void> future = QtConcurrent::run(aFunction);
}



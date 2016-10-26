#include "widget.h"
#include "qwidget.h"
#include"QWindow"
#include "QWidget"
//#include<QProcess>
#include <iostream>
//#include<QString>
//#include<QStringList>
#include<QVBoxLayout>
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    //QString command==
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //QProcess *process = new QProcess(command, parameters);

    QWindow *window = QWindow::fromWinId(90177538);
    window->setFlags(Qt::FramelessWindowHint);
    QWidget *widget = QWidget::createWindowContainer(window);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(widget);
    this->setLayout(layout);
}

Widget::~Widget()
{
    delete ui;
}

#include<iostream>
#include "leapgui.h"
#include "ui_leapgui.h"
#include<QtConcurrent/QtConcurrentRun>

int flag=0;

void open(){
    system("Visualizer &");
}

void close(){
    system("pkill Visualizer");
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
//    if(flag==0){
//        open();
//        flag=1;
//    }
//    else if (flag == 1){
//        flag = 0;
//        close();
//    }
    open();
}

void LeapGui::on_upLeft_toggled(bool checked)
{
//    if(checked == true){
//        std::cout<<"Blah"<<std::endl;
//        open();
//    }
//    else{
//        std::cout<<"no"<<std::endl;
//        close();
//    }
}

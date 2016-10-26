#include "widget.h"
#include <QApplication>
#include <cstring>
#include<iostream>
#include <unistd.h>
#include <QtWidgets>
#include<thread>

void task(){
    Widget w;
    w.resize(1024, 720);
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    thread t1;
   system("Visualizer");
    t1.task();

    t1.join();

    w.show();
    return a.exec();
}


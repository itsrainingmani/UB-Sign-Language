/********************************************************************************
** Form generated from reading UI file 'leapgui.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEAPGUI_H
#define UI_LEAPGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LeapGui
{
public:
    QLabel *imgLabel;
    QLabel *charLabel;
    QPushButton *upLeft;

    void setupUi(QWidget *LeapGui)
    {
        if (LeapGui->objectName().isEmpty())
            LeapGui->setObjectName(QStringLiteral("LeapGui"));
        LeapGui->resize(604, 376);
        imgLabel = new QLabel(LeapGui);
        imgLabel->setObjectName(QStringLiteral("imgLabel"));
        imgLabel->setGeometry(QRect(10, 10, 471, 351));
        imgLabel->setFrameShape(QFrame::Box);
        charLabel = new QLabel(LeapGui);
        charLabel->setObjectName(QStringLiteral("charLabel"));
        charLabel->setGeometry(QRect(500, 10, 91, 91));
        QFont font;
        font.setFamily(QStringLiteral("Liberation Mono"));
        font.setPointSize(60);
        charLabel->setFont(font);
        charLabel->setFrameShape(QFrame::Box);
        charLabel->setFrameShadow(QFrame::Plain);
        charLabel->setAlignment(Qt::AlignCenter);
        upLeft = new QPushButton(LeapGui);
        upLeft->setObjectName(QStringLiteral("upLeft"));
        upLeft->setGeometry(QRect(480, 260, 131, 81));
        upLeft->setStyleSheet(QLatin1String("#upLeft {\n"
"background-color: transparent;\n"
"border-image: url(:unp.png);\n"
"background: none;\n"
"border: none;\n"
"background-repeat: none;\n"
"}\n"
"#upLeft:pressed\n"
"{\n"
"border-image: url(:pr.png);\n"
"}"));

        retranslateUi(LeapGui);

        QMetaObject::connectSlotsByName(LeapGui);
    } // setupUi

    void retranslateUi(QWidget *LeapGui)
    {
        LeapGui->setWindowTitle(QApplication::translate("LeapGui", "LeapGui", 0));
        imgLabel->setText(QString());
        charLabel->setText(QString());
        upLeft->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LeapGui: public Ui_LeapGui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEAPGUI_H

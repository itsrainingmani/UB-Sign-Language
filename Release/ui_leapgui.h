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
    QLabel *charLabel;
    QPushButton *upLeft;
    QLabel *imageLabel;
    QLabel *gameLabel;

    void setupUi(QWidget *LeapGui)
    {
        if (LeapGui->objectName().isEmpty())
            LeapGui->setObjectName(QStringLiteral("LeapGui"));
        LeapGui->resize(500, 398);
        charLabel = new QLabel(LeapGui);
        charLabel->setObjectName(QStringLiteral("charLabel"));
        charLabel->setGeometry(QRect(340, 50, 131, 131));
        QFont font;
        font.setFamily(QStringLiteral("Liberation Mono"));
        font.setPointSize(60);
        charLabel->setFont(font);
        charLabel->setFrameShape(QFrame::Box);
        charLabel->setFrameShadow(QFrame::Plain);
        charLabel->setAlignment(Qt::AlignCenter);
        upLeft = new QPushButton(LeapGui);
        upLeft->setObjectName(QStringLiteral("upLeft"));
        upLeft->setGeometry(QRect(340, 230, 131, 91));
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
        upLeft->setCheckable(false);
        imageLabel = new QLabel(LeapGui);
        imageLabel->setObjectName(QStringLiteral("imageLabel"));
        imageLabel->setGeometry(QRect(10, 10, 301, 241));
        imageLabel->setFrameShape(QFrame::Box);
        gameLabel = new QLabel(LeapGui);
        gameLabel->setObjectName(QStringLiteral("gameLabel"));
        gameLabel->setGeometry(QRect(10, 270, 301, 111));
        gameLabel->setFrameShape(QFrame::Box);

        retranslateUi(LeapGui);

        QMetaObject::connectSlotsByName(LeapGui);
    } // setupUi

    void retranslateUi(QWidget *LeapGui)
    {
        LeapGui->setWindowTitle(QApplication::translate("LeapGui", "LeapGui", 0));
        charLabel->setText(QString());
        upLeft->setText(QString());
        imageLabel->setText(QString());
        gameLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LeapGui: public Ui_LeapGui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEAPGUI_H

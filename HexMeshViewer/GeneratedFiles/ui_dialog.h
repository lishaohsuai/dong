/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLineEdit *xLineEdit;
    QLabel *label;
    QLabel *label_2;
    QPushButton *editButton;
    QLineEdit *yLineEdit;
    QLabel *label_3;
    QLineEdit *zLineEdit;
    QLabel *label_4;
    QLineEdit *rLineEdit;
    QPushButton *addButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(527, 120);
        xLineEdit = new QLineEdit(Dialog);
        xLineEdit->setObjectName(QString::fromUtf8("xLineEdit"));
        xLineEdit->setGeometry(QRect(50, 50, 71, 20));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 50, 21, 16));
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(130, 50, 21, 16));
        editButton = new QPushButton(Dialog);
        editButton->setObjectName(QString::fromUtf8("editButton"));
        editButton->setGeometry(QRect(250, 80, 75, 23));
        yLineEdit = new QLineEdit(Dialog);
        yLineEdit->setObjectName(QString::fromUtf8("yLineEdit"));
        yLineEdit->setGeometry(QRect(150, 50, 71, 20));
        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(230, 50, 21, 16));
        zLineEdit = new QLineEdit(Dialog);
        zLineEdit->setObjectName(QString::fromUtf8("zLineEdit"));
        zLineEdit->setGeometry(QRect(250, 50, 71, 20));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(330, 50, 21, 16));
        rLineEdit = new QLineEdit(Dialog);
        rLineEdit->setObjectName(QString::fromUtf8("rLineEdit"));
        rLineEdit->setGeometry(QRect(350, 50, 71, 20));
        addButton = new QPushButton(Dialog);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setGeometry(QRect(350, 80, 75, 23));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
        label->setText(QApplication::translate("Dialog", "X:", nullptr));
        label_2->setText(QApplication::translate("Dialog", "Y:", nullptr));
        editButton->setText(QApplication::translate("Dialog", "Edit", nullptr));
        yLineEdit->setText(QString());
        label_3->setText(QApplication::translate("Dialog", "Z:", nullptr));
        zLineEdit->setText(QString());
        label_4->setText(QApplication::translate("Dialog", "r:", nullptr));
        rLineEdit->setText(QString());
        addButton->setText(QApplication::translate("Dialog", "Add", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H

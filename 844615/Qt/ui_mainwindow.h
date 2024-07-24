/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_35;
    QPushButton *solveButton;
    QPushButton *ResetLabel;
    QGridLayout *sudokuGrid;
    QLabel *dialog;
    QPushButton *PreviousButton;
    QPushButton *NextButton;
    QPushButton *beginStep;
    QPushButton *lastStep;
    QLabel *StepLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_35 = new QGridLayout(centralwidget);
        gridLayout_35->setObjectName(QString::fromUtf8("gridLayout_35"));
        solveButton = new QPushButton(centralwidget);
        solveButton->setObjectName(QString::fromUtf8("solveButton"));

        gridLayout_35->addWidget(solveButton, 7, 1, 1, 5);

        ResetLabel = new QPushButton(centralwidget);
        ResetLabel->setObjectName(QString::fromUtf8("ResetLabel"));

        gridLayout_35->addWidget(ResetLabel, 10, 1, 1, 5);

        sudokuGrid = new QGridLayout();
        sudokuGrid->setSpacing(0);
        sudokuGrid->setObjectName(QString::fromUtf8("sudokuGrid"));
        sudokuGrid->setSizeConstraint(QLayout::SetDefaultConstraint);

        gridLayout_35->addLayout(sudokuGrid, 0, 1, 1, 5);

        dialog = new QLabel(centralwidget);
        dialog->setObjectName(QString::fromUtf8("dialog"));
        dialog->setMinimumSize(QSize(0, 20));
        dialog->setMaximumSize(QSize(16777215, 15));
        QFont font;
        font.setPointSize(8);
        font.setBold(true);
        font.setKerning(false);
        dialog->setFont(font);
        dialog->setAlignment(Qt::AlignCenter);

        gridLayout_35->addWidget(dialog, 6, 1, 1, 5);

        PreviousButton = new QPushButton(centralwidget);
        PreviousButton->setObjectName(QString::fromUtf8("PreviousButton"));

        gridLayout_35->addWidget(PreviousButton, 8, 1, 1, 1);

        NextButton = new QPushButton(centralwidget);
        NextButton->setObjectName(QString::fromUtf8("NextButton"));

        gridLayout_35->addWidget(NextButton, 8, 5, 1, 1);

        beginStep = new QPushButton(centralwidget);
        beginStep->setObjectName(QString::fromUtf8("beginStep"));

        gridLayout_35->addWidget(beginStep, 9, 1, 1, 1);

        lastStep = new QPushButton(centralwidget);
        lastStep->setObjectName(QString::fromUtf8("lastStep"));

        gridLayout_35->addWidget(lastStep, 9, 5, 1, 1);

        StepLabel = new QLabel(centralwidget);
        StepLabel->setObjectName(QString::fromUtf8("StepLabel"));
        StepLabel->setAlignment(Qt::AlignCenter);

        gridLayout_35->addWidget(StepLabel, 8, 3, 2, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        solveButton->setText(QCoreApplication::translate("MainWindow", "Solve", nullptr));
        ResetLabel->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        dialog->setText(QString());
        PreviousButton->setText(QCoreApplication::translate("MainWindow", "< Previous", nullptr));
        NextButton->setText(QCoreApplication::translate("MainWindow", "Next >", nullptr));
        beginStep->setText(QCoreApplication::translate("MainWindow", "<< Begin", nullptr));
        lastStep->setText(QCoreApplication::translate("MainWindow", "Last >>", nullptr));
        StepLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

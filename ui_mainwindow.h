/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *line_inputFile;
    QToolButton *btn_inputFile;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *line_outputFile;
    QToolButton *btn_outputFile;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *btn_isData;
    QRadioButton *btn_isMC;
    QRadioButton *btn_isMCtruth;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QDoubleSpinBox *dbl_minTOF;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_6;
    QDoubleSpinBox *dbl_maxTOF;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QDoubleSpinBox *dbl_eTOF;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_8;
    QDoubleSpinBox *dbl_ePathLength;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLineEdit *line_CDB_summary;
    QToolButton *btn_CDB_summary;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QLineEdit *line_diffuser_tracking;
    QToolButton *btn_diffuser_tracking;
    QPushButton *btn_go;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(365, 418);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        line_inputFile = new QLineEdit(centralWidget);
        line_inputFile->setObjectName(QStringLiteral("line_inputFile"));

        horizontalLayout->addWidget(line_inputFile);

        btn_inputFile = new QToolButton(centralWidget);
        btn_inputFile->setObjectName(QStringLiteral("btn_inputFile"));

        horizontalLayout->addWidget(btn_inputFile);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        line_outputFile = new QLineEdit(centralWidget);
        line_outputFile->setObjectName(QStringLiteral("line_outputFile"));

        horizontalLayout_2->addWidget(line_outputFile);

        btn_outputFile = new QToolButton(centralWidget);
        btn_outputFile->setObjectName(QStringLiteral("btn_outputFile"));

        horizontalLayout_2->addWidget(btn_outputFile);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        btn_isData = new QRadioButton(centralWidget);
        btn_isData->setObjectName(QStringLiteral("btn_isData"));
        btn_isData->setChecked(true);

        horizontalLayout_3->addWidget(btn_isData);

        btn_isMC = new QRadioButton(centralWidget);
        btn_isMC->setObjectName(QStringLiteral("btn_isMC"));

        horizontalLayout_3->addWidget(btn_isMC);

        btn_isMCtruth = new QRadioButton(centralWidget);
        btn_isMCtruth->setObjectName(QStringLiteral("btn_isMCtruth"));

        horizontalLayout_3->addWidget(btn_isMCtruth);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_7->addWidget(label_5);

        dbl_minTOF = new QDoubleSpinBox(centralWidget);
        dbl_minTOF->setObjectName(QStringLiteral("dbl_minTOF"));
        dbl_minTOF->setValue(27);

        horizontalLayout_7->addWidget(dbl_minTOF);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_8->addWidget(label_6);

        dbl_maxTOF = new QDoubleSpinBox(centralWidget);
        dbl_maxTOF->setObjectName(QStringLiteral("dbl_maxTOF"));
        dbl_maxTOF->setValue(40);

        horizontalLayout_8->addWidget(dbl_maxTOF);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_9->addWidget(label_7);

        dbl_eTOF = new QDoubleSpinBox(centralWidget);
        dbl_eTOF->setObjectName(QStringLiteral("dbl_eTOF"));
        dbl_eTOF->setValue(25.48);

        horizontalLayout_9->addWidget(dbl_eTOF);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_10->addWidget(label_8);

        dbl_ePathLength = new QDoubleSpinBox(centralWidget);
        dbl_ePathLength->setObjectName(QStringLiteral("dbl_ePathLength"));
        dbl_ePathLength->setValue(11);

        horizontalLayout_10->addWidget(dbl_ePathLength);


        verticalLayout->addLayout(horizontalLayout_10);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_6->addWidget(label_3);

        line_CDB_summary = new QLineEdit(centralWidget);
        line_CDB_summary->setObjectName(QStringLiteral("line_CDB_summary"));

        horizontalLayout_6->addWidget(line_CDB_summary);

        btn_CDB_summary = new QToolButton(centralWidget);
        btn_CDB_summary->setObjectName(QStringLiteral("btn_CDB_summary"));

        horizontalLayout_6->addWidget(btn_CDB_summary);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_5->addWidget(label_4);

        line_diffuser_tracking = new QLineEdit(centralWidget);
        line_diffuser_tracking->setObjectName(QStringLiteral("line_diffuser_tracking"));

        horizontalLayout_5->addWidget(line_diffuser_tracking);

        btn_diffuser_tracking = new QToolButton(centralWidget);
        btn_diffuser_tracking->setObjectName(QStringLiteral("btn_diffuser_tracking"));

        horizontalLayout_5->addWidget(btn_diffuser_tracking);


        verticalLayout_2->addLayout(horizontalLayout_5);

        btn_go = new QPushButton(centralWidget);
        btn_go->setObjectName(QStringLiteral("btn_go"));

        verticalLayout_2->addWidget(btn_go);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 365, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label->setText(QApplication::translate("MainWindow", "Input:", 0));
        btn_inputFile->setText(QApplication::translate("MainWindow", "...", 0));
        label_2->setText(QApplication::translate("MainWindow", "Output:", 0));
        btn_outputFile->setText(QApplication::translate("MainWindow", "...", 0));
#ifndef QT_NO_TOOLTIP
        btn_isData->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Select if input is data. </p><p><br/></p><p>(Changes TOF0-1 position calibration and Rogers tracking file)</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btn_isData->setText(QApplication::translate("MainWindow", "Data", 0));
#ifndef QT_NO_TOOLTIP
        btn_isMC->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Select if input is MC. </p><p><br/></p><p>(Changes TOF0-1 position calibration and Rogers tracking file)</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btn_isMC->setText(QApplication::translate("MainWindow", "MC (recon)", 0));
        btn_isMCtruth->setText(QApplication::translate("MainWindow", "MC (truth)", 0));
        label_5->setText(QApplication::translate("MainWindow", "Min TOF cut:", 0));
        dbl_minTOF->setSuffix(QApplication::translate("MainWindow", " ns", 0));
        label_6->setText(QApplication::translate("MainWindow", "Max TOF cut:", 0));
        dbl_maxTOF->setSuffix(QApplication::translate("MainWindow", " ns", 0));
#ifndef QT_NO_TOOLTIP
        label_7->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Note: Electron time-of-flight between TOF0 and TOF1 may be different between data and MC</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_7->setText(QApplication::translate("MainWindow", "e TOF:", 0));
#ifndef QT_NO_TOOLTIP
        dbl_eTOF->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Note: Electron time-of-flight between TOF0 and TOF1 may be different between data and MC</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        dbl_eTOF->setSuffix(QApplication::translate("MainWindow", " ns", 0));
#ifndef QT_NO_TOOLTIP
        label_8->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Electron path length from simulation -- depends on beam momentum and conventional magnet settings. Examples are:</p><p>* (6, 140) mu+, 10.9 mm<br/>* (6, 200) mu+, 11.0 mm<br/>* (6, 240) mu+, 12.7 mm</p><p>* (6, 140) mu-, 13.9 mm<br/>* (6, 200) mu-, 13.8 mm<br/>* (6, 240) mu-, 14.6 mm</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_8->setText(QApplication::translate("MainWindow", "Simulated e path length:", 0));
#ifndef QT_NO_TOOLTIP
        dbl_ePathLength->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Electron path length from simulation -- depends on beam momentum and conventional magnet settings. Examples are:</p><p>* (6, 140) mu+, 10.9 mm<br/>* (6, 200) mu+, 11.0 mm<br/>* (6, 240) mu+, 12.7 mm</p><p>* (6, 140) mu-, 13.9 mm<br/>* (6, 200) mu-, 13.8 mm<br/>* (6, 240) mu-, 14.6 mm</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        dbl_ePathLength->setSuffix(QApplication::translate("MainWindow", " mm", 0));
#ifndef QT_NO_TOOLTIP
        label_3->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>If blank, a default geometry file will be used from run 7469.</p><p>This file comes with reconstructed data/MC in the geo-##### folder, where #### is the run number.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_3->setText(QApplication::translate("MainWindow", "Geometry File:", 0));
#ifndef QT_NO_TOOLTIP
        line_CDB_summary->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>If blank, a default geometry file will be used from run 7469.</p><p>This file comes with reconstructed data/MC in the geo-##### folder, where #### is the run number.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btn_CDB_summary->setText(QApplication::translate("MainWindow", "...", 0));
#ifndef QT_NO_TOOLTIP
        label_4->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>If blank, no tracking file will be used. Tracking variables will not appear in the extracted ROOT file.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("MainWindow", "Diffuser tracking file:", 0));
#ifndef QT_NO_TOOLTIP
        line_diffuser_tracking->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>If blank, no tracking file will be used. Tracking variables will not appear in the extracted ROOT file.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btn_diffuser_tracking->setText(QApplication::translate("MainWindow", "...", 0));
        btn_go->setText(QApplication::translate("MainWindow", "Read MAUS file and save as .root", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "readmaus.h"
#include "betterreadmaus.h"
#include <QFileDialog>
#include <QString>
#include <QHash>
#include <QHashIterator>
#include <TMath.h>
#include <TMatrixD.h>
#include <QVector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void choose_open_file();
    void choose_save_file();
    void choose_CDB_file();
    void choose_diffuser_file();
    void getData();

private:
    Ui::MainWindow *ui;
  //  ReadMAUS* read_data;
    BetterReadMAUS* better_read_data;

    void setup_ui();
    QVector<double> read_CDB_currents();

    QString inputFilename;
    QString outputFilename;
};

#endif // MAINWINDOW_H

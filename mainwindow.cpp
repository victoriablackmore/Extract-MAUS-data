#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setup_ui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup_ui(){
    connect(ui->btn_inputFile, SIGNAL(clicked()), SLOT(choose_open_file()));
    connect(ui->btn_outputFile, SIGNAL(clicked()), SLOT(choose_save_file()));
    connect(ui->btn_go, SIGNAL(clicked()), SLOT(getData()));
    //read_data = new ReadMAUS();
    better_read_data = new BetterReadMAUS();
}


void MainWindow::choose_open_file(){
    QStringList filenames;
    QFileDialog dialog(this);
    dialog.setDirectory(ui->line_inputFile->text());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilter(tr("ROOT Files (*.root)"));
    if(dialog.exec()){
        filenames = dialog.selectedFiles();
    }

    if(!filenames.empty()){
        ui->line_inputFile->setText(filenames.first());
        inputFilename = filenames.first();
        //getData();
    }
}

void MainWindow::choose_save_file(){
    QStringList filenames;
    QFileDialog dialog(this);
    dialog.setDirectory(ui->line_outputFile->text());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilter(tr("ROOT Files (*.root)"));
    if(dialog.exec()){
        filenames = dialog.selectedFiles();
    }

    if(!filenames.empty()){
        ui->line_outputFile->setText(filenames.first());
        outputFilename = filenames.first();
    }
}

void MainWindow::getData(){
    //read_data->Read(inputFilename, outputFilename);

    // need to replace this with dialog box...but it'll do for testing.
    QString calibrationFileName = "run7417_calibration_file.dat";

    double min_tof = 27.0;
    double max_tof = 40.0;
    double sim_ele_path = 11.0;
    double data_ele_tof = 25.48; // 25.48ns for data, 25.68ns for reconstructed MC
    double q7_current = 138.70;
    double q8_current = 209.80;
    double q9_current = 179.20;
    double q7_zPosition = 9499.00;
    double q8_zPosition = 10659.00;
    double q9_zPosition = 11819.00;
    double tof0_zPosition = 5287.66;
    double tof1_zPosition = 12896.60;

    better_read_data->SetBeamlineParameters(min_tof, max_tof, sim_ele_path, data_ele_tof, q7_current, q8_current, q9_current, q7_zPosition, q8_zPosition, q9_zPosition, tof0_zPosition, tof1_zPosition);
    better_read_data->Read(inputFilename, outputFilename, calibrationFileName);
}


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
    connect(ui->btn_CDB_summary, SIGNAL(clicked()), SLOT(choose_CDB_file()));
    connect(ui->btn_diffuser_tracking, SIGNAL(clicked()), SLOT(choose_diffuser_file()));
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

void MainWindow::choose_CDB_file(){
    QStringList filenames;
    QFileDialog dialog(this);
    dialog.setDirectory(ui->line_CDB_summary->text());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    //dialog.setNameFilter(tr("ROOT Files (*.root)"));
    if(dialog.exec()){
        filenames = dialog.selectedFiles();
    }

    if(!filenames.empty()){
        ui->line_CDB_summary->setText(filenames.first());
    }
}

void MainWindow::choose_diffuser_file(){
    QStringList filenames;
    QFileDialog dialog(this);
    dialog.setDirectory(ui->line_diffuser_tracking->text());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    //dialog.setNameFilter(tr("ROOT Files (*.root)"));
    if(dialog.exec()){
        filenames = dialog.selectedFiles();
    }

    if(!filenames.empty()){
        ui->line_diffuser_tracking->setText(filenames.first());
    }
}

void MainWindow::getData(){
    double min_tof = 27.0;
    double max_tof = 40.0;
    double sim_ele_path = 11.0;

    QString calibrationFileName;
    QString rogersTrackingFileName;
    double data_ele_tof;
    if(ui->btn_isData->isChecked()){
        data_ele_tof = 25.48; // 25.48ns for data, 25.68ns for reconstructed MC
        calibrationFileName = "run7417_calibration_file_DATA.dat";

        if(ui->line_diffuser_tracking->text().isEmpty()){
            rogersTrackingFileName = "tracker-at-tof1_geometry-160_M2-5-0_v2.csv";
        }
        else{
            rogersTrackingFileName = ui->line_diffuser_tracking->text();
        }
    }
    else{
        data_ele_tof = 25.68; // 25.48ns for data, 25.68ns for reconstructed MC
        calibrationFileName = "run7417_calibration_file_MC.dat";

        if(ui->line_diffuser_tracking->text().isEmpty()){
            // update this when we have a default MC file to use
            //rogersTrackingFileName = "tracker-at-tof1_geometry-160_M2-5-0_v2.csv";
            rogersTrackingFileName = "test_diffuser_tracking.txt";
        }
        else{
            rogersTrackingFileName = ui->line_diffuser_tracking->text();
        }
    }

    QVector<double> magnet_currents = read_CDB_currents();
    double q7_current = magnet_currents.at(0);
    double q8_current = magnet_currents.at(1);
    double q9_current = magnet_currents.at(2);
    //double q7_current = 138.70;
    //double q8_current = 209.80;
    //double q9_current = 179.20;
    double q7_zPosition = 9499.00;
    double q8_zPosition = 10659.00;
    double q9_zPosition = 11819.00;
    double tof0_zPosition = 5287.66;
    double tof1_zPosition = 12896.60;

    std::cout << "Using Q7 = " << q7_current << ", Q8 = " << q8_current << ", Q9 = " << q9_current << "\n";

    better_read_data->SetBeamlineParameters(min_tof, max_tof, sim_ele_path, data_ele_tof, q7_current, q8_current, q9_current, q7_zPosition, q8_zPosition, q9_zPosition, tof0_zPosition, tof1_zPosition);

    better_read_data->Read(inputFilename, outputFilename, calibrationFileName, rogersTrackingFileName);
}


QVector<double> MainWindow::read_CDB_currents(){
    QString CDB_file;
    if(ui->line_CDB_summary->text().isEmpty()){
        CDB_file = "run7469_parent_geometry_file.txt";
    }
    else{
        CDB_file = ui->line_CDB_summary->text();
    }


    QFile file(CDB_file);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::cerr << "Failed to read tracking file.\n";
        QVector<double> fail(3, 0.0);
        return fail;
    }

    QTextStream in(&file);
    double q7, q8, q9;
    while(!in.atEnd()){
        QString line = in.readLine();

        if(line.contains("Substitution $Q7Current")){
            QStringList list = line.split(" ", QString::SkipEmptyParts);

            //std::cout << "line = " << line.toStdString() << "\n";
            q7 = list.at(2).toDouble();
        }
        else if(line.contains("Substitution $Q8Current")){
            QStringList list = line.split(" ", QString::SkipEmptyParts);
            q8 = list.at(2).toDouble();
        }
        else if(line.contains("Substitution $Q9Current")){
            QStringList list = line.split(" ", QString::SkipEmptyParts);
            q9 = list.at(2).toDouble();
        }
    }

    QVector<double> magnet_currents;
    magnet_currents << q7 << q8 << q9;

    return magnet_currents;
}

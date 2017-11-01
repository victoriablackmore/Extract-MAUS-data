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
   // connect(ui->btn_diffuser_tracking, SIGNAL(clicked()), SLOT(choose_diffuser_file()));
    connect(ui->btn_isData, SIGNAL(clicked()), SLOT(update_TOF()));
    connect(ui->btn_isMC, SIGNAL(clicked()), SLOT(update_TOF()));
    connect(ui->btn_raynerCalibrationFile, SIGNAL(clicked()), SLOT(choose_rayner_calibration_file()));

    better_read_data = new BetterReadMAUS();
}

void MainWindow::update_TOF(){
    if(ui->btn_isData->isChecked()){
        // reset to data value
        ui->dbl_eTOF->setValue(25.48);
    }
    else{
        ui->dbl_eTOF->setValue(25.68);
    }
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
    if(dialog.exec()){
        filenames = dialog.selectedFiles();
    }

    if(!filenames.empty()){
        ui->line_CDB_summary->setText(filenames.first());
    }
}

void MainWindow::choose_rayner_calibration_file(){
    QStringList filenames;
    QFileDialog dialog(this);
    dialog.setDirectory(ui->line_raynerCalibrationFile->text());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec()){
        filenames = dialog.selectedFiles();
    }

    if(!filenames.empty()){
        ui->line_raynerCalibrationFile->setText(filenames.first());
    }
}

/*
void MainWindow::choose_diffuser_file(){
    QStringList filenames;
    QFileDialog dialog(this);
    dialog.setDirectory(ui->line_diffuser_tracking->text());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec()){
        filenames = dialog.selectedFiles();
    }

    if(!filenames.empty()){
        ui->line_diffuser_tracking->setText(filenames.first());
    }
}
*/

void MainWindow::getData(){
    double min_tof = ui->dbl_minTOF->value();
    double max_tof = ui->dbl_maxTOF->value();
    double sim_ele_path = ui->dbl_ePathLength->value();
    double data_ele_tof = ui->dbl_eTOF->value();

    QString calibrationFileName;
    if(ui->line_raynerCalibrationFile->text().isEmpty()){
        if(ui->btn_isData->isChecked()){
            calibrationFileName = "run7417_calibration_file_DATA.dat";
        }
        else{
            calibrationFileName = "run7417_calibration_file_MC.dat";
        }
    }
    else{
        calibrationFileName = ui->line_raynerCalibrationFile->text();
    }

 //   QString rogersTrackingFileName  = ui->line_diffuser_tracking->text();

    QVector<double> magnet_currents = read_CDB_currents();
    double q7_current = magnet_currents.at(0);
    double q8_current = magnet_currents.at(1);
    double q9_current = magnet_currents.at(2);

    QVector<double> magnet_and_TOF_positions = read_CDB_positions();
    double q7_zPosition = magnet_and_TOF_positions.at(0);
    double q8_zPosition = magnet_and_TOF_positions.at(1);
    double q9_zPosition = magnet_and_TOF_positions.at(2);
    double tof0_zPosition = magnet_and_TOF_positions.at(3);
    double tof1_zPosition = magnet_and_TOF_positions.at(4);

    std::cout << "Using Q7 = " << q7_current << ", Q8 = " << q8_current << ", Q9 = " << q9_current << "\n";

    if(ui->btn_isMCtruth->isChecked()){
        better_read_data->SetDataType(false, false, true);
        better_read_data->ReadMC(inputFilename, outputFilename);
    }
    else{
        if(ui->btn_isData->isChecked()){
            better_read_data->SetDataType(true, false, false);
        }
        else{
            better_read_data->SetDataType(false, true, false);
        }

        better_read_data->SetBeamlineParameters(min_tof, max_tof, sim_ele_path, data_ele_tof, q7_current, q8_current, q9_current, q7_zPosition, q8_zPosition, q9_zPosition, tof0_zPosition, tof1_zPosition);

        better_read_data->Read(inputFilename, outputFilename, calibrationFileName); //, rogersTrackingFileName);
    }
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
        std::cerr << "Failed to read geometry file.\n";
        QVector<double> fail(3, 0.0);
        return fail;
    }

    QTextStream in(&file);
    double q7, q8, q9;
    while(!in.atEnd()){
        QString line = in.readLine();

        if(line.contains("Substitution $Q7Current")){
            QStringList list = line.split(" ", QString::SkipEmptyParts);

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

    file.flush();
    file.close();
    return magnet_currents;
}

QVector<double> MainWindow::read_CDB_positions(){
    QString CDB_file;
    if(ui->line_CDB_summary->text().isEmpty()){
        CDB_file = "run7469_parent_geometry_file.txt";
    }
    else{
        CDB_file = ui->line_CDB_summary->text();
    }


    QFile file(CDB_file);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::cerr << "Failed to read geometry file.\n";
        QVector<double> fail(3, 0.0);
        return fail;
    }

    QTextStream in(&file);
    double z_q7, z_q8, z_q9, z_TOF0, z_TOF1;
    double z_correction = 999.592;  // mm, correct for different starting definitions
                                    // we need the centre, the geometry gives the
                                    // start of the field
    while(!in.atEnd()){
        QString line = in.readLine();

        if(line.contains("TOF0.dat")){
            line = in.readLine();
            line = in.readLine();
            if(line.contains("Position")){
                QStringList list = line.split(" ", QString::SkipEmptyParts);
                z_TOF0 = list.at(3).toDouble();
                std::cout << "TOF0 is at " << z_TOF0 << "\n";
            }
        }

        if(line.contains("TOF1.dat")){
            line = in.readLine();
            line = in.readLine();
            if(line.contains("Position")){
                QStringList list = line.split(" ", QString::SkipEmptyParts);
                z_TOF1 = list.at(3).toDouble();
                std::cout << "TOF1 is at " << z_TOF1 << "\n";
            }
        }

        if(line.contains("Module Q7")){
            line = in.readLine();
            line = in.readLine();
            line = in.readLine();
            if(line.contains("Position")){
                QStringList list = line.split(" ", QString::SkipEmptyParts);
                z_q7 = list.at(3).toDouble() + z_correction;
                std::cout << "Q7 is at " << z_q7 << "\n";
            }
        }

        if(line.contains("Module Q8")){
            line = in.readLine();
            line = in.readLine();
            line = in.readLine();
            if(line.contains("Position")){
                QStringList list = line.split(" ", QString::SkipEmptyParts);
                z_q8 = list.at(3).toDouble() + z_correction;
                std::cout << "Q8 is at " << z_q8 << "\n";
            }
        }

        if(line.contains("Module Q9")){
            line = in.readLine();
            line = in.readLine();
            line = in.readLine();
            if(line.contains("Position")){
                QStringList list = line.split(" ", QString::SkipEmptyParts);
                z_q9 = list.at(3).toDouble() + z_correction;
                std::cout << "Q9 is at " << z_q9 << "\n";
            }
        }
    }

    QVector<double> magnet_currents;
    magnet_currents << z_q7 << z_q8 << z_q9 << z_TOF0 << z_TOF1;

    file.flush();
    file.close();
    return magnet_currents;
}

#include "betterreadmaus.h"

#include "JsonCppStreamer/IRStream.hh"


BetterReadMAUS::BetterReadMAUS()
{
    tof0_global_x_vs_slab_number = new TH2F("tof0_global_x_vs_slab_number", "TOF0;global x; vertical slab number", 10, -220.0, 220.0, 10, -0.5, 9.5);
    tof0_global_y_vs_slab_number = new TH2F("tof0_global_y_vs_slab_number", "TOF0;global y; horizontal slab number", 10, -220.0, 220.0, 10, -0.5, 9.5);

    tof1_global_x_vs_slab_number = new TH2F("tof1_global_x_vs_slab_number", "TOF1;global x; vertical slab number", 7, -220.0, 220.0, 7, -0.5, 6.5);
    tof1_global_y_vs_slab_number = new TH2F("tof1_global_y_vs_slab_number", "TOF1;global y; horizontal slab number", 7, -220.0, 220.0, 7, -0.5, 6.5);

    tof0_x_residual = new TH1F("tof0_x_residual", "TOF0; x recon - x pixel", 100, -220.0, 220.0);
    tof1_x_residual = new TH1F("tof1_x_residual", "TOF1; x recon - x pixel", 100, -220.0, 220.0);
    tof0_y_residual = new TH1F("tof0_y_residual", "TOF0; y recon - y pixel", 100, -220.0, 220.0);
    tof1_y_residual = new TH1F("tof1_y_residual", "TOF1; y recon - y pixel", 100, -220.0, 220.0);

}


void BetterReadMAUS::SetDataType(bool thisIsData, bool thisIsMCRecon, bool thisIsMCTruth){
    // This function gets used to determine which PMT is read as being on
    // the top/bottom at TOF0 and TOF1 -- in MC recon, PMT0 is at +y, PMT1 is at -y
    // in data, PMT1 is at +y, PMT0 is at -y
    //
    // Rather than changing the geometry and causing lots of confusion about *when* a
    // PMT was at +/- y in a MC, we're just going to check to see which way around we
    // need to read things.
    //
    // Check the plots in qualityCheck.pdf that this programme produces. If the y
    // residuals look extremely broad (i.e. going well off the horizontal scale)
    // then something has happened with the PMT positions.

    isData = thisIsData;
    isMCRecon = thisIsMCRecon;
    isMCTruth = thisIsMCTruth;  // this won't do anything, but it makes sense for there to
                                // be the option in case it's needed in the future
}

void BetterReadMAUS::set_cut_values(){
   
    min_tof = 27.0;
    max_tof = 32.0;
    
    min_normalised_tof = 1.0;
    max_normalised_tof = 6.0;
    
    max_chindof = 4.0; //2.5;
    max_tku_radius = 150.0;
    min_tku_radius = 0.0;
    max_diffuser = 90.0;
    
    // for tku P vs tof selection:
    mean_dP = 20.0; // difference in total momentum at u/s side of TOF1 and TKU station 1
    min_dP = mean_dP - 22.0;
    max_dP = mean_dP + 22.0;
}


void BetterReadMAUS::print_tof_plots(){
    std::string saveAs = "qualityCheck.pdf(";

    TCanvas canvas;
    tof0_x_residual->Draw("hist");
    canvas.Print(saveAs.c_str());

    tof0_y_residual->Draw("hist");
    canvas.Print(saveAs.c_str());

    tof1_x_residual->Draw("hist");
    canvas.Print(saveAs.c_str());

    tof1_y_residual->Draw("hist");
    canvas.Print(saveAs.c_str());

    tof0_global_x_vs_slab_number->Draw("colz");
    canvas.Print(saveAs.c_str());

    tof0_global_y_vs_slab_number->Draw("colz");
    canvas.Print(saveAs.c_str());

    tof1_global_x_vs_slab_number->Draw("colz");
    canvas.Print(saveAs.c_str());

    tof1_global_y_vs_slab_number->Draw("colz");

    saveAs = "qualityCheck.pdf)";
    canvas.Print(saveAs.c_str());
}




void BetterReadMAUS::reset_particle_variables(){
    // reset's particle variables
    
    reset_TOF0_variables();
    reset_TOF1_variables();
    reset_TKU_variables();
    reset_diffuser_variables();
    
    reconstructed_event_number = -1;
    spill_number = -1;
    
    cut_tof = 0;
    cut_normalised_tof = 0;
    cut_diffuser = 0;
    cut_chindof = 0;
    cut_tku_r = 0;
    cut_one_tof0 = 0;
    cut_one_tof1 = 0;
    cut_one_track = 0;
    cut_tof_p = 0;
    cut_allPassed = 0;
    cut_allButDiffuserPassed = 0;
    
    
    number_of_TOF0_spacepoints = 0;
    number_of_TOF1_spacepoints = 0;
    number_of_TKU_tracks = 0;
    
    time_of_flight = TMath::Infinity();
    normalised_time_of_flight = TMath::Infinity();
}

void BetterReadMAUS::reset_TOF0_variables(){
    TOF0_xPixel = TMath::Infinity();
    TOF0_yPixel = TMath::Infinity();
    TOF0_x = TMath::Infinity();
    TOF0_y = TMath::Infinity();
    TOF0_z = TMath::Infinity();
    TOF0_horizontal_slab = -1;
    TOF0_vertical_slab = -1;
    TOF0_horizontal_slab_tMinus = TMath::Infinity();
    TOF0_horizontal_slab_tPlus = TMath::Infinity();
    TOF0_vertical_slab_tMinus = TMath::Infinity();
    TOF0_vertical_slab_tPlus = TMath::Infinity();
    TOF0_hitTime = TMath::Infinity();
    TOF0_px = TMath::Infinity();
    TOF0_py = TMath::Infinity();
    TOF0_pz = TMath::Infinity();
    TOF0_p = TMath::Infinity();
}

void BetterReadMAUS::reset_TOF1_variables(){
    TOF1_xPixel = TMath::Infinity();
    TOF1_yPixel = TMath::Infinity();
    TOF1_x = TMath::Infinity();
    TOF1_y = TMath::Infinity();
    TOF1_z = TMath::Infinity();
    TOF1_horizontal_slab = -1;
    TOF1_vertical_slab = -1;
    TOF1_horizontal_slab_tMinus = TMath::Infinity();
    TOF1_horizontal_slab_tPlus = TMath::Infinity();
    TOF1_vertical_slab_tMinus = TMath::Infinity();
    TOF1_vertical_slab_tPlus = TMath::Infinity();
    TOF1_hitTime = TMath::Infinity();
    
    TOF1_px = TMath::Infinity();
    TOF1_py = TMath::Infinity();
    TOF1_pz = TMath::Infinity();
    TOF1_p = TMath::Infinity();
}

void BetterReadMAUS::reset_TKU_variables(){
    TKU_assumed_field = TMath::Infinity();
    TKU_plane1_x = TMath::Infinity();
    TKU_plane1_y = TMath::Infinity();
    TKU_plane1_z = TMath::Infinity();
    TKU_plane1_px = TMath::Infinity();
    TKU_plane1_py = TMath::Infinity();
    TKU_plane1_pz = TMath::Infinity();
    TKU_plane1_p = TMath::Infinity();
    TKU_plane1_r = TMath::Infinity();
    TKU_plane1_pt = TMath::Infinity();
    
    TKU_plane1_x_error = TMath::Infinity();
    TKU_plane1_y_error = TMath::Infinity();
    TKU_plane1_px_error = TMath::Infinity();
    TKU_plane1_py_error = TMath::Infinity();
    TKU_plane1_kappa_error = TMath::Infinity();
    
    TKU_plane2_x = TMath::Infinity();
    TKU_plane2_y = TMath::Infinity();
    TKU_plane2_z = TMath::Infinity();
    TKU_plane2_px = TMath::Infinity();
    TKU_plane2_py = TMath::Infinity();
    TKU_plane2_pz = TMath::Infinity();
    TKU_plane2_p = TMath::Infinity();
    TKU_plane2_r = TMath::Infinity();
    TKU_plane2_pt = TMath::Infinity();
    
    TKU_plane2_x_error = TMath::Infinity();
    TKU_plane2_y_error = TMath::Infinity();
    TKU_plane2_px_error = TMath::Infinity();
    TKU_plane2_py_error = TMath::Infinity();
    TKU_plane2_kappa_error = TMath::Infinity();
    
    TKU_plane3_x = TMath::Infinity();
    TKU_plane3_y = TMath::Infinity();
    TKU_plane3_z = TMath::Infinity();
    TKU_plane3_px = TMath::Infinity();
    TKU_plane3_py = TMath::Infinity();
    TKU_plane3_pz = TMath::Infinity();
    TKU_plane3_p = TMath::Infinity();
    TKU_plane3_r = TMath::Infinity();
    TKU_plane3_pt = TMath::Infinity();
    
    TKU_plane3_x_error = TMath::Infinity();
    TKU_plane3_y_error = TMath::Infinity();
    TKU_plane3_px_error = TMath::Infinity();
    TKU_plane3_py_error = TMath::Infinity();
    TKU_plane3_kappa_error = TMath::Infinity();
    
    TKU_plane4_x = TMath::Infinity();
    TKU_plane4_y = TMath::Infinity();
    TKU_plane4_z = TMath::Infinity();
    TKU_plane4_px = TMath::Infinity();
    TKU_plane4_py = TMath::Infinity();
    TKU_plane4_pz = TMath::Infinity();
    TKU_plane4_p = TMath::Infinity();
    TKU_plane4_r = TMath::Infinity();
    TKU_plane4_pt = TMath::Infinity();
    
    TKU_plane4_x_error = TMath::Infinity();
    TKU_plane4_y_error = TMath::Infinity();
    TKU_plane4_px_error = TMath::Infinity();
    TKU_plane4_py_error = TMath::Infinity();
    TKU_plane4_kappa_error = TMath::Infinity();
    
    TKU_plane5_x = TMath::Infinity();
    TKU_plane5_y = TMath::Infinity();
    TKU_plane5_z = TMath::Infinity();
    TKU_plane5_px = TMath::Infinity();
    TKU_plane5_py = TMath::Infinity();
    TKU_plane5_pz = TMath::Infinity();
    TKU_plane5_p = TMath::Infinity();
    TKU_plane5_r = TMath::Infinity();
    TKU_plane5_pt = TMath::Infinity();
    
    TKU_plane5_x_error = TMath::Infinity();
    TKU_plane5_y_error = TMath::Infinity();
    TKU_plane5_px_error = TMath::Infinity();
    TKU_plane5_py_error = TMath::Infinity();
    TKU_plane5_kappa_error = TMath::Infinity();
    
    TKU_chiSquare = TMath::Infinity();
    TKU_ndof = -1;
    TKU_patternRecognition_dipAngle = TMath::Infinity();
    TKU_patternRecognition_R = TMath::Infinity();
    TKU_patternRecognition_x0 = TMath::Infinity();
    TKU_patternRecognition_y0 = TMath::Infinity();
    
    TKU_plane1_pull = TMath::Infinity();
    TKU_plane2_pull = TMath::Infinity();
    TKU_plane3_pull = TMath::Infinity();
    TKU_plane4_pull = TMath::Infinity();
    TKU_plane5_pull = TMath::Infinity();
    
    TKU_charge = -5;
    TKU_assumed_field = TMath::Infinity();
}

void BetterReadMAUS::readCalibrationFile(QString calibrationFile){
    reading_TOF0 = false;
    reading_TOF1 = false;
    reading_horizontal = false;
    reading_vertical = false;
    
    TOF0_horizontal_calibration.clear();
    TOF0_vertical_calibration.clear();
    TOF1_horizontal_calibration.clear();
    TOF1_vertical_calibration.clear();
    
    QFile file(calibrationFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::cerr << "Failed to read calibration file.\n";
        return;
    }
    
    QTextStream in(&file);
    while(!in.atEnd()){
        QString line = in.readLine();
        process_line(line);
    }
    
    check_calibration();
}

void BetterReadMAUS::process_line(QString line){
    if(line.contains("TOF0")){
        reading_TOF0 = true;
        reading_TOF1 = false;
    }
    else if(line.contains("TOF1")){
        reading_TOF0 = false;
        reading_TOF1 = true;
    }
    else if(line.contains("Horizontal")){
        reading_horizontal = true;
        reading_vertical = false;
    }
    else if(line.contains("Vertical")){
        reading_horizontal = false;
        reading_vertical = true;
    }
    else{
        // the line gives us some calibration info...
        QStringList list = line.split("\t");
        double delta = list.at(2).toDouble();
        if(reading_TOF0 && reading_horizontal && !reading_TOF1 && !reading_vertical){
            TOF0_horizontal_calibration << delta;
            calibrated_c_eff = list.at(1).toDouble();
        }
        else if(reading_TOF0 && reading_vertical && !reading_TOF1 && !reading_horizontal){
            TOF0_vertical_calibration << delta;
        }
        else if(reading_TOF1 && reading_horizontal && !reading_TOF0 && !reading_vertical){
            TOF1_horizontal_calibration << delta;
        }
        else if(reading_TOF1 && reading_vertical && !reading_TOF0 && !reading_horizontal){
            TOF1_vertical_calibration << delta;
        }
    }
}

void BetterReadMAUS::check_calibration(){
    // print out the calibration constants
    
    std::cerr << "TOF0_horizontal_calibration holds: (slab, delta)\n"
    << "(0, " << TOF0_horizontal_calibration.at(0) << ")\n"
    << "(1, " << TOF0_horizontal_calibration.at(1) << ")\n"
    << "(2, " << TOF0_horizontal_calibration.at(2) << ")\n"
    << "(3, " << TOF0_horizontal_calibration.at(3) << ")\n"
    << "(4, " << TOF0_horizontal_calibration.at(4) << ")\n"
    << "(5, " << TOF0_horizontal_calibration.at(5) << ")\n"
    << "(6, " << TOF0_horizontal_calibration.at(6) << ")\n"
    << "(7, " << TOF0_horizontal_calibration.at(7) << ")\n"
    << "(8, " << TOF0_horizontal_calibration.at(8) << ")\n"
    << "(9, " << TOF0_horizontal_calibration.at(9) << ")\n";
    
    std::cerr << "TOF0_vertical_calibration holds: (slab, delta)\n"
    << "(0, " << TOF0_vertical_calibration.at(0) << ")\n"
    << "(1, " << TOF0_vertical_calibration.at(1) << ")\n"
    << "(2, " << TOF0_vertical_calibration.at(2) << ")\n"
    << "(3, " << TOF0_vertical_calibration.at(3) << ")\n"
    << "(4, " << TOF0_vertical_calibration.at(4) << ")\n"
    << "(5, " << TOF0_vertical_calibration.at(5) << ")\n"
    << "(6, " << TOF0_vertical_calibration.at(6) << ")\n"
    << "(7, " << TOF0_vertical_calibration.at(7) << ")\n"
    << "(8, " << TOF0_vertical_calibration.at(8) << ")\n"
    << "(9, " << TOF0_vertical_calibration.at(9) << ")\n";
    
    std::cerr << "TOF1_horizontal_calibration holds: (slab, delta)\n"
    << "(0, " << TOF1_horizontal_calibration.at(0) << ")\n"
    << "(1, " << TOF1_horizontal_calibration.at(1) << ")\n"
    << "(2, " << TOF1_horizontal_calibration.at(2) << ")\n"
    << "(3, " << TOF1_horizontal_calibration.at(3) << ")\n"
    << "(4, " << TOF1_horizontal_calibration.at(4) << ")\n"
    << "(5, " << TOF1_horizontal_calibration.at(5) << ")\n"
    << "(6, " << TOF1_horizontal_calibration.at(6) << ")\n";
    
    std::cerr << "TOF1_vertical_calibration holds: (slab, delta)\n"
    << "(0, " << TOF1_vertical_calibration.at(0) << ")\n"
    << "(1, " << TOF1_vertical_calibration.at(1) << ")\n"
    << "(2, " << TOF1_vertical_calibration.at(2) << ")\n"
    << "(3, " << TOF1_vertical_calibration.at(3) << ")\n"
    << "(4, " << TOF1_vertical_calibration.at(4) << ")\n"
    << "(5, " << TOF1_vertical_calibration.at(5) << ")\n"
    << "(6, " << TOF1_vertical_calibration.at(6) << ")\n";
}

void BetterReadMAUS::apply_calibration_TOF0(){
    double horizontal_delta = TOF0_horizontal_calibration.at(TOF0_horizontal_slab);
    double vertical_delta = TOF0_vertical_calibration.at(TOF0_vertical_slab);
    
    double slab_width = 40.0;
    double min_x, max_x, min_y, max_y;
    
    bool good_x = false;
    bool good_y = false;



    tof0_global_x_vs_slab_number->Fill(TOF0_xPixel, TOF0_vertical_slab);
    tof0_global_y_vs_slab_number->Fill(TOF0_yPixel, TOF0_horizontal_slab);

    
    if(!std::isnan(horizontal_delta)){
        TOF0_x = 0.5*calibrated_c_eff*(TOF0_horizontal_slab_tMinus
                                       - TOF0_horizontal_slab_tPlus + horizontal_delta);

        tof0_x_residual->Fill(TOF0_x - TOF0_xPixel);
        
        
        min_x = TOF0_xPixel - 0.5*slab_width;
        max_x = TOF0_xPixel + 0.5*slab_width;
        
        if(TOF0_x >= min_x && TOF0_x <= max_x){
            good_x = true;
        }
        else{
            //good_x = false;
            good_x = true;
        }
    }
    else{
        TOF0_x = TOF0_xPixel;
        good_x = false;
    }
    
    if(!std::isnan(vertical_delta)){
        TOF0_y = 0.5*calibrated_c_eff*(TOF0_vertical_slab_tMinus
                                       - TOF0_vertical_slab_tPlus + vertical_delta);

        // test:
        //TOF0_y = -1.0*TOF0_y;

        tof0_y_residual->Fill(TOF0_y - TOF0_yPixel);


        min_y = TOF0_yPixel - 0.5*slab_width;
        max_y = TOF0_yPixel + 0.5*slab_width;
        if(TOF0_y >= min_y && TOF0_y <= max_x){
            good_y = true;
        }
        else{
            //good_y = false;
            good_y = true;
        }
    }
    else{
        TOF0_y = TOF0_yPixel;
        good_y = false;
    }
    
    if(good_x && good_y){
        TOF0_goodPMTPosition = 1;
    }
    else{
        TOF0_goodPMTPosition = 0;
    }





}

void BetterReadMAUS::apply_calibration_TOF1(){
    double horizontal_delta = TOF1_horizontal_calibration.at(TOF1_horizontal_slab);
    double vertical_delta = TOF1_vertical_calibration.at(TOF1_vertical_slab);
    
    double slab_width = 60.0;
    double min_x, max_x, min_y, max_y;
    
    bool good_x = false;
    bool good_y = false;

    tof1_global_x_vs_slab_number->Fill(TOF1_xPixel, TOF1_vertical_slab);
    tof1_global_y_vs_slab_number->Fill(TOF1_yPixel, TOF1_horizontal_slab);

    
    if(!std::isnan(horizontal_delta)){
        TOF1_x = 0.5*calibrated_c_eff*(TOF1_horizontal_slab_tMinus
                                       - TOF1_horizontal_slab_tPlus + horizontal_delta);
        
        tof1_x_residual->Fill(TOF1_x - TOF1_xPixel);
        
        min_x = TOF1_xPixel - 0.5*slab_width;
        max_x = TOF1_xPixel + 0.5*slab_width;
        
        if(TOF1_x >= min_x && TOF1_x <= max_x){
            good_x = true;
        }
        else{
            //good_x = false;
            good_x = true;
        }
    }
    else{
        TOF1_x = TOF1_xPixel;
        good_x = false;
    }
    
    if(!std::isnan(vertical_delta)){
        TOF1_y = 0.5*calibrated_c_eff*(TOF1_vertical_slab_tMinus
                                       - TOF1_vertical_slab_tPlus + vertical_delta);

        // test:
        //TOF1_y = -1.0*TOF1_y;
        tof1_y_residual->Fill(TOF1_y - TOF1_yPixel);

        min_y = TOF1_yPixel - 0.5*slab_width;
        max_y = TOF1_yPixel + 0.5*slab_width;
        if(TOF1_y >= min_y && TOF1_y <= max_x){
            good_y = true;
        }
        else{
            //good_y = false;
            good_y = true;
        }
    }
    else{
        TOF1_y = TOF1_yPixel;
        good_y = false;
    }
    
    if(good_x && good_y){
        TOF1_goodPMTPosition = 1;
    }
    else{
        TOF1_goodPMTPosition = 0;
    }
}


void BetterReadMAUS::define_root_file(QString saveAs){
    outputFile = new TFile(saveAs.toStdString().c_str(), "RECREATE");
    outputTree = new TTree("T", "T");
    
    outputTree->Branch("SpillNumber", &spill_number, "SpillNumber/I");
    outputTree->Branch("ReconstructedEventNumber", &reconstructed_event_number, "ReconstructedEventNumber/I");

    outputTree->Branch("num_TOF0_spacepoints", &number_of_TOF0_spacepoints, "num_TOF0_spacepoints/I");
    outputTree->Branch("num_TOF1_spacepoints", &number_of_TOF1_spacepoints, "num_TOF1_spacepoints/I");
    outputTree->Branch("num_TKU_tracks", &number_of_TKU_tracks, "num_TKU_tracks/I");
    
    
    // TOF0
    outputTree->Branch("TOF0_x", &TOF0_x, "TOF0_x/D");
    outputTree->Branch("TOF0_y", &TOF0_y, "TOF0_y/D");
    outputTree->Branch("TOF0_z", &TOF0_z, "TOF0_z/D");
    outputTree->Branch("TOF0_hitTime", &TOF0_hitTime, "TOF0_hitTime/D");
    outputTree->Branch("TOF0_px", &TOF0_px, "TOF0_px/D");
    outputTree->Branch("TOF0_py", &TOF0_py, "TOF0_py/D");
    outputTree->Branch("TOF0_pz", &TOF0_pz, "TOF0_pz/D");
    outputTree->Branch("TOF0_p", &TOF0_p, "TOF0_p/D");
    
    outputTree->Branch("TOF0_xPixel", &TOF0_xPixel, "TOF0_xPixel/D");
    outputTree->Branch("TOF0_yPixel", &TOF0_yPixel, "TOF0_yPixel/D");
    outputTree->Branch("TOF0_hSlab_tMinus", &TOF0_horizontal_slab_tMinus, "TOF0_hSlab_tMinus/D");
    outputTree->Branch("TOF0_hSlab_tPlus", &TOF0_horizontal_slab_tPlus, "TOF0_hSlab_tPlus/D");
    outputTree->Branch("TOF0_vSlab_tMinus", &TOF0_vertical_slab_tMinus, "TOF0_vSlab_tMinus/D");
    outputTree->Branch("TOF0_vSlab_tPlus", &TOF0_vertical_slab_tPlus, "TOF0_vSlab_tPlus/D");
    outputTree->Branch("TOF0_hSlab", &TOF0_horizontal_slab, "TOF0_hSlab/I");
    outputTree->Branch("TOF0_vSlab", &TOF0_vertical_slab, "TOF0_vSlab/I");
    
    outputTree->Branch("TOF0_xPrime", &TOF0_xPrime, "TOF0_xPrime/D");
    outputTree->Branch("TOF0_yPrime", &TOF0_yPrime, "TOF0_yPrime/D");
    
    
    // TOF1
    outputTree->Branch("TOF1_x", &TOF1_x, "TOF1_x/D");
    outputTree->Branch("TOF1_xPixel", &TOF1_xPixel, "TOF1_xPixel/D");
    outputTree->Branch("TOF1_y", &TOF1_y, "TOF1_y/D");
    outputTree->Branch("TOF1_yPixel", &TOF1_yPixel, "TOF1_yPixel/D");
    outputTree->Branch("TOF1_z", &TOF1_z, "TOF1_z/D");
    outputTree->Branch("TOF1_hitTime", &TOF1_hitTime, "TOF1_hitTime/D");
    outputTree->Branch("TOF1_hSlab_tMinus", &TOF1_horizontal_slab_tMinus, "TOF1_hSlab_tMinus/D");
    outputTree->Branch("TOF1_hSlab_tPlus", &TOF1_horizontal_slab_tPlus, "TOF1_hSlab_tPlus/D");
    outputTree->Branch("TOF1_vSlab_tMinus", &TOF1_vertical_slab_tMinus, "TOF1_vSlab_tMinus/D");
    outputTree->Branch("TOF1_vSlab_tPlus", &TOF1_vertical_slab_tPlus, "TOF1_vSlab_tPlus/D");
    outputTree->Branch("TOF1_hSlab", &TOF1_horizontal_slab, "TOF1_hSlab/I");
    outputTree->Branch("TOF1_vSlab", &TOF1_vertical_slab, "TOF1_vSlab/I");
    outputTree->Branch("TOF1_px", &TOF1_px, "TOF1_px/D");
    outputTree->Branch("TOF1_py", &TOF1_py, "TOF1_py/D");
    outputTree->Branch("TOF1_pz", &TOF1_pz, "TOF1_pz/D");
    outputTree->Branch("TOF1_p", &TOF1_p, "TOF1_p/D");
    outputTree->Branch("TOF1_xPrime", &TOF1_xPrime, "TOF1_xPrime/D");
    outputTree->Branch("TOF1_yPrime", &TOF1_yPrime, "TOF1_yPrime/D");
    
    // Time of flight
    outputTree->Branch("time_of_flight", &time_of_flight, "time_of_flight/D");
    outputTree->Branch("normalised_time_of_flight", &normalised_time_of_flight, "normalised_time_of_flight/D");

    
    // Diffuser -- values taken from Globals
    outputTree->Branch("diffuser_x", &diffuser_x, "diffuser_x/D");
    outputTree->Branch("diffuser_y", &diffuser_y, "diffuser_y/D");
    outputTree->Branch("diffuser_r", &diffuser_r, "diffuser_r/D");
    outputTree->Branch("diffuser_z", &diffuser_z, "diffuser_z/D");
    
    outputTree->Branch("diffuser_px", &diffuser_px, "diffuser_px/D");
    outputTree->Branch("diffuser_py", &diffuser_py, "diffuser_py/D");
    outputTree->Branch("diffuser_pz", &diffuser_pz, "diffuser_pz/D");
    outputTree->Branch("diffuser_pt", &diffuser_pt, "diffuser_pt/D");
    outputTree->Branch("diffuser_p", &diffuser_p, "diffuser_p/D");

    
    // TKU
    outputTree->Branch("TKU_s1_x", &TKU_plane1_x, "TKU_s1_x/D"); // station 1
    outputTree->Branch("TKU_s1_y", &TKU_plane1_y, "TKU_s1_y/D");
    outputTree->Branch("TKU_s1_r", &TKU_plane1_r, "TKU_s1_r/D");
    outputTree->Branch("TKU_s1_z", &TKU_plane1_z, "TKU_s1_z/D");
    outputTree->Branch("TKU_s1_px", &TKU_plane1_px, "TKU_s1_px/D");
    outputTree->Branch("TKU_s1_py", &TKU_plane1_py, "TKU_s1_py/D");
    outputTree->Branch("TKU_s1_pz", &TKU_plane1_pz, "TKU_s1_pz/D");
    outputTree->Branch("TKU_s1_pt", &TKU_plane1_pt, "TKU_s1_pt/D");
    outputTree->Branch("TKU_s1_p", &TKU_plane1_p, "TKU_s1_p/D");
    
    outputTree->Branch("TKU_s1_pull", &TKU_plane1_pull, "TKU_s1_pull/D");
    outputTree->Branch("TKU_s1_x_error", &TKU_plane1_x_error, "TKU_s1_x_error/D");
    outputTree->Branch("TKU_s1_y_error", &TKU_plane1_y_error, "TKU_s1_y_error/D");
    outputTree->Branch("TKU_s1_px_error", &TKU_plane1_px_error, "TKU_s1_px_error/D");
    outputTree->Branch("TKU_s1_py_error", &TKU_plane1_py_error, "TKU_s1_py_error/D");
    outputTree->Branch("TKU_s1_kappa_error", &TKU_plane1_kappa_error, "TKU_s1_kappa_error/D");
    outputTree->Branch("TKU_s1_pz_error", &TKU_plane1_pz_error, "TKU_s1_pz_error/D");
    
    outputTree->Branch("TKU_s2_x", &TKU_plane2_x, "TKU_s2_x/D"); // station 2
    outputTree->Branch("TKU_s2_y", &TKU_plane2_y, "TKU_s2_y/D");
    outputTree->Branch("TKU_s2_r", &TKU_plane2_r, "TKU_s2_r/D");
    outputTree->Branch("TKU_s2_z", &TKU_plane2_z, "TKU_s2_z/D");
    
    outputTree->Branch("TKU_s2_px", &TKU_plane2_px, "TKU_s2_px/D");
    outputTree->Branch("TKU_s2_py", &TKU_plane2_py, "TKU_s2_py/D");
    outputTree->Branch("TKU_s2_pz", &TKU_plane2_pz, "TKU_s2_pz/D");
    outputTree->Branch("TKU_s2_pt", &TKU_plane2_pt, "TKU_s2_pt/D");
    outputTree->Branch("TKU_s2_p", &TKU_plane2_p, "TKU_s2_p/D");
    
    outputTree->Branch("TKU_s2_pull", &TKU_plane2_pull, "TKU_s2_pull/D");
    outputTree->Branch("TKU_s2_x_error", &TKU_plane2_x_error, "TKU_s2_x_error/D");
    outputTree->Branch("TKU_s2_y_error", &TKU_plane2_y_error, "TKU_s2_y_error/D");
    outputTree->Branch("TKU_s2_px_error", &TKU_plane2_px_error, "TKU_s2_px_error/D");
    outputTree->Branch("TKU_s2_py_error", &TKU_plane2_py_error, "TKU_s2_py_error/D");
    outputTree->Branch("TKU_s2_kappa_error", &TKU_plane2_kappa_error, "TKU_s2_kappa_error/D");
    outputTree->Branch("TKU_s2_pz_error", &TKU_plane2_pz_error, "TKU_s2_pz_error/D");
    
    outputTree->Branch("TKU_s3_x", &TKU_plane3_x, "TKU_s3_x/D"); // station 3
    outputTree->Branch("TKU_s3_y", &TKU_plane3_y, "TKU_s3_y/D");
    outputTree->Branch("TKU_s3_r", &TKU_plane3_r, "TKU_s3_r/D");
    outputTree->Branch("TKU_s3_z", &TKU_plane3_z, "TKU_s3_z/D");
    
    outputTree->Branch("TKU_s3_px", &TKU_plane3_px, "TKU_s3_px/D");
    outputTree->Branch("TKU_s3_py", &TKU_plane3_py, "TKU_s3_py/D");
    outputTree->Branch("TKU_s3_pz", &TKU_plane3_pz, "TKU_s3_pz/D");
    outputTree->Branch("TKU_s3_pt", &TKU_plane3_pt, "TKU_s3_pt/D");
    outputTree->Branch("TKU_s3_p", &TKU_plane3_p, "TKU_s3_p/D");
    
    outputTree->Branch("TKU_s3_pull", &TKU_plane3_pull, "TKU_s3_pull/D");
    outputTree->Branch("TKU_s3_x_error", &TKU_plane3_x_error, "TKU_s3_x_error/D");
    outputTree->Branch("TKU_s3_y_error", &TKU_plane3_y_error, "TKU_s3_y_error/D");
    outputTree->Branch("TKU_s3_px_error", &TKU_plane3_px_error, "TKU_s3_px_error/D");
    outputTree->Branch("TKU_s3_py_error", &TKU_plane3_py_error, "TKU_s3_py_error/D");
    outputTree->Branch("TKU_s3_kappa_error", &TKU_plane3_kappa_error, "TKU_s3_kappa_error/D");
    outputTree->Branch("TKU_s3_pz_error", &TKU_plane3_pz_error, "TKU_s3_pz_error/D");
    
    outputTree->Branch("TKU_s4_x", &TKU_plane4_x, "TKU_s4_x/D"); // station 4
    outputTree->Branch("TKU_s4_y", &TKU_plane4_y, "TKU_s4_y/D");
    outputTree->Branch("TKU_s4_r", &TKU_plane4_r, "TKU_s4_r/D");
    outputTree->Branch("TKU_s4_z", &TKU_plane4_z, "TKU_s4_z/D");
    
    outputTree->Branch("TKU_s4_px", &TKU_plane4_px, "TKU_s4_px/D");
    outputTree->Branch("TKU_s4_py", &TKU_plane4_py, "TKU_s4_py/D");
    outputTree->Branch("TKU_s4_pz", &TKU_plane4_pz, "TKU_s4_pz/D");
    outputTree->Branch("TKU_s4_pt", &TKU_plane4_pt, "TKU_s4_pt/D");
    outputTree->Branch("TKU_s4_p", &TKU_plane4_p, "TKU_s4_p/D");
    
    outputTree->Branch("TKU_s4_pull", &TKU_plane4_pull, "TKU_s4_pull/D");
    outputTree->Branch("TKU_s4_x_error", &TKU_plane4_x_error, "TKU_s4_x_error/D");
    outputTree->Branch("TKU_s4_y_error", &TKU_plane4_y_error, "TKU_s4_y_error/D");
    outputTree->Branch("TKU_s4_px_error", &TKU_plane4_px_error, "TKU_s4_px_error/D");
    outputTree->Branch("TKU_s4_py_error", &TKU_plane4_py_error, "TKU_s4_py_error/D");
    outputTree->Branch("TKU_s4_kappa_error", &TKU_plane4_kappa_error, "TKU_s4_kappa_error/D");
    outputTree->Branch("TKU_s4_pz_error", &TKU_plane4_pz_error, "TKU_s4_pz_error/D");
    
    outputTree->Branch("TKU_s5_x", &TKU_plane5_x, "TKU_s5_x/D"); // station 1
    outputTree->Branch("TKU_s5_y", &TKU_plane5_y, "TKU_s5_y/D");
    outputTree->Branch("TKU_s5_r", &TKU_plane5_r, "TKU_s5_r/D");
    outputTree->Branch("TKU_s5_z", &TKU_plane5_z, "TKU_s5_z/D");
    
    outputTree->Branch("TKU_s5_px", &TKU_plane5_px, "TKU_s5_px/D");
    outputTree->Branch("TKU_s5_py", &TKU_plane5_py, "TKU_s5_py/D");
    outputTree->Branch("TKU_s5_pz", &TKU_plane5_pz, "TKU_s5_pz/D");
    outputTree->Branch("TKU_s5_pt", &TKU_plane5_pt, "TKU_s5_pt/D");
    outputTree->Branch("TKU_s5_p", &TKU_plane5_p, "TKU_s5_p/D");
    
    outputTree->Branch("TKU_s5_pull", &TKU_plane5_pull, "TKU_s5_pull/D");
    outputTree->Branch("TKU_s5_x_error", &TKU_plane5_x_error, "TKU_s5_x_error/D");
    outputTree->Branch("TKU_s5_y_error", &TKU_plane5_y_error, "TKU_s5_y_error/D");
    outputTree->Branch("TKU_s5_px_error", &TKU_plane5_px_error, "TKU_s5_px_error/D");
    outputTree->Branch("TKU_s5_py_error", &TKU_plane5_py_error, "TKU_s5_py_error/D");
    outputTree->Branch("TKU_s5_kappa_error", &TKU_plane5_kappa_error, "TKU_s5_kappa_error/D");
    outputTree->Branch("TKU_s5_pz_error", &TKU_plane5_pz_error, "TKU_s5_pz_error/D");
    
    outputTree->Branch("TKU_assumed_field", &TKU_assumed_field, "TKU_assumed_field/D");
    outputTree->Branch("TKU_chiSquare", &TKU_chiSquare, "TKU_chiSquare/D");
    outputTree->Branch("TKU_ndof", &TKU_ndof, "TKU_ndof/I");
        outputTree->Branch("TKU_chiSquare_per_ndof", &TKU_chiSquare_per_ndof, "TKU_chiSquare_per_ndof/D");
    outputTree->Branch("TKU_pattRec_r", &TKU_patternRecognition_R, "TKU_pattRec_r/D");
    outputTree->Branch("TKU_pattRec_dipAngle", &TKU_patternRecognition_dipAngle, "TKU_pattRec_dipAngle/D");
    outputTree->Branch("TKU_pattRec_x0", &TKU_patternRecognition_x0, "TKU_pattRec_x0/D");
    outputTree->Branch("TKU_pattRec_y0", &TKU_patternRecognition_y0, "TKU_pattRec_y0/D");
    outputTree->Branch("TKU_charge", &TKU_charge, "TKU_charge/I");
    
    
    
    

    
    

    // cuts: these will be 0 or 1 depending on whether the fail (0) or pass (1) the cut
    outputTree->Branch("cut_tof", &cut_tof, "cut_tof/I");
    outputTree->Branch("cut_normalised_tof", &cut_normalised_tof, "cut_normalised_tof/I");
    outputTree->Branch("cut_diffuser", &cut_diffuser, "cut_diffuser/I");
    outputTree->Branch("cut_chindof", &cut_chindof, "cut_chindof/I");
    outputTree->Branch("cut_tku_r", &cut_tku_r, "cut_tku_r/I");
    outputTree->Branch("cut_one_tof0", &cut_one_tof0, "cut_one_tof0/I");
    outputTree->Branch("cut_one_tof1", &cut_one_tof1, "cut_one_tof1/I");
    outputTree->Branch("cut_one_track", &cut_one_track, "cut_one_track/I");
    outputTree->Branch("cut_tof_p", &cut_tof_p, "cut_tof_p/I");
    outputTree->Branch("cut_allPassed", &cut_allPassed, "cut_allPassed/I");
    outputTree->Branch("cut_allButDiffuserPassed", &cut_allButDiffuserPassed, "cut_allButDiffuserPassed/I");
}







void BetterReadMAUS::Read(QString fileToOpen, QString fileToSaveAs, QString calibrationConstantsFile){ //, QString trackingFileName){
    /*
     *  1. read the calibration file
     *  2. create the output root file
     *  3. read the maus input file
     *      a. Get a spacepoint at TOF0, reconstruct its (x, y) using pmt times
     *      b. Get a spacepoint at TOF1, do the same
     *      c. Get a track in the upstream tracker
     *      d. Get the propagated track to the diffuser from Global Track Matching
     */
    multi_track_counter = 0;
    
    set_cut_values();


    readCalibrationFile(calibrationConstantsFile);
    define_root_file(fileToSaveAs);
    
    MAUS::Data data;
    irstream infile(fileToOpen.toStdString().c_str(), "Spill");
    
    int count = 0;
    
    last_event_number = 0;
    mc_spill_counter = 1;
    data_spills = false;
    mc_spills = false;
    
    // iterate over events:
    while(infile >> readEvent != NULL){
        infile >> branchName("data") >> data;
        spill = data.GetSpill();
        
        std::cout << count << "\n";
        count++;
        
        if(spill != NULL && spill->GetDaqEventType() == "physics_event"){
            // Found a spill that contains data. Iterate over all its events...
            
            readParticleEvent();

        }
    }


    print_tof_plots();
    
    outputFile->cd();
    outputTree->Write();
    outputFile->Write();
    
    outputFile->Close();
    
}


void BetterReadMAUS::readParticleEvent(){
    for(size_t i = 0; i < spill->GetReconEvents()->size(); ++i){
        reset_particle_variables();
        spill_number = spill->GetSpillNumber();
        reconstructed_event_number = i;
        
        std::cout << "Reading spill " << spill_number << ", event " << reconstructed_event_number << "\n";

        if(spill_number == 0){
            // data spill start at 0
            std::cout << "Detected spill 0, is this data?\n";
            data_spills = true;
        }

        
        // safety check versus MC files all having spill == 1:
        if(spill_number == 1 && reconstructed_event_number < last_event_number && !data_spills){
            mc_spill_counter += 1;

            std::cout << "spill: " << spill_number << ", event: " << reconstructed_event_number
                      << ", last event: " << last_event_number << ", mc_spill: " << mc_spill_counter << "\n";

            if(!mc_spills){
                // first time detecting that we're actually looking at a funky MC spill/event numbering
                mc_spills = true;
            }
        }
        last_event_number = reconstructed_event_number;
        if(mc_spills){
            spill_number = mc_spill_counter; // set spill number to a more appropriate count
            std::cout << "....Adjusting spill number to " << spill_number << ", with event " << reconstructed_event_number << "\n";
            std::cout << "               spill: " << spill_number << ", event: " << reconstructed_event_number
                      << ", last event: " << last_event_number << ", mc_spill: " << mc_spill_counter << "\n";

        }
        
        tof_event = (*spill->GetReconEvents())[i]->GetTOFEvent();
        scifi_event = (*spill->GetReconEvents())[i]->GetSciFiEvent();
        global_event = (*spill->GetReconEvents())[i]->GetGlobalEvent();
        read_globals();


        if(tof_event != NULL){
            // there are hits at TOFs, estimate pz from them
            particle_at_TOF0();
            particle_at_TOF1();
            
            reconstruct_TOF_momentum();
        }
        
        if(scifi_event != NULL){
            particle_at_tracker(); // get tracker info
        }
        
        
        check_cuts();
        
        
        
        outputTree->Fill();
    }
}


void BetterReadMAUS::write_to_file(){
    outputTree->Fill();
}



void BetterReadMAUS::particle_at_TOF0(){
    /*
     * Want to find one spacepoint, then find the slab hits associated with
     * that spacepoint.  Use those slab hits to improve the particles position
     * information.
     */
    
    MAUS::TOFEventSlabHit slab_hits = tof_event->GetTOFEventSlabHit();
    MAUS::TOFEventSpacePoint space_points = tof_event->GetTOFEventSpacePoint();
    
    MAUS::TOFSlabHit tof0_slab_hits;
    MAUS::TOFSpacePoint tof0_space_points;
    
    
    int horizontalHit, verticalHit;
    
    
    
    reset_TOF0_variables();  // each new space point should be a new set of slab hits
    horizontalHit = -1;
    verticalHit = -1;

    number_of_TOF0_spacepoints = space_points.GetTOF0SpacePointArray().size();



    
    // 1. Loop over TOF0 space points:
    //for(size_t i = 0; i < space_points.GetTOF0SpacePointArray().size(); ++i){
    if(space_points.GetTOF0SpacePointArray().size() >=1){
        reset_TOF0_variables();  // each new space point should be a new set of slab hits
        
        horizontalHit = -1;
        verticalHit = -1;
        
        //tof0_space_points = space_points.GetTOF0SpacePointArray()[i];
        tof0_space_points = space_points.GetTOF0SpacePointArray()[0]; // get first spacepoint
        
        TOF0_horizontal_slab = tof0_space_points.GetHorizSlab(); // returns slabs oriented horizontally
        TOF0_vertical_slab = tof0_space_points.GetVertSlab();   // returns slabs oriented vertically
        
        TOF0_xPixel = tof0_space_points.GetGlobalPosX();
        TOF0_yPixel = tof0_space_points.GetGlobalPosY();
        
        TOF0_z = beamlineTracking_TOF0_zPosition;
        
        TOF0_hitTime = tof0_space_points.GetTime();
        
        // 2. Loop over slab hits and look for matches:
        for(size_t j = 0; j < slab_hits.GetTOF0SlabHitArray().size(); ++j){
            tof0_slab_hits = slab_hits.GetTOF0SlabHitArray()[j];
            
            if(tof0_slab_hits.IsHorizontal()){
                // horizontal slab hit
                horizontalHit = tof0_slab_hits.GetSlab();
                TOF0_horizontal_slab_tPlus = tof0_slab_hits.GetPmt0().GetTime();
                TOF0_horizontal_slab_tMinus = tof0_slab_hits.GetPmt1().GetTime();
            }
            else if(tof0_slab_hits.IsVertical()){
                verticalHit = tof0_slab_hits.GetSlab();


                if(isMCRecon == true){
                    TOF0_vertical_slab_tPlus = tof0_slab_hits.GetPmt0().GetTime();
                    TOF0_vertical_slab_tMinus = tof0_slab_hits.GetPmt1().GetTime();
                }
                else{
                    TOF0_vertical_slab_tPlus = tof0_slab_hits.GetPmt1().GetTime();
                    TOF0_vertical_slab_tMinus = tof0_slab_hits.GetPmt0().GetTime();
                }
            }
            else{
                std::cerr << "Warning: Unusual plane hits at TOF0. Proceed with caution.\n";
            }
            
            apply_calibration_TOF0();
        }
    }
}




void BetterReadMAUS::particle_at_TOF1(){
    /*
     * Want to find one spacepoint, then find the slab hits associated with
     * that spacepoint.  Use those slab hits to improve the particles position
     * information.
     */
    
    MAUS::TOFEventSlabHit slab_hits = tof_event->GetTOFEventSlabHit();
    MAUS::TOFEventSpacePoint space_points = tof_event->GetTOFEventSpacePoint();
    
    MAUS::TOFSlabHit tof1_slab_hits;
    MAUS::TOFSpacePoint tof1_space_points;
    
    
    int horizontalHit, verticalHit;
    //  bool good_particle;

    number_of_TOF1_spacepoints = space_points.GetTOF1SpacePointArray().size();
    
    
    // 1. Loop over TOF1 space points:
    
    //  for(size_t i = 0; i < space_points.GetTOF1SpacePointArray().size(); ++i){
    reset_TOF1_variables();  // each new space point should be a new set of slab hits
    horizontalHit = -1;
    verticalHit = -1;


  //  std::cout << "space_points.GetTOF1SpacePointArray().size() = " << space_points.GetTOF1SpacePointArray().size() << "\n";

    
    if(space_points.GetTOF1SpacePointArray().size() >= 1){
        reset_TOF1_variables();  // each new space point should be a new set of slab hits
        horizontalHit = -1;
        verticalHit = -1;
        
        
        //tof1_space_points = space_points.GetTOF1SpacePointArray()[i];
        tof1_space_points = space_points.GetTOF1SpacePointArray()[0]; // get the first spacepoint
        
        TOF1_horizontal_slab = tof1_space_points.GetHorizSlab(); // returns slabs oriented horizontally
        TOF1_vertical_slab = tof1_space_points.GetVertSlab();   // returns slabs oriented vertically
        
        TOF1_xPixel = tof1_space_points.GetGlobalPosX();
        TOF1_yPixel = tof1_space_points.GetGlobalPosY();
        
        TOF1_z = beamlineTracking_TOF1_zPosition;
        
        TOF1_hitTime = tof1_space_points.GetTime();
        
        // 2. Loop over slab hits and look for matches:
        for(size_t j = 0; j < slab_hits.GetTOF1SlabHitArray().size(); ++j){
            tof1_slab_hits = slab_hits.GetTOF1SlabHitArray()[j];
            
            if(tof1_slab_hits.IsHorizontal()){
                // horizontal slab hit
                horizontalHit = tof1_slab_hits.GetSlab();
                TOF1_horizontal_slab_tMinus = tof1_slab_hits.GetPmt1().GetTime();
                TOF1_horizontal_slab_tPlus = tof1_slab_hits.GetPmt0().GetTime();
            }
            else if(tof1_slab_hits.IsVertical()){
                verticalHit = tof1_slab_hits.GetSlab();

                if(isMCRecon==true){
                    TOF1_vertical_slab_tPlus = tof1_slab_hits.GetPmt0().GetTime();
                    TOF1_vertical_slab_tMinus = tof1_slab_hits.GetPmt1().GetTime();
                }
                else{
                    TOF1_vertical_slab_tPlus = tof1_slab_hits.GetPmt1().GetTime();
                    TOF1_vertical_slab_tMinus = tof1_slab_hits.GetPmt0().GetTime();
                }
            }
            else{
                std::cerr << "Warning: Unusual plane hits at TOF1. Proceed with caution.\n";
            }
            
            apply_calibration_TOF1();
        }
    }
}



void BetterReadMAUS::particle_at_tracker(){
    int tracker;
    MAUS::ThreeVector position;


   /* std::cout << "Found track? Using: \n"
              << "         get_mean_field_up() = " << scifi_event->get_mean_field_up() << "\n"
              << "         get_range_field_up() = " << scifi_event->get_range_field_up() << "\n"
              << "         get_variance_field_up()  = " << scifi_event->get_variance_field_up() << "\n\n"
              << "         get_mean_field_down() = " << scifi_event->get_mean_field_down() << "\n"
              << "         get_range_field_down() = " << scifi_event->get_range_field_down() << "\n"
              << "         get_variance_field_down()  = " << scifi_event->get_variance_field_down() << "\n";
*/

    std::vector<MAUS::SciFiTrack*> tracks = scifi_event->scifitracks();
    std::vector<MAUS::SciFiHelicalPRTrack*> pr_tracks = scifi_event->helicalprtracks();
    
    std::vector<MAUS::SciFiTrack*>::iterator track_iter;
    std::vector<MAUS::SciFiHelicalPRTrack*>::iterator pr_track_iter;
    
    for(pr_track_iter = pr_tracks.begin(); pr_track_iter != pr_tracks.end(); ++pr_track_iter){
        MAUS::SciFiHelicalPRTrack* pr_track = (*pr_track_iter);
        
        if(pr_track->get_tracker() == 0){
            TKU_patternRecognition_R = pr_track->get_R();
            TKU_patternRecognition_dipAngle = pr_track->get_dsdz();  //pr_track->get_phi0();
            TKU_patternRecognition_x0 = pr_track->get_circle_x0();
            TKU_patternRecognition_y0 = pr_track->get_circle_y0();
        }
    }
    
    
    //for(track_iter = tracks.begin(); track_iter != tracks.end(); ++track_iter){ // use if looking at all tracks
    if(tracks.size() != 0){ // use if only want to take the first track
        track_iter = tracks.begin();
        std::vector<MAUS::SciFiTrackPoint*> track_points = (*track_iter)->scifitrackpoints();
        std::vector<MAUS::SciFiTrackPoint*>::iterator track_point_iter;
        
        TKU_Pvalue = (*track_iter)->P_value();
        TKU_chiSquare = (*track_iter)->chi2();
        TKU_ndof = (*track_iter)->ndf();
        TKU_charge = (*track_iter)->charge();
        
        TKU_chiSquare_per_ndof = TKU_chiSquare / (1.0*TKU_ndof);
        
        for(track_point_iter = track_points.begin(); track_point_iter != track_points.end(); ++track_point_iter){
            MAUS::SciFiTrackPoint* point = (*track_point_iter);
            tracker = point->tracker();
            //station = point->station();
            position = point->pos();
            
            if(tracker == 0){
               

                if(point->station() == 1){


                    TKU_plane1_x = point->pos().x();
                    TKU_plane1_y = point->pos().y();
                    TKU_plane1_z = point->pos().z();
                    
                    TKU_plane1_px = point->mom().x();
                    TKU_plane1_py = point->mom().y();
                    TKU_plane1_pz = point->mom().z();
                    TKU_plane1_p = TMath::Sqrt(TKU_plane1_px*TKU_plane1_px
                                               + TKU_plane1_py*TKU_plane1_py
                                               + TKU_plane1_pz*TKU_plane1_pz);
                    
                    TKU_plane1_r = TMath::Sqrt(TKU_plane1_x*TKU_plane1_x + TKU_plane1_y*TKU_plane1_y);
                    TKU_plane1_pt = TMath::Sqrt(TKU_plane1_px*TKU_plane1_px
                                                + TKU_plane1_py*TKU_plane1_py);
                    
                    
                    TKU_plane1_pull = point->pull();
                    
                    /*std::vector<double> errors = point->errors();
                    TKU_plane1_x_error = errors[0];
                    TKU_plane1_px_error = errors[1];
                    TKU_plane1_y_error = errors[2];
                    TKU_plane1_py_error = errors[3];
                    TKU_plane1_kappa_error = errors[4];
                     */
                    TKU_plane1_x_error = point->pos_error().x();
                    TKU_plane1_y_error = point->pos_error().y();
                    TKU_plane1_px_error = point->mom_error().x();
                    TKU_plane1_py_error = point->mom_error().y();
                    TKU_plane1_pz_error = point->mom_error().z();
                    
                }
                else if(point->station() == 2){
                    TKU_plane2_x = point->pos().x();
                    TKU_plane2_y = point->pos().y();
                    TKU_plane2_z = point->pos().z();
                    
                    TKU_plane2_px = point->mom().x();
                    TKU_plane2_py = point->mom().y();
                    TKU_plane2_pz = point->mom().z();
                    TKU_plane2_p = TMath::Sqrt(TKU_plane2_px*TKU_plane2_px
                                               + TKU_plane2_py*TKU_plane2_py
                                               + TKU_plane2_pz*TKU_plane2_pz);
                    
                    TKU_plane2_r = TMath::Sqrt(TKU_plane2_x*TKU_plane2_x + TKU_plane2_y*TKU_plane2_y);
                    TKU_plane2_pt = TMath::Sqrt(TKU_plane2_px*TKU_plane2_px
                                                + TKU_plane2_py*TKU_plane2_py);
                    
                    TKU_plane2_pull = point->pull();
                    
                    /*std::vector<double> errors = point->errors();
                    TKU_plane2_x_error = errors[0];
                    TKU_plane2_px_error = errors[1];
                    TKU_plane2_y_error = errors[2];
                    TKU_plane2_py_error = errors[3];
                    TKU_plane2_kappa_error = errors[4];
                     */
                    TKU_plane2_x_error = point->pos_error().x();
                    TKU_plane2_y_error = point->pos_error().y();
                    TKU_plane2_px_error = point->mom_error().x();
                    TKU_plane2_py_error = point->mom_error().y();
                    TKU_plane2_pz_error = point->mom_error().z();
                    
                }
                else if(point->station() == 3){
                    TKU_plane3_x = point->pos().x();
                    TKU_plane3_y = point->pos().y();
                    TKU_plane3_z = point->pos().z();
                    
                    TKU_plane3_px = point->mom().x();
                    TKU_plane3_py = point->mom().y();
                    TKU_plane3_pz = point->mom().z();
                    TKU_plane3_p = TMath::Sqrt(TKU_plane3_px*TKU_plane3_px
                                               + TKU_plane3_py*TKU_plane3_py
                                               + TKU_plane3_pz*TKU_plane3_pz);
                    
                    TKU_plane3_r = TMath::Sqrt(TKU_plane3_x*TKU_plane3_x + TKU_plane3_y*TKU_plane3_y);
                    TKU_plane3_pt = TMath::Sqrt(TKU_plane3_px*TKU_plane3_px
                                                + TKU_plane3_py*TKU_plane3_py);
                    
                    
                    TKU_plane3_pull = point->pull();
                    
                    /*std::vector<double> errors = point->errors();
                    TKU_plane3_x_error = errors[0];
                    TKU_plane3_px_error = errors[1];
                    TKU_plane3_y_error = errors[2];
                    TKU_plane3_py_error = errors[3];
                    TKU_plane3_kappa_error = errors[4];
                     */
                    TKU_plane3_x_error = point->pos_error().x();
                    TKU_plane3_y_error = point->pos_error().y();
                    TKU_plane3_px_error = point->mom_error().x();
                    TKU_plane3_py_error = point->mom_error().y();
                    TKU_plane3_pz_error = point->mom_error().z();
                    
                }
                else if(point->station() == 4){
                    TKU_plane4_x = point->pos().x();
                    TKU_plane4_y = point->pos().y();
                    TKU_plane4_z = point->pos().z();
                    
                    TKU_plane4_px = point->mom().x();
                    TKU_plane4_py = point->mom().y();
                    TKU_plane4_pz = point->mom().z();
                    TKU_plane4_p = TMath::Sqrt(TKU_plane4_px*TKU_plane4_px
                                               + TKU_plane4_py*TKU_plane4_py
                                               + TKU_plane4_pz*TKU_plane4_pz);
                    
                    TKU_plane4_r = TMath::Sqrt(TKU_plane4_x*TKU_plane4_x + TKU_plane4_y*TKU_plane4_y);
                    TKU_plane4_pt = TMath::Sqrt(TKU_plane4_px*TKU_plane4_px
                                                + TKU_plane4_py*TKU_plane4_py);
                    
                    TKU_plane4_pull = point->pull();
                    
                    /* std::vector<double> errors = point->errors();
                    TKU_plane4_x_error = errors[0];
                    TKU_plane4_px_error = errors[1];
                    TKU_plane4_y_error = errors[2];
                    TKU_plane4_py_error = errors[3];
                    TKU_plane4_kappa_error = errors[4];
                     */
                    TKU_plane4_x_error = point->pos_error().x();
                    TKU_plane4_y_error = point->pos_error().y();
                    TKU_plane4_px_error = point->mom_error().x();
                    TKU_plane4_py_error = point->mom_error().y();
                    TKU_plane4_pz_error = point->mom_error().z();
                    
                    
                }
                else{
                    TKU_plane5_x = point->pos().x();
                    TKU_plane5_y = point->pos().y();
                    TKU_plane5_z = point->pos().z();
                    
                    TKU_plane5_px = point->mom().x();
                    TKU_plane5_py = point->mom().y();
                    TKU_plane5_pz = point->mom().z();
                    TKU_plane5_p = TMath::Sqrt(TKU_plane5_px*TKU_plane5_px
                                               + TKU_plane5_py*TKU_plane5_py
                                               + TKU_plane5_pz*TKU_plane5_pz);
                    
                    TKU_plane5_r = TMath::Sqrt(TKU_plane5_x*TKU_plane5_x + TKU_plane5_y*TKU_plane5_y);
                    TKU_plane5_pt = TMath::Sqrt(TKU_plane5_px*TKU_plane5_px
                                                + TKU_plane5_py*TKU_plane5_py);
                    
                    TKU_plane5_pull = point->pull();
                    
                    /*std::vector<double> errors = point->errors();
                    TKU_plane5_x_error = errors[0];
                    TKU_plane5_px_error = errors[1];
                    TKU_plane5_y_error = errors[2];
                    TKU_plane5_py_error = errors[3];
                    TKU_plane5_kappa_error = errors[4];
                    */
                    TKU_plane5_x_error = point->pos_error().x();
                    TKU_plane5_y_error = point->pos_error().y();
                    TKU_plane5_px_error = point->mom_error().x();
                    TKU_plane5_py_error = point->mom_error().y();
                    TKU_plane5_pz_error = point->mom_error().z();
                }
            }
        }
    }
    


    // count tracks in TKU:
    for(track_iter = tracks.begin(); track_iter != tracks.end(); ++track_iter){ // use if looking at all tracks
        //track_iter = tracks.begin();
        std::vector<MAUS::SciFiTrackPoint*> track_points = (*track_iter)->scifitrackpoints();
        std::vector<MAUS::SciFiTrackPoint*>::iterator track_point_iter;
        for(track_point_iter = track_points.begin(); track_point_iter != track_points.end(); ++track_point_iter){
            MAUS::SciFiTrackPoint* point = (*track_point_iter);
            tracker = point->tracker();

            if(tracker == 0){
                number_of_TKU_tracks++;

            }
        }
    }


    number_of_TKU_tracks = number_of_TKU_tracks / 15;
}


void BetterReadMAUS::SetBeamlineParameters(double min_tof, double max_tof, double sim_ele_path, double data_ele_tof,
                                           double q7_current, double q8_current, double q9_current,
                                           double q7_zPosition, double q8_zPosition, double q9_zPosition,
                                           double tof0_zPosition, double tof1_zPosition){
    beamlineTracking_data_ele_tof = data_ele_tof;
    beamlineTracking_max_tof = max_tof;
    beamlineTracking_min_tof = min_tof;
    beamlineTracking_q7_current = q7_current;
    beamlineTracking_q8_current = q8_current;
    beamlineTracking_q9_current = q9_current;
    beamlineTracking_sim_ele_path = sim_ele_path;
    beamlineTracking_q7_zPosition = q7_zPosition*u.mm();
    beamlineTracking_q8_zPosition = q8_zPosition*u.mm();
    beamlineTracking_q9_zPosition = q9_zPosition*u.mm();
    beamlineTracking_TOF0_zPosition = tof0_zPosition*u.mm();
    beamlineTracking_TOF1_zPosition = tof1_zPosition*u.mm();
    
    beamline = new StepIBeamLine(false);
    beamline->SetQ7(beamlineTracking_q7_current, beamlineTracking_q7_zPosition);
    beamline->SetQ8(beamlineTracking_q8_current, beamlineTracking_q8_zPosition);
    beamline->SetQ9(beamlineTracking_q9_current, beamlineTracking_q9_zPosition);
    beamline->SetTOF0(beamlineTracking_TOF0_zPosition);
    beamline->SetTOF1(beamlineTracking_TOF1_zPosition);
    beamline->Initialise();
    
    tracking = new StepITracking("run7469", beamline);
}

void BetterReadMAUS::reconstruct_TOF_momentum(){
    QHash<QString, double> result;
    double dt = TOF1_hitTime - TOF0_hitTime;
    double L = TOF1_z - TOF0_z;
    double mu_mass = 105.658367*u.MeV();
    double dt_cal;
    
    if(dt >= beamlineTracking_min_tof && dt <= beamlineTracking_max_tof){
      //  goodTimeOfFlight = 1;
        
        dt_cal = dt - beamlineTracking_data_ele_tof + (L + beamlineTracking_sim_ele_path)/u.c_light();
        
        tracking->ReconstructEvent(dt_cal, TOF0_x, TOF1_x, TOF0_y, TOF1_y, mu_mass);
    }
    else{
       // goodTimeOfFlight = 0;
        dt_cal = dt - beamlineTracking_data_ele_tof + (L + beamlineTracking_sim_ele_path)/u.c_light();
        tracking->Bad(dt_cal, TOF0_x, TOF1_x, TOF0_y, TOF1_y, mu_mass);
    }
    
    time_of_flight = dt;
    normalised_time_of_flight = dt - beamlineTracking_data_ele_tof + (L + beamlineTracking_sim_ele_path)/TMath::C();
    
    result = tracking->Result();
    TOF0_xPrime = result.value("ax0");
    TOF0_yPrime = result.value("ay0");
    
    TOF1_xPrime = result.value("ax1");
    TOF1_yPrime = result.value("ay1");
    
    TOF1_pz = result.value("P");
    TOF0_pz = TOF1_pz + result.value("dP"); // made a momentum correction based on air...
    
    
    
    TOF0_px = TOF0_xPrime * TOF0_pz;
    TOF0_py = TOF0_yPrime * TOF0_pz;
    TOF1_px = TOF1_xPrime * TOF1_pz;
    TOF1_py = TOF1_yPrime * TOF1_pz;
    
    TOF0_p = TMath::Sqrt(TOF0_px*TOF0_px + TOF0_py*TOF0_py + TOF0_pz*TOF0_pz);
    TOF1_p = TMath::Sqrt(TOF1_px*TOF1_px + TOF1_py*TOF1_py + TOF1_pz*TOF1_pz);
    
    //goodRaynerReconstruction = result.value("good");
    
    //std::cout << ".... reconstruction returned good = " << goodRaynerReconstruction << "\n";
}





void BetterReadMAUS::reset_diffuser_variables(){
    cut_diffuser = -1;

    diffuser_x = TMath::Infinity();
    diffuser_y = TMath::Infinity();
    diffuser_z = 13740.0;

    diffuser_px = TMath::Infinity();
    diffuser_py = TMath::Infinity();
    diffuser_pz = TMath::Infinity();
}





void BetterReadMAUS::reset_mc_particle_variables(){
    // reset's particle variables
    
    // approx locations of detector stations
    // used to find closest virtual plane.
    mc_tof0_z = 5260.66; //5286.0;
    mc_tof1_z_us = 12904.4; //12930.0;
    mc_tof1_z_ds = 12954.5; //12930.0;

    mc_tku_s1_z = 15062.3;
    mc_tku_s2_z = 14861.4;
    mc_tku_s3_z = 14612.4;
    mc_tku_s4_z = 14312.4;
    mc_tku_s5_z = 13962.4;
    mc_diffuser_z1 = 13720.0; // z of most upstream plane of diffuser
    mc_diffuser_z2 = 13730.0;  // z of the middle of the diffuser
    mc_diffuser_z3 = 13740.0;
    
    reset_mc_TOF0_variables();
    reset_mc_TOF1_variables();
    reset_mc_TKU_variables();
    reset_mc_diffuser_variables();
    
    mc_event_number = -1;
    mc_spill_number = -1;
    mc_particle_id = -1;
    
    
}

void BetterReadMAUS::reset_mc_TOF0_variables(){
    mc_tof0_x = TMath::Infinity();
    mc_tof0_y = TMath::Infinity();
    mc_tof0_px = TMath::Infinity();
    mc_tof0_py = TMath::Infinity();
    mc_tof0_pz = TMath::Infinity();
    mc_tof0_p = TMath::Infinity();
    mc_tof0_xPrime = TMath::Infinity();
    mc_tof0_yPrime = TMath::Infinity();
    mc_tof0_t = TMath::Infinity();
}

void BetterReadMAUS::reset_mc_TOF1_variables(){
    mc_tof1_x_us = TMath::Infinity();
    mc_tof1_y_us = TMath::Infinity();
    mc_tof1_px_us = TMath::Infinity();
    mc_tof1_py_us = TMath::Infinity();
    mc_tof1_pz_us = TMath::Infinity();
    mc_tof1_p_us = TMath::Infinity();
    mc_tof1_xPrime_us = TMath::Infinity();
    mc_tof1_yPrime_us = TMath::Infinity();
    mc_tof1_t_us = TMath::Infinity();
    
    mc_tof1_x_ds = TMath::Infinity();
    mc_tof1_y_ds = TMath::Infinity();
    mc_tof1_px_ds = TMath::Infinity();
    mc_tof1_py_ds = TMath::Infinity();
    mc_tof1_pz_ds = TMath::Infinity();
    mc_tof1_p_ds = TMath::Infinity();
    mc_tof1_xPrime_ds = TMath::Infinity();
    mc_tof1_yPrime_ds = TMath::Infinity();
    mc_tof1_t_ds = TMath::Infinity();
}

void BetterReadMAUS::reset_mc_TKU_variables(){
    mc_tku_s1_x = TMath::Infinity();
    mc_tku_s1_y = TMath::Infinity();
    mc_tku_s1_px = TMath::Infinity();
    mc_tku_s1_py = TMath::Infinity();
    mc_tku_s1_pz = TMath::Infinity();
    mc_tku_s1_p = TMath::Infinity();
    mc_tku_s1_B = TMath::Infinity();
    
    mc_tku_s2_x = TMath::Infinity();
    mc_tku_s2_y = TMath::Infinity();
    mc_tku_s2_px = TMath::Infinity();
    mc_tku_s2_py = TMath::Infinity();
    mc_tku_s2_pz = TMath::Infinity();
    mc_tku_s2_p = TMath::Infinity();
    mc_tku_s2_B = TMath::Infinity();
    
    mc_tku_s3_x = TMath::Infinity();
    mc_tku_s3_y = TMath::Infinity();
    mc_tku_s3_px = TMath::Infinity();
    mc_tku_s3_py = TMath::Infinity();
    mc_tku_s3_pz = TMath::Infinity();
    mc_tku_s3_p = TMath::Infinity();
    mc_tku_s3_B = TMath::Infinity();
    
    mc_tku_s4_x = TMath::Infinity();
    mc_tku_s4_y = TMath::Infinity();
    mc_tku_s4_px = TMath::Infinity();
    mc_tku_s4_py = TMath::Infinity();
    mc_tku_s4_pz = TMath::Infinity();
    mc_tku_s4_p = TMath::Infinity();
    mc_tku_s4_B = TMath::Infinity();
    
    mc_tku_s5_x = TMath::Infinity();
    mc_tku_s5_y = TMath::Infinity();
    mc_tku_s5_px = TMath::Infinity();
    mc_tku_s5_py = TMath::Infinity();
    mc_tku_s5_pz = TMath::Infinity();
    mc_tku_s5_p = TMath::Infinity();
    mc_tku_s5_B = TMath::Infinity();
}

void BetterReadMAUS::reset_mc_diffuser_variables(){
    mc_diffuser_x1 = TMath::Infinity();
    mc_diffuser_y1 = TMath::Infinity();
    mc_diffuser_px1 = TMath::Infinity();
    mc_diffuser_py1 = TMath::Infinity();
    mc_diffuser_pz1 = TMath::Infinity();
    mc_diffuser_p1 = TMath::Infinity();
    mc_diffuser_B1 = TMath::Infinity();
    
    mc_diffuser_x2 = TMath::Infinity();
    mc_diffuser_y2 = TMath::Infinity();
    mc_diffuser_px2 = TMath::Infinity();
    mc_diffuser_py2 = TMath::Infinity();
    mc_diffuser_pz2 = TMath::Infinity();
    mc_diffuser_p2 = TMath::Infinity();
    mc_diffuser_B2 = TMath::Infinity();
    
    mc_diffuser_x3 = TMath::Infinity();
    mc_diffuser_y3 = TMath::Infinity();
    mc_diffuser_px3 = TMath::Infinity();
    mc_diffuser_py3 = TMath::Infinity();
    mc_diffuser_pz3 = TMath::Infinity();
    mc_diffuser_p3 = TMath::Infinity();
    mc_diffuser_B3 = TMath::Infinity();
}




void BetterReadMAUS::define_mc_root_file(QString saveAs){
    outputFile = new TFile(saveAs.toStdString().c_str(), "RECREATE");
    outputTree = new TTree("T", "T");
    
    outputTree->Branch("MC_SpillNumber", &mc_spill_number, "MC_SpillNumber/I");
    outputTree->Branch("MC_EventNumber", &mc_event_number, "MC_EventNumber/I");
    outputTree->Branch("MC_ParticleID", &mc_particle_id, "MC_ParticleID/I");
    
    outputTree->Branch("MC_TOF0_x", &mc_tof0_x, "MC_TOF0_x/D");
    outputTree->Branch("MC_TOF0_y", &mc_tof0_y, "MC_TOF0_y/D");
    outputTree->Branch("MC_TOF0_z", &mc_tof0_z, "MC_TOF0_z/D");
    outputTree->Branch("MC_TOF0_px", &mc_tof0_px, "MC_TOF0_px/D");
    outputTree->Branch("MC_TOF0_py", &mc_tof0_py, "MC_TOF0_py/D");
    outputTree->Branch("MC_TOF0_pz", &mc_tof0_pz, "MC_TOF0_pz/D");
    outputTree->Branch("MC_TOF0_p", &mc_tof0_p, "MC_TOF0_p/D");
    outputTree->Branch("MC_TOF0_xPrime", &mc_tof0_xPrime, "MC_TOF0_xPrime/D");
    outputTree->Branch("MC_TOF0_yPrime", &mc_tof0_yPrime, "MC_TOF0_yPrime/D");
    outputTree->Branch("MC_TOF0_t", &mc_tof0_t, "MC_TOF0_t/D");
    
    outputTree->Branch("MC_TOF1_x_us", &mc_tof1_x_us, "MC_MC_TOF1_x_us/D");
    outputTree->Branch("MC_TOF1_y_us", &mc_tof1_y_us, "MC_TOF1_y_us/D");
    outputTree->Branch("MC_TOF1_z_us", &mc_tof1_z_us, "MC_TOF1_z_us/D");
    outputTree->Branch("MC_TOF1_px_us", &mc_tof1_px_us, "MC_TOF1_px_us/D");
    outputTree->Branch("MC_TOF1_py_us", &mc_tof1_py_us, "MC_TOF1_py_us/D");
    outputTree->Branch("MC_TOF1_pz_us", &mc_tof1_pz_us, "MC_TOF1_pz_us/D");
    outputTree->Branch("MC_TOF1_p_us", &mc_tof1_p_us, "MC_TOF1_p_us/D");
    outputTree->Branch("MC_TOF1_xPrime_us", &mc_tof1_xPrime_us, "MC_TOF1_xPrime_us/D");
    outputTree->Branch("MC_TOF1_yPrime_us", &mc_tof1_yPrime_us, "MC_TOF1_yPrime_us/D");
    outputTree->Branch("MC_TOF1_t_us", &mc_tof1_t_us, "MC_TOF1_t_us/D");
    
    outputTree->Branch("MC_TOF1_x_ds", &mc_tof1_x_ds, "MC_MC_TOF1_x_ds/D");
    outputTree->Branch("MC_TOF1_y_ds", &mc_tof1_y_ds, "MC_TOF1_y_ds/D");
    outputTree->Branch("MC_TOF1_z_ds", &mc_tof1_z_ds, "MC_TOF1_z_ds/D");
    outputTree->Branch("MC_TOF1_px_ds", &mc_tof1_px_ds, "MC_TOF1_px_ds/D");
    outputTree->Branch("MC_TOF1_py_ds", &mc_tof1_py_ds, "MC_TOF1_py_ds/D");
    outputTree->Branch("MC_TOF1_pz_ds", &mc_tof1_pz_ds, "MC_TOF1_pz_ds/D");
    outputTree->Branch("MC_TOF1_p_ds", &mc_tof1_p_ds, "MC_TOF1_p_ds/D");
    outputTree->Branch("MC_TOF1_xPrime_ds", &mc_tof1_xPrime_ds, "MC_TOF1_xPrime_ds/D");
    outputTree->Branch("MC_TOF1_yPrime_ds", &mc_tof1_yPrime_ds, "MC_TOF1_yPrime_ds/D");
    outputTree->Branch("MC_TOF1_t_ds", &mc_tof1_t_ds, "MC_TOF1_t_ds/D");
    
    outputTree->Branch("MC_TKU_s1_x", &mc_tku_s1_x, "MC_TKU_s1_x/D"); // station 1
    outputTree->Branch("MC_TKU_s1_y", &mc_tku_s1_y, "MC_TKU_s1_y/D");
    outputTree->Branch("MC_TKU_s1_z", &mc_tku_s1_z, "MC_TKU_s1_z/D");
    outputTree->Branch("MC_TKU_s1_px", &mc_tku_s1_px, "MC_TKU_s1_px/D");
    outputTree->Branch("MC_TKU_s1_py", &mc_tku_s1_py, "MC_TKU_s1_py/D");
    outputTree->Branch("MC_TKU_s1_pz", &mc_tku_s1_pz, "MC_TKU_s1_pz/D");
    outputTree->Branch("MC_TKU_s1_p", &mc_tku_s1_p, "MC_TKU_s1_p/D");
    
    outputTree->Branch("MC_TKU_s2_x", &mc_tku_s2_x, "MC_TKU_s2_x/D"); // station 2
    outputTree->Branch("MC_TKU_s2_y", &mc_tku_s2_y, "MC_TKU_s2_y/D");
    outputTree->Branch("MC_TKU_s2_z", &mc_tku_s2_z, "MC_TKU_s2_z/D");
    outputTree->Branch("MC_TKU_s2_px", &mc_tku_s2_px, "MC_TKU_s2_px/D");
    outputTree->Branch("MC_TKU_s2_py", &mc_tku_s2_py, "MC_TKU_s2_py/D");
    outputTree->Branch("MC_TKU_s2_pz", &mc_tku_s2_pz, "MC_TKU_s2_pz/D");
    outputTree->Branch("MC_TKU_s2_p", &mc_tku_s2_p, "MC_TKU_s2_p/D");
    
    outputTree->Branch("MC_TKU_s3_x", &mc_tku_s3_x, "MC_TKU_s3_x/D"); // station 3
    outputTree->Branch("MC_TKU_s3_y", &mc_tku_s3_y, "MC_TKU_s3_y/D");
    outputTree->Branch("MC_TKU_s3_z", &mc_tku_s3_z, "MC_TKU_s3_z/D");
    outputTree->Branch("MC_TKU_s3_px", &mc_tku_s3_px, "MC_TKU_s3_px/D");
    outputTree->Branch("MC_TKU_s3_py", &mc_tku_s3_py, "MC_TKU_s3_py/D");
    outputTree->Branch("MC_TKU_s3_pz", &mc_tku_s3_pz, "MC_TKU_s3_pz/D");
    outputTree->Branch("MC_TKU_s3_p", &mc_tku_s3_p, "MC_TKU_s3_p/D");
    
    outputTree->Branch("MC_TKU_s4_x", &mc_tku_s4_x, "MC_TKU_s4_x/D"); // station 4
    outputTree->Branch("MC_TKU_s4_y", &mc_tku_s4_y, "MC_TKU_s4_y/D");
    outputTree->Branch("MC_TKU_s4_z", &mc_tku_s4_z, "MC_TKU_s4_z/D");
    outputTree->Branch("MC_TKU_s4_px", &mc_tku_s4_px, "MC_TKU_s4_px/D");
    outputTree->Branch("MC_TKU_s4_py", &mc_tku_s4_py, "MC_TKU_s4_py/D");
    outputTree->Branch("MC_TKU_s4_pz", &mc_tku_s4_pz, "MC_TKU_s4_pz/D");
    outputTree->Branch("MC_TKU_s4_p", &mc_tku_s4_p, "MC_TKU_s4_p/D");
    
    outputTree->Branch("MC_TKU_s5_x", &mc_tku_s5_x, "MC_TKU_s5_x/D"); // station 5
    outputTree->Branch("MC_TKU_s5_y", &mc_tku_s5_y, "MC_TKU_s5_y/D");
    outputTree->Branch("MC_TKU_s5_z", &mc_tku_s5_z, "MC_TKU_s5_z/D");
    outputTree->Branch("MC_TKU_s5_px", &mc_tku_s5_px, "MC_TKU_s5_px/D");
    outputTree->Branch("MC_TKU_s5_py", &mc_tku_s5_py, "MC_TKU_s5_py/D");
    outputTree->Branch("MC_TKU_s5_pz", &mc_tku_s5_pz, "MC_TKU_s5_pz/D");
    outputTree->Branch("MC_TKU_s5_p", &mc_tku_s5_p, "MC_TKU_s5_p/D");
    
    outputTree->Branch("MC_diffuser_x1", &mc_diffuser_x1, "MC_diffuser_x1/D");
    outputTree->Branch("MC_diffuser_y1", &mc_diffuser_y1, "MC_diffuser_y1/D");
    outputTree->Branch("MC_diffuser_z1", &mc_diffuser_z1, "MC_diffuser_z1/D");
    outputTree->Branch("MC_diffuser_px1", &mc_diffuser_px1, "MC_diffuser_px1/D");
    outputTree->Branch("MC_diffuser_py1", &mc_diffuser_py1, "MC_diffuser_py1/D");
    outputTree->Branch("MC_diffuser_pz1", &mc_diffuser_pz1, "MC_diffuser_pz1/D");
    outputTree->Branch("MC_diffuser_p1", &mc_diffuser_p1, "MC_diffuser_p1/D");
    outputTree->Branch("MC_diffuser_B1", &mc_diffuser_B1, "MC_diffuser_B1/D");
    
    outputTree->Branch("MC_diffuser_x2", &mc_diffuser_x2, "MC_diffuser_x2/D");
    outputTree->Branch("MC_diffuser_y2", &mc_diffuser_y2, "MC_diffuser_y2/D");
    outputTree->Branch("MC_diffuser_z2", &mc_diffuser_z2, "MC_diffuser_z2/D");
    outputTree->Branch("MC_diffuser_px2", &mc_diffuser_px2, "MC_diffuser_px2/D");
    outputTree->Branch("MC_diffuser_py2", &mc_diffuser_py2, "MC_diffuser_py2/D");
    outputTree->Branch("MC_diffuser_pz2", &mc_diffuser_pz2, "MC_diffuser_pz2/D");
    outputTree->Branch("MC_diffuser_p2", &mc_diffuser_p2, "MC_diffuser_p2/D");
    outputTree->Branch("MC_diffuser_B2", &mc_diffuser_B2, "MC_diffuser_B2/D");
    
    outputTree->Branch("MC_diffuser_x3", &mc_diffuser_x3, "MC_diffuser_x3/D");
    outputTree->Branch("MC_diffuser_y3", &mc_diffuser_y3, "MC_diffuser_y3/D");
    outputTree->Branch("MC_diffuser_z3", &mc_diffuser_z3, "MC_diffuser_z3/D");
    outputTree->Branch("MC_diffuser_px3", &mc_diffuser_px3, "MC_diffuser_px3/D");
    outputTree->Branch("MC_diffuser_py3", &mc_diffuser_py3, "MC_diffuser_py3/D");
    outputTree->Branch("MC_diffuser_pz3", &mc_diffuser_pz3, "MC_diffuser_pz3/D");
    outputTree->Branch("MC_diffuser_p3", &mc_diffuser_p3, "MC_diffuser_p3/D");
    outputTree->Branch("MC_diffuser_B3", &mc_diffuser_B3, "MC_diffuser_B3/D");
    
}






void BetterReadMAUS::ReadMC(QString fileToOpen, QString fileToSaveAs){
    //
    //  1. read the calibration file
    //  2. create the output root file
    //  3. read the maus input file
    //      a. Get a spacepoint at TOF0, reconstruct its (x, y) using pmt times
    //      b. Get a spacepoint at TOF1, do the same
    //      c. Get a track in the upstream tracker
    //
    
    define_mc_root_file(fileToSaveAs);
    
    MAUS::Data data;
    irstream infile(fileToOpen.toStdString().c_str(), "Spill");
    
    
    
    last_event_number = 0;
    mc_spill_counter = 1;
    mc_spills = false;
    
    // iterate over events:
    while(infile >> readEvent != NULL){
        infile >> branchName("data") >> data;
        spill = data.GetSpill();
        
        if(spill != NULL && spill->GetDaqEventType() == "physics_event"){
            // Found a spill that contains data. Iterate over all its events...
            
            readMCParticleEvent();
        }
    }
    
    outputFile->cd();
    outputTree->Write();
    outputFile->Write();
    
    outputFile->Close();
    
}

void BetterReadMAUS::readMCParticleEvent(){
    
    double dz = 0.1;
    double diffuser_dz = 0.1;
    MAUS::ThreeVector position;
    MAUS::ThreeVector momentum;
    MAUS::ThreeVector field;
    MAUS::VirtualHit hit;
    MAUS::VirtualHitArray* vhit_array;
    
    for(size_t i = 0; i < spill->GetMCEvents()->size(); ++i){
        reset_mc_particle_variables();
        
        mc_spill_number = spill->GetSpillNumber();
        mc_event_number = i;
        
        
        std::cout << "Reading spill " << mc_spill_number << ", event " << mc_event_number;
        
        // safety check versus MC files all having spill == 1:
        if(mc_spill_number == 1 && mc_event_number < last_event_number){
            mc_spill_counter += 1;
            if(!mc_spills){
                // first time detecting that we're actually looking at a funky MC spill/event numbering
                mc_spills = true;
            }
        }
        last_event_number = mc_event_number;
        if(mc_spills){
            mc_spill_number = mc_spill_counter; // set spill number to a more appropriate count
            std::cout << " --> " << mc_spill_number << ", with event " << mc_event_number << "\n";
        }
        else{
            std::cout << "\n";
        }
        
        
        bool testping = false;
        
        vhit_array = (*spill->GetMCEvents())[i]->GetVirtualHits();
        for (int v=0; v < vhit_array->size(); ++v) {
            hit = vhit_array->at(v);
            position = hit.GetPosition();
            
            
            testping = false;
            
            if(position.z() >= mc_diffuser_z1 - 1500.0 && position.z() <= mc_diffuser_z1 + 1500.0){
                std::cout << "...................Looking for z = " << mc_diffuser_z1 <<  "--> found z = " << position.z() << "\n";
                testping = true;
            }
            
            std::cout << "Plane position = " << position.z() << "\n";
            if(position.z() >= 10000.0){
                std::cout << "       Plane > 9000!\n";
            }
            
            if(position.z() >= mc_tof0_z - dz && position.z() <= mc_tof0_z + dz){
                if(testping){
                    std::cout << "ping, tof0 with z = " << position.z() << "\n";
                }
                
                momentum = hit.GetMomentum();
                
                mc_tof0_x = position.x();
                mc_tof0_y = position.y();
                mc_tof0_z = position.z();
                mc_tof0_px = momentum.x();
                mc_tof0_py = momentum.y();
                mc_tof0_pz = momentum.z();
                mc_tof0_p = TMath::Sqrt(mc_tof0_px*mc_tof0_px + mc_tof0_py*mc_tof0_py + mc_tof0_pz*mc_tof0_pz);
                mc_tof0_t = hit.GetTime();
            }
            else if(position.z() >= mc_tof1_z_us - dz && position.z() <= mc_tof1_z_us + dz){
                if(testping){
                    std::cout << "ping, tof1 with z = " << position.z() << "\n";
                }
                momentum = hit.GetMomentum();
                
                mc_tof1_x_us = position.x();
                mc_tof1_y_us = position.y();
                mc_tof1_z_us = position.z();
                mc_tof1_px_us = momentum.x();
                mc_tof1_py_us = momentum.y();
                mc_tof1_pz_us = momentum.z();
                mc_tof1_p_us = TMath::Sqrt(mc_tof1_px_us*mc_tof1_px_us + mc_tof1_py_us*mc_tof1_py_us + mc_tof1_pz_us*mc_tof1_pz_us);
                mc_tof1_xPrime_us = mc_tof1_px_us/mc_tof1_pz_us;
                mc_tof1_yPrime_us = mc_tof1_py_us/mc_tof1_pz_us;
                mc_tof1_t_us = hit.GetTime();
                
            }
            else if(position.z() >= mc_tof1_z_ds - dz && position.z() <= mc_tof1_z_ds + dz){
                if(testping){
                    std::cout << "ping, tof1 with z = " << position.z() << "\n";
                }
                momentum = hit.GetMomentum();
                
                mc_tof1_x_ds = position.x();
                mc_tof1_y_ds = position.y();
                mc_tof1_z_ds = position.z();
                mc_tof1_px_ds = momentum.x();
                mc_tof1_py_ds = momentum.y();
                mc_tof1_pz_ds = momentum.z();
                mc_tof1_p_ds = TMath::Sqrt(mc_tof1_px_ds*mc_tof1_px_ds + mc_tof1_py_ds*mc_tof1_py_ds + mc_tof1_pz_ds*mc_tof1_pz_ds);
                mc_tof1_xPrime_ds = mc_tof1_px_ds/mc_tof1_pz_ds;
                mc_tof1_yPrime_ds = mc_tof1_py_ds/mc_tof1_pz_ds;
                mc_tof1_t_ds = hit.GetTime();
            }
            else if(position.z() >= mc_tku_s1_z - dz && position.z() <= mc_tku_s1_z + dz){
                if(testping){
                    std::cout << "ping, tku_s1 with z = " << position.z() << "\n";
                }
                momentum = hit.GetMomentum();
                
                mc_tku_s1_x = position.x();
                mc_tku_s1_y = position.y();
                mc_tku_s1_z = position.z();
                mc_tku_s1_px = momentum.x();
                mc_tku_s1_py = momentum.y();
                mc_tku_s1_pz = momentum.z();
                mc_tku_s1_p = TMath::Sqrt(mc_tku_s1_px*mc_tku_s1_px + mc_tku_s1_py*mc_tku_s1_py + mc_tku_s1_pz*mc_tku_s1_pz);
                field = hit.GetBField();
                mc_tku_s1_B = TMath::Sqrt(field.x()*field.x() + field.y()*field.y() + field.z()*field.z());

                std::cout << "Field at TKU station 1 = " << mc_tku_s1_B << "\n";
                
                mc_particle_id = hit.GetParticleId();
            }
            else if(position.z() >= mc_tku_s2_z - dz && position.z() <= mc_tku_s2_z + dz){
                if(testping){
                    std::cout << "ping, tku_s2 with z = " << position.z() << "\n";
                }
                momentum = hit.GetMomentum();
                
                mc_tku_s2_x = position.x();
                mc_tku_s2_y = position.y();
                mc_tku_s2_z = position.z();
                mc_tku_s2_px = momentum.x();
                mc_tku_s2_py = momentum.y();
                mc_tku_s2_pz = momentum.z();
                mc_tku_s2_p = TMath::Sqrt(mc_tku_s2_px*mc_tku_s2_px + mc_tku_s2_py*mc_tku_s2_py + mc_tku_s2_pz*mc_tku_s2_pz);
                field = hit.GetBField();
                mc_tku_s2_B = TMath::Sqrt(field.x()*field.x() + field.y()*field.y() + field.z()*field.z());

                std::cout << "Field at TKU station 2 = " << mc_tku_s2_B << "\n";
            }
            else if(position.z() >= mc_tku_s3_z - dz && position.z() <= mc_tku_s3_z + dz){
                if(testping){
                    std::cout << "ping, tku_s3 with z = " << position.z() << "\n";
                }
                momentum = hit.GetMomentum();
                
                mc_tku_s3_x = position.x();
                mc_tku_s3_y = position.y();
                mc_tku_s3_z = position.z();
                mc_tku_s3_px = momentum.x();
                mc_tku_s3_py = momentum.y();
                mc_tku_s3_pz = momentum.z();
                mc_tku_s3_p = TMath::Sqrt(mc_tku_s3_px*mc_tku_s3_px + mc_tku_s3_py*mc_tku_s3_py + mc_tku_s3_pz*mc_tku_s3_pz);
                field = hit.GetBField();
                mc_tku_s3_B = TMath::Sqrt(field.x()*field.x() + field.y()*field.y() + field.z()*field.z());

                std::cout << "Field at TKU station 3 = " << mc_tku_s3_B << "\n";
            }
            else if(position.z() >= mc_tku_s4_z - dz && position.z() <= mc_tku_s4_z + dz){
                if(testping){
                    std::cout << "ping, tku_s4 with z = " << position.z() << "\n";
                }
                momentum = hit.GetMomentum();
                
                mc_tku_s4_x = position.x();
                mc_tku_s4_y = position.y();
                mc_tku_s4_z = position.z();
                mc_tku_s4_px = momentum.x();
                mc_tku_s4_py = momentum.y();
                mc_tku_s4_pz = momentum.z();
                mc_tku_s4_p = TMath::Sqrt(mc_tku_s4_px*mc_tku_s4_px + mc_tku_s4_py*mc_tku_s4_py + mc_tku_s4_pz*mc_tku_s4_pz);
                field = hit.GetBField();
                mc_tku_s4_B = TMath::Sqrt(field.x()*field.x() + field.y()*field.y() + field.z()*field.z());

                std::cout << "Field at TKU station 4 = " << mc_tku_s4_B << "\n";
            }
            else if(position.z() >= mc_tku_s5_z - dz && position.z() <= mc_tku_s5_z + dz){
                if(testping){
                    std::cout << "ping, tku_s5 with z = " << position.z() << "\n";
                }
                momentum = hit.GetMomentum();
                
                mc_tku_s5_x = position.x();
                mc_tku_s5_y = position.y();
                mc_tku_s5_z = position.z();
                mc_tku_s5_px = momentum.x();
                mc_tku_s5_py = momentum.y();
                mc_tku_s5_pz = momentum.z();
                mc_tku_s5_p = TMath::Sqrt(mc_tku_s5_px*mc_tku_s5_px + mc_tku_s5_py*mc_tku_s5_py + mc_tku_s5_pz*mc_tku_s5_pz);
                field = hit.GetBField();
                mc_tku_s5_B = TMath::Sqrt(field.x()*field.x() + field.y()*field.y() + field.z()*field.z());

                std::cout << "Field at TKU station 5 = " << mc_tku_s5_B << "\n";
            }
            else if(position.z() >= mc_diffuser_z1 - diffuser_dz && position.z() <= mc_diffuser_z1 + diffuser_dz){
                if(testping){
                    std::cout << "ping, diff1 with z = " << position.z() << "\n";
                }
                std::cout << "ping, diff1 with z = " << position.z() << " and testping = " << testping << "\n";

                momentum = hit.GetMomentum();
                
                mc_diffuser_x1 = position.x();
                mc_diffuser_y1 = position.y();
                mc_diffuser_z1 = position.z();
                mc_diffuser_px1 = momentum.x();
                mc_diffuser_py1 = momentum.y();
                mc_diffuser_pz1 = momentum.z();
                mc_diffuser_p1 = TMath::Sqrt(mc_diffuser_px1*mc_diffuser_px1 + mc_diffuser_py1*mc_diffuser_py1 + mc_diffuser_pz1*mc_diffuser_pz1);
                field = hit.GetBField();
                mc_diffuser_B1 = TMath::Sqrt(field.x()*field.x() + field.y()*field.y() + field.z()*field.z());
            }
            else if(position.z() >= mc_diffuser_z2 - diffuser_dz && position.z() <= mc_diffuser_z2 + diffuser_dz){
                if(testping){
                    std::cout << "ping, diff2 with z = " << position.z() << "\n";
                }
                momentum = hit.GetMomentum();
                
                mc_diffuser_x2 = position.x();
                mc_diffuser_y2 = position.y();
                mc_diffuser_z2 = position.z();
                mc_diffuser_px2 = momentum.x();
                mc_diffuser_py2 = momentum.y();
                mc_diffuser_pz2 = momentum.z();
                mc_diffuser_p2 = TMath::Sqrt(mc_diffuser_px2*mc_diffuser_px2 + mc_diffuser_py2*mc_diffuser_py2 + mc_diffuser_pz2*mc_diffuser_pz2);
                field = hit.GetBField();
                mc_diffuser_B2 = TMath::Sqrt(field.x()*field.x() + field.y()*field.y() + field.z()*field.z());
            }
            else if(position.z() >= mc_diffuser_z3 - diffuser_dz && position.z() <= mc_diffuser_z3 + diffuser_dz){
                if(testping){
                    std::cout << "ping, diff3 with z = " << position.z() << "\n";
                }
                momentum = hit.GetMomentum();
                
                mc_diffuser_x3 = position.x();
                mc_diffuser_y3 = position.y();
                mc_diffuser_z3 = position.z();
                mc_diffuser_px3 = momentum.x();
                mc_diffuser_py3 = momentum.y();
                mc_diffuser_pz3 = momentum.z();
                mc_diffuser_p3 = TMath::Sqrt(mc_diffuser_px3*mc_diffuser_px3 + mc_diffuser_py3*mc_diffuser_py3 + mc_diffuser_pz3*mc_diffuser_pz3);
                field = hit.GetBField();
                mc_diffuser_B3 = TMath::Sqrt(field.x()*field.x() + field.y()*field.y() + field.z()*field.z());
            }
        }
        
        
        
        
        
        
        
        outputTree->Fill();
    }
}


void BetterReadMAUS::write_mc_to_file(){
    outputTree->Fill();
}



void BetterReadMAUS::read_globals(){
    double dz = 0.1;
    
    std::cout << "Globals...\n";

    std::vector<MAUS::DataStructure::Global::PrimaryChain*>* pchains = global_event->get_primary_chains();
    std::vector<MAUS::DataStructure::Global::PrimaryChain*>::iterator pchains_iterator;


    MAUS::DataStructure::Global::DetectorPoint diffuser_plane = MAUS::DataStructure::Global::kVirtual;
    for(pchains_iterator = pchains->begin(); pchains_iterator != pchains->end(); ++ pchains_iterator){
        
        std::cout << "iterating?\n";
        MAUS::DataStructure::Global::PrimaryChain* chain = (*pchains_iterator);

        // chain types 1 & 3 are upstream tracks that have been matched through to
        // the downstream half of the experiment or are just upstream only.
        if(chain->get_chain_type() == 1 || chain->get_chain_type() == 3){

            std::vector<MAUS::DataStructure::Global::Track*> some_tracks = chain->GetMatchedTracks();

            std::vector<MAUS::DataStructure::Global::Track*>::iterator some_track_iterator;

            for(some_track_iterator = some_tracks.begin(); some_track_iterator < some_tracks.end(); ++some_track_iterator){
                MAUS::DataStructure::Global::Track* a_track = (*some_track_iterator);

                if(a_track != NULL){
                    if(a_track->get_pid() == -13 || a_track->get_pid() == 13){


                        std::vector< const MAUS::DataStructure::Global::TrackPoint * > track_points = a_track->GetTrackPoints(diffuser_plane);
                        for(int p = 0; p < track_points.size(); p++){
                            TLorentzVector a_track_mom = track_points.at(p)->get_momentum();
                            TLorentzVector a_track_pos = track_points.at(p)->get_position();
                            
                            std::cout << "Looking for z = " << diffuser_z << " and have z = " << a_track_pos.Z();
                            
                            if(a_track_pos.Z() >= diffuser_z-dz && a_track_pos.Z() <= diffuser_z+dz){
                                std::cout << "-------------- FOUND IT!!!\n";
                                diffuser_x = a_track_pos.X();
                                diffuser_y = a_track_pos.Y();
                                diffuser_px=  a_track_mom.X();
                                diffuser_py = a_track_mom.Y();
                                diffuser_pz = a_track_mom.Z();
                                
                                diffuser_r = TMath::Sqrt(diffuser_x*diffuser_x + diffuser_y*diffuser_y);
                                diffuser_pt = TMath::Sqrt(diffuser_px*diffuser_px + diffuser_py*diffuser_py);
                                diffuser_p = TMath::Sqrt(diffuser_px*diffuser_px + diffuser_py*diffuser_py + diffuser_pz*diffuser_pz);
                            }
                            else{
                                std::cout << "\n";
                            }
                        }
                    }
                }
            }
        }
    }
}




void BetterReadMAUS::check_cuts(){
    
    // time of flight
    if(time_of_flight >= min_tof && time_of_flight <= max_tof){
        cut_tof = 1;
    }
    else{
        cut_tof = 0;
    }
    
    // 'normalised' time of flight (relative to mean electron time of flight)
    if(normalised_time_of_flight >= min_normalised_tof && normalised_time_of_flight <= max_normalised_tof){
        cut_normalised_tof = 1;
    }
    else{
        cut_normalised_tof = 0;
    }
    
    // diffuser aperture
    if(diffuser_r <= max_diffuser){
        cut_diffuser = 1;
    }
    else{
        cut_diffuser = 0;
    }
    
    // chi-square per degree of freedom
    if(TKU_chiSquare_per_ndof <= max_chindof){
        cut_chindof = 1;
    }
    else{
        cut_chindof = 0;
    }
    
    // 'banana' cut on time of flight and tku total momentum
    double m = 105.668;
    double L = TOF1_z - TOF0_z;
    L = L * 1.0e-3;
    
    double t_min, t_max;
    
    double p_min = TKU_plane1_p + min_dP;
    double p_max = TKU_plane1_p + max_dP;
    
    t_min = L * TMath::Sqrt(m*m + p_min*p_min) / (TMath::C()*p_min);
    t_max = L * TMath::Sqrt(m*m + p_max*p_max) / (TMath::C()*p_max);
    
    t_min = t_min/1.0e-9; // ns
    t_max = t_max/1.0e-9;
    
    
    if(time_of_flight <= t_min && time_of_flight >= t_max){
        cut_tof_p = 1;
    }
    else{
        cut_tof_p = 0;
    }

    
    // TKU radius cut
    if(muon_stays_in_tracker() == true){
        cut_tku_r = 1;
    }
    else{
        cut_tku_r = 0;
    }
    
    
    // one spacepoint at TOF0
    if(number_of_TOF0_spacepoints == 1){
        cut_one_tof0 = 1;
    }
    else{
        cut_one_tof0 = 0;
    }

    // one spacepoint at TOF1
    if(number_of_TOF1_spacepoints == 1){
        cut_one_tof1 = 1;
    }
    else{
        cut_one_tof1 = 0;
    }
    
    // one track in TKU
    if(number_of_TKU_tracks == 1){
        cut_one_track = 1;
    }
    else{
        cut_one_track = 0;
    }

    
    
    if(cut_tof == 1
       && cut_normalised_tof == 1
       && cut_diffuser == 1
       && cut_chindof == 1
       && cut_one_tof0 == 1
       && cut_one_tof1 == 1
       && cut_one_track == 1
       && cut_tku_r == 1
       && cut_tof_p == 1){
        cut_allPassed = 1;
    }
    else{
        cut_allPassed = 0;
    }
    
    if(cut_tof == 1
       && cut_normalised_tof == 1
       && cut_chindof == 1
       && cut_one_tof0 == 1
       && cut_one_tof1 == 1
       && cut_one_track == 1
       && cut_tku_r == 1
       && cut_tof_p == 1){
        cut_allButDiffuserPassed = 1;
    }
    else{
        cut_allButDiffuserPassed = 0;
    }
}




bool BetterReadMAUS::muon_stays_in_tracker(){
    // Propagate from TKU station 5 to 1 first, checking for r > 150mm
    bool stays_inside_tracker = true;
    double x_i = TKU_plane5_x;
    double y_i = TKU_plane5_y;
    double z_i = TKU_plane5_z;
    double px_i = TKU_plane5_px;
    double py_i = TKU_plane5_py;
    double pz_i = TKU_plane5_pz;
    
    double dz = 1.0;
    double B = 3.998;
    double u = -0.3*B;
    double kappa = 1.0/pz_i;
    double theta = u * kappa * dz;
    double dPz = -2.8/1100.0; // make this + for propagating from 1-->5
    
    double target_z = TKU_plane1_z;
    
    double r;
    
    
    
    while(z_i < target_z){
        x_i = x_i + (px_i/u)*TMath::Sin(theta) - (py_i/u)*(1.0 - TMath::Cos(theta));
        y_i = y_i + (py_i/u)*TMath::Sin(theta) + (px_i/u)*(1.0 - TMath::Cos(theta));
        z_i = z_i + dz;
        px_i = px_i*TMath::Cos(theta) - py_i*TMath::Sin(theta);
        py_i = py_i*TMath::Cos(theta) + px_i*TMath::Sin(theta);
        pz_i = pz_i + dPz*dz;
        
        r = TMath::Sqrt(x_i*x_i + y_i*y_i);
        if(r >= max_tku_radius){
            stays_inside_tracker = false;
            return stays_inside_tracker;
        }
    }
    
    
    // and if that didn't go outside the tracker, check by propagating backwards:
    x_i = TKU_plane1_x;
    y_i = TKU_plane1_y;
    z_i = TKU_plane1_z;
    px_i = TKU_plane1_px;
    py_i = TKU_plane1_py;
    pz_i = TKU_plane1_pz;
    
    dz = -1.0;
    B = 3.998;
    u = -0.3*B;
    kappa = 1.0/pz_i;
    theta = u * kappa * dz;
    dPz = 2.8/1100.0; // make this + for propagating from 1-->5
    
    target_z = TKU_plane5_z;
    
    
    while(z_i > target_z){
        x_i = x_i + (px_i/u)*TMath::Sin(theta) - (py_i/u)*(1.0 - TMath::Cos(theta));
        y_i = y_i + (py_i/u)*TMath::Sin(theta) + (px_i/u)*(1.0 - TMath::Cos(theta));
        z_i = z_i + dz;
        px_i = px_i*TMath::Cos(theta) - py_i*TMath::Sin(theta);
        py_i = py_i*TMath::Cos(theta) + px_i*TMath::Sin(theta);
        pz_i = pz_i + dPz*dz;
        
        r = TMath::Sqrt(x_i*x_i + y_i*y_i);
        if(r >= max_tku_radius){
            stays_inside_tracker = false;
            return stays_inside_tracker;
        }
    }
    
    
    // and if none of that triggers:
    return stays_inside_tracker;
}

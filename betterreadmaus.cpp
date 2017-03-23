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
    reset_rogers_tracking();
    
    reconstructed_event_number = -1;
    spill_number = -1;
    
    goodParticle = 0;
    goodRaynerReconstruction = 0;
    goodTimeOfFlight = 0;
    all_detectors_hit = 0;
    good_mass_cut = 0;
    
    particle_mass = TMath::Infinity();
}

void BetterReadMAUS::reset_TOF0_variables(){
    TOF0_goodPMTPosition = 0;
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
    only_hit_at_TOF0 = 0;
    
}

void BetterReadMAUS::reset_TOF1_variables(){
    TOF1_goodPMTPosition = 0;
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
    
    only_hit_at_TOF1 = 0;
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
    
    TKU_plane5_x_error = TMath::Infinity();
    TKU_plane5_y_error = TMath::Infinity();
    TKU_plane5_px_error = TMath::Infinity();
    TKU_plane5_py_error = TMath::Infinity();
    TKU_plane5_kappa_error = TMath::Infinity();
    
    TKU_Pvalue = TMath::Infinity();
    TKU_chiSquare = TMath::Infinity();
    TKU_patternRecognition_dipAngle = TMath::Infinity();
    TKU_patternRecognition_R = TMath::Infinity();
    TKU_patternRecognition_x0 = TMath::Infinity();
    TKU_patternRecognition_y0 = TMath::Infinity();
    
    TKU_plane1_pull = TMath::Infinity();
    TKU_plane2_pull = TMath::Infinity();
    TKU_plane3_pull = TMath::Infinity();
    TKU_plane4_pull = TMath::Infinity();
    TKU_plane5_pull = TMath::Infinity();
    
    TKU_station_hits = 0;
    TKU_goodParticle = 0;
    only_track_in_TKU = 0;
    TKU_charge = -5;
    TKU_assumed_field = TMath::Infinity();
    goodPValue = 0;
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
    
    outputTree->Branch("TKU_s1_x", &TKU_plane1_x, "TKU_s1_x/D"); // station 1
    outputTree->Branch("TKU_s1_y", &TKU_plane1_y, "TKU_s1_y/D");
    outputTree->Branch("TKU_s1_z", &TKU_plane1_z, "TKU_s1_z/D");
    outputTree->Branch("TKU_s1_px", &TKU_plane1_px, "TKU_s1_px/D");
    outputTree->Branch("TKU_s1_py", &TKU_plane1_py, "TKU_s1_py/D");
    outputTree->Branch("TKU_s1_pz", &TKU_plane1_pz, "TKU_s1_pz/D");
    outputTree->Branch("TKU_s1_p", &TKU_plane1_p, "TKU_s1_p/D");
    outputTree->Branch("TKU_s1_pull", &TKU_plane1_pull, "TKU_s1_pull/D");
    outputTree->Branch("TKU_s1_x_error", &TKU_plane1_x_error, "TKU_s1_x_error/D");
    outputTree->Branch("TKU_s1_y_error", &TKU_plane1_y_error, "TKU_s1_y_error/D");
    outputTree->Branch("TKU_s1_px_error", &TKU_plane1_px_error, "TKU_s1_px_error/D");
    outputTree->Branch("TKU_s1_py_error", &TKU_plane1_py_error, "TKU_s1_py_error/D");
    outputTree->Branch("TKU_s1_kappa_error", &TKU_plane1_kappa_error, "TKU_s1_kappa_error/D");
    
    outputTree->Branch("TKU_s2_x", &TKU_plane2_x, "TKU_s2_x/D"); // station 1
    outputTree->Branch("TKU_s2_y", &TKU_plane2_y, "TKU_s2_y/D");
    outputTree->Branch("TKU_s2_z", &TKU_plane2_z, "TKU_s2_z/D");
    outputTree->Branch("TKU_s2_px", &TKU_plane2_px, "TKU_s2_px/D");
    outputTree->Branch("TKU_s2_py", &TKU_plane2_py, "TKU_s2_py/D");
    outputTree->Branch("TKU_s2_pz", &TKU_plane2_pz, "TKU_s2_pz/D");
    outputTree->Branch("TKU_s2_p", &TKU_plane2_p, "TKU_s2_p/D");
    outputTree->Branch("TKU_s2_pull", &TKU_plane2_pull, "TKU_s2_pull/D");
    outputTree->Branch("TKU_s2_x_error", &TKU_plane2_x_error, "TKU_s2_x_error/D");
    outputTree->Branch("TKU_s2_y_error", &TKU_plane2_y_error, "TKU_s2_y_error/D");
    outputTree->Branch("TKU_s2_px_error", &TKU_plane2_px_error, "TKU_s2_px_error/D");
    outputTree->Branch("TKU_s2_py_error", &TKU_plane2_py_error, "TKU_s2_py_error/D");
    outputTree->Branch("TKU_s2_kappa_error", &TKU_plane2_kappa_error, "TKU_s2_kappa_error/D");
    
    outputTree->Branch("TKU_s3_x", &TKU_plane3_x, "TKU_s3_x/D"); // station 1
    outputTree->Branch("TKU_s3_y", &TKU_plane3_y, "TKU_s3_y/D");
    outputTree->Branch("TKU_s3_z", &TKU_plane3_z, "TKU_s3_z/D");
    outputTree->Branch("TKU_s3_px", &TKU_plane3_px, "TKU_s3_px/D");
    outputTree->Branch("TKU_s3_py", &TKU_plane3_py, "TKU_s3_py/D");
    outputTree->Branch("TKU_s3_pz", &TKU_plane3_pz, "TKU_s3_pz/D");
    outputTree->Branch("TKU_s3_p", &TKU_plane3_p, "TKU_s3_p/D");
    outputTree->Branch("TKU_s3_pull", &TKU_plane3_pull, "TKU_s3_pull/D");
    outputTree->Branch("TKU_s3_x_error", &TKU_plane3_x_error, "TKU_s3_x_error/D");
    outputTree->Branch("TKU_s3_y_error", &TKU_plane3_y_error, "TKU_s3_y_error/D");
    outputTree->Branch("TKU_s3_px_error", &TKU_plane3_px_error, "TKU_s3_px_error/D");
    outputTree->Branch("TKU_s3_py_error", &TKU_plane3_py_error, "TKU_s3_py_error/D");
    outputTree->Branch("TKU_s3_kappa_error", &TKU_plane3_kappa_error, "TKU_s3_kappa_error/D");
    
    outputTree->Branch("TKU_s4_x", &TKU_plane4_x, "TKU_s4_x/D"); // station 1
    outputTree->Branch("TKU_s4_y", &TKU_plane4_y, "TKU_s4_y/D");
    outputTree->Branch("TKU_s4_z", &TKU_plane4_z, "TKU_s4_z/D");
    outputTree->Branch("TKU_s4_px", &TKU_plane4_px, "TKU_s4_px/D");
    outputTree->Branch("TKU_s4_py", &TKU_plane4_py, "TKU_s4_py/D");
    outputTree->Branch("TKU_s4_pz", &TKU_plane4_pz, "TKU_s4_pz/D");
    outputTree->Branch("TKU_s4_p", &TKU_plane4_p, "TKU_s4_p/D");
    outputTree->Branch("TKU_s4_pull", &TKU_plane4_pull, "TKU_s4_pull/D");
    outputTree->Branch("TKU_s4_x_error", &TKU_plane4_x_error, "TKU_s4_x_error/D");
    outputTree->Branch("TKU_s4_y_error", &TKU_plane4_y_error, "TKU_s4_y_error/D");
    outputTree->Branch("TKU_s4_px_error", &TKU_plane4_px_error, "TKU_s4_px_error/D");
    outputTree->Branch("TKU_s4_py_error", &TKU_plane4_py_error, "TKU_s4_py_error/D");
    outputTree->Branch("TKU_s4_kappa_error", &TKU_plane4_kappa_error, "TKU_s4_kappa_error/D");
    
    outputTree->Branch("TKU_s5_x", &TKU_plane5_x, "TKU_s5_x/D"); // station 1
    outputTree->Branch("TKU_s5_y", &TKU_plane5_y, "TKU_s5_y/D");
    outputTree->Branch("TKU_s5_z", &TKU_plane5_z, "TKU_s5_z/D");
    outputTree->Branch("TKU_s5_px", &TKU_plane5_px, "TKU_s5_px/D");
    outputTree->Branch("TKU_s5_py", &TKU_plane5_py, "TKU_s5_py/D");
    outputTree->Branch("TKU_s5_pz", &TKU_plane5_pz, "TKU_s5_pz/D");
    outputTree->Branch("TKU_s5_p", &TKU_plane5_p, "TKU_s5_p/D");
    outputTree->Branch("TKU_s5_pull", &TKU_plane5_pull, "TKU_s5_pull/D");
    outputTree->Branch("TKU_s5_x_error", &TKU_plane5_x_error, "TKU_s5_x_error/D");
    outputTree->Branch("TKU_s5_y_error", &TKU_plane5_y_error, "TKU_s5_y_error/D");
    outputTree->Branch("TKU_s5_px_error", &TKU_plane5_px_error, "TKU_s5_px_error/D");
    outputTree->Branch("TKU_s5_py_error", &TKU_plane5_py_error, "TKU_s5_py_error/D");
    outputTree->Branch("TKU_s5_kappa_error", &TKU_plane5_kappa_error, "TKU_s5_kappa_error/D");
    
    outputTree->Branch("TKU_PValue", &TKU_Pvalue, "TKU_PValue/D");
    outputTree->Branch("TKU_chiSquare", &TKU_chiSquare, "TKU_chiSquare/D");
    outputTree->Branch("TKU_pattRec_r", &TKU_patternRecognition_R, "TKU_pattRec_r/D");
    outputTree->Branch("TKU_pattRec_dipAngle", &TKU_patternRecognition_dipAngle, "TKU_pattRec_dipAngle/D");
    outputTree->Branch("TKU_pattRec_x0", &TKU_patternRecognition_x0, "TKU_pattRec_x0/D");
    outputTree->Branch("TKU_pattRec_y0", &TKU_patternRecognition_y0, "TKU_pattRec_y0/D");
    outputTree->Branch("TKU_good", &TKU_goodParticle, "TKU_good/I");
    outputTree->Branch("TKU_mass", &particle_mass, "TKU_mass/D");
    outputTree->Branch("TKU_station_hits", &TKU_station_hits, "TKU_station_hits/I");
    outputTree->Branch("TKU_charge", &TKU_charge, "TKU_charge/I");
    
    
    
    
    
    
    
    
    
    // Rogers' particle tracking
    if(!rogersTrackingFileName.isEmpty()){
        outputTree->Branch("rogers_tracking_tof01_us", &rogers_tof01_us, "rogers_tracking_tof01_us/D");
        outputTree->Branch("rogers_tracking_x_tof1_us", &rogers_x_tof1_us, "rogers_tracking_x_tof1_us/D");
        outputTree->Branch("rogers_tracking_y_tof1_us", &rogers_y_tof1_us, "rogers_tracking_y_tof1_us/D");
        outputTree->Branch("rogers_tracking_px_tof1_us", &rogers_px_tof1_us, "rogers_tracking_px_tof1_us/D");
        outputTree->Branch("rogers_tracking_py_tof1_us", &rogers_py_tof1_us, "rogers_tracking_py_tof1_us/D");
        outputTree->Branch("rogers_tracking_pz_tof1_us", &rogers_pz_tof1_us, "rogers_tracking_pz_tof1_us/D");
        
        outputTree->Branch("rogers_tracking_tof01_ds", &rogers_tof01_ds, "rogers_tracking_tof01_ds/D");
        outputTree->Branch("rogers_tracking_x_tof1_ds", &rogers_x_tof1_ds, "rogers_tracking_x_tof1_ds/D");
        outputTree->Branch("rogers_tracking_y_tof1_ds", &rogers_y_tof1_ds, "rogers_tracking_y_tof1_ds/D");
        outputTree->Branch("rogers_tracking_px_tof1_ds", &rogers_px_tof1_ds, "rogers_tracking_px_tof1_ds/D");
        outputTree->Branch("rogers_tracking_py_tof1_ds", &rogers_py_tof1_ds, "rogers_tracking_py_tof1_ds/D");
        outputTree->Branch("rogers_tracking_pz_tof1_ds", &rogers_pz_tof1_ds, "rogers_tracking_pz_tof1_ds/D");
        
        outputTree->Branch("rogers_tracking_x_diffuser1", &rogers_x_diffuser1, "rogers_tracking_x_diffuser1/D");
        outputTree->Branch("rogers_tracking_y_diffuser1", &rogers_y_diffuser1, "rogers_tracking_y_diffuser1/D");
        outputTree->Branch("rogers_tracking_z_diffuser1", &rogers_z_diffuser1, "rogers_tracking_z_diffuser1/D");
        outputTree->Branch("rogers_tracking_px_diffuser1", &rogers_px_diffuser1, "rogers_tracking_px_diffuser1/D");
        outputTree->Branch("rogers_tracking_py_diffuser1", &rogers_py_diffuser1, "rogers_tracking_py_diffuser1/D");
        outputTree->Branch("rogers_tracking_pz_diffuser1", &rogers_pz_diffuser1, "rogers_tracking_pz_diffuser1/D");
        
        outputTree->Branch("rogers_tracking_x_diffuser2", &rogers_x_diffuser2, "rogers_tracking_x_diffuser2/D");
        outputTree->Branch("rogers_tracking_y_diffuser2", &rogers_y_diffuser2, "rogers_tracking_y_diffuser2/D");
        outputTree->Branch("rogers_tracking_z_diffuser2", &rogers_z_diffuser2, "rogers_tracking_z_diffuser2/D");
        outputTree->Branch("rogers_tracking_px_diffuser2", &rogers_px_diffuser2, "rogers_tracking_px_diffuser2/D");
        outputTree->Branch("rogers_tracking_py_diffuser2", &rogers_py_diffuser2, "rogers_tracking_py_diffuser2/D");
        outputTree->Branch("rogers_tracking_pz_diffuser2", &rogers_pz_diffuser2, "rogers_tracking_pz_diffuser2/D");
        
        
        outputTree->Branch("rogers_tracking_x_diffuser3", &rogers_x_diffuser3, "rogers_tracking_x_diffuser3/D");
        outputTree->Branch("rogers_tracking_y_diffuser3", &rogers_y_diffuser3, "rogers_tracking_y_diffuser3/D");
        outputTree->Branch("rogers_tracking_z_diffuser3", &rogers_z_diffuser3, "rogers_tracking_z_diffuser3/D");
        outputTree->Branch("rogers_tracking_px_diffuser3", &rogers_px_diffuser3, "rogers_tracking_px_diffuser3/D");
        outputTree->Branch("rogers_tracking_py_diffuser3", &rogers_py_diffuser3, "rogers_tracking_py_diffuser3/D");
        outputTree->Branch("rogers_tracking_pz_diffuser3", &rogers_pz_diffuser3, "rogers_tracking_pz_diffuser3/D");
    }
    
    /* cuts: these will be 0 or 1 depending on whether the fail (0) or pass (1) the cut
     *
     *    cut_TOF0_goodPMTPosition (aka TOF0_goodPMTPosition), does this particle go through slabs that have PMT-position calibrations
     *    cut_TOF1_goodPMTPosition (aka TOF1_goodPMTPosition), ditto
     *    cut_goodRaynerReconstruction (aka TOF0_goodRaynerReconstruction), does this particle get a good result from M. Rayner's reconstruction process?
     */
    outputTree->Branch("cut_TOF0_goodPMTPosition", &TOF0_goodPMTPosition, "cut_TOF0_goodPMTPosition/I");
    outputTree->Branch("cut_TOF1_goodPMTPosition", &TOF1_goodPMTPosition, "cut_TOF1_goodPMTPosition/I");
    outputTree->Branch("cut_goodRaynerReconstruction", &goodRaynerReconstruction, "cut_goodRaynerReconstruction/I");
    outputTree->Branch("cut_TKU_hitAllStations", &TKU_goodParticle, "cut_TKU_hitAllStations/I");
    outputTree->Branch("cut_TimeOfFlight", &goodTimeOfFlight, "cut_TimeOfFlight/I");
    outputTree->Branch("cut_hit_all_detectors", &all_detectors_hit, "cut_hit_all_detectors/I");
    outputTree->Branch("cut_TOF0_singleHit", &only_hit_at_TOF0, "cut_TOF0_singleHit/I");
    outputTree->Branch("cut_TOF1_singleHit", &only_hit_at_TOF1, "cut_TOF1_singleHit/I");
    outputTree->Branch("cut_TKU_singleTrack", &only_track_in_TKU, "cut_TKU_singleTrack/I");
    outputTree->Branch("cut_TKU_PValue", &goodPValue, "cut_TKU_PValue/I");
    //outputTree->Branch("cut_muon_mass", &good_mass_cut, "cut_muon_mass/I");
    outputTree->Branch("cut_momentum_loss", &good_momentum_loss_cut, "cut_momentum_loss/I");
    
    if(!rogersTrackingFileName.isEmpty()){
        outputTree->Branch("cut_diffuser", &cut_diffuser, "cut_diffuser/I");
    }
    
    // remember to update the selection for goodParticle if more cuts are added above
    outputTree->Branch("cut_allPassed", &goodParticle, "cut_allPassed/I");
}







void BetterReadMAUS::Read(QString fileToOpen, QString fileToSaveAs, QString calibrationConstantsFile, QString trackingFileName){
    /*
     *  1. read the calibration file
     *  2. create the output root file
     *  3. read the maus input file
     *      a. Get a spacepoint at TOF0, reconstruct its (x, y) using pmt times
     *      b. Get a spacepoint at TOF1, do the same
     *      c. Get a track in the upstream tracker
     */
    rogersTrackingFileName = trackingFileName;
    file_line = 0;
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
        
        if(tof_event != NULL){
            // there are hits at TOFs, estimate pz from them
            particle_at_TOF0();
            particle_at_TOF1();
        }
        
        if(scifi_event != NULL){
            particle_at_tracker(); // get tracker info
        }
        
        calculate_particle_mass();
        good_momentum_loss_cut = check_momentum_loss();
        
        if(TOF0_goodPMTPosition == 1 && TOF1_goodPMTPosition == 1){
            // reconstruct momentum at TOFs
            reconstruct_TOF_momentum();
        }
        
        if(TOF0_goodPMTPosition == 1 && TOF1_goodPMTPosition == 1
           && TKU_goodParticle ==1){
            all_detectors_hit = 1;
        }
        else{
            all_detectors_hit = 0;
        }
        
        if(particle_in_mass_range()){
            good_mass_cut = 1;
        }
        else{
            good_mass_cut = 0;
        }
        
        /*
         * Particles pass ALL cuts if:
         *      -- all_detectors_hit = 1 (TOF0_goodPMTPosition, TOF1_goodPMTPosition, TKU_goodParticle = 1)
         *      -- goodRaynerReconstruction = 1
         *      -- only_hit_at_TOF0 = 1
         *      -- only_hit_at_TOF1 = 1
         *      -- only_track_in_TKU = 1
         *      -- goodPValue = 1
         *      -- good_mass_cut = 1
         */
        if(goodRaynerReconstruction ==1 && goodTimeOfFlight == 1
           && all_detectors_hit == 1    && only_hit_at_TOF0 == 1
           && only_hit_at_TOF1 == 1     && only_track_in_TKU == 1
           && goodPValue == 1 && good_momentum_loss_cut == 1){
            goodParticle = 1;
        }
        else{
            goodParticle = 0;
        }
        
        if(all_detectors_hit == 1 && !rogersTrackingFileName.isEmpty())
        ReadRogersExtrapolation(spill_number, reconstructed_event_number);
        
        
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
    
    // 1. Loop over TOF0 space points:
    //for(size_t i = 0; i < space_points.GetTOF0SpacePointArray().size(); ++i){
    if(space_points.GetTOF0SpacePointArray().size() >=1){
        reset_TOF0_variables();  // each new space point should be a new set of slab hits
        
        if(space_points.GetTOF0SpacePointArray().size() == 1){
            only_hit_at_TOF0 = 1;
        }
        else{
            only_hit_at_TOF0 = 0;
        }
        
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
    
    
    // 1. Loop over TOF1 space points:
    
    //  for(size_t i = 0; i < space_points.GetTOF1SpacePointArray().size(); ++i){
    reset_TOF1_variables();  // each new space point should be a new set of slab hits
    horizontalHit = -1;
    verticalHit = -1;
    
    if(space_points.GetTOF1SpacePointArray().size() >= 1){
        reset_TOF1_variables();  // each new space point should be a new set of slab hits
        horizontalHit = -1;
        verticalHit = -1;
        
        if(space_points.GetTOF1SpacePointArray().size() == 1){
            only_hit_at_TOF1 = 1;
        }
        else{
            only_hit_at_TOF1 = 0;
        }
        
        
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
    int tracker, station;
    MAUS::ThreeVector position;
    
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
            
            TKU_station_hits = pr_track->get_num_points();
        }
    }
    
    if(tracks.size() == 1){
        only_track_in_TKU = 1;
    }
    else{
        only_track_in_TKU = 0;
    }
    
    //for(track_iter = tracks.begin(); track_iter != tracks.end(); ++track_iter){ // use if looking at all tracks
    if(tracks.size() != 0){ // use if only want to take the first track
        track_iter = tracks.begin();
        std::vector<MAUS::SciFiTrackPoint*> track_points = (*track_iter)->scifitrackpoints();
        std::vector<MAUS::SciFiTrackPoint*>::iterator track_point_iter;
        
        TKU_Pvalue = (*track_iter)->P_value();
        TKU_chiSquare = (*track_iter)->chi2();
        TKU_charge = (*track_iter)->charge();
        
        for(track_point_iter = track_points.begin(); track_point_iter != track_points.end(); ++track_point_iter){
            MAUS::SciFiTrackPoint* point = (*track_point_iter);
            tracker = point->tracker();
            station = point->station();
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
                    
                    TKU_plane1_pull = point->pull();
                    
                    std::vector<double> errors = point->errors();
                    TKU_plane1_x_error = errors[0];
                    TKU_plane1_px_error = errors[1];
                    TKU_plane1_y_error = errors[2];
                    TKU_plane1_py_error = errors[3];
                    TKU_plane1_kappa_error = errors[4];
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
                    
                    TKU_plane2_pull = point->pull();
                    
                    std::vector<double> errors = point->errors();
                    TKU_plane2_x_error = errors[0];
                    TKU_plane2_px_error = errors[1];
                    TKU_plane2_y_error = errors[2];
                    TKU_plane2_py_error = errors[3];
                    TKU_plane2_kappa_error = errors[4];
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
                    
                    TKU_plane3_pull = point->pull();
                    
                    std::vector<double> errors = point->errors();
                    TKU_plane3_x_error = errors[0];
                    TKU_plane3_px_error = errors[1];
                    TKU_plane3_y_error = errors[2];
                    TKU_plane3_py_error = errors[3];
                    TKU_plane3_kappa_error = errors[4];
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
                    
                    TKU_plane4_pull = point->pull();
                    
                    std::vector<double> errors = point->errors();
                    TKU_plane4_x_error = errors[0];
                    TKU_plane4_px_error = errors[1];
                    TKU_plane4_y_error = errors[2];
                    TKU_plane4_py_error = errors[3];
                    TKU_plane4_kappa_error = errors[4];
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
                    
                    TKU_plane5_pull = point->pull();
                    
                    std::vector<double> errors = point->errors();
                    TKU_plane5_x_error = errors[0];
                    TKU_plane5_px_error = errors[1];
                    TKU_plane5_y_error = errors[2];
                    TKU_plane5_py_error = errors[3];
                    TKU_plane5_kappa_error = errors[4];
                }
            }
        }
    }
    if(TKU_station_hits == 5){
        TKU_goodParticle = 1;
    }
    else{
        TKU_goodParticle = 0;
    }
    
    if(TKU_Pvalue >= 0.01){
        goodPValue = 1;
    }
    else{
        goodPValue = 0;
    }
    
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
    double L = beamlineTracking_TOF1_zPosition - beamlineTracking_TOF0_zPosition;
    double mu_mass = 105.658367*u.MeV();
    double dt_cal;
    
    if(dt >= beamlineTracking_min_tof && dt <= beamlineTracking_max_tof){
        goodTimeOfFlight = 1;
        
        dt_cal = dt - beamlineTracking_data_ele_tof + (L + beamlineTracking_sim_ele_path)/u.c_light();
        
        //std::cout << "About to reconstruct a TOF event with: "
        //          << "dt = " << dt_cal << ", (x0, y0, x1, y1) = ("
        //          << TOF0_x << ", " << TOF0_y << ", " << TOF1_x << ", " << TOF1_y << ")\n";
        
        tracking->ReconstructEvent(dt_cal, TOF0_x, TOF1_x, TOF0_y, TOF1_y, mu_mass);
    }
    else{
        goodTimeOfFlight = 0;
        dt_cal = dt - beamlineTracking_data_ele_tof + (L + beamlineTracking_sim_ele_path)/u.c_light();
        tracking->Bad(dt_cal, TOF0_x, TOF1_x, TOF0_y, TOF1_y, mu_mass);
    }
    
    result = tracking->Result();
    TOF0_xPrime = result.value("ax0");
    TOF0_yPrime = result.value("ay0");
    
    TOF1_xPrime = result.value("ax1");
    TOF1_yPrime = result.value("ay1");
    
    TOF1_pz = result.value("P");
    TOF0_pz = TOF1_pz + result.value("dP"); // made a momentum correction based on air...
    
    //std::cout << "Pz at TOF1 = " << TOF1_pz << ", TOF0 = " << TOF0_pz << ", dPz = " << result.value("dP") << "\n";
    
    TOF0_px = TOF0_xPrime * TOF0_pz;
    TOF0_py = TOF0_yPrime * TOF0_pz;
    TOF1_px = TOF1_xPrime * TOF1_pz;
    TOF1_py = TOF1_yPrime * TOF1_pz;
    
    TOF0_p = TMath::Sqrt(TOF0_px*TOF0_px + TOF0_py*TOF0_py + TOF0_pz*TOF0_pz);
    TOF1_p = TMath::Sqrt(TOF1_px*TOF1_px + TOF1_py*TOF1_py + TOF1_pz*TOF1_pz);
    
    goodRaynerReconstruction = result.value("good");
    
    //std::cout << ".... reconstruction returned good = " << goodRaynerReconstruction << "\n";
}




void BetterReadMAUS::calculate_particle_mass(){
    double p_corr = 18.82;
    double t_mu = TOF1_hitTime - TOF0_hitTime; // time in ns
    double t_electron = 25.48;
    double beta = t_electron/t_mu;
    double gamma = 1.0/TMath::Sqrt(1.0 - beta*beta);
    
    double p_track = TKU_plane1_p;
    
    particle_mass = (p_track + p_corr) / (beta * gamma);
    //std::cout << "p_track = " << p_track << ", beta = "  << beta
    //          << ", gamma = " << gamma << ", m_calc =  " << particle_mass << "\n";
}

bool BetterReadMAUS::particle_in_mass_range(){
    if(particle_mass >= 99.0 && particle_mass <= 112.0){
        return true;
    }
    else{
        return false;
    }
    return false;
}




bool BetterReadMAUS::check_momentum_loss(){
    double m = 105.6583715;
    momentum_loss_cut_lower_limit = 5.0;
    momentum_loss_cut_upper_limit = 43.4;//35.0;
    
    bool passes = false;
    
    
    double tof = TOF1_hitTime - TOF0_hitTime;
    
    double beta_tof = 25.48/tof;
    double gamma_tof = 1.0/TMath::Sqrt(1.0 - beta_tof*beta_tof);
    double beta_gamma_tof = beta_tof*gamma_tof;
    
    double min_cut = (TKU_plane1_p + momentum_loss_cut_lower_limit) / m;
    double max_cut = (TKU_plane1_p + momentum_loss_cut_upper_limit) / m;
    
    
    if(beta_gamma_tof >= min_cut && beta_gamma_tof <= max_cut){
        passes = true;
    }
    
    return passes;
}




void BetterReadMAUS::ReadRogersExtrapolation(int some_spill, int some_event){
    /*
     * Chris Rogers has extrapolated tracks from TKU station 5
     * back to TOF1 and through the diffuser. We want to add
     * this information to our ROOT file for easier analysis
     *
     * NB: This file can be for either DATA or MONTE CARLO.
     */
    
    //std::cout << "Reading Chris Rogers extrapolated tracks from file " << rogersTrackingFileName.toStdString() << "...";
    
    rogers_z_diffuser1 = 13961.7; // z of most upstream plane of diffuser
    rogers_z_diffuser2 = 13942.4;  // z of the middle of the diffuser
    rogers_z_diffuser3 = 13963.0;
    
    QFile file(rogersTrackingFileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::cerr << "Failed to read tracking file.\n";
        return;
    }
    
    QTextStream in(&file);
    in.seek(file_line);
    bool gotTOF1_us = false;
    bool gotTOF1_ds = false;
    bool gotDiffuser1 = false;
    bool gotDiffuser2 = false;
    bool gotDiffuser3 = false;
    bool done = false;
    
    while(!done && !in.atEnd()){
        QString line = in.readLine();
        
        if(!line.contains("#")){
            // the line gives us some tracking info...
            QStringList list = line.split(" ", QString::SkipEmptyParts);
            
            // is this the spill and event we're looking for?
            if(list.at(0).toInt() == some_spill && list.at(1).toInt() == some_event){
                // now we've got one of four lines associated with the (spill, event) of interest
                if(list.at(5).toDouble() == 12904.4){
                    gotTOF1_us = true;
                    rogers_tof01_us = list.at(2).toDouble();
                    rogers_x_tof1_us = list.at(3).toDouble();
                    rogers_y_tof1_us = list.at(4).toDouble();
                    rogers_px_tof1_us = list.at(6).toDouble();
                    rogers_py_tof1_us = list.at(7).toDouble();
                    rogers_pz_tof1_us = list.at(8).toDouble();
                }
                else if(list.at(5).toDouble() == 12954.4){
                    // this is TOF1 tracked data
                    gotTOF1_ds = true;
                    rogers_tof01_ds = list.at(2).toDouble();
                    rogers_x_tof1_ds = list.at(3).toDouble();
                    rogers_y_tof1_ds = list.at(4).toDouble();
                    rogers_px_tof1_ds = list.at(6).toDouble();
                    rogers_py_tof1_ds = list.at(7).toDouble();
                    rogers_pz_tof1_ds = list.at(8).toDouble();
                }
                else if(list.at(5).toDouble() == rogers_z_diffuser1){
                    gotDiffuser1 = true;
                    rogers_x_diffuser1 = list.at(3).toDouble();
                    rogers_y_diffuser1 = list.at(4).toDouble();
                    rogers_px_diffuser1 = list.at(6).toDouble();
                    rogers_py_diffuser1 = list.at(7).toDouble();
                    rogers_pz_diffuser1 = list.at(8).toDouble();
                }
                else if(list.at(5).toDouble() == rogers_z_diffuser2){
                    gotDiffuser2 = true;
                    rogers_x_diffuser2 = list.at(3).toDouble();
                    rogers_y_diffuser2 = list.at(4).toDouble();
                    rogers_px_diffuser2 = list.at(6).toDouble();
                    rogers_py_diffuser2 = list.at(7).toDouble();
                    rogers_pz_diffuser2 = list.at(8).toDouble();
                }
                else{
                    gotDiffuser3 = true;
                    rogers_x_diffuser3 = list.at(3).toDouble();
                    rogers_y_diffuser3 = list.at(4).toDouble();
                    rogers_px_diffuser3 = list.at(6).toDouble();
                    rogers_py_diffuser3 = list.at(7).toDouble();
                    rogers_pz_diffuser3 = list.at(8).toDouble();
                }
                
            }
            
            // are we done?
            if(gotTOF1_us && gotTOF1_ds && gotDiffuser1 && gotDiffuser2 && gotDiffuser3){
                
                double r1 = TMath::Sqrt(rogers_x_diffuser1*rogers_x_diffuser1 + rogers_y_diffuser1*rogers_y_diffuser1);
                double r2 = TMath::Sqrt(rogers_x_diffuser2*rogers_x_diffuser2 + rogers_y_diffuser2*rogers_y_diffuser2);
                double r3 = TMath::Sqrt(rogers_x_diffuser3*rogers_x_diffuser3 + rogers_y_diffuser3*rogers_y_diffuser3);
                double max_radius = 90.0;
                
                file_line = in.pos();
                if(r1 <= max_radius && r2 <= max_radius && r3 <= max_radius){
                    cut_diffuser = 1;
                }
                else{
                    cut_diffuser = 0;
                }
                
                done = true;
                //std::cout << "... done\n";
            }
        }
    }
    
    file.flush();
    file.close();
}




void BetterReadMAUS::reset_rogers_tracking(){
    rogers_spill = -1;
    rogers_event = -1;
    cut_diffuser = -1;
    rogers_tof01_us = TMath::Infinity();
    rogers_x_tof1_us = TMath::Infinity();
    rogers_y_tof1_us = TMath::Infinity();
    rogers_px_tof1_us = TMath::Infinity();
    rogers_py_tof1_us = TMath::Infinity();
    rogers_pz_tof1_us = TMath::Infinity();
    rogers_tof01_ds = TMath::Infinity();
    rogers_x_tof1_ds = TMath::Infinity();
    rogers_y_tof1_ds = TMath::Infinity();
    rogers_px_tof1_ds = TMath::Infinity();
    rogers_py_tof1_ds = TMath::Infinity();
    rogers_pz_tof1_ds = TMath::Infinity();
    rogers_x_diffuser1 = TMath::Infinity();
    rogers_y_diffuser1 = TMath::Infinity();
    rogers_z_diffuser1 = TMath::Infinity();
    rogers_px_diffuser1 = TMath::Infinity();
    rogers_py_diffuser1 = TMath::Infinity();
    rogers_pz_diffuser1 = TMath::Infinity();
    rogers_x_diffuser2 = TMath::Infinity();
    rogers_y_diffuser2 = TMath::Infinity();
    rogers_z_diffuser2 = TMath::Infinity();
    rogers_px_diffuser2 = TMath::Infinity();
    rogers_py_diffuser2 = TMath::Infinity();
    rogers_pz_diffuser2 = TMath::Infinity();
    rogers_x_diffuser3 = TMath::Infinity();
    rogers_y_diffuser3 = TMath::Infinity();
    rogers_z_diffuser3 = TMath::Infinity();
    rogers_px_diffuser3 = TMath::Infinity();
    rogers_py_diffuser3 = TMath::Infinity();
    rogers_pz_diffuser3 = TMath::Infinity();
}








void BetterReadMAUS::reset_mc_particle_variables(){
    // reset's particle variables
    
    // approx locations of detector stations
    // used to find closest virtual plane.
    mc_tof0_z = 5260.66; //5286.0;
    mc_tof1_z_us = 12904.4; //12930.0;
    mc_tof1_z_ds = 12954.5; //12930.0;
    /*mc_tku_s1_z = 15060.0;
     mc_tku_s2_z = 14860.0;
     mc_tku_s3_z = 14610.0;
     mc_tku_s4_z = 14310.0;
     mc_tku_s5_z = 13960.0;
     mc_diffuser_z1 = 13961.7; // z of most upstream plane of diffuser
     mc_diffuser_z2 = 13962.4;  // z of the middle of the diffuser
     mc_diffuser_z3 = 13963.0;
     */
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
    /*
     *  1. read the calibration file
     *  2. create the output root file
     *  3. read the maus input file
     *      a. Get a spacepoint at TOF0, reconstruct its (x, y) using pmt times
     *      b. Get a spacepoint at TOF1, do the same
     *      c. Get a track in the upstream tracker
     */
    
    define_mc_root_file(fileToSaveAs);
    
    MAUS::Data data;
    irstream infile(fileToOpen.toStdString().c_str(), "Spill");
    
    int count = 0;
    
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
            
            //std::cout << "Plane position = " << position.z() << " compared to z = " << mc_diffuser_z1 << ", "  << mc_diffuser_z2 << ", or " << mc_diffuser_z3 << "\n";
            
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
                mc_particle_id = hit.GetParticleId();
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
            }
            else if(position.z() == mc_diffuser_z1 - diffuser_dz && position.z() <= mc_diffuser_z1 + diffuser_dz){
                if(testping){
                    std::cout << "ping, diff1 with z = " << position.z() << "\n";
                }
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


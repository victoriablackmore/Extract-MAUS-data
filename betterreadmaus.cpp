#include "betterreadmaus.h"

#include "JsonCppStreamer/IRStream.hh"


BetterReadMAUS::BetterReadMAUS()
{
}




void BetterReadMAUS::reset_particle_variables(){
    // reset's particle variables

    reset_TOF0_variables();
    reset_TOF1_variables();
    reset_TKU_variables();

    reconstructed_event_number = -1;
    spill_number = -1;

    goodParticle = 0;
    goodRaynerReconstruction = 0;
    goodTimeOfFlight = 0;
    all_detectors_hit = 0;
    good_Wilbur_cut = 0;
    good_tof_tracker_momentum = 0;
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

    TKU_plane2_x = TMath::Infinity();
    TKU_plane2_y = TMath::Infinity();
    TKU_plane2_z = TMath::Infinity();
    TKU_plane2_px = TMath::Infinity();
    TKU_plane2_py = TMath::Infinity();
    TKU_plane2_pz = TMath::Infinity();
    TKU_plane2_p = TMath::Infinity();

    TKU_plane3_x = TMath::Infinity();
    TKU_plane3_y = TMath::Infinity();
    TKU_plane3_z = TMath::Infinity();
    TKU_plane3_px = TMath::Infinity();
    TKU_plane3_py = TMath::Infinity();
    TKU_plane3_pz = TMath::Infinity();
    TKU_plane3_p = TMath::Infinity();

    TKU_plane4_x = TMath::Infinity();
    TKU_plane4_y = TMath::Infinity();
    TKU_plane4_z = TMath::Infinity();
    TKU_plane4_px = TMath::Infinity();
    TKU_plane4_py = TMath::Infinity();
    TKU_plane4_pz = TMath::Infinity();
    TKU_plane4_p = TMath::Infinity();

    TKU_plane5_x = TMath::Infinity();
    TKU_plane5_y = TMath::Infinity();
    TKU_plane5_z = TMath::Infinity();
    TKU_plane5_px = TMath::Infinity();
    TKU_plane5_py = TMath::Infinity();
    TKU_plane5_pz = TMath::Infinity();
    TKU_plane5_p = TMath::Infinity();

    TKU_Pvalue = TMath::Infinity();
    TKU_chiSquare = TMath::Infinity();
    TKU_patternRecognition_dipAngle = TMath::Infinity();
    TKU_patternRecognition_R = TMath::Infinity();
    TKU_patternRecognition_x0 = TMath::Infinity();
    TKU_patternRecognition_y0 = TMath::Infinity();

    TKU_goodPlane1 = 0;
    TKU_goodPlane2 = 0;
    TKU_goodPlane3 = 0;
    TKU_goodPlane4 = 0;
    TKU_goodPlane5 = 0;
    TKU_goodParticle = 0;
    only_track_in_TKU = 0;
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

    if(!std::isnan(horizontal_delta)){
        TOF0_x = 0.5*calibrated_c_eff*(TOF0_horizontal_slab_tMinus
                                   - TOF0_horizontal_slab_tPlus + horizontal_delta);


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

    if(!std::isnan(horizontal_delta)){
        TOF1_x = 0.5*calibrated_c_eff*(TOF1_horizontal_slab_tMinus
                                   - TOF1_horizontal_slab_tPlus + horizontal_delta);


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
    
    outputTree->Branch("SpillNumber", &spill_number, "SpillNumber/D");
    outputTree->Branch("ReconstructedEventNumber", &reconstructed_event_number, "ReconstructedEventNumber/D");

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

    outputTree->Branch("TKU_s2_x", &TKU_plane2_x, "TKU_s2_x/D"); // station 1
    outputTree->Branch("TKU_s2_y", &TKU_plane2_y, "TKU_s2_y/D");
    outputTree->Branch("TKU_s2_z", &TKU_plane2_z, "TKU_s2_z/D");
    outputTree->Branch("TKU_s2_px", &TKU_plane2_px, "TKU_s2_px/D");
    outputTree->Branch("TKU_s2_py", &TKU_plane2_py, "TKU_s2_py/D");
    outputTree->Branch("TKU_s2_pz", &TKU_plane2_pz, "TKU_s2_pz/D");
    outputTree->Branch("TKU_s2_p", &TKU_plane2_p, "TKU_s2_p/D");

    outputTree->Branch("TKU_s3_x", &TKU_plane3_x, "TKU_s3_x/D"); // station 1
    outputTree->Branch("TKU_s3_y", &TKU_plane3_y, "TKU_s3_y/D");
    outputTree->Branch("TKU_s3_z", &TKU_plane3_z, "TKU_s3_z/D");
    outputTree->Branch("TKU_s3_px", &TKU_plane3_px, "TKU_s3_px/D");
    outputTree->Branch("TKU_s3_py", &TKU_plane3_py, "TKU_s3_py/D");
    outputTree->Branch("TKU_s3_pz", &TKU_plane3_pz, "TKU_s3_pz/D");
    outputTree->Branch("TKU_s3_p", &TKU_plane3_p, "TKU_s3_p/D");

    outputTree->Branch("TKU_s4_x", &TKU_plane4_x, "TKU_s4_x/D"); // station 1
    outputTree->Branch("TKU_s4_y", &TKU_plane4_y, "TKU_s4_y/D");
    outputTree->Branch("TKU_s4_z", &TKU_plane4_z, "TKU_s4_z/D");
    outputTree->Branch("TKU_s4_px", &TKU_plane4_px, "TKU_s4_px/D");
    outputTree->Branch("TKU_s4_py", &TKU_plane4_py, "TKU_s4_py/D");
    outputTree->Branch("TKU_s4_pz", &TKU_plane4_pz, "TKU_s4_pz/D");
    outputTree->Branch("TKU_s4_p", &TKU_plane4_p, "TKU_s4_p/D");

    outputTree->Branch("TKU_s5_x", &TKU_plane5_x, "TKU_s5_x/D"); // station 1
    outputTree->Branch("TKU_s5_y", &TKU_plane5_y, "TKU_s5_y/D");
    outputTree->Branch("TKU_s5_z", &TKU_plane5_z, "TKU_s5_z/D");
    outputTree->Branch("TKU_s5_px", &TKU_plane5_px, "TKU_s5_px/D");
    outputTree->Branch("TKU_s5_py", &TKU_plane5_py, "TKU_s5_py/D");
    outputTree->Branch("TKU_s5_pz", &TKU_plane5_pz, "TKU_s5_pz/D");
    outputTree->Branch("TKU_s5_p", &TKU_plane5_p, "TKU_s5_p/D");

    outputTree->Branch("TKU_PValue", &TKU_Pvalue, "TKU_PValue/D");
    outputTree->Branch("TKU_chiSquare", &TKU_chiSquare, "TKU_chiSquare/D");
    outputTree->Branch("TKU_pattRec_r", &TKU_patternRecognition_R, "TKU_pattRec_r/D");
    outputTree->Branch("TKU_pattRec_dipAngle", &TKU_patternRecognition_dipAngle, "TKU_pattRec_dipAngle/D");
    outputTree->Branch("TKU_pattRec_x0", &TKU_patternRecognition_x0, "TKU_pattRec_x0/D");
    outputTree->Branch("TKU_pattRec_y0", &TKU_patternRecognition_y0, "TKU_pattRec_y0/D");
    outputTree->Branch("TKU_good", &TKU_goodParticle, "TKU_good/I");

    /* cuts: these will be 0 or 1 depending on whether the fail (0) or pass (1) the cut
     *
     *    cut_TOF0_goodPMTPosition (aka TOF0_goodPMTPosition), does this particle go through slabs that have PMT-position calibrations
     *    cut_TOF1_goodPMTPosition (aka TOF1_goodPMTPosition), ditto
     *    cut_goodRaynerReconstruction (aka TOF0_goodRaynerReconstruction), does this particle get a good result from M. Rayner's reconstruction process?
     */
    outputTree->Branch("cut_TOF0_goodPMTPosition", &TOF0_goodPMTPosition, "cut_TOF0_goodPMTPosition/I");
    outputTree->Branch("cut_TOF1_goodPMTPosition", &TOF1_goodPMTPosition, "cut_TOF1_goodPMTPosition/I");
    outputTree->Branch("cut_goodRaynerReconstruction", &goodRaynerReconstruction, "cut_goodRaynerReconstruction/I");
    outputTree->Branch("cut_TKU_hitStation1", &TKU_goodPlane1, "cut_TKU_hitStation1/I");
    outputTree->Branch("cut_TKU_hitStation2", &TKU_goodPlane2, "cut_TKU_hitStation2/I");
    outputTree->Branch("cut_TKU_hitStation3", &TKU_goodPlane3, "cut_TKU_hitStation3/I");
    outputTree->Branch("cut_TKU_hitStation4", &TKU_goodPlane4, "cut_TKU_hitStation4/I");
    outputTree->Branch("cut_TKU_hitStation5", &TKU_goodPlane5, "cut_TKU_hitStation5/I");
    outputTree->Branch("cut_TKU_hitAllStations", &TKU_goodParticle, "cut_TKU_hitAllStations/I");
    outputTree->Branch("cut_TimeOfFlight", &goodTimeOfFlight, "cut_TimeOfFlight/I");
    outputTree->Branch("cut_hit_all_detectors", &all_detectors_hit, "cut_hit_all_detectors/I");
    outputTree->Branch("cut_TOF0_singleHit", &only_hit_at_TOF0, "cut_TOF0_singleHit/I");
    outputTree->Branch("cut_TOF1_singleHit", &only_hit_at_TOF1, "cut_TOF1_singleHit/I");
    outputTree->Branch("cut_TKU_singleTrack", &only_track_in_TKU, "cut_TKU_singleTrack/I");
    outputTree->Branch("cut_TKU_PValue", &goodPValue, "cut_TKU_PValue/I");
    outputTree->Branch("cut_TOF_TKU_momentum", &good_Wilbur_cut, "cut_TOF_TKU_momentum/I");
    outputTree->Branch("cut_tof1_tku_momentum", &good_tof_tracker_momentum, "cut_tof1_tku_momentum/I");
    
    // remember to update the selection for goodParticle if more cuts are added above
    outputTree->Branch("cut_allPassed", &goodParticle, "cut_allPassed/I");
}







void BetterReadMAUS::Read(QString fileToOpen, QString fileToSaveAs, QString calibrationConstantsFile){
    /*
     *  1. read the calibration file
     *  2. create the output root file
     *  3. read the maus input file
     *      a. Get a spacepoint at TOF0, reconstruct its (x, y) using pmt times
     *      b. Get a spacepoint at TOF1, do the same
     *      c. Get a track in the upstream tracker
     */

    readCalibrationFile(calibrationConstantsFile);
    define_root_file(fileToSaveAs);

    MAUS::Data data;
    irstream infile(fileToOpen.toStdString().c_str(), "Spill");

    // iterate over events:
    while(infile >> readEvent != NULL){
        infile >> branchName("data") >> data;
        spill = data.GetSpill();

        if(spill != NULL && spill->GetDaqEventType() == "physics_event"){
            // Found a spill that contains data. Iterate over all its events...

            readParticleEvent();
        }
    }

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
        
        if(particle_within_tof1_tku_momentum_selection()){
            good_tof_tracker_momentum = 1;
        }
        else{
            good_tof_tracker_momentum = 0;
        }
        
        
        if(particle_within_Wilbur_cut()){
            good_Wilbur_cut = 1;
        }
        else{
            good_Wilbur_cut = 0;
        }

        /*
         * Particles pass ALL cuts if:
         *      -- all_detectors_hit = 1 (TOF0_goodPMTPosition, TOF1_goodPMTPosition, TKU_goodParticle = 1)
         *      -- goodRaynerReconstruction = 1
         *      -- only_hit_at_TOF0 = 1
         *      -- only_hit_at_TOF1 = 1
         *      -- only_track_in_TKU = 1
         *      -- goodPValue = 1
         *      -- good_Wilbur_cut = 1 (previously used goodTimeOfFlight=1, which still
         *   exists, but this should be an improvement)
         */
        if(goodRaynerReconstruction ==1 && goodTimeOfFlight == 1//good_tof_tracker_momentum == 1//good_Wilbur_cut == 1
           && all_detectors_hit == 1    && only_hit_at_TOF0 == 1
           && only_hit_at_TOF1 == 1     && only_track_in_TKU == 1
           && goodPValue == 1){
            goodParticle = 1;
        }
        else{
            goodParticle = 0;
        }




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
                TOF0_vertical_slab_tPlus = tof0_slab_hits.GetPmt1().GetTime();
                TOF0_vertical_slab_tMinus = tof0_slab_hits.GetPmt0().GetTime();
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
                TOF1_vertical_slab_tMinus = tof1_slab_hits.GetPmt0().GetTime();
                TOF1_vertical_slab_tPlus = tof1_slab_hits.GetPmt1().GetTime();
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

        std::cout << "----> P value of track = " << TKU_Pvalue << "\n";

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

                    TKU_goodPlane1 = 1;
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

                    TKU_goodPlane2 = 1;
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

                    TKU_goodPlane3 = 1;
                }
                else if(point->station() == 4){
                    TKU_plane4_x = point->pos().x();
                    TKU_plane4_y = point->pos().y();
                    TKU_plane4_z = point->pos().z();

                    TKU_plane4_px = point->mom().x();
                    TKU_plane4_py = point->mom().y();
                    TKU_plane4_pz = point->mom().z();
                    TKU_plane1_p = TMath::Sqrt(TKU_plane4_px*TKU_plane4_px
                                             + TKU_plane4_py*TKU_plane4_py
                                             + TKU_plane4_pz*TKU_plane4_pz);

                    TKU_goodPlane4 = 1;
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

                    TKU_goodPlane5 = 1;
                }
            }
        }
    }
    if(TKU_goodPlane1 == 1 && TKU_goodPlane2 == 1 && TKU_goodPlane3 == 1
            && TKU_goodPlane4 == 1 && TKU_goodPlane5 == 1){
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

        std::cout << "About to reconstruct a TOF event with: "
                  << "dt = " << dt_cal << ", (x0, y0, x1, y1) = ("
                  << TOF0_x << ", " << TOF0_y << ", " << TOF1_x << ", " << TOF1_y << ")\n";

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

    std::cout << "Pz at TOF1 = " << TOF1_pz << ", TOF0 = " << TOF0_pz << ", dPz = " << result.value("dP") << "\n";

    TOF0_px = TOF0_xPrime * TOF0_pz;
    TOF0_py = TOF0_yPrime * TOF0_pz;
    TOF1_px = TOF1_xPrime * TOF1_pz;
    TOF1_py = TOF1_yPrime * TOF1_pz;

    TOF0_p = TMath::Sqrt(TOF0_px*TOF0_px + TOF0_py*TOF0_py + TOF0_pz*TOF0_pz);
    TOF1_p = TMath::Sqrt(TOF1_px*TOF1_px + TOF1_py*TOF1_py + TOF1_pz*TOF1_pz);

    goodRaynerReconstruction = result.value("good");

    std::cout << ".... reconstruction returned good = " << goodRaynerReconstruction << "\n";
}





bool BetterReadMAUS::particle_within_Wilbur_cut(){
    /*
     These cuts are based on an email from S. Wilbur on 17/03/16 at 15:46. The final
     numbers still need checking with T. Mohayai.  When done, this function should be
     updated.
     */
    
    double e_tof = 25.95;
    double mu_tof = TOF1_hitTime - TOF0_hitTime;
    
    if(mu_tof <= e_tof){
        return false;
    }
    double velocity = e_tof/mu_tof;
    double mu_mass = 105.658367;
    
    double p = mu_mass*velocity/TMath::Sqrt(1.0 - velocity*velocity);
    
    double tof_correction = mu_tof - e_tof + 25.95;
    double p_correction = -1103.98 + 75.7111*tof_correction
                            - 1.32053*tof_correction*tof_correction;
    
    
    
    return false; // this needs correcting when I hear back from Scott
}



bool BetterReadMAUS::particle_within_tof1_tku_momentum_selection(){
    /*
     These cuts are based on plotting (tracker P, tof1 P) and selecting
     a region 0.9*rms around the maxima
     */
    
    double upper_gradient = 0.7698;
    double upper_intercept = 72.855;
    
    double lower_gradient = 0.9082;
    double lower_intercept = 26.985;
    
    double upper, lower;
    
    if(TOF1_p == TMath::Infinity() || TKU_plane1_p == TMath::Infinity()){
        return false;
    }
    else{
        upper = upper_gradient*TKU_plane1_p + upper_intercept;
        lower = lower_gradient*TKU_plane1_p + lower_intercept;
        if(TOF1_p >= lower && TOF1_p <= upper){
            return true;
        }
        else{
            return false;
        }
    }
    
    return false;
}
#ifndef BETTERREADMAUS_H
#define BETTERREADMAUS_H

#include <iostream>
#include "string.h"
#include <TFile.h>
#include <TTree.h>
#include <QString>
#include "TMath.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QVector>
#include "stepibeamline.h"
#include "stepitracking.h"
#include "units.h"
#include <QHash>


#include <DataStructure/Spill.hh>
#include <DataStructure/TOFEvent.hh>
#include <DataStructure/Data.hh>
#include <DataStructure/ReconEvent.hh>
#include <DataStructure/SciFiEvent.hh>
#include <DataStructure/SciFiTrack.hh>
#include <DataStructure/SciFiTrackPoint.hh>
#include <DataStructure/SciFiSpacePoint.hh>
#include <DataStructure/SciFiStraightPRTrack.hh>
#include <DataStructure/SciFiCluster.hh>
#include <DataStructure/SciFiDigit.hh>
#include <DataStructure/SciFiChannelId.hh>
#include <DataStructure/Hit.hh>
#include <DataStructure/Track.hh>
#include <DataStructure/ThreeVector.hh>



class BetterReadMAUS
{
public:
    BetterReadMAUS();

    void Read(QString fileToOpen, QString fileToSaveAs, QString calibrationConstantsFile);
    void SetBeamlineParameters(double min_tof, double max_tof, double sim_ele_path, double data_ele_tof,
                               double q7_current, double q8_current, double q9_current, double q7_zPosition,
                               double q8_zPosition, double q9_zPosition,
                               double tof0_zPosition, double tof1_zPosition);


private:
    MAUS::Spill *spill;
    MAUS::TOFEvent *tof_event;
    MAUS::SciFiEvent *scifi_event;

    QFile file;
    TFile *dataFile;
    TFile *outputFile;
    TTree *outputTree;
    void define_root_file(QString saveAs);


    int reconstructed_event_number, spill_number;

    double TOF0_xPixel, TOF0_yPixel; // (x, y) positions of hits
    double TOF0_x, TOF0_y, TOF0_z; // (x, y) positions of hits based on PMT times
    double TOF0_px, TOF0_py, TOF0_pz, TOF0_p;
    double TOF0_xPrime, TOF0_yPrime;
    double TOF0_hitTime; // time TOF0 was hit, for time-of-flight calculations
    int TOF0_horizontal_slab, TOF0_vertical_slab;
    double TOF0_horizontal_slab_tMinus, TOF0_horizontal_slab_tPlus;
    double TOF0_vertical_slab_tMinus, TOF0_vertical_slab_tPlus;
    int TOF0_goodPMTPosition;


    double TOF1_xPixel, TOF1_yPixel; // (x, y) positions of hits
    double TOF1_x, TOF1_y, TOF1_z;
    double TOF1_px, TOF1_py, TOF1_pz, TOF1_p;
    double TOF1_xPrime, TOF1_yPrime;
    double TOF1_hitTime;
    int TOF1_horizontal_slab, TOF1_vertical_slab;
    double TOF1_horizontal_slab_tMinus, TOF1_horizontal_slab_tPlus;
    double TOF1_vertical_slab_tMinus, TOF1_vertical_slab_tPlus;
    int TOF1_goodPMTPosition;

    double TKU_plane1_x, TKU_plane1_y, TKU_plane1_z;
    double TKU_plane2_x, TKU_plane2_y, TKU_plane2_z;
    double TKU_plane3_x, TKU_plane3_y, TKU_plane3_z;
    double TKU_plane4_x, TKU_plane4_y, TKU_plane4_z;
    double TKU_plane5_x, TKU_plane5_y, TKU_plane5_z;

    double TKU_plane1_px, TKU_plane1_py, TKU_plane1_pz, TKU_plane1_p;
    double TKU_plane2_px, TKU_plane2_py, TKU_plane2_pz, TKU_plane2_p;
    double TKU_plane3_px, TKU_plane3_py, TKU_plane3_pz, TKU_plane3_p;
    double TKU_plane4_px, TKU_plane4_py, TKU_plane4_pz, TKU_plane4_p;
    double TKU_plane5_px, TKU_plane5_py, TKU_plane5_pz, TKU_plane5_p;

    double TKU_Pvalue;
    double TKU_chiSquare;
    double TKU_patternRecognition_R;
    double TKU_patternRecognition_dipAngle;
    double TKU_patternRecognition_x0;
    double TKU_patternRecognition_y0;
    double TKU_assumed_field;
    int TKU_goodParticle;
    int TKU_goodPlane1, TKU_goodPlane2, TKU_goodPlane3, TKU_goodPlane4, TKU_goodPlane5;

    int goodParticle;
    int goodRaynerReconstruction;
    int goodTimeOfFlight;
    int all_detectors_hit;
    int only_hit_at_TOF0, only_hit_at_TOF1, only_track_in_TKU;
    int goodPValue;
    int good_Wilbur_cut;
    int good_tof_tracker_momentum;

    QVector<double> TOF0_horizontal_calibration;
    QVector<double> TOF0_vertical_calibration;
    QVector<double> TOF1_horizontal_calibration;
    QVector<double> TOF1_vertical_calibration;
    bool reading_TOF0, reading_TOF1, reading_horizontal, reading_vertical;
    double calibrated_c_eff;
    void check_calibration();
    void readCalibrationFile(QString calibrationFile);
    void process_line(QString line);
    void apply_calibration_TOF0();
    void apply_calibration_TOF1();

    // variables needed to do the Rayner-style TOF reconstruction
    double beamlineTracking_min_tof, beamlineTracking_max_tof;
    double beamlineTracking_sim_ele_path, beamlineTracking_data_ele_tof;
    double beamlineTracking_q7_current, beamlineTracking_q8_current, beamlineTracking_q9_current;
    double beamlineTracking_q7_zPosition, beamlineTracking_q8_zPosition, beamlineTracking_q9_zPosition;
    double beamlineTracking_TOF0_zPosition, beamlineTracking_TOF1_zPosition;
    StepIBeamLine *beamline;
    StepITracking *tracking;
    Units u;
    void reconstruct_TOF_momentum();


    void readParticleEvent();
    void reset_particle_variables();
    void reset_TOF0_variables();
    void reset_TOF1_variables();
    void reset_TKU_variables();
    void write_to_file();
    void particle_at_TOF0();
    void particle_at_TOF1();
    void particle_at_tracker();
    
    bool particle_within_Wilbur_cut();
    bool particle_within_tof1_tku_momentum_selection();



};

#endif // BETTERREADMAUS_H

#ifndef READ_MC_H
#define READ_MC_H

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
#include <QPair>
#include <iterator>


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
#include <DataStructure/VirtualHit.hh>


class read_mc
{
public:
    read_mc();
    void Read(QString fileToOpen, QString fileToSaveAs);

private:
    MAUS::Spill *spill;
    MAUS::MCEvent *mc_event;

    QFile file;
    TFile *dataFile;
    TFile *outputFile;
    TTree *outputTree;
    void define_root_file(QString saveAs);
    void readParticleEvent();
    void write_to_file();

    int event_number, spill_number, particle_id;
    double tof0_x, tof0_y, tof0_z, tof0_px, tof0_py, tof0_pz, tof0_p;
    double tof1_x, tof1_y, tof1_z, tof1_px, tof1_py, tof1_pz, tof1_p;
    double tku_s1_x, tku_s1_y, tku_s1_z, tku_s1_px, tku_s1_py, tku_s1_pz, tku_s1_p, tku_s1_B;
    double tku_s2_x, tku_s2_y, tku_s2_z, tku_s2_px, tku_s2_py, tku_s2_pz, tku_s2_p, tku_s2_B;
    double tku_s3_x, tku_s3_y, tku_s3_z, tku_s3_px, tku_s3_py, tku_s3_pz, tku_s3_p, tku_s3_B;
    double tku_s4_x, tku_s4_y, tku_s4_z, tku_s4_px, tku_s4_py, tku_s4_pz, tku_s4_p, tku_s4_B;
    double tku_s5_x, tku_s5_y, tku_s5_z, tku_s5_px, tku_s5_py, tku_s5_pz, tku_s5_p, tku_s5_B;

    double diffuser1_x, diffuser2_x, diffuser3_x;
    double diffuser1_y, diffuser2_y, diffuser3_y;
    double diffuser1_z, diffuser2_z, diffuser3_z;
    double diffuser1_px, diffuser2_px, diffuser3_px;
    double diffuser1_py, diffuser2_py, diffuser3_py;
    double diffuser1_pz, diffuser2_pz, diffuser3_pz;
    double diffuser1_p, diffuser2_p, diffuser3_p;
    double diffuser1_B, diffuser2_B, diffuser3_B;

    void reset_particle_variables();
    void reset_TOF0_variables();
    void reset_TOF1_variables();
    void reset_TKU_variables();
    void reset_diffuser_variables();


};

#endif // READ_MC_H

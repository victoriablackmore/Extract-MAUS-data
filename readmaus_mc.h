#ifndef READMAUS_MC_H
#define READMAUS_MC_H


#include <iostream>
#include "string.h"
#include <TFile.h>
#include <TTree.h>
#include <QString>
#include "TMath.h"
#include <QFile>
#include <QTextStream>


#include <DataStructure/Spill.hh>
#include <DataStructure/Data.hh>
#include <DataStructure/Hit.hh>
#include <DataStructure/ThreeVector.hh>
#include <DataStructure/VirtualHit.hh>


class ReadMAUS_MC
{
public:
    ReadMAUS_MC();
    ~ReadMAUS_MC();

    void Read(QString fileToOpen);


private:
    MAUS::Spill *spill;
    MAUS::MCEvent event;
    MAUS::VirtualHit virtualhit;

    void readSpill();

};

#endif // READMAUS_MC_H

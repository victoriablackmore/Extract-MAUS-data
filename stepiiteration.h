#ifndef STEPIITERATION_H
#define STEPIITERATION_H

#include "stepibeamline.h"
#include "units.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include <string>
#include "units.h"
#include <QHash>
#include <QString>

class StepIIteration
{
public:
    StepIIteration(StepIBeamLine *beamline);
    ~StepIIteration();
    void ReconstructEvent(double t, double &delta, double x0, double x1, double y0, double y1, double mass);
    void ReconstructEvent(double t, double &delta, double x0, double x1, double y0, double y1, double mass, int muonIDNumber);

    void Bad(double t, double x0, double x1, double y0, double y1, double mass);
    void Bad(double t, double x0, double x1, double y0, double y1, double mass, int muonIDNumber);

    void CHEAT(double t, double &delta, double x0, double ax0, double y0, double ay0, double mass);
    void Write();

    QHash<QString, double> Result();


private:
    StepIBeamLine *_beamline;
    TFile *_file;
    TTree *_T;
    int _good;
    double _mass;
    double _delta_in;
    double _t;
    double _z;
    double _beta;
    double _gamma;
    double _P, _dP;
    double _x0, _ax0;
    double _x1, _ax1;
    double _y0, _ay0;
    double _y1, _ay1;
    double _delta_out;

    // quantities added by VBlackmore, Nov 2012
    double _Ax;
    double _Ay;
    double _Bx;
    double _By;
    int _muonIDNumber;


    bool VBS;

    std::string _name;

    void calculateMomentum(double t, double delta, double mass, double &beta, double &gamma, double &P);
    double einstein_beta(double z, double t);
    double einstein_gamma(double beta);
    double rel_mom(double beta, double gamma, double mass);
    double momentumCorrection(double p);
    double dEdx(double p, double ZoverA, double I);

    Units u;
};

#endif // STEPIITERATION_H

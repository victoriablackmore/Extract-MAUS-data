#ifndef STEPIBEAMLINE_H
#define STEPIBEAMLINE_H

#include "TMatrixD.h"
#include "TMath.h"
#include "TFile.h"
#include "TLegend.h"
#include "TAxis.h"
#include <string>
#include <iostream>

#include "quadrupole.h"
#include "tophat.h"
#include "slicequad.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "units.h"




class StepIBeamLine
{
public:
    StepIBeamLine(bool verbose=false);
    ~StepIBeamLine();
    void CalculateAngles(double const x0, double &ax0, double const y0, double &ay0, double const x1, double &ax1, double const y1, double &ay1, double const P);
    double CalculatePathLength(double x0, double ax0, double y0, double ay0, double P0);
    double GetZ0() { return _z0; };
    double GetZ1() { return _z1; };
    double GetZ7() { return _z7; };
    double GetZ8() { return _z8; };
    double GetZ9() { return _z9; };
    double GetDz() { return _z1-_z0; };
    void CalculateTransferMatrices(TMatrixD &Mx, TMatrixD &My, double P);
    Quadrupole *GetQ7() { return Q7; };
    Quadrupole *GetQ8() { return Q8; };
    Quadrupole *GetQ9() { return Q9; };
    void WriteOptics(std::string tag);
    TMatrixD GetInitialScatteringErrorCovarianceMatrix(double p);
    void CalculateInitialScatteringErrorCovarianceMatrices(TMatrixD &X, TMatrixD &Y, double p, int nsteps=4000);

    double GetAx() { return _Ax; };	// return transfer matrix
    double GetAy() { return _Ay; };	// elements so we can
    double GetBx() { return _Bx; };	// use them in our root
    double GetBy() { return _By; };	// files

    void SetQ7(double current, double position);
    void SetQ8(double current, double position);
    void SetQ9(double current, double position);
    void SetTOF0(double position);
    void SetTOF1(double position);

    void Initialise();





private:
    std::string _mode;
    bool _verbose;
    double _a, _b, _l;


    double _Ax, _Ay, _Bx, _By;

    double _z0, _zCKOV, _z1, _z7, _z8, _z9;
    double _g7, _g8, _g9;
    double _low7, _high7;
    double _low8, _high8;
    double _low9, _high9;
    Quadrupole *Q7, *Q8, *Q9;
    double pythagoras(double x, double y, double z);
    TMatrixD Drift(double L);
    double DriftPath(double &x0, double &ax0, double &y0, double &ay0, double const L);
    double g0(double dz);
    void PrepareForSlicing();
    TGraph *_gg0;
    TGraph *_gg7, *_gg8, *_gg9;
    double RmsThetaPlane(double m, double p, double x, double X0);
    Units u;

    double IPerg0;

};

#endif // STEPIBEAMLINE_H

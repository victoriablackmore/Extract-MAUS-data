#ifndef SLICEQUAD_H
#define SLICEQUAD_H

#include "quadrupole.h"
#include <string>
#include "tophat.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TMatrixD.h"
#include "TFile.h"
#include "units.h"

class SliceQuad : public Quadrupole
{
public:
    SliceQuad(std::string name, TGraph *ggradient, double z1, double z2, int ndiv);
    virtual TMatrixD Focus(double P);
    virtual TMatrixD Defocus(double P);
    virtual double dz(){return _l;};
    virtual double PathLength(double &xF, double &axF, double &xD, double &axD, const double P);
    double gradient(double z);
    TCanvas* GetCanvas() {return _can;};

    ~SliceQuad();

private:
    std::string _name;
    int _ndiv;
    std::vector<TopHat*> hats;
    double phi(double k0, double l);
    TCanvas *_can;
    TMultiGraph *_mg11, *_mg12, *_mg21, *_mg22;
    TGraph *_ggradient, *_gstep;
    TGraph *_gF11, *_gD11;
    TGraph *_gF12, *_gD12;
    TGraph *_gF21, *_gD21;
    TGraph *_gF22, *_gD22;

    Units u;
};

#endif // SLICEQUAD_H

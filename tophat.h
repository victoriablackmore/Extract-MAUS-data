#ifndef TOPHAT_H
#define TOPHAT_H

#include "quadrupole.h"
#include "TMatrixD.h"
#include "TMath.h"

class TopHat : public Quadrupole
{
public:
    TopHat(double g0, double l);
    virtual TMatrixD Focus(double P);
    virtual TMatrixD Defocus(double P);
    virtual double dz() {return _l;};
    virtual double PathLength(double &xF, double &axF, double &xD, double &axD, const double P);

    ~TopHat(){/* nothing to delete */ };
private:
    double phi(double k0, double l);
};

#endif // TOPHAT_H

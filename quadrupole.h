#ifndef QUADRUPOLE_H
#define QUADRUPOLE_H

#include "TMatrixD.h"
#include "TMath.h"
#include "iostream"
#include "units.h"



class Quadrupole
{
public:
    Quadrupole(double g0, double l);
    virtual TMatrixD Focus(double P) = 0;
    virtual TMatrixD Defocus(double P) = 0;
    double k0(double P); // Focussing strength according to Wiedemann
    void printk0(double P); // Focussing strength according to Wiedemann

    virtual double dz() = 0;
    virtual double PathLength(double &xF, double &axF, double &xD, double &axD, double const P) = 0;

    virtual ~Quadrupole(){};


protected:
    double _g0, _l;
    Units u;
};

#endif // QUADRUPOLE_H

#ifndef UNITS_H
#define UNITS_H

#include "TMath.h"

/*
 * An implementation of CLHEP's units
 */

class Units
{
public:
    Units();
    double T();
    double m();
    double mm();
    double V();
    double MV();
    double MeV();
    double eV();
    double ePlus();
    double s();
    double ns();
    double cm();
    double cm3();

    double c_light();

    double A();
    double Coulomb();
    double g();



private:
    double megaelectronvolt;
    double millimeter;
    double e_plus;
    double nanosecond;
    double e_SI;
    double kg;
};

#endif // UNITS_H

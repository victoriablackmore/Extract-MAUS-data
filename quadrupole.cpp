#include "quadrupole.h"

Quadrupole::Quadrupole(double g0, double l){
    _g0 = g0*1.02;
    _l = l;
}

double Quadrupole::k0(double P){
    return u.c_light() * TMath::Abs(_g0)/(P*u.MeV());
}




void Quadrupole::printk0(double P){
    std::cout << ", g = " << _g0;
}

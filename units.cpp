#include "units.h"

Units::Units()
{
    megaelectronvolt = 1.0;
    nanosecond = 1.0;
    millimeter = 1.0;
    nanosecond = 1.0;
    e_plus = 1.0;
    e_SI = 1.60217733e-19;
    kg = 1.0;
}


double Units::c_light(){
    return TMath::C()*(m()/s());
}


double Units::T(){
    // tesla = volt * second / meter2
    // meter2 = meter*meter
    return V()*s()/(m()*m());
}

double Units::mm(){
    return millimeter;
}
double Units::cm(){
    return 10.0*mm();
}

double Units::cm3(){
    return cm()*cm()*cm();
}

double Units::m(){
    return 1000.0*mm();
}

double Units::ns(){
    return nanosecond;
}

double Units::s(){
    return 1.0e9*ns();
}

double Units::ePlus(){
    return e_plus;
}

double Units::eV(){
    return 1.0e-6*MeV();
}

double Units::MeV(){
    return megaelectronvolt;
}
double Units::MV(){
    return MeV()/ePlus();
}
double Units::V(){
    return 1.0e-6*MV();
}

double Units::Coulomb(){
    return e_plus/e_SI;
}

double Units::A(){
    //return Coulomb()/s();
    return 1.0;
}

double Units::g(){
    return 1.0e-3*kg;
}

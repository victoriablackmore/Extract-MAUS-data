#include "tophat.h"

TopHat::TopHat(double g0, double l) : Quadrupole(g0, l)
{
}

TMatrixD TopHat::Focus(double P){
    double K = k0(P);
    double Omega = phi(K, _l);
    TMatrixD M(2, 2);
    M(0, 0) = TMath::Cos(Omega);
    M(0, 1) = TMath::Power(K, -0.5)*TMath::Sin(Omega);
    M(1, 0) = -TMath::Sqrt(K) * TMath::Sin(Omega);
    M(1, 1) = TMath::Cos(Omega);
    return M;


}

TMatrixD TopHat::Defocus(double P){
    double K = k0(P);
    double Omega = phi(K, _l);
    TMatrixD M(2, 2);
    M(0, 0) = TMath::CosH(Omega);
    M(0, 1) = TMath::Power(K, -0.5)*TMath::SinH(Omega);
    M(1, 0) = TMath::Sqrt(K)*TMath::SinH(Omega);
    M(1, 1) = TMath::CosH(Omega);
    return M;
}

double TopHat::phi(double k0, double l){
    return TMath::Sqrt(TMath::Abs(k0))*l;
}

double TopHat::PathLength(double &xF, double &aF, double &xD, double &aD, double const P){
    // returns the (curved) path length through the quadrupole,
    // and transports the transverse variables through it by reference

    // calculate the path in the drift using a 1st order Taylor expansion
    double K = k0(P);
    double Omega = phi(K, _l);

    double term1 = (K*xF*xF)*(Omega - TMath::Sin(Omega)*TMath::Cos(Omega))/(4.0*TMath::Sqrt(K));

    double term2 = (aF*aF)*(Omega + TMath::Sin(Omega)*TMath::Cos(Omega))/(4.0*TMath::Sqrt(K));

    double term3 = xF*aF*TMath::Power(TMath::Sin(Omega), 2)/2.0;

    double term4 = K*xD*xD*(TMath::SinH(2.0*Omega) - 2.0*Omega)/(8.0*TMath::Sqrt(K));

    double term5 = aD*aD*(Omega + TMath::SinH(Omega)*TMath::CosH(Omega))/(4.0*TMath::Sqrt(K));

    double term6 = xD*aD*TMath::Power(TMath::SinH(Omega), 2)/2.0;


    TMatrixD F = Focus(P);
    TMatrixD D = Defocus(P);
    double xF1 = F(0, 0) * xF + F(0, 1)*aF;
    double aF1 = F(1, 0) * xF + F(1, 1)*aF;
    double xD1 = D(0, 0) * xD + D(0, 1)*aD;
    double aD1 = D(1, 0) * xD + D(1, 1)*aD;


    double dx = xF1 - xF;
    double dy = xD1 - xD;
    double dz = _l;

    xF = xF1;
    aF = aF1;
    xD = xD1;
    aD = aD1;


    return _l;
    //return _l + term1 + term2 + term3 + term4 + term5 + term6;




/*
    // contribution from initial position, x, in the focussing plane
    double sxF = TMath::Power(xF, 2) * (Omega - 0.5*TMath::Sin(2*Omega)) * TMath::Sqrt(K);

    // cross term in the focussing plane
    double scF = -2.0 * xF * aF * TMath::Power(TMath::Sin(Omega), 2);

    // contribution from initial angle x' in the focussing plane
    double saF = TMath::Power(aF, 2) * (Omega + 0.5*TMath::Sin(2.0*Omega)) / TMath::Sqrt(K);

    // contribution from initial position, x, in the defocussing plane
    double sxD = TMath::Power(xD, 2) * (0.5*TMath::SinH(2.0*Omega) - Omega)*TMath::Sqrt(K);

    // cross term in the defocussing plane
    double scD = 2.0 * xD * aD * TMath::Power(TMath::SinH(Omega), 2);

    // contribution from initial angle x' in the defocussing plane
    double saD = TMath::Power(aD, 2) * (0.5*TMath::SinH(2.0*Omega) + Omega) / TMath::Sqrt(K);

    // output variables for this beamline element are the input variables for the next
    // bit of beam (they have been passed by reference)

    TMatrixD F = Focus(P);
    TMatrixD D = Defocus(P);
    double xF1 = F(0, 0) * xF + F(0, 1)*aF;
    double aF1 = F(1, 0) * xF + F(1, 1)*aF;
    double xD1 = D(0, 0) * xD + D(0, 1)*aD;
    double aD1 = D(1, 0) * xD + D(1, 1)*aD;

    xF = xF1;
    aF = aF1;
    xD = xD1;
    aD = aD1;

//    std::cout << "Omega = " << Omega << ", l = "  << _l << ", k = " << K << ", P = " << P;
//    printk0(P);
//    std::cout << ", sxF = " << sxF << ", scF = " << scF << ", saF = " << saF << ", sxD = " << sxD
//              << ", scD = " << scD << ", saD = " << saD << "\n";

    // return the path length through this element

    return _l + (sxF + saF + scF + sxD + saD + scD)/4.0;

    //return TMath::Sqrt(dx*dx + dy*dy + dz*dz);

*/
}

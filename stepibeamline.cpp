#include "stepibeamline.h"

StepIBeamLine::StepIBeamLine(bool verbose)
{
    IPerg0 = 174.09;
    _verbose = verbose;
}


void StepIBeamLine::Initialise(){
    _a = 330.899*u.mm();
    _b = 125.28*u.mm();
    _l = 2.0*_a;

    PrepareForSlicing();

    // define the quads:
    Q7 = new SliceQuad("Q7", _gg7, _low7, _high7, 10);
    Q8 = new SliceQuad("Q8", _gg8, _low8, _high8, 10);
    Q9 = new SliceQuad("Q9", _gg9, _low9, _high9, 10);
}

void StepIBeamLine::SetQ7(double current, double position){
    _g7 = current / IPerg0;
    _z7 = position;
}

void StepIBeamLine::SetQ8(double current, double position){
    _g8 = current / IPerg0;
    _z8 = position;
}

void StepIBeamLine::SetQ9(double current, double position){
    _g9 = current / IPerg0;
    _z9 = position;
}

void StepIBeamLine::SetTOF0(double position){
    _z0 = position;
    _zCKOV = _z0 + 65.0*u.cm();
}

void StepIBeamLine::SetTOF1(double position){
    _z1 = position;
}


double StepIBeamLine::g0(double dz){
    return (TMath::TanH((_a-dz)/_b)+TMath::TanH((_a+dz)/_b))/2.;
}

void StepIBeamLine::CalculateAngles(const double x0, double &ax0, const double y0, double &ay0, const double x1, double &ax1, const double y1, double &ay1, const double P){
    TMatrixD Mx(2, 2);
    TMatrixD My(2, 2);
    CalculateTransferMatrices(Mx, My, P);

    // angles passed by reference -- change these, they are the output
    ax0 = (x1 - Mx(0, 0) * x0) / Mx(0, 1);
    ay0 = (y1 - My(0, 0) * y0) / My(0, 1);

    ax1 = (Mx(1, 1) * x1 - x0) / Mx(0, 1);
    ay1 = (My(1, 1) * y1 - y0) / My(0, 1);

    _Ax = -1.0 / Mx(0, 1);
    _Ay = -1.0 / My(0, 1);
    _Bx = Mx(1, 1) / Mx(0, 1);
    _By = My(1, 1) / My(0, 1);
}


void StepIBeamLine::CalculateTransferMatrices(TMatrixD &Mx, TMatrixD &My, double P){
    Mx = Drift(_z1-_high9) * Q9->Focus(P)   * Q8->Defocus(P) * Q7->Focus(P)   * Drift(_low7-_z0);
    My = Drift(_z1-_high9) * Q9->Defocus(P) * Q8->Focus(P)   * Q7->Defocus(P) * Drift(_low7-_z0);
}



double StepIBeamLine::CalculatePathLength(double x0, double ax0, double y0, double ay0, double const P0) {

    //Calculate path length S
    if (_verbose) printf("(%.1f,\t%.4f)\t(%.1f,\t%.4f)\n", x0, ax0, y0, ay0);

    //Drift TOF0 - Q7
    double dz07 = _low7 - _z0;
    double s07 = DriftPath(x0, ax0, y0, ay0, dz07);
    if (_verbose) printf("dz (Drift): %f\n", dz07);
    if (_verbose) printf("(%.1f,\t%.4f)\t(%.1f,\t%.4f)\n", x0, ax0, y0, ay0);

    //Quadrupole 7
    double dzq7 = _high7-_low7;
    double sq7 = Q7->PathLength(x0, ax0, y0, ay0, P0);
    if (_verbose) printf("dz (Q7): %f\n", dzq7);
    if (_verbose) printf("(%.1f,\t%.4f)\t(%.1f,\t%.4f)\n", x0, ax0, y0, ay0);

    //Quadrupole 8
    double dzq8 = _high8-_low8;
    double sq8 = Q8->PathLength(x0, ax0, y0, ay0, P0);//x and y not switched intentionally
    if (_verbose) printf("dz (Q8): %f\n", dzq8);
    if (_verbose) printf("(%.1f,\t%.4f)\t(%.1f,\t%.4f)\n", x0, ax0, y0, ay0);

    //Quadrupole 9
    double dzq9 = _high9-_low9;
    double sq9 = Q9->PathLength(x0, ax0, y0, ay0, P0);
    if (_verbose) printf("dz (Q9): %f\n", dzq9);
    if (_verbose) printf("(%.1f,\t%.4f)\t(%.1f,\t%.4f)\n", x0, ax0, y0, ay0);

    //Drift Q9 - TOF1
    double dz91 = _z1 - _high9;
    double s91 = DriftPath(x0, ax0, y0, ay0, dz91);
    if (_verbose) printf("dz (Drift): %f\n", dz91);
    if (_verbose) printf("(%.1f,\t%.4f)\t(%.1f,\t%.4f)\n", x0, ax0, y0, ay0);

    if (_verbose) {
        printf("s07:  %f   delta07:  %f\n", s07, s07 - dz07);
        printf("sq7:  %f   deltaq7:  %f\n", sq7, sq7 - dzq7);
        printf("sq8:  %f   deltaq8:  %f\n", sq8, sq8 - dzq8);
        printf("sq9:  %f   deltaq9:  %f\n", sq9, sq9 - dzq9);
        printf("s91:  %f   delta91:  %f\n", s91, s91 - dz91);
    }

//    std::cout << "s07 = " << s07 << ", sq7 = " << sq7 << ", sq8 = " << sq8
//              << ", sq9 = " << sq9 << ", s91 = " << s91 << ", total = " << s07 + sq7 + sq8 + sq9 + s91 << "\n";



    return s07 + sq7 + sq8 + sq9 + s91;

}




double StepIBeamLine::DriftPath(double &x0, double &ax0, double &y0, double &ay0, double const L) {
    //Returns the (straight line) path length through the drift, and transports the transverse variables through it by reference

    //Find the final position
    TMatrixD M = Drift(L);

    double  x1 = M(0,0) * x0 + M(0,1) * ax0;
    double ax1 = M(1,0) * x0 + M(1,1) * ax0;
    double  y1 = M(0,0) * y0 + M(0,1) * ay0;
    double ay1 = M(1,0) * y0 + M(1,1) * ay0;

    //Calculate the path in the drift
    double s = pythagoras(x1-x0, y1-y0, L);

    //The output variables for this BL element are input variables for the next bit of beam line (they have been passed by reference)
    x0 = x1;
    ax0 = ax1;
    y0 = y1;
    ay0 = ay1;
    //Return the path through this element

    s = L;
    return s;
}

TMatrixD StepIBeamLine::Drift(double L) {
    TMatrixD M(2, 2);
    M(0, 0) = 1.;
    M(0, 1) = L;
    M(1, 0) = 0.;
    M(1, 1) = 1.;
    return M;
}

double StepIBeamLine::pythagoras(double x, double y, double z) {
    return TMath::Sqrt(x*x + y*y + z*z);
}

void StepIBeamLine::WriteOptics(std::string tag) {
    TFile *f = new TFile("../QuadrupoleOutputs/optics.root", "recreate");
    TGraph *gMx11 = new TGraph();
    TGraph *gMx12 = new TGraph();
    TGraph *gMx21 = new TGraph();
    TGraph *gMx22 = new TGraph();
    TGraph *gMy11 = new TGraph();
    TGraph *gMy12 = new TGraph();
    TGraph *gMy21 = new TGraph();
    TGraph *gMy22 = new TGraph();
    TGraph *gAx = new TGraph();
    TGraph *gBx = new TGraph();
    TGraph *gAy = new TGraph();
    TGraph *gBy = new TGraph();
    TGraph *gdAxdP = new TGraph();
    TGraph *gdBxdP = new TGraph();
    TGraph *gdAydP = new TGraph();
    TGraph *gdBydP = new TGraph();
    int i=0;
    TMatrixD Mx(2,2), My(2,2);
    TMatrixD prevx(2,2), prevy(2,2);
    TMatrixD nextx(2,2), nexty(2,2);
    const double dP = .05;
    for (double P=1.; P<600.5; P+=1.) {
        CalculateTransferMatrices(Mx, My, P);
        CalculateTransferMatrices(prevx, prevy, P-dP/2.);
        CalculateTransferMatrices(nextx, nexty, P+dP/2.);
        gMx11->SetPoint(i, P, Mx(0, 0));
        gMx12->SetPoint(i, P, Mx(0, 1));
        gMx21->SetPoint(i, P, Mx(1,0));
        gMx22->SetPoint(i, P, Mx(1,1));
        gMy11->SetPoint(i, P, My(0,0));
        gMy12->SetPoint(i, P, My(0,1));
        gMy21->SetPoint(i, P, My(1,0));
        gMy22->SetPoint(i, P, My(1,1));
        gAx->SetPoint(i, P, -1./Mx(0,1));
        gAy->SetPoint(i, P, -1./My(0,1));
        gBx->SetPoint(i, P, Mx(1,1)/Mx(0,1));
        gBy->SetPoint(i, P, My(1,1)/My(0,1));
        double Anextx = -1./nextx(0,1);
        double Anexty = -1./nexty(0,1);
        double Bnextx = nextx(1,1)/nextx(0,1);
        double Bnexty = nexty(1,1)/nexty(0,1);
        double Aprevx = -1./prevx(0,1);
        double Aprevy = -1./prevy(0,1);
        double Bprevx = prevx(1,1)/prevx(0,1);
        double Bprevy = prevy(1,1)/prevy(0,1);
        gdAxdP->SetPoint(i, P, (Anextx-Aprevx)/dP);
        gdBxdP->SetPoint(i, P, (Bnextx-Bprevx)/dP);
        gdAydP->SetPoint(i, P, (Anexty-Aprevy)/dP);
        gdBydP->SetPoint(i, P, (Bnexty-Bprevy)/dP);
        ++i;
    }
    gMx11->Write("gMx11");
    gMx12->Write("gMx12");
    gMx21->Write("gMx21");
    gMx22->Write("gMx22");
    gMy11->Write("gMy11");
    gMy12->Write("gMy12");
    gMy21->Write("gMy21");
    gMy22->Write("gMy22");
    gAx->Write("gAx");
    gAy->Write("gAy");
    gBx->Write("gBx");
    gBy->Write("gBy");
    gdAxdP->Write("gdAxdP");
    gdBxdP->Write("gdBxdP");
    gdAydP->Write("gdAydP");
    gdBydP->Write("gdBydP");
    f->Write();
    delete f;
}

void StepIBeamLine::PrepareForSlicing() {

    _gg0 = new TGraph();
    _gg0->SetLineColor(kRed    );
    _gg0->SetTitle(";z (mm);g_{0} = dB_{y}(y=0)/dx  (T/m)");
    _gg0->SetLineWidth(1);
    _gg7 = new TGraph();
    _gg7->SetLineColor(kAzure  );_gg7->SetLineWidth(1);
    _gg8 = new TGraph();_gg8->SetLineColor(kGreen+1);
    _gg8->SetLineWidth(1);
    _gg9 = new TGraph();
    _gg9->SetLineColor(kAzure  );
    _gg9->SetLineWidth(1);
    int n = 20000;
    double step = (_z1 - _z0)/static_cast<double>(n);

    bool nolow7 = true;
    bool nohigh9 = true;

    bool small;
    std::string last="+";
    std::string sign;

    for (int i=0; i<=n; ++i) {
        double z = _z0 + i*step;
        double g = _g7*g0(z-_z7) - _g8*g0(z-_z8) + _g9*g0(z-_z9);
        if (TMath::Abs(g)<1./1000.) {
            small = true;
        }
        else {
            small = false;
        }
        if (g>0.){
            sign = "+";
        }
        else {
            sign = "-";
        }
        if (z<_z7 && !small && nolow7) {
            _low7 = z;
            nolow7 = false;
        }
        if (z>_z7 && z<_z8 && last != sign){
            _low8 = z;
        }
        if (z>_z8 && z<_z9 && last != sign){
            _low9 = z;
        }
        if (z>_z9 && small && nohigh9) {
            _high9 = z;
            nohigh9 = false;
        }
        last = sign;
    }

    _high7 = _low8;
    _high8 = _low9;

    int i7=0;
    int i8=0;
    int i9=0;
    for (int i=0; i<=n; ++i) {
        double z = _z0 + i*step;
        double g = + _g7*g0(z-_z7) - _g8*g0(z-_z8) + _g9*g0(z-_z9);
        _gg0->SetPoint(i,z,g);
        if (z>=_low7 && z<=_high7) _gg7->SetPoint(i7++,z,g);
        if (z>=_low8 && z<=_high8) _gg8->SetPoint(i8++,z,g);
        if (z>=_low9 && z<=_high9) _gg9->SetPoint(i9++,z,g);
    }

    TCanvas *cg = new TCanvas("cg",";z (mm);gradient (T/m)",900,450);
    _gg0->Draw("AL");
    _gg0->GetYaxis()->SetTitleOffset(.9);
    TGraph *gbox = new TGraph();
    gbox->SetPoint(0,_z0,0);
    gbox->SetPoint(1,_z7-_l/2.,0);
    gbox->SetPoint(2,_z7-_l/2.,+_g7);
    gbox->SetPoint(3,_z7+_l/2.,+_g7);
    gbox->SetPoint(4,_z7+_l/2.,0);
    gbox->SetPoint(5,_z8-_l/2.,0);
    gbox->SetPoint(6,_z8-_l/2.,-_g8);
    gbox->SetPoint(7,_z8+_l/2.,-_g8);
    gbox->SetPoint(8,_z8+_l/2.,0);
    gbox->SetPoint(9,_z9-_l/2.,0);
    gbox->SetPoint(10,_z9-_l/2.,+_g9);
    gbox->SetPoint(11,_z9+_l/2.,+_g9);
    gbox->SetPoint(12,_z9+_l/2.,0);
    gbox->SetPoint(13,_z1,0);
    _gg0->GetXaxis()->SetRangeUser(_z0,_z1);
    _gg0->GetYaxis()->SetRangeUser(-1.5,1.5);
    gbox->Draw("L");
    _gg0->Draw("L");
    _gg7->Draw("L");
    _gg8->Draw("L");
    _gg9->Draw("L");

    TGraph *gcentre7 = new TGraph(); gcentre7->SetPoint(0,_z7,0); gcentre7->SetMarkerStyle(5); gcentre7->SetMarkerSize(.7); gcentre7->Draw("P");
    TGraph *gcentre8 = new TGraph(); gcentre8->SetPoint(0,_z8,0); gcentre8->SetMarkerStyle(5); gcentre8->SetMarkerSize(.7); gcentre8->Draw("P");
    TGraph *gcentre9 = new TGraph(); gcentre9->SetPoint(0,_z9,0); gcentre9->SetMarkerStyle(5); gcentre9->SetMarkerSize(.7); gcentre9->Draw("P");
    TLegend *leg = new TLegend(.2,.22,.5,.52);
    leg->SetBorderSize(1);
    leg->AddEntry(gcentre7, "Quadrupole centre", "P");
    leg->AddEntry(_gg7, "Focusing", "L");
    leg->AddEntry(_gg8, "Defocusing", "L");
    leg->AddEntry(_gg0, "Drift", "L");
    leg->AddEntry(gbox, "Top hat model", "L");
    leg->Draw("sames");

    cg->SaveAs("../QuadrupoleOutputs/gradient.png");
    cg->SaveAs("../QuadrupoleOutputs/gradient.pdf");
    cg->SaveAs("../QuadrupoleOutputs/gradient.eps");

}

void StepIBeamLine::CalculateInitialScatteringErrorCovarianceMatrices(TMatrixD &Xall, TMatrixD &Yall, double p, int nsteps) {
    //FDF plane
    Xall(0,0) = 0.0;
    Xall(0,1) = 0.0;
    Xall(1,0) = 0.0;
    Xall(1,1) = 0.0;

    //DFD plane
    Yall(0,0) = 0.0;
    Yall(0,1) = 0.0;
    Yall(1,0) = 0.0;
    Yall(1,1) = 0.0;

    for (int NumToMiss=0; NumToMiss<nsteps; ++NumToMiss) {

        //Initialize the error covariance matrix at TOF0 due to scattering
        TMatrixD X(2,2,0);//FDF plane
        TMatrixD Y(2,2,0);//DFD plane

        int ierrO, ierrF, ierrD;
        double L = _z1 - _z0;
        double step = L / static_cast<double>(nsteps);
        for (int i=NumToMiss; i<nsteps; ++i) {

            //Where are we at the beginning of the step?
            double zbeg = _z1 - i*step;

            //Where is the middle of the step?
            double zmid = zbeg - step/2.;

            //What is the gradient in the middle of the step?
            double g = _g7*g0(zmid-_z7) - _g8*g0(zmid-_z8) + _g9*g0(zmid-_z9);

            TMatrixD *Oinv=0, *Finv=0, *Dinv=0;
            //Are we in a drift?
            if (TMath::Abs(g)<1./1000.) {
                //Swim the error covariance matrix backwards one step
                Oinv = new TMatrixD(2,2,0);
                //*Oinv = Drift(step).Invert(ierrO);
                *Oinv = Drift(step).Invert();
                X = (*Oinv) * X * Oinv->T();
                Y = (*Oinv) * Y * Oinv->T();
            }

            //Then we must be in a quadrupole field
            else {
                //Approximate this slice as a top hat quadrupole
                TopHat QuadSlice(g * u.T() / u.m(), step);
                Finv = new TMatrixD(2,2,0);
                Dinv = new TMatrixD(2,2,0);
                //*Finv = QuadSlice.Focus(p).Invert(ierrF);
                //*Dinv = QuadSlice.Defocus(p).Invert(ierrD);
                *Finv = QuadSlice.Focus(p).Invert();
                *Dinv = QuadSlice.Defocus(p).Invert();

                //Focusing in x, defocusing in y
                if (g>0.) {
                    //Swim the error covariance matrix backwards one step
                    X = (*Finv) * X * Finv->T();
                    Y = (*Dinv) * Y * Dinv->T();
                }

                //Defocusing in x, focusing in y
                else {
                    //Swim the error covariance matrix backwards one step
                    X = (*Dinv) * X * Dinv->T();
                    Y = (*Finv) * Y * Finv->T();
                }
            }

            //Calculate and add on the scatter of the slice we are going to propogate back
            if (NumToMiss==0) {
                //Are we inside the Cherenkov? What is the radiation length X0?
                double X0;
                const double dzCKOV = 2*2.3*u.cm();
                const double zBPM = -360*u.mm();
                const double dzBPM = 1.6*u.mm();
                const double factor = 50;
                if (zmid>_zCKOV-dzCKOV/2. && zmid<=_zCKOV+dzCKOV/2.) X0 = 300.*u.mm();
                else if (zmid>zBPM-factor*dzBPM/2. && zmid<=zBPM+factor*dzBPM/2.) X0 = factor*41.31*u.cm();
                else X0 = 303.9*u.m(); //PDG air dry 1 atm

                //Get the RMS scattering angle per plane
                double theta0 = RmsThetaPlane(105.658367*u.MeV(), p, step, X0);

                //Add on the scattering in the step
                X(2,2) += pow(theta0, 2);
                Y(2,2) += pow(theta0, 2);
            }

        }
        Xall = Xall + X;
        Yall = Yall + Y;
    }
}

double StepIBeamLine::RmsThetaPlane(double m, double p, double x, double X0) {
    return 13.6*u.MeV() * sqrt(x/X0) * (1. + 0.038*log(x/X0)) * sqrt(pow(m,2) + pow(p,2)) / pow(p,2);
}



StepIBeamLine::~StepIBeamLine(){
    delete _gg0;
    delete _gg7;
    delete _gg8;
    delete _gg9;
    delete Q7;
    delete Q8;
    delete Q9;

}

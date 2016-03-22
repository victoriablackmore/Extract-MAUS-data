#include "slicequad.h"

SliceQuad::SliceQuad(std::string name, TGraph *ggradient, double z1, double z2, int ndiv=100) : Quadrupole(0, 0) {
    _name = "../QuadrupoleOutputs/"+name;

    TFile *fout = new TFile((_name+".root").c_str(), "recreate");

    std::cout << "Starting " << _name << std::endl;
    _ndiv = ndiv;
    _ggradient = ggradient;

    // step length
    const int initialsteps = 200;
    double initialstep = (z2 - z1)/static_cast<double>(initialsteps);

    // output graphs
    _gF11 = new TGraph(); _gD11 = new TGraph();
    _gF12 = new TGraph(); _gD12 = new TGraph();
    _gF21 = new TGraph(); _gD21 = new TGraph();
    _gF22 = new TGraph(); _gD22 = new TGraph();
    int ip = 0;

    // for a range of momenta....
    for(double p=50.0; p<500.0; p+=1.0){
        TMatrixD F(2, 2);
        TMatrixD D(2, 2);

        F(0, 0) = 1.0;
        F(0, 1) = 0.0;
        F(1, 0) = 0.0;
        F(1, 1) = 1.0;

        D(0, 0) = 1.0;
        D(0, 1) = 0.0;
        D(1, 0) = 0.0;
        D(1, 1) = 1.0;

        // take steps in z:
        for(int i = 0; i < initialsteps; ++i){
            double z = z1 + 0.5*initialstep + initialstep*i*1.0;

            TopHat *slice = new TopHat(TMath::Abs(ggradient->Eval(z))*u.T()/u.m(), initialstep);

            D = slice->Defocus(p) * D;
            F = slice->Focus(p) * F;
        }
        _gD11->SetPoint(ip, p, D(0, 0));
        _gD12->SetPoint(ip, p, D(0, 1));
        _gD21->SetPoint(ip, p, D(1, 0));
        _gD22->SetPoint(ip, p, D(1, 1));
        _gF11->SetPoint(ip, p, F(0, 0));
        _gF12->SetPoint(ip, p, F(0, 1));
        _gF21->SetPoint(ip, p, F(1, 0));
        _gF22->SetPoint(ip, p, F(1, 1));
        ++ip;
    }

    //Format curves
    _gD11->SetLineStyle(2);_gD11->SetLineColor(kRed);_gF11->SetLineColor(kBlue);
    _gD12->SetLineStyle(2);_gD12->SetLineColor(kRed);_gF12->SetLineColor(kBlue);
    _gD21->SetLineStyle(2);_gD21->SetLineColor(kRed);_gF21->SetLineColor(kBlue);
    _gD22->SetLineStyle(2);_gD22->SetLineColor(kRed);_gF22->SetLineColor(kBlue);

    //Draw canvas
    _mg11 = new TMultiGraph (("_mg11"+_name).c_str(),"M_{11}");
    _mg12 = new TMultiGraph (("_mg12"+_name).c_str(),"M_{12}");
    _mg21 = new TMultiGraph (("_mg21"+_name).c_str(),"M_{21}");
    _mg22 = new TMultiGraph (("_mg22"+_name).c_str(),"M_{22}");
    _mg11->Add(_gF11,"L");_mg11->Add(_gD11,"L");
    _mg12->Add(_gF12,"L");_mg12->Add(_gD12,"L");
    _mg21->Add(_gF21,"L");_mg21->Add(_gD21,"L");
    _mg22->Add(_gF22,"L");_mg22->Add(_gD22,"L");
    _can = new TCanvas(("_can"+_name).c_str(), _name.c_str(), 1000, 1000);
    _can->Divide(2,2);
    _can->cd(1);_mg11->Draw("A");
    _can->cd(2);_mg12->Draw("A");
    _can->cd(3);_mg21->Draw("A");
    _can->cd(4);_mg22->Draw("A");
    _can->Write("c");
    _mg11->Write("mg11");
    _mg12->Write("mg12");
    _mg21->Write("mg21");
    _mg22->Write("mg22");
    _gF11->Write("gF11");_gD11->Write("gD11");
    _gF12->Write("gF12");_gD12->Write("gD12");
    _gF21->Write("gF21");_gD21->Write("gD21");
    _gF22->Write("gF22");_gD22->Write("gD22");

    //For path length measurement
    double step = (z2-z1)/static_cast<double>(_ndiv);
    _gstep = new TGraph();
    int p = 0;
    for (int i=0; i<_ndiv; ++i) {
        double z = z1 + .5*step + step*i;
        double g = _ggradient->Eval(z);
        _gstep->SetPoint(p++, z-.5*step, g);
        _gstep->SetPoint(p++, z+.5*step, g);
        hats.push_back(new TopHat(g*u.T()/u.m(), step));
    }
    _gstep->Write(("gstep"+_name).c_str());

    fout->Write();
    std::cout << "Finishing " << _name << std::endl;
}

TMatrixD SliceQuad::Defocus(double P){
    TMatrixD M(2, 2);
    M(0, 0) = _gD11->Eval(P);
    M(0, 1) = _gD12->Eval(P);
    M(1, 0) = _gD21->Eval(P);
    M(1, 1) = _gD22->Eval(P);
    return M;
}

TMatrixD SliceQuad::Focus(double P){
    TMatrixD M(2, 2);
    M(0, 0) = _gF11->Eval(P);
    M(0, 1) = _gF12->Eval(P);
    M(1, 0) = _gF21->Eval(P);
    M(1, 1) = _gF22->Eval(P);
    return M;
}

double SliceQuad::phi(double k0, double l){
    return 0.0;
}

double SliceQuad::PathLength(double &xF, double &aF, double &xD, double &aD, double const P) {
    double path = 0.0;
   // printf("---------------------------------\n");
    for (int i=0; i<_ndiv; ++i) {
        double dpath = hats[i]->PathLength(xF, aF, xD, aD, P);
        path += dpath;



       // printf("xF=%f\t dpath=%f\t path=%f\n",xF,dpath,path);
    }
    return path;
}



SliceQuad::~SliceQuad(){

    delete _gF11;
    delete _gF12;
    delete _gF21;
    delete _gF22;
    delete _gD11;
    delete _gD12;
    delete _gD21;
    delete _gD22;
    delete _mg11;
    delete _mg12;
    delete _mg21;
    delete _mg22;
    delete _can;



    for (int i = 0; i < hats.size(); i++){
        delete hats.at(i);
    }
    hats.clear();

    //delete _ggradient;
    delete _gstep;
}

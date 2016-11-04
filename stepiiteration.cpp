#include "stepiiteration.h"



StepIIteration::StepIIteration(StepIBeamLine *beamline)
{
    //_name = name;
    //_file = new TFile(_name.c_str(), "recreate");


    _beamline = beamline;
    _z = _beamline->GetZ1() - _beamline->GetZ0();

    //The output tree
    /*
    _T = new TTree("T", "T");
    _T->Branch("good", &_good, "good/I");
    _T->Branch("delta_in", &_delta_in, "delta_in/D");
    _T->Branch("t", &_t,"t/D");
    _T->Branch("z", &_z,"z/D");
    _T->Branch("beta", &_beta, "beta/D");
    _T->Branch("gamma", &_gamma, "gamma/D");
    _T->Branch("P", &_P,"P/D");
    _T->Branch("dP", &_dP,"dP/D");
    _T->Branch("x0",&_x0,"x0/D");
    _T->Branch("y0",&_y0,"y0/D");
    _T->Branch("x1",&_x1,"x1/D");
    _T->Branch("y1",&_y1,"y1/D");
    _T->Branch("ax0",&_ax0,"ax0/D");
    _T->Branch("ay0",&_ay0,"ay0/D");
    _T->Branch("ax1",&_ax1,"ax1/D");
    _T->Branch("ay1",&_ay1,"ay1/D");
    _T->Branch("delta_out", &_delta_out, "delta_out/D");
    _T->Branch("Ax", &_Ax, "Ax/D");
    _T->Branch("Ay", &_Ay, "Ay/D");
    _T->Branch("Bx", &_Bx, "Bx/D");
    _T->Branch("By", &_By, "By/D");
    _T->Branch("muonID", &_muonIDNumber, "muonID/I");
    */

    VBS = false;
}

void StepIIteration::ReconstructEvent(double t, double &delta, double x0, double x1, double y0, double y1, double mass, int muonIDNumber){
    _muonIDNumber = muonIDNumber;
    ReconstructEvent(t, delta, x0, x1, y0, y1, mass);
}



void StepIIteration::ReconstructEvent(double t, double &delta, double x0, double x1, double y0, double y1, double mass) {

    //Input estimate of excess path length for these calculations
    _delta_in = delta;

    //Fill TTree branches with new leaves
    _t = t;
    _mass = 105.658;
    _x0 = x0;
    _y0 = y0;
    _x1 = x1;
    _y1 = y1;

    //Calculate momentum estimate based on input estimate of delta
    calculateMomentum(_t, _delta_in, _mass, _beta, _gamma, _P);

    //Reconstruct angles
    if (VBS) printf("%f %f %f %f %f %f %f %f %f\n",_x0, _ax0, _y0, _ay0, _x1, _ax1, _y1, _ay1, _P);
    _beamline->CalculateAngles(_x0, _ax0, _y0, _ay0, _x1, _ax1, _y1, _ay1, _P);
    if (VBS) printf("%f %f %f %f %f %f %f %f %f\n",_x0, _ax0, _y0, _ay0, _x1, _ax1, _y1, _ay1, _P);

    //Calculate new improved estimate of excess path length delta
    _delta_out = _beamline->CalculatePathLength(_x0, _ax0, _y0, _ay0, _P) - _z;

    if(_delta_out > 50.0*u.mm()){
        _good = 0;
    }
    //_delta_out = 30.0*u.mm();

    if (VBS) printf("delta = %f\n",_delta_out);

    //Make P an estimate of p before TOF1 by accounting for air (we think)
    //_P -= 0.9*CLHEP::MeV;
    _dP = momentumCorrection(_P);
    _P -= _dP;


    //Fill TTree
    _good = 1;

   // if(_delta_out > 100.0*u.mm()){
   //     _good = 0;
   //     std::cerr << "Warning: _delta_out > 100 mm\n";
   // }
    _Ax = _beamline->GetAx();
    _Ay = _beamline->GetAy();
    _Bx = _beamline->GetBx();
    _By = _beamline->GetBy();
   // _T->Fill();

    //Better delta estimate by reference is output here, input for another ietartion if required
    //delta = _delta_out;
    delta = _delta_in + 0.5*(_delta_out-_delta_in);//damp delta
}

QHash<QString, double> StepIIteration::Result(){
    QHash<QString, double> result;
    result["x0"] = _x0;
    result["y0"] = _y0;
    result["ax0"] = _ax0;
    result["ay0"] = _ay0;
    result["x1"] = _x1;
    result["y1"] = _y1;
    result["ax1"] = _ax1;
    result["ay1"] = _ay1;
    result["P"] = _P;
    result["dP"] = _dP;
    result["good"] = _good;
    result["mass"] = _mass;

    return result;
}


void StepIIteration::Bad(double t, double x0, double x1, double y0, double y1, double mass, int muonIDNumber){
    _muonIDNumber = muonIDNumber;
    Bad(t, x0, x1, y0, y1, mass);
}

void StepIIteration::Bad(double t, double x0, double x1, double y0, double y1, double mass) {

    _good = 0;
    _delta_in = 0;
    _t = t;
    _mass = mass;
    _x0 = x0;
    _y0 = y0;
    _x1 = x1;
    _y1 = y1;
    _delta_out = 0;
    _dP = 0;
    _P = 0;
    //_T->Fill();

}

void StepIIteration::CHEAT(double t, double &delta, double x0, double ax0, double y0, double ay0, double mass) {

    //Input estimate of excess path length for these calculations
    _delta_in = delta;

    //Fill TTree branches with new leaves
    _t = t;
    _mass = mass;
    _x0  = x0;
    _ax0 = ax0;
    _y0  = y0;
    _ay0 = ay0;

    //Calculate momentum estimate based on input estimate of delta
    calculateMomentum(_t, _delta_in, _mass, _beta, _gamma, _P);

    //No need to reconstruct angles - that's our cheat

    //Calculate new improved estimate of excess path length delta
    _delta_out = _beamline->CalculatePathLength(_x0, _ax0, _y0, _ay0, _P) - _z;
    //if (_delta_out<0.) printf("-ve delta: x0=%f, ax0=%f, y0=%f, y1=%f\n", );

    //Fill TTree
    _good = 1;
    //_T->Fill();

    //Make P an estimate of p before TOF1 by accounting for air (we think)
    _dP = momentumCorrection(_P);
    _P -= _dP;

    //Better delta estimate by reference is output here, input for another ietartion if required
    //delta = _delta_out;
    delta = _delta_in + 0.5*(_delta_out-_delta_in);//damp delta
}

void StepIIteration::calculateMomentum(double t, double delta, double mass, double &beta, double &gamma, double &P) {
    beta = einstein_beta(_z + delta, t);
    if (VBS) printf("beta = %f\n",beta);
    gamma = einstein_gamma(beta);
    P = rel_mom(beta, gamma, mass);

    //std::cerr << "P = " << P << ", given beta = " << beta << " and gamma = " << gamma << "\n";
}

void StepIIteration::Write() {
    //_T->CloneTree();
    //_file->Write();
}

double StepIIteration::einstein_beta(double z, double t) {
    return ( z / t ) / u.c_light();
}

double StepIIteration::einstein_gamma(double beta) {
    return 1. / sqrt( 1 - beta*beta );
}

double StepIIteration::rel_mom(double beta, double gamma, double mass) {
    return beta * gamma * mass;//no c as m in natural units
}

double StepIIteration::momentumCorrection(double p) {
    double E = sqrt(105.658*105.658 + p*p);

    //Air
    double density_air = 1.205e-3*u.g()/u.cm3();//g cm-3
    double dp_air = (E/p) * (_z/2.)/u.cm() * dEdx(p, 0.49919, 85.7 * u.eV()) * density_air;

    //BPM was present in Step I.. only need this if looking at old data:
//    double density_BPM = 1.06*u.g()/u.cm3();//g cm-3
//    double dp_BPM = (E/p) * ((-360*u.mm() -_beamline->GetZ0())/(_beamline->GetZ1()-_beamline->GetZ0()))*(2*0.8*u.mm())/u.cm() * dEdx(p, 0.53768, 67.8 *u.eV()) * density_BPM;//BPM is at -360mm

    return dp_air; // + dp_BPM;

}

double StepIIteration::dEdx(double p, double ZoverA, double I) {
    //I should be in eV
    double K = 0.307075 * u.MeV();//g-1 cm2
    double m_e = 0.510998910 * u.MeV();
    double E = sqrt(105.658*105.658 + p*p);
    double beta = p / E;
    double gamma = einstein_gamma(beta);
    return K * ZoverA * (log(2.*m_e*pow(beta*gamma,2.)/I)/pow(beta,2) - 1.);
}

StepIIteration::~StepIIteration(){
    //delete _beamline;
    //delete _T;
    //delete _file;

}

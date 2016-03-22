#include "stepitracking.h"

StepITracking::StepITracking(std::string tag, StepIBeamLine *beamLine)//, QString outputFileName)
{
    beamline = beamLine;
    beamline->WriteOptics(tag);



    iteration0 = new StepIIteration(beamline);
    iteration1 = new StepIIteration(beamline);
    iteration2 = new StepIIteration(beamline);
    iteration3 = new StepIIteration(beamline);
    iteration4 = new StepIIteration(beamline);
    iteration5 = new StepIIteration(beamline);
    iteration6 = new StepIIteration(beamline);
    iteration7 = new StepIIteration(beamline);
    iteration8 = new StepIIteration(beamline);
    iteration9 = new StepIIteration(beamline);
}
void StepITracking::ReconstructEvent(double const t, double x0, double x1, double y0, double y1, double mass) {

    //Excess path length - initial guess is 0
    double delta = 0.0;

    iteration0->ReconstructEvent(t, delta, x0, x1, y0, y1, mass);
    iteration1->ReconstructEvent(t, delta, x0, x1, y0, y1, mass);
    iteration2->ReconstructEvent(t, delta, x0, x1, y0, y1, mass);
    iteration3->ReconstructEvent(t, delta, x0, x1, y0, y1, mass);
    iteration4->ReconstructEvent(t, delta, x0, x1, y0, y1, mass);
    iteration5->ReconstructEvent(t, delta, x0, x1, y0, y1, mass);
    iteration6->ReconstructEvent(t, delta, x0, x1, y0, y1, mass);
    iteration7->ReconstructEvent(t, delta, x0, x1, y0, y1, mass);
    iteration8->ReconstructEvent(t, delta, x0, x1, y0, y1, mass);
    iteration9->ReconstructEvent(t, delta, x0, x1, y0, y1, mass);

}

void StepITracking::ReconstructEvent(double const t, double x0, double x1, double y0, double y1, double mass, int muonIDNumber){

    double delta = 0.0;

    iteration0->ReconstructEvent(t, delta, x0, x1, y0, y1, mass, muonIDNumber);
    iteration1->ReconstructEvent(t, delta, x0, x1, y0, y1, mass, muonIDNumber);
    iteration2->ReconstructEvent(t, delta, x0, x1, y0, y1, mass, muonIDNumber);
    iteration3->ReconstructEvent(t, delta, x0, x1, y0, y1, mass, muonIDNumber);
    iteration4->ReconstructEvent(t, delta, x0, x1, y0, y1, mass, muonIDNumber);
    iteration5->ReconstructEvent(t, delta, x0, x1, y0, y1, mass, muonIDNumber);
    iteration6->ReconstructEvent(t, delta, x0, x1, y0, y1, mass, muonIDNumber);
    iteration7->ReconstructEvent(t, delta, x0, x1, y0, y1, mass, muonIDNumber);
    iteration8->ReconstructEvent(t, delta, x0, x1, y0, y1, mass, muonIDNumber);
    iteration9->ReconstructEvent(t, delta, x0, x1, y0, y1, mass, muonIDNumber);
}

QHash<QString, double> StepITracking::Result(){
    return iteration9->Result();
}

void StepITracking::Bad(double t, double x0, double x1, double y0, double y1, double mass) {

    iteration0->Bad(t, x0, x1, y0, y1, mass);
    iteration1->Bad(t, x0, x1, y0, y1, mass);
    iteration2->Bad(t, x0, x1, y0, y1, mass);
    iteration3->Bad(t, x0, x1, y0, y1, mass);
    iteration4->Bad(t, x0, x1, y0, y1, mass);
    iteration5->Bad(t, x0, x1, y0, y1, mass);
    iteration6->Bad(t, x0, x1, y0, y1, mass);
    iteration7->Bad(t, x0, x1, y0, y1, mass);
    iteration8->Bad(t, x0, x1, y0, y1, mass);
    iteration9->Bad(t, x0, x1, y0, y1, mass);
}

void StepITracking::Bad(double t, double x0, double x1, double y0, double y1, double mass, int muonIDNumber){
    iteration0->Bad(t, x0, x1, y0, y1, mass, muonIDNumber);
    iteration1->Bad(t, x0, x1, y0, y1, mass, muonIDNumber);
    iteration2->Bad(t, x0, x1, y0, y1, mass, muonIDNumber);
    iteration3->Bad(t, x0, x1, y0, y1, mass, muonIDNumber);
    iteration4->Bad(t, x0, x1, y0, y1, mass, muonIDNumber);
    iteration5->Bad(t, x0, x1, y0, y1, mass, muonIDNumber);
    iteration6->Bad(t, x0, x1, y0, y1, mass, muonIDNumber);
    iteration7->Bad(t, x0, x1, y0, y1, mass, muonIDNumber);
    iteration8->Bad(t, x0, x1, y0, y1, mass, muonIDNumber);
    iteration9->Bad(t, x0, x1, y0, y1, mass, muonIDNumber);
}

void StepITracking::CHEAT(double const t, double x0, double ax0, double y0, double ay0, double mass) {

    //Excess path length - initial guess a simple 0
    double delta = 0.;

    iteration0->CHEAT(t, delta, x0, ax0, y0, ay0, mass);
    iteration1->CHEAT(t, delta, x0, ax0, y0, ay0, mass);
    iteration2->CHEAT(t, delta, x0, ax0, y0, ay0, mass);
    iteration3->CHEAT(t, delta, x0, ax0, y0, ay0, mass);
    iteration4->CHEAT(t, delta, x0, ax0, y0, ay0, mass);
    iteration5->CHEAT(t, delta, x0, ax0, y0, ay0, mass);
    iteration6->CHEAT(t, delta, x0, ax0, y0, ay0, mass);
    iteration7->CHEAT(t, delta, x0, ax0, y0, ay0, mass);
    iteration8->CHEAT(t, delta, x0, ax0, y0, ay0, mass);
    iteration9->CHEAT(t, delta, x0, ax0, y0, ay0, mass);
}

void StepITracking::CHEAT(double const t, double x0, double ax0, double y0, double ay0, double mass, int muonIDNumber){
    CHEAT(t, x0, ax0, y0, ay0, mass);
}
/*
void StepITracking::Write() {
    iteration0->Write();
    iteration1->Write();
    iteration2->Write();
    iteration3->Write();
    iteration4->Write();
    iteration5->Write();
    iteration6->Write();
    iteration7->Write();
    iteration8->Write();
    iteration9->Write();
}
*/

StepITracking::~StepITracking(){
    delete iteration0;
    delete iteration1;
    delete iteration2;
    delete iteration3;
    delete iteration4;
    delete iteration5;
    delete iteration6;
    delete iteration7;
    delete iteration8;
    delete iteration9;
    delete beamline;

}

#ifndef STEPITRACKING_H
#define STEPITRACKING_H

#include "stepibeamline.h"
#include "stepiiteration.h"
#include "units.h"
#include "TMath.h"
#include "TTree.h"
#include <string>
#include <QString>
#include <QHash>

class StepITracking
{
public:
    StepITracking(std::string tag, StepIBeamLine *beamLine);//, QString outputFileName);
    ~StepITracking();
    void ReconstructEvent(double dt, double x0, double x1, double y0, double y1, double mass);
    void Bad(double dt, double x0, double x1, double y0, double y1, double mass);
    void CHEAT(double dt, double x0, double ax0, double y0, double ay0, double mass);
    //void Write();
    StepIBeamLine *GetBeamLine() {return beamline;};

    // added by VBlackmore, Nov 2012
    void ReconstructEvent(double dt, double x0, double x1, double y0, double y1, double mass, int muonIDNumber);
    void Bad(double dt, double x0, double x1, double y0, double y1, double mass, int muonIDNumber);
    void CHEAT(double dt, double x0, double ax0, double y0, double ay0, double mass, int muonIDNumber);

    QHash<QString, double> Result();

private:
    StepIBeamLine *beamline;
    StepIIteration *iteration0, *iteration1, *iteration2, *iteration3, *iteration4, *iteration5, *iteration6, *iteration7, *iteration8, *iteration9;

};

#endif // STEPITRACKING_H

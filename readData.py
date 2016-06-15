#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from ROOT import TFile, TCanvas, gDirectory, TTree, TH1F, TH2F, TMath, TF1


def main():
    filename = "../../MICE Data/Data run 7469/run7469_extracted_data__MAUS2pt3pt1_b.root"
    #filename = "../../MICE Data/MC run 7469/mc_3mm200_07469_MAUS2pt3pt1_extracted_data_a.root"
    
    f = TFile(filename, 'read')
    tree = f.Get("T")
    
    cut_numbers = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17]
    
    for i in cut_numbers:
        print 'Processing cut ', i
        plot_pass_fail_cut(tree, i)

#plot_pass_fail_cut(tree, 16)
#    plot_pass_fail_cut(tree, 17)



def plot_pass_fail_cut(tree, cut_number):
    """
        _noCut: Fill into histogram, regardless of whether it passes any cuts
        _cut1: Fill histogram if PMT-timed position at TOF0 is OK
        _cut2: Fill histogram if PMT-timed position at TOF1 is OK
        _cut3: Fill histogram if Rayner Reconstruction at TOFs is OK
        _cut4: Fill histogram if time-of-flight is OK
        _cut5: Fill histogram if all 5 tracker planes were hit
        _cut6: Fill histogram if all detectors hit
        _cut7: Fill histogram if there was only one spacepoint at TOF0
        _cut8: Fill histogram if there was only one spacepoint at TOF1
        _cut9: Fill histogram if there was only one track in TKU
        
        _cut10: Fill histogram if _cut1 && _cut2 && _cut3 && _cut4
        _cut11: Fill histogram if _cut10 && _cut7 && _cut8
        _cut12: Fill histogram if _cut11 && _cut6 && _cut9
        _cut13: Fill if _cut12 && Pvalue of track > 0.01
        _cut14: Fill if all cuts are passed
        
        _cut16: Fill if _cut6, _cut7, _cut8, _cut9 true
        _cut17: Fill if Pvalue of track > 0.01
        
        """
    
    tof0_x_pass = TH1F('tof0_x_pass', 'TOF0 x pass; x (mm)', 100, -200.0, 200.0)
    tof0_y_pass = TH1F('tof0_y_pass', 'TOF0 y pass; y (mm)', 100, -200.0, 200.0)
    tof0_px_pass = TH1F('tof0_px_pass', 'TOF0 px pass; px (MeV/c)', 50, -100.0, 100.0)
    tof0_py_pass = TH1F('tof0_py_pass', 'TOF0 py pass; py (MeV/c)', 50, -100.0, 100.0)
    tof0_pz_pass = TH1F('tof0_pz_pass', 'TOF0 pz pass; pz (MeV/c)', 100, 100.0, 300.0)
    tof0_xPixel_pass = TH1F('tof0_xPixel_pass', 'TOF0 xPixel pass; x pixel (mm)', 10, -200.0, 200.0)
    tof0_yPixel_pass = TH1F('tof0_yPixel_pass', 'TOF0 yPixel pass; y pixel (mm)', 10, -200.0, 200.0)
    
    tof0_xpx_pass = TH2F('tof0_xpx_pass', 'TOF0 xPx pass; x (mm); px (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    tof0_ypy_pass = TH2F('tof0_ypy_pass', 'TOF0 yPy pass; y (mm); py (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    
    tof0_x_fail = TH1F('tof0_x_fail', 'TOF0 x fail; x (mm)', 100, -200.0, 200.0)
    tof0_y_fail = TH1F('tof0_y_fail', 'TOF0 y fail; y (mm)', 100, -200.0, 200.0)
    tof0_px_fail = TH1F('tof0_px_fail', 'TOF0 px fail; px (MeV/c)', 50, -100.0, 100.0)
    tof0_py_fail = TH1F('tof0_py_fail', 'TOF0 py fail; py (MeV/c)', 50, -100.0, 100.0)
    tof0_pz_fail = TH1F('tof0_pz_fail', 'TOF0 pz fail; pz (MeV/c)', 100, 100.0, 300.0)
    tof0_xPixel_fail = TH1F('tof0_xPixel_fail', 'TOF0 xPixel fail; x pixel (mm)', 10, -200.0, 200.0)
    tof0_yPixel_fail = TH1F('tof0_yPixel_fail', 'TOF0 yPixel fail; y pixel (mm)', 10, -200.0, 200.0)
    tof0_xpx_fail = TH2F('tof0_xpx_fail', 'TOF0 xPx fail; x (mm); px (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    tof0_ypy_fail = TH2F('tof0_ypy_fail', 'TOF0 yPy fail; y (mm); py (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    
    tof1_x_pass = TH1F('tof1_x_pass', 'TOF1 x pass; x (mm)', 100, -200.0, 200.0)
    tof1_y_pass = TH1F('tof1_y_pass', 'TOF1 y pass; y (mm)', 100, -200.0, 200.0)
    tof1_px_pass = TH1F('tof1_px_pass', 'TOF1 px pass; px (MeV/c)', 50, -100.0, 100.0)
    tof1_py_pass = TH1F('tof1_py_pass', 'TOF1 py pass; py (MeV/c)', 50, -100.0, 100.0)
    tof1_pz_pass = TH1F('tof1_pz_pass', 'TOF1 pz pass; pz (MeV/c)', 100, 100.0, 300.0)
    tof1_xPixel_pass = TH1F('tof1_xPixel_pass', 'TOF1 xPixel pass; x pixel (mm)', 10, -200.0, 200.0)
    tof1_yPixel_pass = TH1F('tof1_yPixel_pass', 'TOF1 yPixel pass; y pixel (mm)', 10, -200.0, 200.0)
    
    tof1_xpx_pass = TH2F('tof1_xpx_pass', 'TOF1 xPx pass; x (mm); px (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    tof1_ypy_pass = TH2F('tof1_ypy_pass', 'TOF1 yPy pass; y (mm); py (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    
    tof1_x_fail = TH1F('tof1_x_fail', 'TOF1 x fail; x (mm)', 100, -200.0, 200.0)
    tof1_y_fail = TH1F('tof1_y_fail', 'TOF1 y fail; y (mm)', 100, -200.0, 200.0)
    tof1_px_fail = TH1F('tof1_px_fail', 'TOF1 px fail; px (MeV/c)', 50, -100.0, 100.0)
    tof1_py_fail = TH1F('tof1_py_fail', 'TOF1 py fail; py (MeV/c)', 50, -100.0, 100.0)
    tof1_pz_fail = TH1F('tof1_pz_fail', 'TOF1 pz fail; pz (MeV/c)', 100, 100.0, 300.0)
    tof1_xPixel_fail = TH1F('tof1_xPixel_fail', 'TOF1 xPixel fail; x pixel (mm)', 10, -200.0, 200.0)
    tof1_yPixel_fail = TH1F('tof1_yPixel_fail', 'TOF1 yPixel fail; y pixel (mm)', 10, -200.0, 200.0)
    
    tof1_xpx_fail = TH2F('tof1_xpx_fail', 'TOF1 xPx fail; x (mm); px (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    tof1_ypy_fail = TH2F('tof1_ypy_fail', 'TOF1 yPy fail; y (mm); py (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    
    tku_x_pass = TH1F('tku_x_pass', 'TKU x pass; x (mm)', 100, -200.0, 200.0)
    tku_y_pass = TH1F('tku_y_pass', 'TKU y pass; y (mm)', 100, -200.0, 200.0)
    tku_px_pass = TH1F('tku_px_pass', 'TKU px pass; px (MeV/c)', 50, -100.0, 100.0)
    tku_py_pass = TH1F('tku_py_pass', 'TKU py pass; py (MeV/c)', 50, -100.0, 100.0)
    tku_pz_pass = TH1F('tku_pz_pass', 'TKU pz pass; pz (MeV/c)', 100, 100.0, 300.0)
    
    tku_xpx_pass = TH2F('tku_xpx_pass', 'TKU xPx pass; x (mm); px (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    tku_ypy_pass = TH2F('tku_ypy_pass', 'TKU yPy pass; y (mm); py (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    
    tku_x_fail = TH1F('tku_x_fail', 'TKU x fail; x (mm)', 100, -200.0, 200.0)
    tku_y_fail = TH1F('tku_y_fail', 'TKU y fail; y (mm)', 100, -200.0, 200.0)
    tku_px_fail = TH1F('tku_px_fail', 'TKU px fail; px (MeV/c)', 50, -100.0, 100.0)
    tku_py_fail = TH1F('tku_py_fail', 'TKU py fail; py (MeV/c)', 50, -100.0, 100.0)
    tku_pz_fail = TH1F('tku_pz_fail', 'TKU pz fail; pz (MeV/c)', 100, 100.0, 300.0)
    tku_xpx_fail = TH2F('tku_xpx_fail', 'TKU xPx fail; x (mm); px (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    tku_ypy_fail = TH2F('tku_ypy_fail', 'TKU yPy fail; y (mm); py (MeV/c)', 100, -200.0, 200.0, 50, -100.0, 100.0)
    
    
    for entry in tree:
        passes = False
        if cut_number == 0:
            passes = True
        if cut_number == 1:
            if entry.cut_TOF0_goodPMTPosition == 1:
                passes = True
        if cut_number == 2:
            if entry.cut_TOF1_goodPMTPosition == 1:
                passes = True
        if cut_number == 3:
            if entry.cut_goodRaynerReconstruction == 1:
                passes = True
        if cut_number == 4:
            if entry.cut_TimeOfFlight == 1:
                passes = True
        if cut_number == 5:
            if entry.cut_TKU_hitAllStations == 1:
                passes = True
        if cut_number == 6:
            if entry.cut_hit_all_detectors == 1:
                passes = True
        if cut_number == 7:
            if entry.cut_TOF0_singleHit == 1:
                passes = True
        if cut_number == 8:
            if entry.cut_TOF1_singleHit == 1:
                passes = True
        if cut_number == 9:
            if entry.cut_TKU_singleTrack == 1:
                passes = True
        if cut_number == 10:
            if entry.cut_TOF0_goodPMTPosition == 1 and entry.cut_TOF1_goodPMTPosition == 1 and entry.cut_goodRaynerReconstruction == 1 and entry.cut_TimeOfFlight == 1:
                passes = True
        if cut_number == 11:
            if entry.cut_TOF0_goodPMTPosition == 1 and entry.cut_TOF1_goodPMTPosition == 1 and entry.cut_goodRaynerReconstruction == 1 and entry.cut_TimeOfFlight == 1 and entry.cut_TOF0_singleHit == 1 and entry.cut_TOF1_singleHit == 1:
                passes = True
        if cut_number == 12:
            if entry.cut_TOF0_goodPMTPosition == 1 and entry.cut_TOF1_goodPMTPosition == 1 and entry.cut_goodRaynerReconstruction == 1 and entry.cut_TimeOfFlight == 1 and entry.cut_TOF0_singleHit == 1 and entry.cut_TOF1_singleHit == 1 and entry.cut_hit_all_detectors == 1 and entry.cut_TKU_singleTrack == 1:
                passes = True
        if cut_number == 13:
            if entry.cut_TOF0_goodPMTPosition == 1 and entry.cut_TOF1_goodPMTPosition == 1 and entry.cut_goodRaynerReconstruction == 1 and entry.cut_TimeOfFlight == 1 and entry.cut_TOF0_singleHit == 1 and entry.cut_TOF1_singleHit == 1 and entry.cut_hit_all_detectors == 1 and entry.cut_TKU_singleTrack == 1 and entry.cut_TKU_PValue == 1:
                passes = True
        if cut_number == 14:
            if entry.cut_allPassed == 1:
                passes = True
        if cut_number == 15:
            if entry.cut_allPassed == 1 and entry.cut_tof1_tku_momentum == 1:
                passes = True
    
        if cut_number == 16:
            #_cut16: Fill if _cut6, _cut7, _cut8, _cut9 true
            #_cut17: Fill if Pvalue of track > 0.01
            if entry.cut_TOF0_singleHit == 1 and entry.cut_TOF1_singleHit == 1 and entry.cut_TKU_singleTrack == 1 and entry.cut_hit_all_detectors == 1:
                passes = True
        if cut_number == 17:
            if entry.cut_TKU_PValue == 1:
                passes = True
                        

    
    
        if passes == True:
            # fill pass histograms
            tof0_x_pass.Fill(entry.TOF0_x)
            tof0_y_pass.Fill(entry.TOF0_y)
            tof0_px_pass.Fill(entry.TOF0_px)
            tof0_py_pass.Fill(entry.TOF0_py)
            tof0_pz_pass.Fill(entry.TOF0_pz)
            tof0_xPixel_pass.Fill(entry.TOF0_xPixel)
            tof0_yPixel_pass.Fill(entry.TOF0_yPixel)
            tof0_xpx_pass.Fill(entry.TOF0_x, entry.TOF0_px)
            tof0_ypy_pass.Fill(entry.TOF0_y, entry.TOF0_py)
            
            tof1_x_pass.Fill(entry.TOF1_x)
            tof1_y_pass.Fill(entry.TOF1_y)
            tof1_px_pass.Fill(entry.TOF1_px)
            tof1_py_pass.Fill(entry.TOF1_py)
            tof1_pz_pass.Fill(entry.TOF1_pz)
            tof1_xPixel_pass.Fill(entry.TOF1_xPixel)
            tof1_yPixel_pass.Fill(entry.TOF1_yPixel)
            tof1_xpx_pass.Fill(entry.TOF1_x, entry.TOF1_px)
            tof1_ypy_pass.Fill(entry.TOF1_y, entry.TOF1_py)
            
            tku_x_pass.Fill(entry.TKU_s1_x)
            tku_y_pass.Fill(entry.TKU_s1_y)
            tku_px_pass.Fill(entry.TKU_s1_px)
            tku_py_pass.Fill(entry.TKU_s1_py)
            tku_pz_pass.Fill(entry.TKU_s1_pz)
            
            tku_xpx_pass.Fill(entry.TKU_s1_x, entry.TKU_s1_px)
            tku_ypy_pass.Fill(entry.TKU_s1_y, entry.TKU_s1_py)
        else:
            # fill fail histograms
            tof0_x_fail.Fill(entry.TOF0_x)
            tof0_y_fail.Fill(entry.TOF0_y)
            tof0_px_fail.Fill(entry.TOF0_px)
            tof0_py_fail.Fill(entry.TOF0_py)
            tof0_pz_fail.Fill(entry.TOF0_pz)
            tof0_xPixel_fail.Fill(entry.TOF0_xPixel)
            tof0_yPixel_fail.Fill(entry.TOF0_yPixel)
            tof0_xpx_fail.Fill(entry.TOF0_x, entry.TOF0_px)
            tof0_ypy_fail.Fill(entry.TOF0_y, entry.TOF0_py)
            
            tof1_x_fail.Fill(entry.TOF1_x)
            tof1_y_fail.Fill(entry.TOF1_y)
            tof1_px_fail.Fill(entry.TOF1_px)
            tof1_py_fail.Fill(entry.TOF1_py)
            tof1_pz_fail.Fill(entry.TOF1_pz)
            tof1_xPixel_fail.Fill(entry.TOF1_xPixel)
            tof1_yPixel_fail.Fill(entry.TOF1_yPixel)
            tof1_xpx_fail.Fill(entry.TOF1_x, entry.TOF1_px)
            tof1_ypy_fail.Fill(entry.TOF1_y, entry.TOF1_py)
            
            tku_x_fail.Fill(entry.TKU_s1_x)
            tku_y_fail.Fill(entry.TKU_s1_y)
            tku_px_fail.Fill(entry.TKU_s1_px)
            tku_py_fail.Fill(entry.TKU_s1_py)
            tku_pz_fail.Fill(entry.TKU_s1_pz)
            
            tku_xpx_fail.Fill(entry.TKU_s1_x, entry.TKU_s1_px)
            tku_ypy_fail.Fill(entry.TKU_s1_y, entry.TKU_s1_py)

    canvas = TCanvas('c', 'c')
    saveAs = 'cut_'+str(cut_number)+'_'
    
    canvas.Divide(2, 1)
    canvas.cd(1)
    tof0_xPixel_pass.Draw('hist')
    canvas.cd(2)
    tof0_xPixel_fail.Draw('hist')
    canvas.Print(saveAs+'TOF0.pdf(')
    
    canvas.cd(1)
    tof0_x_pass.Draw('hist')
    canvas.cd(2)
    tof0_x_fail.Draw('hist')
    canvas.Print(saveAs+'TOF0.pdf(')
    
    canvas.cd(1)
    tof0_yPixel_pass.Draw('hist')
    canvas.cd(2)
    tof0_yPixel_fail.Draw('hist')
    canvas.Print(saveAs+'TOF0.pdf(')
    
    canvas.cd(1)
    tof0_y_pass.Draw('hist')
    canvas.cd(2)
    tof0_y_fail.Draw('hist')
    canvas.Print(saveAs+'TOF0.pdf(')
    
    canvas.cd(1)
    tof0_px_pass.Draw('hist')
    canvas.cd(2)
    tof0_px_fail.Draw('hist')
    canvas.Print(saveAs+'TOF0.pdf(')
    
    canvas.cd(1)
    tof0_py_pass.Draw('hist')
    canvas.cd(2)
    tof0_py_fail.Draw('hist')
    canvas.Print(saveAs+'TOF0.pdf(')
    
    canvas.cd(1)
    tof0_pz_pass.Draw('hist')
    canvas.cd(2)
    tof0_pz_fail.Draw('hist')
    canvas.Print(saveAs+'TOF0.pdf(')
    
    canvas.cd(1)
    tof0_xpx_pass.Draw('colz')
    canvas.cd(2)
    tof0_xpx_fail.Draw('colz')
    canvas.Print(saveAs+'TOF0.pdf(')
    
    canvas.cd(1)
    tof0_ypy_pass.Draw('colz')
    canvas.cd(2)
    tof0_ypy_fail.Draw('colz')
    canvas.Print(saveAs+'TOF0.pdf)')
    
    
    
    
    canvas.cd(1)
    tof1_xPixel_pass.Draw('hist')
    canvas.cd(2)
    tof1_xPixel_fail.Draw('hist')
    canvas.Print(saveAs+'TOF1.pdf(')
    
    canvas.cd(1)
    tof1_x_pass.Draw('hist')
    canvas.cd(2)
    tof1_x_fail.Draw('hist')
    canvas.Print(saveAs+'TOF1.pdf(')
    
    canvas.cd(1)
    tof1_yPixel_pass.Draw('hist')
    canvas.cd(2)
    tof1_yPixel_fail.Draw('hist')
    canvas.Print(saveAs+'TOF1.pdf(')
    
    canvas.cd(1)
    tof1_y_pass.Draw('hist')
    canvas.cd(2)
    tof1_y_fail.Draw('hist')
    canvas.Print(saveAs+'TOF1.pdf(')
    
    canvas.cd(1)
    tof1_px_pass.Draw('hist')
    canvas.cd(2)
    tof1_px_fail.Draw('hist')
    canvas.Print(saveAs+'TOF1.pdf(')
    
    canvas.cd(1)
    tof1_py_pass.Draw('hist')
    canvas.cd(2)
    tof1_py_fail.Draw('hist')
    canvas.Print(saveAs+'TOF1.pdf(')
    
    canvas.cd(1)
    tof1_pz_pass.Draw('hist')
    canvas.cd(2)
    tof1_pz_fail.Draw('hist')
    canvas.Print(saveAs+'TOF1.pdf(')
    
    canvas.cd(1)
    tof1_xpx_pass.Draw('colz')
    canvas.cd(2)
    tof1_xpx_fail.Draw('colz')
    canvas.Print(saveAs+'TOF1.pdf(')
    
    canvas.cd(1)
    tof1_ypy_pass.Draw('colz')
    canvas.cd(2)
    tof1_ypy_fail.Draw('colz')
    canvas.Print(saveAs+'TOF1.pdf)')
    
    
    
    
    
    
    
    canvas.cd(1)
    tku_x_pass.Draw('hist')
    canvas.cd(2)
    tku_x_fail.Draw('hist')
    canvas.Print(saveAs+'TKU.pdf(')
    
    canvas.cd(1)
    tku_y_pass.Draw('hist')
    canvas.cd(2)
    tku_y_fail.Draw('hist')
    canvas.Print(saveAs+'TKU.pdf(')
    
    canvas.cd(1)
    tku_px_pass.Draw('hist')
    canvas.cd(2)
    tku_px_fail.Draw('hist')
    canvas.Print(saveAs+'TKU.pdf(')
    
    canvas.cd(1)
    tku_py_pass.Draw('hist')
    canvas.cd(2)
    tku_py_fail.Draw('hist')
    canvas.Print(saveAs+'TKU.pdf(')
    
    canvas.cd(1)
    tku_pz_pass.Draw('hist')
    canvas.cd(2)
    tku_pz_fail.Draw('hist')
    canvas.Print(saveAs+'TKU.pdf(')
    
    canvas.cd(1)
    tku_xpx_pass.Draw('colz')
    canvas.cd(2)
    tku_xpx_fail.Draw('colz')
    canvas.Print(saveAs+'TKU.pdf(')
    
    canvas.cd(1)
    tku_ypy_pass.Draw('colz')
    canvas.cd(2)
    tku_ypy_fail.Draw('colz')
    canvas.Print(saveAs+'TKU.pdf)')








main()

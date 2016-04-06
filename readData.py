#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from ROOT import TFile, TCanvas, gDirectory, TTree, TH1F, TH2F, TMath, TF1


def main():
    filename = "/Users/Victoria/Documents/Work/MICE Data/Data run 7469/run7469_extracted_data__MAUS2pt1c.root"

    f = TFile(filename, 'read')
    tree = f.Get("T")

    print_tree_info(tree)
    do_plots(tree)




def print_tree_info(tree):
    print 'Found tree with ', tree.GetEntries(), ' entries.'




def do_plots(tree):
    detector = ["TOF0", "TOF1", "TKU"]
    #make_plots(detector, cut_number, tree, saveAs)

    saveAs = "cutTest_tof_momentum_vs_tracker_momentum.pdf"
    make_tof_vs_tracker_cut_plot(tree, saveAs)
    
    #saveAs = "plots_tof_momentum_vs_tracker_momentum.pdf"
    #make_tof_vs_tracker_plots(tree, saveAs)


#for cut in range(15, 16):
#       for det in detector:
#           print "Printing plots for cut "+str(cut)+" at "+det
#
#           saveAs = "plots_"+det+"_cut"+str(cut)+".pdf"
#           make_plots(det, cut, tree, saveAs)









def get_cut_description(cut_number):
    if cut_number == 0:
        return "no cuts"
    else:
        return "cut "+str(cut_number)+" "


def  getValues(detector, cut_number, tree_entry):
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

    """
    cut_passed = False
    cut1, cut2, cut3, cut4, cut5 = False, False, False, False, False
    cut6, cut7, cut8, cut9, cut10 = False, False, False, False, False
    cut11, cut12, cut13, cut14 = False, False, False, False
    cut15 = False

    if tree_entry.cut_TOF0_goodPMTPosition == 1:
        cut1 = True
    if tree_entry.cut_TOF1_goodPMTPosition == 1:
        cut2 = True
    if tree_entry.cut_goodRaynerReconstruction == 1:
        cut3 = True
    if tree_entry.cut_TimeOfFlight == 1:
        cut4 = True
    if tree_entry.cut_TKU_hitAllStations == 1:
        cut5 = True
    if tree_entry.cut_hit_all_detectors == 1:
        cut6 = True
    if tree_entry.cut_TOF0_singleHit == 1:
        cut7 = True
    if tree_entry.cut_TOF1_singleHit == 1:
        cut8 = True
    if tree_entry.cut_TKU_singleTrack == 1:
        cut9 = True

    if cut1 and cut2 and cut3 and cut4:
        cut10 = True
    if cut10 and cut7 and cut8:
        cut11 = True
    if cut11 and cut6 and cut9:
        cut12 = True
    if cut12 and tree_entry.cut_TKU_PValue == 1:
        cut13 = True
    if tree_entry.cut_allPassed == 1:
        cut14 = True
    if tree_entry.cut_tof1_tku_momentum == 1:
        cut15 = True

    if cut_number == 0:
        cut_passed = True
    if cut_number == 1 and cut1:
        cut_passed = True
    if cut_number == 2 and cut2:
        cut_passed = True
    if cut_number == 3 and cut3:
        cut_passed = True
    if cut_number == 4 and cut4:
        cut_passed = True
    if cut_number == 5 and cut5:
        cut_passed = True
    if cut_number == 6 and cut6:
        cut_passed = True
    if cut_number == 7 and cut7:
        cut_passed = True
    if cut_number == 8 and cut8:
        cut_passed = True
    if cut_number == 9 and cut9:
        cut_passed = True
    if cut_number == 10 and cut10:
        cut_passed = True
    if cut_number == 11 and cut11:
        cut_passed = True
    if cut_number == 12 and cut12:
        cut_passed = True
    if cut_number == 13 and cut13:
        cut_passed = True
    if cut_number == 14 and cut14:
        cut_passed = True
    
    if cut_number == 15 and cut14 and cut15:
        cut_passed = True


    if detector == 'TOF0':
        if tree_entry.TOF0_xPixel != TMath.Infinity():
            xPixel = [tree_entry.TOF0_xPixel, cut_passed]
        else:
            xPixel = [None, None]
        if tree_entry.TOF0_yPixel != TMath.Infinity():
            yPixel = [tree_entry.TOF0_yPixel, cut_passed]
        else:
            yPixel = [None, None]
        if tree_entry.TOF0_x != TMath.Infinity():
            x = [tree_entry.TOF0_x, cut_passed]
        else:
            x = [None, None]
        if tree_entry.TOF0_y != TMath.Infinity():
            y = [tree_entry.TOF0_y, cut_passed]
        else:
            y = [None, None]
        if tree_entry.TOF0_px != TMath.Infinity():
            px = [tree_entry.TOF0_px, cut_passed]
        else:
            px = [None, None]
        if tree_entry.TOF0_py != TMath.Infinity():
            py = [tree_entry.TOF0_py, cut_passed]
        else:
            py = [None, None]
        if tree_entry.TOF0_pz != TMath.Infinity():
            pz = [tree_entry.TOF0_pz, cut_passed]
        else:
            pz = [None, None]

        return xPixel, yPixel, x, y, px, py, pz

    if detector == 'TOF1':
        if tree_entry.TOF1_xPixel != TMath.Infinity():
            xPixel = [tree_entry.TOF1_xPixel, cut_passed]
        else:
            xPixel = [None, None]
        if tree_entry.TOF1_yPixel != TMath.Infinity():
            yPixel = [tree_entry.TOF1_yPixel, cut_passed]
        else:
            yPixel = [None, None]
        if tree_entry.TOF1_x != TMath.Infinity():
            x = [tree_entry.TOF1_x, cut_passed]
        else:
            x = [None, None]
        if tree_entry.TOF1_y != TMath.Infinity():
            y = [tree_entry.TOF1_y, cut_passed]
        else:
            y = [None, None]
        if tree_entry.TOF1_px != TMath.Infinity():
            px = [tree_entry.TOF1_px, cut_passed]
        else:
            px = [None, None]
        if tree_entry.TOF1_py != TMath.Infinity():
            py = [tree_entry.TOF1_py, cut_passed]
        else:
            py = [None, None]
        if tree_entry.TOF1_pz != TMath.Infinity():
            pz = [tree_entry.TOF1_pz, cut_passed]
        else:
            pz = [None, None]

        return xPixel, yPixel, x, y, px, py, pz

    if detector == 'TKU':
        if tree_entry.TKU_s5_x != TMath.Infinity():
            x = [tree_entry.TKU_s5_x, cut_passed]
        else:
            x = [None, None]
        if tree_entry.TKU_s5_y != TMath.Infinity():
            y = [tree_entry.TKU_s5_y, cut_passed]
        else:
            y = [None, None]
        if tree_entry.TKU_s5_px != TMath.Infinity():
            px = [tree_entry.TKU_s5_px, cut_passed]
        else:
            px = [None, None]
        if tree_entry.TKU_s5_py != TMath.Infinity():
            py = [tree_entry.TKU_s5_py, cut_passed]
        else:
            py = [None, None]
        if tree_entry.TKU_s5_pz != TMath.Infinity():
            pz = [tree_entry.TKU_s5_pz, cut_passed]
        else:
            pz = [None, None]

        return x, y, px, py, pz




def make_plots(detector, cut_number, tree, saveAs):
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

    """
    cut_description = get_cut_description(cut_number)
    xPixel_title_pass = detector+", xPixel, "+cut_description+"(pass);xPixel at "+detector+" (mm)"
    yPixel_title_pass = detector+", yPixel, "+cut_description+"(pass);yPixel at "+detector+" (mm)"
    x_title_pass = detector+", x, "+cut_description+"(pass);x at "+detector+" (mm)"
    y_title_pass = detector+", y, "+cut_description+"(pass);y at "+detector+" (mm)"
    px_title_pass = detector+", px, "+cut_description+"(pass);px at "+detector+" (MeV/c)"
    py_title_pass = detector+", py, "+cut_description+"(pass);py at "+detector+" (MeV/c)"
    pz_title_pass = detector+", pz, "+cut_description+"(pass);pz at "+detector+" (MeV/c)"

    xPx_title_pass = detector+", x-px, "+cut_description+"(pass);x at "+detector+" (mm);Px at "+detector+" (MeV/c)"
    yPy_title_pass = detector+", y-py, "+cut_description+"(pass);y at "+detector+" (mm);Py at "+detector+" (MeV/c)"

    xPixel_title_fail = detector+", xPixel, "+cut_description+"(fail);xPixel at "+detector+" (mm)"
    yPixel_title_fail = detector+", yPixel, "+cut_description+"(fail);yPixel at "+detector+" (mm)"
    x_title_fail = detector+", x, "+cut_description+"(fail);x at "+detector+" (mm)"
    y_title_fail = detector+", y, "+cut_description+"(fail);y at "+detector+" (mm)"
    px_title_fail = detector+", px, "+cut_description+"(fail);px at "+detector+" (MeV/c)"
    py_title_fail = detector+", py, "+cut_description+"(fail);py at "+detector+" (MeV/c)"
    pz_title_fail = detector+", pz, "+cut_description+"(fail);pz at "+detector+" (MeV/c)"

    xPx_title_fail = detector+", x-px, "+cut_description+"(fail);x at "+detector+" (mm);Px at "+detector+" (MeV/c)"
    yPy_title_fail = detector+", y-py, "+cut_description+"(fail);y at "+detector+" (mm);Py at "+detector+" (MeV/c)"

    if detector is "TOF0":
        bins = 10
    else:
        bins = 7

    det_xPixel_pass = TH1F('det_xPixel_pass', xPixel_title_pass, bins, -200.0, 200.0)
    det_xPixel_fail = TH1F('det_xPixel_fail', xPixel_title_fail, bins, -200.0, 200.0)

    det_yPixel_pass = TH1F('det_yPixel_pass', yPixel_title_pass, bins, -200.0, 200.0)
    det_yPixel_fail = TH1F('det_yPixel_fail', yPixel_title_fail, bins, -200.0, 200.0)

    det_x_pass = TH1F('det_x_pass', x_title_pass, 100, -200.0, 200.0)
    det_x_fail = TH1F('det_x_fail', x_title_fail, 100, -200.0, 200.0)

    det_y_pass = TH1F('det_y_pass', y_title_pass, 100, -200.0, 200.0)
    det_y_fail = TH1F('det_y_fail', y_title_fail, 100, -200.0, 200.0)

    det_px_pass = TH1F('det_px_pass', px_title_pass, 50, -100.0, 100.0)
    det_px_fail = TH1F('det_px_fail', px_title_fail, 50, -100.0, 100.0)

    det_py_pass = TH1F('det_py_pass', py_title_pass, 50, -100.0, 100.0)
    det_py_fail = TH1F('det_py_fail', py_title_fail, 50, -100.0, 100.0)

    det_pz_pass = TH1F('det_pz_pass', pz_title_pass, 100, 100.0, 300.0)
    det_pz_fail = TH1F('det_pz_fail', pz_title_fail, 100, 100.0, 300.0)

    det_xpx_pass = TH2F('det_xpx_pass', xPx_title_pass, 100, -200.0, 200.0, 50, -100.0, 100.0)
    det_xpx_fail = TH2F('det_xpx_fail', xPx_title_fail, 100, -200.0, 200.0, 50, -100.0, 100.0)

    det_ypy_pass = TH2F('det_ypy_pass', yPy_title_pass, 100, -200.0, 200.0, 50, -100.0, 100.0)
    det_ypy_fail = TH2F('det_ypy_fail', yPy_title_fail, 100, -200.0, 200.0, 50, -100.0, 100.0)

    for entry in tree:
        if detector != "TKU":
            xPixel, yPixel, x, y, px, py, pz = getValues(detector, cut_number, entry)
            if xPixel[1] is True:
                det_xPixel_pass.Fill(xPixel[0])
            elif xPixel[1] is False:
                det_xPixel_fail.Fill(xPixel[0])

            if yPixel[1] is True:
                det_yPixel_pass.Fill(yPixel[0])
            elif yPixel[1] is False:
                det_yPixel_fail.Fill(yPixel[0])
        else:
            x, y, px, py, pz = getValues(detector, cut_number, entry)

        if x[1] is True:
            det_x_pass.Fill(x[0])
        elif x[1] is False:
            det_x_fail.Fill(x[0])

        if y[1] is True:
            det_y_pass.Fill(y[0])
        elif y[1] is False:
            det_y_fail.Fill(y[0])

        if px[1] is True:
            det_px_pass.Fill(px[0])
        elif px[1] is False:
            det_px_fail.Fill(px[0])

        if py[1] is True:
            det_py_pass.Fill(py[0])
        elif py[1] is False:
            det_py_fail.Fill(py[0])

        if pz[1] is True:
            det_pz_pass.Fill(pz[0])
        elif pz[1] is False:
            det_pz_fail.Fill(pz[0])

        if x[1] is True and px[1] is True:
            det_xpx_pass.Fill(x[0], px[0])
        elif x[1] is False and px[1] is False:
            det_xpx_fail.Fill(x[0], px[0])


        if y[1] is True and py[1] is True:
            det_ypy_pass.Fill(y[0], py[0])
        elif y[1] is False and py[1] is False:
            det_ypy_fail.Fill(y[0], py[0])

    canvas = TCanvas('canvas', 'canvas', 800, 400)
    canvas.Divide(2, 1)

    if detector != "TKU":
        canvas.cd(1)
        det_xPixel_pass.Draw("hist")
        canvas.cd(2)
        det_xPixel_fail.Draw("hist")
        canvas.Print(saveAs+"(")

        canvas.cd(1)
        det_yPixel_pass.Draw("hist")
        canvas.cd(2)
        det_yPixel_fail.Draw("hist")
        canvas.Print(saveAs+"(")

    canvas.cd(1)
    det_x_pass.Draw("hist")
    canvas.cd(2)
    det_x_fail.Draw("hist")
    canvas.Print(saveAs+"(")

    canvas.cd(1)
    det_y_pass.Draw("hist")
    canvas.cd(2)
    det_y_fail.Draw("hist")
    canvas.Print(saveAs+"(")

    canvas.cd(1)
    det_px_pass.Draw("hist")
    canvas.cd(2)
    det_px_fail.Draw("hist")
    canvas.Print(saveAs+"(")

    canvas.cd(1)
    det_py_pass.Draw("hist")
    canvas.cd(2)
    det_py_fail.Draw("hist")
    canvas.Print(saveAs+"(")

    canvas.cd(1)
    det_pz_pass.Draw("hist")
    canvas.cd(2)
    det_pz_fail.Draw("hist")
    canvas.Print(saveAs+"(")

    canvas.cd(1)
    det_xpx_pass.Draw("colz")
    canvas.cd(2)
    det_xpx_fail.Draw("colz")
    canvas.Print(saveAs+"(")

    canvas.cd(1)
    det_ypy_pass.Draw("colz")
    canvas.cd(2)
    det_ypy_fail.Draw("colz")
    canvas.Print(saveAs+")")






def make_tof_vs_tracker_plots(tree, saveAs):
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
        
        """
    canvas = TCanvas('canvas', 'canvas', 800, 400)
    canvas.Divide(2, 1)
    
    cuts = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
    for cut in cuts:
        cut_description = get_cut_description(cut)
        title_pass = "TOF P vs Tracker P, "+cut_description+"(pass);P at TKU (MeV/c);P at TOF1 (MeV/c)"
        title_fail = "TOF P vs Tracker P, "+cut_description+"(fail);P at TKU (MeV/c);P at TOF1 (MeV/c)"
    
        cut_passed = TH2F('pass_'+cut_description, title_pass, 75, 150.0, 300.0, 75, 150.0, 300.0)
        cut_failed = TH2F('fail_'+cut_description, title_fail, 75, 150.0, 300.0, 75, 150.0, 300.0)

        for entry in tree:
            tof1_xPixel, tof1_yPixel, tof1_x, tof1_y, tof1_px, tof1_py, tof1_pz = getValues("TOF1", cut, entry)
            tku_x, tku_y, tku_px, tku_py, tku_pz = getValues("TKU", cut, entry)
            
            
        
            if tof1_px[1] is True and tof1_py[1] is True and tof1_pz[1] is True and tku_px[1] is True and tku_py[1] is True and tku_pz[1] is True:
                tof1_p = np.sqrt(tof1_px[0]*tof1_px[0] + tof1_py[0]*tof1_py[0] + tof1_pz[0]*tof1_pz[0])
                tku_p = np.sqrt(tku_px[0]*tku_px[0] + tku_py[0]*tku_py[0] + tku_pz[0]*tku_pz[0])
                
                cut_passed.Fill(tku_p, tof1_p)
            else:
                if tof1_px[0] != None and tof1_py[0] != None and tof1_pz[0] != None and tku_px[0] != None and tku_py[0] != None and tku_pz[0] != None:
                    # Have to do this check as particles that don't make it to the tracker fail this cut
                    # but also have no (px, py, pz) to fill into this histogram.
                
                    tof1_p = np.sqrt(tof1_px[0]*tof1_px[0] + tof1_py[0]*tof1_py[0] + tof1_pz[0]*tof1_pz[0])
                    tku_p = np.sqrt(tku_px[0]*tku_px[0] + tku_py[0]*tku_py[0] + tku_pz[0]*tku_pz[0])
                    cut_failed.Fill(tku_p, tof1_p)

        canvas.cd(1)
        cut_passed.Draw("colz")
        canvas.cd(2)
        cut_failed.Draw("colz")

        if cut != 14:
            canvas.Print(saveAs+"(")
        else:
            canvas.Print(saveAs+")")






def make_tof_vs_tracker_cut_plot(tree, saveAs):
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
        
        """
    canvas = TCanvas('canvas', 'canvas', 1200, 400)
    canvas.Divide(3, 1)
    
    cut_description = 'Test cut '
    title_pass = "TOF P vs Tracker P, "+cut_description+"(pass);P at TKU (MeV/c);P at TOF1 (MeV/c)"
    title_fail = "TOF P vs Tracker P, "+cut_description+"(fail);P at TKU (MeV/c);P at TOF1 (MeV/c)"
        
    cut_passed = TH2F('pass_'+cut_description, title_pass, 75, 150.0, 300.0, 75, 150.0, 300.0)
    cut_failed = TH2F('fail_'+cut_description, title_fail, 75, 150.0, 300.0, 75, 150.0, 300.0)

    momentum_difference = TH1F("momentum_diff", "Tracker P - TOF P; Tracker P - TOF P (MeV)", 50, -40.0, 10.0)
    
    p180_hist = TH1F("p180_hist", "Tracker P = 180MeV; TOF P (MeV)", 100, 175.0, 275.0)
    p190_hist = TH1F("p190_hist", "Tracker P = 190MeV; TOF P (MeV)", 100, 175.0, 275.0)
    p200_hist = TH1F("p200_hist", "Tracker P = 200MeV; TOF P (MeV)", 100, 175.0, 275.0)
    p210_hist = TH1F("p210_hist", "Tracker P = 210MeV; TOF P (MeV)", 100, 175.0, 275.0)
    p220_hist = TH1F("p220_hist", "Tracker P = 220MeV; TOF P (MeV)", 100, 175.0, 275.0)


    lower_gradient = 0.9082
    lower_intercept = 26.985
    
    upper_gradient = 0.7698
    upper_intercept = 72.855
    
    for entry in tree:
        tof1_xPixel, tof1_yPixel, tof1_x, tof1_y, tof1_px, tof1_py, tof1_pz = getValues("TOF1", 0, entry)
        tku_x, tku_y, tku_px, tku_py, tku_pz = getValues("TKU", 0, entry)
            
            
        if tof1_px[0] != None and tof1_py[0] != None and tof1_pz[0] != None and tku_px[0] != None and tku_py[0] != None and tku_pz[0] != None:
            # Have to do this check as particles that don't make it to the tracker fail this cut
            # but also have no (px, py, pz) to fill into this histogram.
                    
            tof1_p = np.sqrt(tof1_px[0]*tof1_px[0] + tof1_py[0]*tof1_py[0] + tof1_pz[0]*tof1_pz[0])
            tku_p = np.sqrt(tku_px[0]*tku_px[0] + tku_py[0]*tku_py[0] + tku_pz[0]*tku_pz[0])
            diff = tku_p - tof1_p
            
            momentum_difference.Fill(diff)

            tof1_p_lower_limit = lower_gradient * tku_p + lower_intercept
            tof1_p_upper_limit = upper_gradient * tku_p + upper_intercept
            
            #print 'tof1_p = ', tof1_p, ' --> given tku_p = ', tku_p, ' then upper limit is at ', tof1_p_upper_limit, ' and lower limit is at ', tof1_p_lower_limit

            if tof1_p < tof1_p_upper_limit and tof1_p > tof1_p_lower_limit and entry.cut_allPassed==1:
                cut_passed.Fill(tku_p, tof1_p)
            else:
                cut_failed.Fill(tku_p, tof1_p)


            if tku_p >= 179.0 and tku_p <= 181.0:
                p180_hist.Fill(tof1_p)
            elif tku_p >= 189.0 and tku_p <= 191.0:
                p190_hist.Fill(tof1_p)
            elif tku_p >= 199.0 and tku_p <= 201.0:
                p200_hist.Fill(tof1_p)
            elif tku_p >= 209.0 and tku_p <= 211.0:
                p210_hist.Fill(tof1_p)
            elif tku_p >= 219.0 and tku_p <= 221.0:
                p220_hist.Fill(tof1_p)



    canvas.cd(1)
    cut_passed.Draw("colz")
    canvas.cd(2)
    cut_failed.Draw("colz")
    canvas.cd(3)
    momentum_difference.Draw('hist')

    canvas.Print(saveAs)




















main()

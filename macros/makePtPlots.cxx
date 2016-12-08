/// \file makePtPlots.Cxx
/// \brief implementation of the makePtPlots macro.
/// \since 2016-12-08
/// \author R.G.A. Deckers
/// \copyright
///  This program is free software; you can redistribute it and/or
/// modify it under the terms of the GNU General Public License as
/// published by the Free Software Foundation; either version 3 of
/// the License, or (at your option) any later version.
/// This program is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
/// General Public License for more details at
/// https://www.gnu.org/copyleft/gpl.html

#include <TCanvas.h>
#include <TColor.h>
#include <TFile.h>
#include <TH1F.h>
#include <TImage.h>
#include <TList.h>
#include <TStyle.h>
/// Short makePtPlots description
///
/// More detailed makePtPlots description

void makePtPlots() {
  TFile *ESDsFile = TFile::Open("ESDsSpectrum.root", "READ");
  TList *ESDsList = dynamic_cast<TList *>(ESDsFile->Get("Pt spectrum"));
  TH1F *ESDsHistograms[] = {(TH1F *)ESDsList->At(0), (TH1F *)ESDsList->At(1),
                            (TH1F *)ESDsList->At(2)};
  TFile *TimeframeFile = TFile::Open("TimeframeSpectrum.root", "READ");
  TList *TimeframeList =
      dynamic_cast<TList *>(TimeframeFile->Get("Pt spectrum"));
  TH1F *TimeframeHistograms[] = {
      (TH1F *)TimeframeList->At(0), (TH1F *)TimeframeList->At(1),
      (TH1F *)TimeframeList->At(2), (TH1F *)TimeframeList->At(3),
      (TH1F *)TimeframeList->At(4), (TH1F *)TimeframeList->At(5)};

  gStyle->SetPalette(70);
  // create a new canvas to paint onto
  TCanvas *canvas = new TCanvas("c1", "", 0, 0, 1280, 720);
  // canvas->SetLogy();
  // h->FillRandom("gaus", 10000);
  // ESDsHistograms[0]->Draw();
  TImage *img = TImage::Create();

  TimeframeHistograms[1]->SetAxisRange(0, 3, "X");
  TimeframeHistograms[1]->SetLineColor(kRed);
  TimeframeHistograms[1]->Draw("");
  TimeframeHistograms[2]->SetLineColor(kRed);
  TimeframeHistograms[2]->Draw("SAME");

  ESDsHistograms[1]->SetLineColor(kBlack);
  ESDsHistograms[2]->SetLineColor(kBlack);
  ESDsHistograms[1]->Draw("SAME");
  ESDsHistograms[2]->Draw("SAME");

  // h->FillRandom("gaus", 10000);
  //  TimeframeHistograms[0]->Draw();

  TimeframeHistograms[4]->SetLineColor(kBlue);
  TimeframeHistograms[5]->SetLineColor(kBlue);
  TimeframeHistograms[4]->Draw("SAME");
  TimeframeHistograms[5]->Draw("SAME");

  canvas->Update();
  img->FromPad(canvas);
  img->WriteImage("Results.png");

  delete canvas;
  canvas = new TCanvas("c1", "", 0, 0, 1280, 720);

  // everything, global
  TimeframeHistograms[1]->SetAxisRange(0.8, 1.2, "Y");
  TimeframeHistograms[1]->Divide(ESDsHistograms[1]);
  TimeframeHistograms[1]->SetLineColor(kRed);
  TimeframeHistograms[1]->Draw("SAME");

  TH1F tmp = *TimeframeHistograms[4];
  tmp.Divide(ESDsHistograms[1]);
  tmp.SetLineColor(kBlue);
  tmp.Draw("SAME");
  canvas->Update();
  img->FromPad(canvas);
  img->WriteImage("ratio_global.png");
  delete canvas;
  canvas = new TCanvas("c1", "", 0, 0, 1280, 720);

  // TimeframeHistograms[2]->Add(ESDsHistograms[2], -1);
  TimeframeHistograms[2]->SetAxisRange(0.0, 1, "X");
  TimeframeHistograms[2]->SetAxisRange(0.5, 1.5, "Y");
  TimeframeHistograms[2]->Divide(ESDsHistograms[2]);
  TimeframeHistograms[2]->SetLineColor(kRed);
  TimeframeHistograms[2]->Draw("SAME");

  tmp = *TimeframeHistograms[5];
  // tmp.Add(TimeframeHistograms[5], -1);
  tmp.Divide(ESDsHistograms[2]);
  tmp.SetLineColor(kBlue);
  tmp.Draw("SAME");
  canvas->Update();
  img->FromPad(canvas);
  img->WriteImage("ratio ITS.png");
  //
  // c1->Clear();
  // c1->SetLogy(0);
  // TH2F *h2 = analysisTask->getRelMomentum2DHist();
  // TObjArray aSlices;
  // h2->FitSlicesY(0, 0, -1, 0, "QNR", &aSlices);
  // // h2->FitSlicesX();//Doesn't work, can't us gDirertory(h2_1) in compiled
  // mode at least https
  //     : //
  //     root.cern.ch/doc/master/classTH2.html#a9bfd1ea19dee46c25861d58ee60eaa3d
  //       TH1D *h2_1 = (TH1D *)aSlices[1]; // Mean
  // if (!h2_1) {
  //   report(FAIL, "Failed to open fitted histogram!");
  // }
  // // without col it just draws dots, does this mean it stores all the events?
  // h2_1->Draw("colz");
  // c1->Update();
  // img->FromPad(c1);
  // img->WriteImage("canvas2.png");
  // report(PASS, "Made image 2");
}

#ifndef __CINT__
int main(int argc, char **argv) {
  makePtPlots();
  return 0;
}
#endif

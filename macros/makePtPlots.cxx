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
#include <TLegend.h>
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
  for (unsigned u = 0; u < 6; u++) {
    TimeframeHistograms[u]->SetStats(kFALSE);
  }
  for (unsigned u = 0; u < 3; u++) {
    ESDsHistograms[u]->SetStats(kFALSE);
  }
  gStyle->SetPalette(70);
  ESDsHistograms[2]->SetLineColor(kBlack);
  TimeframeHistograms[2]->SetLineColor(kRed);
  TimeframeHistograms[5]->SetLineColor(kBlue);
  TLegend *leg = new TLegend(0.9, 0.7, 0.48, 0.9);
  leg->AddEntry(ESDsHistograms[2], "ESD results", "lep");
  leg->AddEntry(TimeframeHistograms[2],
                "Timeframe using any associatable track", "lep");
  leg->AddEntry(TimeframeHistograms[5], "Using only unambigous tracks", "lep");
  // create a new canvas to paint onto
  TCanvas *canvas = new TCanvas("c1", "PLC ][", 128, 72, 1280, 720);
  // h->FillRandom("gaus", 10000);
  // ESDsHistograms[0]->Draw();
  TImage *img = TImage::Create();
  // PLC ][ SAME for all bins & histos.
  // Int_t binwidth = TimeframeHistograms[0]->GetXaxis()->GetBinWidth(1);
  TH1F tmp = *TimeframeHistograms[1];
  tmp.SetAxisRange(0, 3, "X");
  tmp.SetAxisRange(0, 1, "Y");
  tmp.GetXaxis()->SetTitle("Pt");
  tmp.GetYaxis()->SetTitle("dN/dPt");
  tmp.SetLineColor(kRed);
  tmp.Scale(1 / ESDsHistograms[0]->Integral());
  tmp.Draw("PLC ][ SAME");
  //
  TH1F tmp2 = *TimeframeHistograms[2];
  tmp2.SetLineColor(kRed);
  tmp2.Scale(1 / ESDsHistograms[0]->Integral());
  tmp2.Draw("PLC ][ SAME");
  //
  TH1F tmp3 = *TimeframeHistograms[4];
  tmp3.SetLineColor(kBlue);
  tmp3.Scale(1 / ESDsHistograms[0]->Integral());
  tmp3.Draw("PLC ][ SAME");

  TH1F tmp4 = *TimeframeHistograms[5];
  tmp4.SetLineColor(kBlue);
  tmp4.Scale(1 / ESDsHistograms[0]->Integral());
  tmp4.Draw("PLC ][ SAME");

  TH1F tmp5 = *ESDsHistograms[1];
  tmp5.Scale(1 / ESDsHistograms[0]->Integral());
  tmp5.SetLineColor(kBlack);
  tmp5.Draw("PLC ][ SAME");
  TH1F tmp6 = *ESDsHistograms[2];
  tmp6.Scale(1 / ESDsHistograms[0]->Integral());
  tmp6.SetLineColor(kBlack);
  tmp6.Draw("PLC ][ SAME");
  leg->Draw();
  canvas->Update();
  img->FromPad(canvas);
  // Tracks, normalized for total track count ESD
  img->WriteImage("Results.png");
  delete canvas;

  // Normalize each track individually
  for (unsigned u = 0; u < 6; u++) {
    TimeframeHistograms[u]->Scale(1 / TimeframeHistograms[u]->Integral());
  }
  for (unsigned u = 0; u < 3; u++) {
    ESDsHistograms[u]->Scale(1 / ESDsHistograms[u]->Integral());
  }

  canvas = new TCanvas("c1", "PLC ][", 0, 0, 1280, 720);
  // canvas->SetLogy();
  ESDsHistograms[1]->SetLineColor(kBlack);
  ESDsHistograms[1]->SetAxisRange(0.05, 0.5, "X");
  ESDsHistograms[1]->GetXaxis()->SetTitle("Pt");
  ESDsHistograms[1]->GetYaxis()->SetTitle("dN/dPt");
  ESDsHistograms[1]->Draw("");
  TimeframeHistograms[1]->SetLineColor(kRed);
  TimeframeHistograms[1]->Draw("SAME");
  TimeframeHistograms[4]->SetLineColor(kBlue);
  TimeframeHistograms[4]->Draw("SAME");
  leg->Draw();
  canvas->Update();
  img->FromPad(canvas);
  // normalized global tracks
  img->WriteImage("Global.png");
  delete canvas;

  canvas = new TCanvas("c1", "", 0, 0, 1280, 720);
  // canvas->SetLogy();
  ESDsHistograms[2]->SetAxisRange(0.05, 0.5, "X");
  ESDsHistograms[2]->SetLineColor(kBlack);
  ESDsHistograms[2]->GetXaxis()->SetTitle("Pt");
  ESDsHistograms[2]->GetYaxis()->SetTitle("dN/dPt");
  ESDsHistograms[2]->Draw("");
  TimeframeHistograms[2]->SetLineColor(kRed);
  TimeframeHistograms[2]->Draw("SAME");
  TimeframeHistograms[5]->SetLineColor(kBlue);
  TimeframeHistograms[5]->Draw("SAME");
  leg->Draw();

  canvas->Update();
  img->FromPad(canvas);
  img->WriteImage("ITS.png");
  delete canvas;
  // h->FillRandom("gaus", 10000);
  //  TimeframeHistograms[0]->Draw();

  canvas = new TCanvas("c1", "PLC ][", 0, 0, 1280, 720);
  // everything, global
  TimeframeHistograms[1]->SetAxisRange(0.05, 1, "X");
  TimeframeHistograms[1]->Divide(ESDsHistograms[1]);
  // Ratio of TF global tracks over ESD global tracks
  TimeframeHistograms[1]->SetLineColor(kRed);
  TimeframeHistograms[1]->GetXaxis()->SetTitle("Pt");
  TimeframeHistograms[1]->GetYaxis()->SetTitle("dN/dPt ratio Timeframe/ESDs");
  TimeframeHistograms[1]->Draw("][ SAME ");

  tmp = *TimeframeHistograms[4];
  tmp.SetStats(kFALSE);
  tmp.Divide(ESDsHistograms[1]);
  tmp.SetLineColor(kBlue);
  tmp.Draw("][ SAME");
  leg->Draw();
  canvas->Update();
  img->FromPad(canvas);
  img->WriteImage("ratio_global.png");
  delete canvas;
  canvas = new TCanvas("c1", " ][", 0, 0, 1280, 720);

  // TimeframeHistograms[2]->Add(ESDsHistograms[2], -1);
  TimeframeHistograms[2]->SetAxisRange(0.05, 1, "X");
  // ratio of ITS track
  TimeframeHistograms[2]->Divide(ESDsHistograms[2]);
  TimeframeHistograms[2]->SetLineColor(kRed);
  TimeframeHistograms[2]->GetXaxis()->SetTitle("Pt");
  TimeframeHistograms[2]->GetYaxis()->SetTitle("dN/dPt ratio Timeframe/ESDs");
  TimeframeHistograms[2]->Draw("][ SAME");

  tmp = *TimeframeHistograms[5];
  // tmp.Add(TimeframeHistograms[5], -1);
  tmp.Divide(ESDsHistograms[2]);
  tmp.SetStats(kFALSE);
  tmp.SetLineColor(kBlue);
  tmp.Draw("][ SAME");
  leg->Draw();
  canvas->Update();
  img->FromPad(canvas);
  img->WriteImage("ratio ITS.png");
}

#ifndef __CINT__
int main(int argc, char **argv) {
  makePtPlots();
  return 0;
}
#endif

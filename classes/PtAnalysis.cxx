/// \file PtAnalysis.cxx
/// \brief implementation of the PtAnalysis class.
/// \since 2016-11-15
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
///

#include "O2Timeframe.h"

#include "PtAnalysis.h"
#include <AliESDtrack.h>
#include <AliLog.h>
#include <AliO2Event.h>
#include <TChain.h>
#include <utilities/logging.h>
// root specific
ClassImp(PtAnalysis);

// default constructor
PtAnalysis::PtAnalysis() {}
// named constructor
PtAnalysis::PtAnalysis(const char *name) : AliO2AnalysisTaskSE(name) {
  DefineInput(0, TChain::Class());
  DefineOutput(1, TList::Class());
}
// default destructor
PtAnalysis::~PtAnalysis() { delete mList; }

void PtAnalysis::UserCreateOutputObjects() {
  mList = new TList();
  mList->SetOwner(kTRUE);
  mHistograms.push_back(new TH1F("All Tracks", "", 600, 0, 5));
  mHistograms.push_back(new TH1F("Global Tracks", "", 600, 0, 5));
  mHistograms.push_back(new TH1F("ITS Tracks", "", 600, 0, 5));
  mHistograms.push_back(new TH1F("All Unambigous Tracks", "", 600, 0, 5));
  mHistograms.push_back(new TH1F("Global Unambigous Tracks", "", 600, 0, 5));
  mHistograms.push_back(new TH1F("ITS Unambigous Tracks", "", 600, 0, 5));
  for (TH1F *hist : mHistograms) {
    mList->Add(hist);
  }
  PostData(1, mList);
}
// per event
void PtAnalysis::UserExec(Option_t *option) {
  // TODO: Ask why AODEvent() doesn't work
  const AliVEvent *event = InputEvent();
  const AliO2Event *O2Event = dynamic_cast<const AliO2Event *>(event);
  if (O2Event) {
    // All global tracks
    Int_t nTracks = O2Event->GetNumberOfGlobalTracks();
    for (Int_t i = 0; i < nTracks; i++) {
      const AliVParticle *track = O2Event->GetGlobalTrack(i);
      Double_t tranverse_momentum = track->Pt();
      (mHistograms[0])->Fill(tranverse_momentum);
      (mHistograms[1])->Fill(tranverse_momentum);
    }
    nTracks = O2Event->GetNumberOfITSTracks();
    for (Int_t i = 0; i < nTracks; i++) {
      const AliVParticle *track = O2Event->GetITSTrack(i);
      Double_t tranverse_momentum = track->Pt();
      (mHistograms[0])->Fill(tranverse_momentum);
      (mHistograms[2])->Fill(tranverse_momentum);
    }
    nTracks = O2Event->GetNumberOfUnambigousGlobalTracks();
    for (Int_t i = 0; i < nTracks; i++) {
      const AliVParticle *track = O2Event->GetUnambigousGlobalTrack(i);
      Double_t tranverse_momentum = track->Pt();
      (mHistograms[3])->Fill(tranverse_momentum);
      (mHistograms[4])->Fill(tranverse_momentum);
    }
    nTracks = O2Event->GetNumberOfUnambigousITSTracks();
    for (Int_t i = 0; i < nTracks; i++) {
      const AliVParticle *track = O2Event->GetUnambigousITSTrack(i);
      Double_t tranverse_momentum = track->Pt();
      (mHistograms[3])->Fill(tranverse_momentum);
      (mHistograms[5])->Fill(tranverse_momentum);
    }
  } else {
    Int_t nTracks = event->GetNumberOfTracks();
    for (Int_t i = 0; i < nTracks; i++) {
      AliESDtrack *track = dynamic_cast<AliESDtrack *>(event->GetTrack(i));
      Double_t tranverse_momentum = track->Pt();
      if (track->GetInnerParam()) {
        (mHistograms[1])->Fill(tranverse_momentum);
      } else {
        (mHistograms[2])->Fill(tranverse_momentum);
      }
      (mHistograms[0])->Fill(tranverse_momentum);
    }
  }
  PostData(1, mList);
}
// Cleanup
void PtAnalysis::Terminate(Option_t *option) {
  // Double_t scale =
  //     mHistogram->GetXaxis()->GetBinWidth(1) / *(mHistogram->GetIntegral());
  // mHistogram->Scale(scale);
  // PostData(1, mHistograms);
  // report(WARN, "Terminate CALLED");
}

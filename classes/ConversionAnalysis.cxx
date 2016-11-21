/// \file ConversionAnalysis.cxx
/// \brief implementation of the ConversionAnalysis class.
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

#include "ConversionAnalysis.h"
#include "AliO2Event.h"
#include <AliAODEvent.h>
#include <AliAODv0.h>
#include <AliESDtrack.h>
#include <AliExternalTrackParam.h>
#include <AliLog.h>
#include <TChain.h>
#include <TList.h>
#include <vector>
// root specific
ClassImp(ConversionAnalysis);

// default constructor
ConversionAnalysis::ConversionAnalysis() {}
// named constructor
ConversionAnalysis::ConversionAnalysis(const char *name)
    : AliAnalysisTaskSE(name) {
  DefineInput(0, TChain::Class());
  DefineOutput(1, TList::Class());
}
// default destructor
ConversionAnalysis::~ConversionAnalysis() { delete mResults; }

void ConversionAnalysis::UserCreateOutputObjects() {
  mResults = new TList();
  mResults->SetOwner(kTRUE);
  PostData(1, mResults);
}
// per event
void ConversionAnalysis::UserExec(Option_t *option) {
  event_counter += 1;
  // TODO: Ask why AODEvent() doesn't work
  const AliAODEvent *event = dynamic_cast<AliAODEvent *>(InputEvent());
  if (!event) {
    AliError(TString::Format("Failed to fetch AoD event\n"));
    failed_event_counter += 1;
    return;
  }
  AliO2Event *newEvent = new AliO2Event(event);
  mResults->Add(newEvent);
  PostData(1, mResults);
}
// Cleanup
void ConversionAnalysis::Terminate(Option_t *option) {}

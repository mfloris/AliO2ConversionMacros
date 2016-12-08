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

#include "O2Timeframe.h"

#include "ConversionAnalysis.h"
#include <AliESDEvent.h>
#include <AliLog.h>
#include <TChain.h>
// root specific
ClassImp(ConversionAnalysis);

// default constructor
ConversionAnalysis::ConversionAnalysis() {}
// named constructor
ConversionAnalysis::ConversionAnalysis(const char *name)
    : AliO2AnalysisTaskSE(name) {
  DefineInput(0, TChain::Class());
  DefineOutput(1, O2Timeframe::Class());
}
// default destructor
ConversionAnalysis::~ConversionAnalysis() { delete mResults; }

void ConversionAnalysis::UserCreateOutputObjects() {
  mResults = new O2Timeframe();
  PostData(1, mResults);
}
// per event
void ConversionAnalysis::UserExec(Option_t *option) {
  event_counter += 1;
  // TODO: Ask why AODEvent() doesn't work
  const AliESDEvent *event = dynamic_cast<AliESDEvent *>(InputEvent());
  if (!event) {
    AliError(TString::Format("Failed to fetch ESD event"));
    failed_event_counter += 1;
    return;
  } else if (0 == event->GetNumberOfTracks()) {
    AliInfo(TString::Format("No Tracks in event"));
    failed_event_counter += 1;
    return;
  }
  // TODO: We can probably do this much faster by using math
  Double_t mu = 25.0 / 20000.0;
  timestamp_t offset = 0;
  while (0 == eventsOnQueue) {
    eventsOnQueue = rng.Poisson(mu);
    if (eventsOnQueue > 1) {
      report(WARN, "Created pileup!");
    }
    offset += 25;
  }
  currentTimestamp += offset;
  mResults->addEvent(event, currentTimestamp);
  eventsOnQueue -= 1;
  PostData(1, mResults);
}
// Cleanup
void ConversionAnalysis::Terminate(Option_t *option) {
  AliInfo(TString::Format("Failed %u/%u events", failed_event_counter,
                          event_counter));
}

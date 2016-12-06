/// \file TestTask.cxx
/// \brief implementation of the TestTask class.
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

#include "TestTask.h"
#include <AliLog.h>
#include <AliO2Event.h>
#include <TChain.h>
#include <utilities/logging.h>
// root specific
ClassImp(TestTask);

// default constructor
TestTask::TestTask() {}
// named constructor
TestTask::TestTask(const char *name) : AliO2AnalysisTaskSE(name) {
  DefineInput(0, TChain::Class());
  DefineOutput(1, O2Timeframe::Class());
}
// default destructor
TestTask::~TestTask() { delete mResults; }

void TestTask::UserCreateOutputObjects() {
  report(WARN, "OUTPUT OBJECTS CREATED");
  mResults = new O2Timeframe();
  PostData(1, mResults);
}
// per event
void TestTask::UserExec(Option_t *option) {
  report(WARN, "UserExec CALLED");
  // TODO: Ask why AODEvent() doesn't work
  const AliVEvent *event = InputEvent();
  report(INFO, "hello from task! got %p",
         dynamic_cast<const AliO2Event *>(event));
  // TODO: We can probably do this much faster by using math
  // Double_t mu = 25.0 / 20000.0;
  // timestamp_t offset = 0;
  // while (0 == eventsOnQueue) {
  //   eventsOnQueue = rng.Poisson(mu);
  //   offset += 25;
  // }
  // currentTimestamp += offset;
  // mResults->addEvent(event, currentTimestamp);
  // eventsOnQueue -= 1;
  // PostData(1, mResults);
}
// Cleanup
void TestTask::Terminate(Option_t *option) {
  report(WARN, "Terminate CALLED");
  // AliInfo(TString::Format("Failed %u/%u events", failed_event_counter,
  //                         event_counter));
}

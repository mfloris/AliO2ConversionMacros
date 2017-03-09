/// \file PtSpectrum.Cxx
/// \brief A demonstration of how to use the new analysis framework,
/// \since 2017-03-09
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

#include <Entities/Track.h>
#include <Entities/Vertex.h>
#include <O2AnalysisManager.h>
#include <O2AnalysisTask.h>
#include <TFile.h>
#include <TH1F.h>
#include <TRandom2.h>

// What type of track and vertex we want to use for our events.
using Track_t = ecs::Track<ecs::track::Pt>;
using Vertex_t = ecs::Vertex<>;

// our analysis object. By deriving from O2ESDAnalysisTask the framework will
// build events for us from datastreams using the same mapping of vertex->tracks
// that was used in the original ESD files.
class PtAnalysis : public O2ESDAnalysisTask<Vertex_t, Track_t> {
public:
  // Gets called once.
  virtual void UserInit() { mHistogram = TH1F("All Tracks", "", 600, 0, 5); }
  // Gets called once per event.
  virtual void UserExec() {
    // get the current event as before.
    auto event = getEvent();
    // std::cout << "This event contains " << getEvent().getNumberOfTracks()
    //           << " tracks\n";
    for (int i = 0; i < event.getNumberOfTracks(); i++) {
      mHistogram.Fill(event.getTrack(i).pt());
    }
  }
  TH1F &getHistogramRef() { return mHistogram; }

protected:
  // protected stuff goes here

private:
  TH1F mHistogram;
};

int PtSpectrum(const char **files, int fileCount) {

  // create the analysis manager
  O2AnalysisManager mgr;
  for (int i = 0; i < fileCount; i++) {
    mgr.addFile(files[i]);
  }
  // Note the '&'! this has to be a reference otherwise we create a copy of the
  // newly created task and it will not be updated.
  // TODO: maybe only expose this as a reference object which wraps. That way
  // auto works and it is also clear to the user that what is being returned is
  // a reference.
  PtAnalysis &task = mgr.createNewTask<PtAnalysis>();
  mgr.startAnalysis();

  // Save the output:
  // open a file
  auto file = TFile::Open("AnalysisResult.root", "RECREATE");
  // Write the histogram
  task.getHistogramRef().Write();
  // Close the file.
  file->Close();
  return 0;
}

#ifndef __CINT__
int main(int argc, const char **argv) { return PtSpectrum(argv + 1, argc - 1); }
#endif

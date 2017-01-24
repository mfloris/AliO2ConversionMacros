/// \file ptSpectrum.Cxx
/// \brief implementation of the ptSpectrum macro.
/// \since 2016-11-16
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

#include "ConversionAnalysis.h"
#include "O2Timeframe.h"
#include "PtAnalysis.h"
#include "getFiles.h"
#include <AliESDInputHandler.h>
#include <AliO2AnalysisManager.h>
#include <AliO2InputHandler.h>
#include <O2logging.h>
#include <boost/filesystem.hpp>
#include <list>
#include <string>
#include <sys/wait.h>

/// Converts existing ESDs into O2Timeframes and computes the pt-spectrum for
/// both. Stores the converted ESDs in timeframe.root and the histograms in two
/// files {ESD,Timeframe}sSpectrum.root
int ptSpectrum(const char **files, int nFiles) {
  // create the analysis manager
  AliO2AnalysisManager *mgr = new AliO2AnalysisManager("AnalysisTaskExample");

  AliESDInputHandler *ESDH = new AliESDInputHandler();
  mgr->SetInputEventHandler(ESDH);

  // create an instance of your analysis task
  ConversionAnalysis *conversionTask =
      new ConversionAnalysis("Conversion Analysis");
  if (nullptr == conversionTask) {
    return -1;
  }
  // create an instance of your analysis task
  PtAnalysis *ptAnalysisTask = new PtAnalysis("Pt Analysis");
  if (nullptr == ptAnalysisTask) {
    return -1;
  }
  mgr->ConnectInput(ptAnalysisTask, 0, mgr->GetCommonInputContainer());
  mgr->ConnectInput(conversionTask, 0, mgr->GetCommonInputContainer());
  // same for the output
  mgr->ConnectOutput(
      ptAnalysisTask, 1,
      mgr->CreateContainer("Pt spectrum", TList::Class(),
                           AliO2AnalysisManager::kOutputContainer,
                           "ESDsSpectrum.root"));

  mgr->ConnectOutput(
      conversionTask, 1,
      mgr->CreateContainer("Timeframe", O2Timeframe::Class(),
                           AliO2AnalysisManager::kOutputContainer,
                           "timeframe.root"));
  // add a few files to the chain
  std::list<std::string> filelist = getFiles(files, nFiles);
  TChain *chain = new TChain("esdTree");
  for (auto it = filelist.begin(); it != filelist.end(); it++) {
    chain->Add(it->c_str());
  }
  report(PASS, "MADE FILELIST WITH %lu ENTRIES, startred with %d",
         filelist.size(), nFiles);
  // call init
  // mgr->InitAnalysis();
  // // and perform analysis.
  // mgr->StartAnalysis("local", chain);
  mgr->doAnalysis(filelist);
  report(PASS, "Ran conversion");
  delete mgr;
  mgr = new AliO2AnalysisManager("AnalysisTaskExample");
  AliO2InputHandler *O2H = new AliO2InputHandler();
  O2H->skipPileups(true);
  mgr->SetInputEventHandler(O2H);
  ptAnalysisTask = new PtAnalysis("Pt Analysis");
  if (nullptr == ptAnalysisTask) {
    return -1;
  }
  mgr->ConnectInput(ptAnalysisTask, 0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput(
      ptAnalysisTask, 1,
      mgr->CreateContainer("Pt spectrum", TList::Class(),
                           AliO2AnalysisManager::kOutputContainer,
                           "TimeframeSpectrum.root"));
  filelist.clear();
  filelist.push_back("timeframe.root");
  mgr->doAnalysis(filelist);
  return 0;
}

#ifndef __CINT__
int main(int argc, const char **argv) { return ptSpectrum(argv + 1, argc - 1); }
#endif

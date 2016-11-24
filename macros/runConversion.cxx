/// \file runConversion.Cxx
/// \brief implementation of the runConversion macro.
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

#include "AliO2Timeframe.h"
#include "ConversionAnalysis.h"
#include <AliAnalysisAlien.h>
#include <AliAnalysisManager.h>
#include <AliESDInputHandler.h>
#include <AliVEventHandler.h>
#include <TChain.h>
#include <utilities/logging.h>

// based on RBertens tutorial

/// /alice/data/2010/LHC10b/000117042/ESDs/pass3/10000117042035.230/.root
const TString localESDFile("./AliESDs.root");
enum Mode { Local, Grid, TestGrid, Terminate };

/// Converts existing ESDs into AliO2Timeframes
///
/// \param mode Determines in what mode the conversion script runs.
/// \return returns 0 on success
int runConversion(Mode mode = Local) {

  // create the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisTaskExample");
  AliESDInputHandler *ESDH = new AliESDInputHandler();
  mgr->SetInputEventHandler(ESDH);

  // create an instance of your analysis task
  ConversionAnalysis *task = new ConversionAnalysis("task name");
  if (nullptr == task) {
    return -1;
  }
  TString fileName = AliAnalysisManager::GetCommonFileName();
  fileName += ":MyTask"; // create a subfolder in the file
  // now we create an instance of your task
  mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
  // same for the output
  mgr->ConnectOutput(task, 1, mgr->CreateContainer(
                                  "MyOutputContainer", AliO2Timeframe::Class(),
                                  AliAnalysisManager::kOutputContainer,
                                  fileName.Data()));
  AliAnalysisAlien *alienHandler = nullptr;
  TChain *chain = nullptr;
  if (mode != Local) {
    alienHandler = new AliAnalysisAlien();
    alienHandler->AddIncludePath(
        "-I. -I./classes -I$ROOTSYS/include -I$ALICE_ROOT "
        "-I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");
    // make sure your source files get copied to grid
    alienHandler->EnablePackage("O2.par");
    alienHandler->SetAdditionalLibs(
        "ConversionAnalysis.cxx ConversionAnalysis.h");
    alienHandler->SetAnalysisSource("ConversionAnalysis.cxx");
    // select the aliphysics version. all other packages
    // are LOADED AUTOMATICALLY!
    alienHandler->SetAliPhysicsVersion("vAN-20161030-1");
    // set the Alien API version
    alienHandler->SetAPIVersion("V1.1x");
    // select the input data
    // LHC11a10a_bis
    alienHandler->SetGridDataDir("/alice/data/2011/LHC11h_2");
    // esds than ESDs?
    alienHandler->SetDataPattern("*ESDs/pass2/ESD145/*ESD.root");
    // MC has no prefix, data has prefix 000
    alienHandler->SetRunPrefix("000");
    // runnumber
    alienHandler->AddRunNumber(167813);
    // number of files per subjob
    alienHandler->SetSplitMaxInputFileNumber(10);
    // alienHandler->SetExecutable("myTask.sh");
    // specify how many seconds your job may take
    alienHandler->SetTTL(10000);
    // alienHandler->SetJDLName("myTask.jdl");

    alienHandler->SetOutputToRunNo(kTRUE);
    alienHandler->SetKeepLogs(kTRUE);
    // merging: run with kTRUE to merge on grid
    // after re-running the jobs in SetRunMode("terminate")
    // (see below) mode, set SetMergeViaJDL(kFALSE)
    // to collect final results
    alienHandler->SetMaxMergeStages(1);
    alienHandler->SetMergeViaJDL(kTRUE);

    // define the output folders
    // can't set it to a folder because the JDL doesn't make nested folders.
    alienHandler->SetGridWorkingDir("MyWorkDir");
    alienHandler->SetGridOutputDir("MyOutDit");
    // connect the alien plugin to the manager
    mgr->SetGridHandler(alienHandler);
  } else {
    chain = new TChain("esdTree");
    // add a few files to the chain (change this so that your local files are
    // added)
    chain->Add(localESDFile);
  }
  if (!mgr->InitAnalysis()) {
    return -2;
  }
  mgr->PrintStatus();
  mgr->SetUseProgressBar(1, 25);
  switch (mode) {
  case Local:
    mgr->StartAnalysis("local", chain);
    break;
  case Grid:
    alienHandler->SetRunMode("full");
    mgr->StartAnalysis("grid");
    break;
  case TestGrid:
    // speficy on how many files you want to run
    alienHandler->SetNtestFiles(4);
    // and launch the analysis
    alienHandler->SetRunMode("test");
    mgr->StartAnalysis("grid");
    break;
  case Terminate:
    alienHandler->SetRunMode("terminate");
    mgr->StartAnalysis("grid");
  }
  return 0;
}

#ifndef __CINT__
int main(int argc, char **argv) { return runConversion(); }
#endif

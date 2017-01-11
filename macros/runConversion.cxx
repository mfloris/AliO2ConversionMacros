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

#include "ConversionAnalysis.h"
#include "O2Timeframe.h"
#include <AliAnalysisAlien.h>
#include <AliAnalysisManager.h>
#include <AliESDInputHandler.h>
#include <AliVEventHandler.h>
#include <O2logging.h>
#include <TAlienFile.h>
#include <TChain.h>
#include <TGrid.h>
#include <TGridResult.h>
#include <sys/wait.h>

const int runNumber = 138275;

// TODO: Error checking
// TODO: proper async, proccess the ones that finish first, first.
// TODO: Load balance across different sites.
// TODO: This doesn't work... only one file can be open at a time...
void asyncGrab(/*TGridResult *directories*/) {
  // How many connections we try and keep open at a given time.
  const unsigned simultanious_connections = 64;
  pid_t child_pid[simultanious_connections];
  // how many files we need to fetch
  // Int_t nEntries = directories->GetEntries();
  // report(INFO, "Grabbing %d files from the grid", nEntries);
  Int_t active_children = 0;
  int return_code;
  while (active_children < simultanious_connections) {
    child_pid[active_children] = fork();
    if (child_pid[active_children] == 0) {
      TGrid::Connect("alien://", 0, 0, "t");
      TString basedir = Form("alien://%sConversions_%09d/Output/000%d/",
                             gGrid->GetHomeDirectory(), runNumber, runNumber);
      TGridResult *directories =
          gGrid->Ls(Form("Conversions_%09d/Output/%09d", runNumber, runNumber));
      Int_t nEntries = directories->GetEntries();
      for (Int_t i = active_children; i < nEntries;
           i += simultanious_connections) {
        TString inputfile = TString(directories->GetFileName(i))
                                .Append("/AnalysisResults.root")
                                .Prepend(basedir);
        TString outputfile =
            Form("~/alice/data/timeframes/%09d_%04d.root", runNumber, i);
        // report(INFO, "Copying %s to %s", inputfile.Data(),
        // outputfile.Data());
        // TFile *file = TFile::Open(inputfile);
        // if (!file || file->IsZombie()) {
        //   report(FAIL, "couldn't open inputfile");
        // } else {
        //   report(PASS, "Could open file");
        // }
        Bool_t result = TFile::Cp(inputfile.Data(), outputfile.Data(), kFALSE);
        if (!result) {
          report(FAIL, "failed to copy %s to %s", inputfile.Data(),
                 outputfile.Data());
        } else {
          report(PASS, "Copied %s to %s", inputfile.Data(), outputfile.Data());
        }
      }
      delete directories;
      exit(0);
    }
    active_children++;
    report(PASS, "spawned %d children", active_children);
  }
  report(INFO, "Waiting on children to finish");
  for (int i = 0; i < active_children; i++) {
    waitpid(child_pid[i], &return_code, 0);
    report(INFO, "Child %d exited with %d", i, return_code);
  }
  report(PASS, "Children finished");
}

// based on RBertens tutorial
const TString
    localESDFile("/home/roel/alice/data/2010/LHC10h/000138275/ESDs/pass2/"
                 "10000138275063.80/AliESDs.root");
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
  mgr->ConnectOutput(
      task, 1, mgr->CreateContainer("AliO2Timeframe", O2Timeframe::Class(),
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
    alienHandler->SetGridDataDir("/alice/data/2010/LHC10h/");
    alienHandler->SetDataPattern("ESDs/pass2/10000138275063.*/AliESDs.root");
    // MC has no prefix, data has prefix 000
    alienHandler->SetRunPrefix("000");
    // runnumber
    alienHandler->AddRunNumber(runNumber);
    // number of files per subjob
    alienHandler->SetSplitMaxInputFileNumber(20);
    // alienHandler->SetExecutable("myTask.sh");
    // specify how many seconds your job may take
    alienHandler->SetTTL(10000);
    // alienHandler->SetJDLName("myTask.jdl");

    alienHandler->SetOutputToRunNo(kTRUE);
    // alienHandler->SetKeepLogs(kTRUE);
    // merging: run with kTRUE to merge on grid
    // after re-running the jobs in SetRunMode("terminate")
    // (see below) mode, set SetMergeViaJDL(kFALSE)
    // to collect final results
    alienHandler->SetMaxMergeStages(0);
    alienHandler->SetMergeViaJDL(kFALSE);

    // define the output folders
    // can't set it to a folder because the JDL doesn't make nested folders.
    alienHandler->SetGridWorkingDir(Form("Conversions_000%d", runNumber));
    alienHandler->SetGridOutputDir("Output");
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
    // faster and more reliable...
    asyncGrab();
  }

  return 0;
}

#ifndef __CINT__
int main(int argc, char **argv) { return runConversion(); }
#endif

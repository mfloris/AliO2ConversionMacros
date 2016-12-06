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

#include "O2Timeframe.h"
#include "TestTask.h"
#include <AliO2AnalysisManager.h>
#include <AliO2InputHandler.h>
#include <boost/filesystem.hpp>
#include <list>
#include <string>
#include <sys/wait.h>
#include <utilities/logging.h>

class test : public InterfaceTimestampped {};
class test2 : public virtual InterfaceTimestampped {};
/// /alice/data/2010/LHC10b/000117042/O2s/pass3/10000117042035.230/.root
const std::string localO2File = "/home/roel/alice/data/timeframes/";

/// Converts existing O2s into AliO2Timeframes
///
/// \param mode Determines in what mode the conversion script runs.
/// \return returns 0 on success
int runConversion(const char **files, int nFiles) {
  // create the analysis manager
  AliO2AnalysisManager *mgr = new AliO2AnalysisManager("AnalysisTaskExample");
  AliO2InputHandler *O2H = new AliO2InputHandler();
  mgr->SetInputEventHandler(O2H);

  // create an instance of your analysis task
  TestTask *task = new TestTask("task name");
  if (nullptr == task) {
    return -1;
  }
  TString fileName = AliO2AnalysisManager::GetCommonFileName();
  mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
  // same for the output
  mgr->ConnectOutput(
      task, 1, mgr->CreateContainer("AliO2Timeframe", O2Timeframe::Class(),
                                    AliO2AnalysisManager::kOutputContainer,
                                    fileName.Data()));

  // add a few files to the chain (change this so that your local files are
  // added)

  std::list<std::string> chain;
  for (int i = 0; i < nFiles; i++) {
    boost::filesystem::path p(files[i]);
    if (is_directory(p)) {
      for (auto i = boost::filesystem::directory_iterator(p);
           i != boost::filesystem::directory_iterator(); i++) {
        if (!is_directory(i->path())) // we eliminate directories
        {
          std::string new_name = p.string();
          chain.push_back(new_name.append(i->path().filename().string()));
        } else
          continue;
      }
    } else {
      chain.push_back(files[i]);
    }
  }

  // if (!mgr->InitAnalysis()) {
  //   return -2;
  // }
  report(PASS, "DOING ANALYSIS NOW");
  mgr->doAnalysis(chain);
  return 0;
}

#ifndef __CINT__
int main(int argc, const char **argv) {
  return runConversion(argv + 1, argc - 1);
}
#endif

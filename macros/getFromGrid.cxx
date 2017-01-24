/// \file getFromGrid.Cxx
/// \brief implementation of the getFromGrid macro.
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

#include <O2logging.h>
#include <TAlienFile.h>
#include <TGrid.h>
#include <TGridResult.h>
#include <boost/filesystem.hpp>
#include <sys/wait.h>

const char *DEFAULT_PATTERN = "*";
const char *DEFAULT_OUT = ".";

// parallel grabs files from the grid using a syntax similar to find. do *NOT*
// call
// when already connected to the grid.
void parallelGrab(const char *directory, const char *pattern = DEFAULT_PATTERN,
                  const char *outputDirectory = DEFAULT_OUT) {
  // How many connections we try and keep open at a given time.
  const unsigned simultanious_connections = 64;
  pid_t child_pid[simultanious_connections];
  Int_t active_children = 0;
  int return_code;

  while (active_children < simultanious_connections) {
    child_pid[active_children] = fork();
    if (child_pid[active_children] == 0) {
      auto query = gGrid->Query(directory, pattern, "", "");
      TGrid::Connect("alien://", 0, 0, "t");

      Int_t nEntries = query->GetEntries();
      report(PASS, "Found %d entries", nEntries);
      for (Int_t i = active_children; i < nEntries;
           i += simultanious_connections) {
        const char *turl = query->GetKey(i, "turl");
        if (turl) {
          report(INFO, "fetching: %s", turl);
        }
        const char *dirStart = strstr(turl, directory);
        if (!dirStart) {
          report(FAIL, "Couldn't form output file name. Couldn't find %s in %s",
                 directory, turl);
          continue;
        }
        const char *file_out = dirStart + strlen(directory);
        TString outputfile = Form("%s%s", outputDirectory, file_out);
        boost::filesystem::create_directories(
            gSystem->DirName(gSystem->ExpandPathName(outputfile.Data())));
        Bool_t result = TFile::Cp(turl, outputfile.Data(), kFALSE);
        if (!result) {
          report(FAIL, "failed to copy %s to %s", turl, outputfile.Data());
        } else {
          report(PASS, "Copied %s to %s", turl, outputfile.Data());
        }
      }
      delete query;
      exit(0);
    }
    active_children++;
    // report(PASS, "spawned %d children", active_children);
  }
  report(INFO, "Waiting on children to finish");
  for (int i = 0; i < active_children; i++) {
    waitpid(child_pid[i], &return_code, 0);
    report(INFO, "Child %d exited with %d", i, return_code);
  }
  report(PASS, "Children finished");
}

// based on RBertens tutorial

/// Download several files from the grid in parallel, has a syntax similar to
/// aliensh's find. Output file names are {folder}/{result of pattern}.
///
/// \param folder The folder ot search
/// \param pattern The matching pattern to use
/// \param folder The folder to write the output files to. Will be created
/// (recusively) if it doesn't exist.
/// \return returns 0 on success
int getFromGrid(const char *folder, const char *pattern = DEFAULT_PATTERN,
                const char *output = DEFAULT_OUT) {
  parallelGrab(folder, pattern, output);
  return 0;
}

#ifndef __CINT__
int main(int argc, char **argv) {
  if (argc == 1) {
    report(FAIL, "usage: getFromGrid <folder> [pattern] [output directory]");
  }
  return getFromGrid(argv[1], argc >= 3 ? argv[2] : DEFAULT_PATTERN,
                     argc >= 4 ? argv[3] : DEFAULT_OUT);
}
#endif

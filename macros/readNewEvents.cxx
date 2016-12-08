/// \file readNewEvents.Cxx
/// \brief implementation of the readNewEvents macro.
/// \since 2016-11-18
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

#include "O2Event.h"
#include "O2Timeframe.h"
#include <TFile.h>
#include <TKey.h>
#include <TList.h>
#include <TObject.h>
#include <iostream>
#include <utilities/logging.h>
/// Reads events from the new timeframe format
///
/// Prints an overview of the tracks and the timestamp for events which contain
/// ambiguities
/// \return Returns 0 on succes.
int readNewEvents(int argc, char **filenames) {
  for (int i = 0; i < argc; i++) {
    TString filename(filenames[i]);
    // MyTask;1/MyOutputContainer;1/
    TFile *file = new TFile(filename, "READ");
    // file->ls();
    auto directory = file->GetDirectory("MyTask");
    // std::cout << directory->ClassName() << std::endl;
    directory->ls();
    // this line leaks memory
    // while ((key = (TKey *)next())) {
    O2Timeframe *container =
        dynamic_cast<O2Timeframe *>(directory->Get("O2Timeframe"));
    if (!container) {
      std::cerr << "Failed to read timeframe!\n";
      return -1;
    }
    container->WriteToFile("testfile.bin");
    report(PASS, "estimated compressed size: %lu",
           container->estimate_compression());
    // size_t event_count = container->getNumberOfEvents();
    // std::cout << "Timeframe contains " << event_count << " events\n";
    // for (size_t i = 0; i < event_count; i++) {
    //   O2Event event = container->getEvent(i);
    //   if (event.GetNumberOfAmbigousTracks()) {
    //     printf("%lu %f\n Total: %lu, %lu, %lu\n Global: %lu, %lu, %lu\nITS: "
    //            "%lu,"
    //            "%lu, %lu\n",
    //            i, event.getTimestamp(), event.GetNumberOfTracks(),
    //            event.GetNumberOfUnambigousTracks(),
    //            event.GetNumberOfAmbigousTracks(),
    //            event.GetNumberOfGlobalTracks(),
    //            event.GetNumberOfUnambigousGlobalTracks(),
    //            event.GetNumberOfAmbigousGlobalTracks(),
    //            event.GetNumberOfITSTracks(),
    //            event.GetNumberOfUnambigousITSTracks(),
    //            event.GetNumberOfAmbigousITSTracks());
    //   }
    // }
    delete container;
    //}
    delete (file);
  }
  return 0;
}

#ifndef __CINT__
int main(int argc, char **argv) { return readNewEvents(argc - 1, argv + 1); }
#endif

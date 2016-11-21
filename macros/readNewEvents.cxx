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

#include "AliO2Event.h"
#include <TFile.h>
#include <TList.h>
#include <TObject.h>
#include <iostream>
/// Short readNewEvents description
///
/// More detailed readNewEvents description
/// \return What this function returns
int readNewEvents(int argc, char **filenames) {
  size_t eventCounter = 0;
  size_t trackCounter = 0;
  size_t vertexCounter = 0;
  AliO2Event *event;
  for (int i = 0; i < argc; i++) {
    TString filename(filenames[i]);
    // MyTask;1/MyOutputContainer;1/
    TFile *file = new TFile(filename, "READ");
    auto directory = file->GetDirectory("MyTask");
    // std::cout << directory->ClassName() << std::endl;
    // this line leaks memory
    TObject *container = directory->Get("MyOutputContainer");
    // std::cout << container->ClassName() << std::endl;
    TList *list = static_cast<TList *>(container);
    TIter next(list);
    while ((event = static_cast<AliO2Event *>(next()))) {
      trackCounter += event->GetNumberOfTracks();
      vertexCounter += event->GetNumberOfVertices();
      eventCounter++;
    }
    std::cout << "Found a total of " << trackCounter << " tracks, "
              << vertexCounter << " vertices, spread over " << eventCounter
              << " events \n";
    delete container;
    delete (file);
  }
  return 0;
}

#ifndef __CINT__
int main(int argc, char **argv) { return readNewEvents(argc - 1, argv + 1); }
#endif

/// \file PtAnalysis.h
/// \author R.G.A. Deckers
/// \brief Definition of the PtAnalysis class.
///
/// See implementation file for copyright details.

#ifndef PtAnalysis_H
#define PtAnalysis_H

// Include the base classes
#include <AliO2AnalysisTaskSE.h>

#include "InterfaceTimestampped.h"
#include <TRandom2.h>
#include <TRefArray.h>

class O2Timeframe;
/// Short PtAnalysis description
///
/// More detailed PtAnalysis description

class PtAnalysis : public AliO2AnalysisTaskSE {
public:
  /// Default constructor
  PtAnalysis();
  /// Named constructor
  PtAnalysis(const char *name);
  /// Destructor
  ~PtAnalysis();
  // intialization
  virtual void UserCreateOutputObjects();
  // per event
  virtual void UserExec(Option_t *option);
  // Cleanup
  virtual void Terminate(Option_t *option);

protected:
  // protected stuff goes here

private:
  /// copy constructor prohibited
  PtAnalysis(const PtAnalysis &);
  /// assignment operator prohibited
  PtAnalysis &operator=(const PtAnalysis &);
  std::vector<TH1F *> mHistograms;
  TList *mList;
  // TH1F *mHistogram = nullptr; //!
  // root specific
  ClassDef(PtAnalysis, 1);
};

#endif

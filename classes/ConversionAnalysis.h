/// \file ConversionAnalysis.h
/// \author R.G.A. Deckers
/// \brief Definition of the ConversionAnalysis class.
///
/// See implementation file for copyright details.

#ifndef CONVERSIONANALYSIS_H
#define CONVERSIONANALYSIS_H

// Include the base classes
#include <AliAnalysisTaskSE.h>
class TList;

/// Short ConversionAnalysis description
///
/// More detailed ConversionAnalysis description

class ConversionAnalysis : public AliAnalysisTaskSE {
public:
  /// Default constructor
  ConversionAnalysis();
  /// Named constructor
  ConversionAnalysis(const char *name);
  /// Destructor
  ~ConversionAnalysis();
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
  ConversionAnalysis(const ConversionAnalysis &);
  /// assignment operator prohibited
  ConversionAnalysis &operator=(const ConversionAnalysis &);

  unsigned event_counter = 0;
  unsigned failed_event_counter = 0;
  TList *mResults = nullptr;
  // root specific
  ClassDef(ConversionAnalysis, 1);
};

#endif

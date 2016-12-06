/// \file TestTask.h
/// \author R.G.A. Deckers
/// \brief Definition of the TestTask class.
///
/// See implementation file for copyright details.

#ifndef TestTask_H
#define TestTask_H

// Include the base classes
#include <AliO2AnalysisTaskSE.h>

#include "InterfaceTimestampped.h"
#include <TRandom2.h>

class O2Timeframe;
/// Short TestTask description
///
/// More detailed TestTask description

class TestTask : public AliO2AnalysisTaskSE {
public:
  /// Default constructor
  TestTask();
  /// Named constructor
  TestTask(const char *name);
  /// Destructor
  ~TestTask();
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
  TestTask(const TestTask &);
  /// assignment operator prohibited
  TestTask &operator=(const TestTask &);
  O2Timeframe *mResults = nullptr; //!
  // root specific
  ClassDef(TestTask, 1);
};

#endif

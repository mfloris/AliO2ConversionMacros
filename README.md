# AliO2ConversionMacros
Experimental conversion macro for creating new AoDs from old data for the ALICE's O2 upgrade.

## Pre-requisites

### Install the Aliroot branch for the O2 AOD developement

These are the minimum install requirements to run thise repository, there is also an AliPhysics branch for the AOD upgrade but it is not needed for now & contains no changes. **TODO**: I would install the full stack: if you want to run analysis, you will need classes from AliPhysics, even if there are no differences for the moment.

AliRoot and AliPhysics should be compiled against ROOT6.

1. Init alibuild with a separate folder, set both aliphyscs and aliroot as developement packages

   ```
   aliBuild init AliRoot,AliPhysics -z ali-aod-dev
   ```
2. Manually change the aliroot branch to the AOD developement branch

   ```bash
   $ cd ali-aod-dev/AliRoot
   $ git checkout aod-upgrade
   ```
3. Checkout root 6 manually   

   ```
   cd ali-aod-dev 
   git clone -b v6-08-02 --depth 1 http://root.cern.ch/git/root.git 

   ```
   This is a temporary workaround: it should be possible to use alibuild's defaults (see below)
   
3. Compile

   Currently, the AliDist recipe does not correctly pull in the right version of gcc if the system's version is too new (as happens with e.g. Ubuntu). If compilation fails due to an incorrect version of gcc it is recommended to use clang (or an older gcc version) instead. On ubuntu the default c++ compiler can be set with the command
   
   ```bash
   sudo update-alternatives --config c++ 
   ```
4. Troubleshoot 

   Compilation of Root6 might also fail if CPLUS_INCLUDE_PATH or C_INCLUDE_PATH are set as is the case when one is in a 'alienv enter' shell. To fix this run 
   
   ```bash
   unset CPLUS_INCLUDE_PATH
   unset C_INCLUDE_PATH
   ```

Finally, to actually install AliRoot,AluPhysics run:

   ```bash
   $ aliBuild -j 6 -z -w ../sw build AliPhysics --disable GEANT3,GEANT4_VMC,fastjet
   ```
   **TODO**:
   Should build with root6. Something like 
   ```
   aliBuild -d --defaults ROOT6 -j 2 -z -w ../sw build AliPhysics
   ```
   Gets stuck. To be clarified why.
   **TODO**
   I would suggest not to disable other packages.
   
### Install custom macros to produce the new AODs

Clone https://github.com/RDeckers/AliO2ConversionMacros and compile with make
```bash
$ git clone https://github.com/RDeckers/AliO2ConversionMacros.git  
$ make
```

This will produce executable versions of the scripts in the macros folder

## How to use the code

After running make, several executables are placed in ./bin/
Unfortunately, they currently contain hardcoded paths and will therefore not run without modification. This wil be fixed soon^tm  .

1. Download some ESDs (or use it online) TODO: remove hardcoded values and clarify this part
2. Run the runConversion script 
3. Run pt_spectrum TODO: check naming conistency in the script
   * This runs the analysis tast (it's the usual "Steering" macro)
   * It runs the "PtAnalysis.cxx" analysis task
   * This produces the histogram as output
   * This actually run twice: once on the O2 time frome and the other time on the "old" ESDs TODO: remove hardcoded parts
4. The make_pt_plots script will produce the plots

The basic usage would be 
1. getFilesFromGrid <folder> <pattern> <output directory> //where folder 
and patter follow 'find' syntax. 
2. ptSpectrum <output directory>*ESD*.root 
3. makePtPlots 


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
   
3. Optional steps, should not be needed on most systems:

      a .Currently, the AliDist recipe does not correctly pull in the right version of gcc if the system's version is too new (as happens with e.g. Ubuntu). If compilation fails due to an incorrect version of gcc it is recommended to use clang (or an older gcc version) instead. On ubuntu the default c++ compiler can be set with the command
   
      ```bash
      sudo update-alternatives --config c++ 
      ```
     b. Troubleshoot 

     Compilation of Root6 might also fail if CPLUS_INCLUDE_PATH or C_INCLUDE_PATH are set as is the case when one is in a 'alienv enter' shell. To fix this run 
   
      ```bash
      unset CPLUS_INCLUDE_PATH
      unset C_INCLUDE_PATH
     ```

Finally, to actually install AliRoot,AliPhysics run:

   ```bash
   $ aliBuild -j 6 -z -w ../sw build AliPhysics --disable GEANT3,GEANT4_VMC,fastjet
   ```
   **TODO**: This does not work. If you cloned root manually above it is not needed.
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

This will produce executable versions of the scripts in the macros folder.

Note: on OSx you may need to install clang with home brew:
```
    brew install clang
```


## How to use the code

After running make, several executables are placed in ./bin/
Because the code depends on modifications to AliRoot which are not on the grid the analysis tasks can only be run locally.
The executable getFromGrid is a wrapper around aliensh's find command which can be used to fetch multiple files from the grid in parallel based on a {folder}/{pattern} combination similiar to the 'find' command.

 Once you have some ESDs availible, you can run a pt-spectrum analysis over them using the ptSpectrum executable. This will produce two root files containing the pt-spectrum computed using the original ESDs as well as one containing the spectrum computed using the timeframe (which is dynamically build from the given ESDs). Plots can be created by running makePtPlots afterwards.

There is also the executable runConversion, this executable takes a list of ESDs as argument and builds a single timeframe based on the data of the ESDs. 

readNewEvents shows how to read back the data created using runConversion. It takes as argument a list of .root files produced by runConversion.

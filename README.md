# AliO2ConversionMacros
Programs for testing the new AOD format of AliRoot/aod-upgrade for Run3. Contains example analysis tasks and can convert (local) ESD root files to the new format.

## Pre-requisites

### Install the Aliroot branch for the O2 AOD developement

These are the minimum install requirements to run thise repository, there is also an AliPhysics branch for the AOD upgrade but it is not needed for now & contains no changes.

1. Init alibuild with a separate folder
   ```bash
   aliBuild init AliRoot -z ali-aod-dev
   ```

2. Manually change the aliroot branch to the AOD developement branch
   ```bash
   $ cd ali-aod-dev/AliRoot
   $ git checkout aod-upgrade
   ```

3. Compile
Currently, the AliDist recipe does not correctly pull in the right version of gcc if the system's version is too new (as happens with e.g. Ubuntu). If compilation fails due to an incorrect version of gcc it is recommended to use clang (or an older gcc version) instead. On ubuntu the default c++ compiler can be set with the command
```bash
   sudo update-alternatives --config c++ 
```

Compilation of Root6 might also fail if CPLUS_INCLUDE_PATH or C_INCLUDE_PATH are set as is the case when one is in a 'alienv enter' shell. To fix this run 
```bash
unset CPLUS_INCLUDE_PATH
unset C_INCLUDE_PATH
```

Finally, to actually install AliRoot, run:

   ```bash
   $ aliBuild -j 6 -z -w ../sw build AliRoot --disable GEANT3,GEANT4_VMC,fastjet
   ```
   
### Install custom macros to produce the new AODs

Clone https://github.com/RDeckers/AliO2ConversionMacros and compile with make
```bash
$ git clone https://github.com/RDeckers/AliO2ConversionMacros.git  
$ make
```

This will produce executable versions of the scripts in the macros folder

## How to use the code

After running make, several executables are placed in ./bin/
Because the code depends on modifications to AliRoot which are not on the grid (and cannot be pushed in a par file because they require c++11) the analysis tasks can only be run locally.

I recommend downloading ESDs using the following script
```bash
 for FILE in $(alien_find  /alice/data/2010/LHC10h/000139038/ESDs/pass2/ 10000139038065.1*/AliESDs.root|head -n -2);
 do mkdir -p ~/`dirname $FILE` && alien_cp -m alien:$FILE ~/$FILE& done
```
 Which will, in parallel, fetch all the `AliESD.root` files given by the `alien_find` command and put them in your home folder with the same path as they would in aliensh.
 
 Once you have a couple of ESD files availible, you can convert them by running `./bin/runConversion <FILES>`, this will generate a single file `aod.bin`.  

 The command `./bin/PtAnalysis <NEW FILES>` produces a Pt plot of all the tracks in the new files and shows how to use the new analysis framework (same as the old).
 
 The macro `readBackData` uses a more direct interface for reading files.

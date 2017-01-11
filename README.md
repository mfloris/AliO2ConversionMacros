# AliO2ConversionMacros
Experimental conversion macro for creating new AoDs from old data for the ALICE's O2 upgrade.

## Pre-requisites

### Install the Aliroot/aliphysics branch for the O2 AOD developement

1. Init alibuild with a separate folder
   ```bash
   aliBuild init AliRoot,AliPhysics -z ali-aod-dev
   ```

2. Manually change the aliroot and aliphysics branch to the AOD developement branch
   ```bash
   $ cd ali-aod-dev/AliPhysics
   $ git checkout aod-upgrade
   $ cd ../AliRoot
   $ git checkout aod-upgrade
   ```

3. Complile
   ```bash
   $ aliBuild -j 2 -z -w ../sw build AliPhysics
   ```
   
   
### Install custom macros to produce the new AODs

Clone https://github.com/RDeckers/AliO2ConversionMacros and compile with make
```bash
$ git clone https://github.com/RDeckers/AliO2ConversionMacros.git  
$ make
```

This will produce executable versions of the scripts in the macros folder

## How to use the code

1. Download some ESDs (or use it online) TODO: remove hardcoded values and clarify this part
2. Run the runConversion script 
3. Run pt_spectrum TODO: check naming conistency in the script
   * This runs the analysis tast (it's the usual "Steering" macro)
   * It runs the "PtAnalysis.cxx" analysis task
   * This produces the histogram as output
   * This actually run twice: once on the O2 time frome and the other time on the "old" ESDs TODO: remove hardcoded parts
4. The make_pt_plots script will produce the plots

# AliO2ConversionMacros
Programs for testing the new AOD format of AliRoot/aod-upgrade for Run3. Contains example analysis tasks and can convert (local) ESD root files to the new format.

## How to use the code

run `make`, after which several executables are placed in ./bin/
Because the code depends on modifications to AliRoot which are not on the grid (and cannot be pushed in a par file because they require c++11) the analysis tasks can only be run locally.

I recommend downloading ESDs using the following script
```bash
 for FILE in $(alien_find  /alice/sim/LHC11a10a/138837/ 0*/*.root|head -n -2);
 do mkdir -p ~/`dirname $FILE` && alien_cp -m alien:$FILE ~/$FILE& done
```
 Which will, in parallel, fetch all the root files given by the `alien_find` command and put them in your home folder with the same path as they would in aliensh.

 Once you have a couple of ESD files availible, you can convert them by running `./bin/runConversion <FILES>`, this will generate a single file `aod.bin`.  

 The command `./bin/PtAnalysis <NEW FILES>` produces a Pt plot of all the tracks in the new files and shows how to use the new analysis framework (same as the old).

 The macro `readBackData` uses a more direct interface for reading files.

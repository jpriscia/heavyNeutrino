# heavyNeutrino
New repository for the heavyNeutrino analysis

# Set-up instructions
```
cmsrel CMSSW_8_0_28_patch1
cd CMSSW_8_0_28_patch1/src
cmsenv
git cms-init
git clone https://github.com/GhentAnalysis/heavyNeutrino
cd heavyNeutrino
./setup.sh
```

# Running a test job
```
cd heavyNeutrino/multilep/test
cmsRun multilep.py
```

# Mass production of tuples
```
cd heavyNeutrino/multilep/test
./submitAll.py <datasetsFile>
```

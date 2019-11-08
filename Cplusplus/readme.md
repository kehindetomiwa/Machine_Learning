
## BDT tool (Boosted decision tree too)
This tool is based on the TMVA framework 
https://root.cern.ch/tmva



Run with  RunInterface.C
 
 or

 ```bash

source runInterface.sh

```
Model weights are stored in ./wieghts/*


Visualization are done in in plotEvalution.C this code reads the output XML files and ROOT ntuple to make test/train comparison 
 ```bash

source plotROC.sh

```


Model evaluation is done bu scan.C

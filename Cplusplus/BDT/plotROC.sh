rm -rf plotROC
g++ -I. `root-config --libs --cflags` -lXMLIO -lRooFitCore -lRooFit -lRooStats -lTMVA -lTreePlayer -o plotROC src/plotEvaluation.C src/Plotter.cxx src/Plotter.h

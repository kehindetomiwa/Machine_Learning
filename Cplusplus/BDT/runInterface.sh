#source getinputcard.sh
rm runMVA
g++ -I. `root-config --libs --cflags` -lRooFitCore -lRooFit -lRooStats -lTMVA -lXMLIO -lTreePlayer -lMathMore -lm -o runMVA src/MVATools.cxx src/RunInterface.C 
./runMVA -t data/MVATool.cfg

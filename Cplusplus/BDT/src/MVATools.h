/*  Run interface for MVATools
 *  Code uses configuration file on data/MVATool.cfg
 *  Autors: Xifeng Ruan, Rob Reed, Kehinde Tomiwa
 *         kehinde.tomiwa4@gmail.com
 */

#ifndef MVATools_h
#define MVATools_h

#include "TRandom.h"
#include <string>
#include <iostream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/Factory.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooExponential.h"
#include "RooFit.h"
#include "TLeaf.h"
#include "TLegend.h"
#include "TSystem.h"

using namespace std;

class MVATools
{
public:
    
    MVATools(TString filesig, TString filebkg, TString fileout,TString strvar,TString sigTree,TString bkgTree,TString eventweight);
    MVATools(TString filesigtr, TString filesigte, TString filebkgtr, TString filebkgte, TString fileout,TString strvar, TString Stree,TString Btree,TString eventweight);
    MVATools();
    void Loop();
    void MVA_Redumper();

    void Training_setupFactory(TString factConfig);
    void Training_setInputs(TString traingCfg, TString mvaalgo, TString algocofig);
    void Training_execute(TCut mycuts="",TCut mycutb="");

    void Evaluate_setMVAReader(TString file,std::vector<TString> xml, TString fileout, std::vector<TString> strvar);
    void Evaluate_saveBDTOutput();

    void addvarlist(TString str);
    void DoCutFlow();
    void Testing2D();
    void Plotting();
    std::vector<TString> splitstr(string input, const char *sign);
    void PrintProgressBar(int index, int total);
    void setdir(TString dir);
//void SetMVAInput(TString optionfile);


////for mva training
    TTree *signaltr;
    TTree *backgroundtr;
    TTree *signalte;
    TTree *backgroundte;
    TTree *treebkg2;
    TFile *filebkg2;
    TFile* inputstr;
    TFile* inputbtr;
    TFile* inputste;
    TFile* inputbte;

    TTree *signal;
    TTree *background;
    std::vector<TString> mvaVars;
    std::vector<TString> spectatorVars;
    TString mvaoutname;
/////for mva train initializing
    TFile* outputFile;
    TFile* inputs;
    TFile* inputb;
    TMVA::Factory *factory ;
    #if ROOT_VERSION_CODE >= ROOT_VERSION(6,8,0)
      TMVA::DataLoader* dataloader;
    #endif
		TString m_trainconfig;
		TString m_mvaAlgo;
		TString m_algoConfig;
/////for mva test initializing
    std::vector<TMVA::Reader*> reader;
    std::vector<TString> xml;
    TChain *readerchain;
    TString readeroutputfile;
    Float_t  *arrayvar;
    Float_t  *arrayspe;
    TTree *outputtree;
    Float_t  mvaVarsHolder[100];
    Float_t  mvaVarsSpecHolder[100];
	
    Float_t  arrayspe2d[10][100];

    //Float_t  mvaVarsHolder[100];

    std::vector< std::vector< TString > > stringset;
    TString trainingDir;
    TString weight;
};
#endif

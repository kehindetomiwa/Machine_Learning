/*  Evaluation code for Test samples
 *  Autors: Xifeng Ruan,  Kehinde Tomiwa
 *         kehinde.tomiwa4@gmail.com
 */

#include "Plotter.h"
#include "TFile.h"
#include "TChain.h"
#include "TGraph.h"
#include "TMath.h"
#include "TH2.h"
#include "TLine.h"
#include <iostream>
#include <fstream>

void PrintProgressBar(int index, int total);
void makeEffRoc(TString bdtclass,std::map<TString, double> &Xvals,std::map<TString, double> &Yvals);
TGraph* getNoImp(TH1* hist);
int main()
{
    Plotter *plotter = new Plotter();
    plotter->initialize();
    plotter->SetAtlasStyle();
    plotter->setLogY(false);
    plotter->dataToMC(false);
    plotter->drawRatio(false);
    plotter->drawRatioErr(false);
    plotter->setDrawOptions("L");

    TString baseDir = "MVA/Testout/";
    std::vector<TString> trainBDTFiles;

    std::cout<<"Running over files in: "<<baseDir<<std::endl;
    TChain *chainSig = new TChain("tree");
    chainSig->AddFile(baseDir+"/out_mr275mx60_jpt20.root");
    TChain *chainBkg = new TChain("tree");
    //chainBkg->AddFile(baseDir+"/out_SherpaFast_sim_jpt20.root");
    chainBkg->AddFile(baseDir+"/out_background.root");
    std::map<TString,std::vector<std::vector<double>>> sigYield;
    std::map<TString,std::vector<std::vector<double>>> bkgYield;
    std::map<TString,std::vector<double>> sigTotals;
	
	TFile *outfile=new TFile(baseDir+"varout2.root", "recreate");
	std::map<TString, TH1F*> histo;
	

    std::vector<double> temp;
    for(int y = 0; y< 20; y++)
        {
         temp.push_back(0.0);
        }

    //double sigmet4Nom, sigmet4VtxNom, sigmet55Nom, sigmet55VtxNom , bkgmet4Nom, bkgmet4VtxNom, bkgmet55Nom, bkgmet55VtxNom = 0.0;

    //Crease the category set names
    std::map<TString,TString> setDisplayName;
    //std::vector<TString> sets = {"MetSig35","MetSig35Vtx","MetSig35Vtx2J","MetSig55","MetSig55Vtx","MetSig55Vtx2J"};
	  std::vector<TString> sets = {"MetSig25","MetSig35","MetSig55","MetSig25Vtx","MetSig35Vtx","MetSig55Vtx"};
	  std::vector<TString> jetsets = {"NJ0","NJ1","NJ2"};
	
	  for(auto jetset: jetsets){
			TString jetLab = "";
			if(jetset.Contains("1"))jetLab="N_{j} == 1";
			if(jetset.Contains("2"))jetLab="N_{j} #geq 2";
			setDisplayName["MetSig25"+jetset]  = "metsig>2.5 && metsig<3.5"+jetLab;
			setDisplayName["MetSig35"+jetset]  = "metsig>3.5 && metsig<5.5"+jetLab;
			setDisplayName["MetSig55"+jetset]  = "metsig>5.5"+jetLab;
	    setDisplayName["MetSig25Vtx"+jetset]  = "metsig>2.5 && metsig<3.5,SV"+jetLab;
	    setDisplayName["MetSig35Vtx"+jetset]  = "metsig>3.5 && metsig<5.5,SV"+jetLab;
	    setDisplayName["MetSig55Vtx"+jetset]  = "metsig>5.5,SV"+jetLab;
		}
	
    //Create map to store nominal yields for each category
    std::map<TString, double> nomBkgYields;
    std::map<TString, double> nomSigYields;
	for(auto set : sets){
		for(auto jetset : jetsets){
        nomBkgYields[set+jetset]=0.0;
        nomSigYields[set+jetset]=0.0;
		}
	}
    //Create yields variable for each BDT variable for each category.
    //In this example there are 5 different BDT types for each metSig category.
    int BDTCount = 9;
    for(int cats = 0; cats <= BDTCount; cats++)
			for(auto set : sets){
				for(auto jetset : jetsets){
						sigYield[set+jetset].push_back(temp);
            bkgYield[set+jetset].push_back(temp);
				}
			}
	std::cout<<"size sig: "<<sigYield.size()<<std::endl;
	std::cout<<"size bkg: "<<bkgYield.size()<<std::endl;
	std::cout<<"size sig: "<<sigYield["MetSig55NJ2"].size()<<std::endl;
	std::cout<<"size bkg: "<<bkgYield["MetSig55NJ2"].size()<<std::endl;
	
	
    std::cout<<"Vectors initialised..."<<std::endl;
	
	//this for the BDT variables
	std::vector<TString> bdtclasses = {"metsig25noJ","metsig25NJ1","metsig25NJ2","metsig35noJ","metsig35NJ1","metsig35NJ2","metsig55noJ","metsig55NJ1","metsig55NJ2"};
	
	for(TString sample: {"sig","bkg"}){
		for(TString bdtVar :bdtclasses){
			histo[sample+bdtVar]  = new TH1F(bdtVar+sample, bdtVar+sample, 100,-1.15,0.95);
		}
	}
	
    float metSig = 0.0;
    std::vector<float> BDTValues;
    for(int i = 0; i <=9; i++)
        BDTValues.push_back(0.0);
    float diff_sumpt2 = 0;
    float weight = 0;
    float dz = 100;
    float myy = 0.0;
    float pt1 = 0.0;
    float pt2 = 0.0;
    float N_j_central = 0.0;
	float jets_Rpt1 = 0.0;
	float jets_JvfCorr1 = 0.0;
	float jets_Rpt2 = 0.0;
	float jets_JvfCorr2 = 0.0;
	float jets_Rpt3 = 0.0;
	float jets_JvfCorr3 = 0.0;
	float jets_Rpt4 = 0.0;
	float jets_JvfCorr4 = 0.0;
	float dphi_yy_j1 = 0.0;
	float dphi_FjetV_met = 0.0;
    chainSig->SetBranchAddress("dz",&dz);
    chainSig->SetBranchAddress("metsig", &metSig);
    chainSig->SetBranchAddress("weight", &weight);
    chainSig->SetBranchAddress("diff_sumpt2", &diff_sumpt2);
    chainSig->SetBranchAddress("m_yy", &myy);
    chainSig->SetBranchAddress("phpt1", &pt1);
    chainSig->SetBranchAddress("phpt2", &pt2);
    chainSig->SetBranchAddress("N_j_central", &N_j_central);
	
	chainSig->SetBranchAddress("jets_Rpt1", &jets_Rpt1);
	chainSig->SetBranchAddress("jets_JvfCorr1", &jets_JvfCorr1);
	chainSig->SetBranchAddress("jets_Rpt2", &jets_Rpt2);
	chainSig->SetBranchAddress("jets_JvfCorr2", &jets_JvfCorr2);
	chainSig->SetBranchAddress("jets_Rpt3", &jets_Rpt3);
	chainSig->SetBranchAddress("jets_JvfCorr3", &jets_JvfCorr3);
	chainSig->SetBranchAddress("jets_Rpt4", &jets_Rpt4);
	chainSig->SetBranchAddress("jets_JvfCorr4", &jets_JvfCorr4);
	chainSig->SetBranchAddress("dphi_yy_j1", &dphi_yy_j1);
	chainSig->SetBranchAddress("dphi_FjetV_met", &dphi_FjetV_met);
   // for(int i =1; i<=9; i++)
    //    chainSig->SetBranchAddress(Form("met_MVA%d",i), &BDTValues[i]);
	int sigVin=1;
	for(TString bdtclass: bdtclasses){
		chainSig->SetBranchAddress("met_MVA"+bdtclass,&BDTValues[sigVin]);
		sigVin++;
	}
    chainBkg->SetBranchAddress("dz",&dz);
    chainBkg->SetBranchAddress("metsig", &metSig);
    chainBkg->SetBranchAddress("weight", &weight);
    chainBkg->SetBranchAddress("diff_sumpt2", &diff_sumpt2);
    chainBkg->SetBranchAddress("m_yy", &myy);
    chainBkg->SetBranchAddress("phpt1", &pt1);
    chainBkg->SetBranchAddress("phpt2", &pt2);
    chainBkg->SetBranchAddress("N_j_central", &N_j_central);
	chainBkg->SetBranchAddress("jets_Rpt1", &jets_Rpt1);
	chainBkg->SetBranchAddress("jets_JvfCorr1", &jets_JvfCorr1);
	chainBkg->SetBranchAddress("jets_Rpt2", &jets_Rpt2);
	chainBkg->SetBranchAddress("jets_JvfCorr2", &jets_JvfCorr2);
	chainBkg->SetBranchAddress("jets_Rpt3", &jets_Rpt3);
	chainBkg->SetBranchAddress("jets_JvfCorr3", &jets_JvfCorr3);
	chainBkg->SetBranchAddress("jets_Rpt4", &jets_Rpt4);
	chainBkg->SetBranchAddress("jets_JvfCorr4", &jets_JvfCorr4);
	chainBkg->SetBranchAddress("dphi_yy_j1", &dphi_yy_j1);
	chainBkg->SetBranchAddress("dphi_FjetV_met", &dphi_FjetV_met);
    //for(int i =1; i<=9; i++)
    //    chainBkg->SetBranchAddress(Form("met_MVA%d",i), &BDTValues[i]);
	int bkgVin=1;
	for(TString bdtclass: bdtclasses){
		chainBkg->SetBranchAddress("met_MVA"+bdtclass,&BDTValues[bkgVin]);
		bkgVin++;
	}
    std::cout<<"Branches initialised..."<<std::endl;
    double nEvents = chainSig->GetEntries();

    //--------------------------------------//
    // Loop over the Trees:
    double sig_sumw = 0;
    double bkg_sumw = 0;

    double normSig = 363.81;
    //double normBkg = 31677.96;
    double normBkg = 41030.0;

    double norm_sig_sumw = 0.0;
    double norm_bkg_sumw = 0.0;

    std::cout << "There are "<< nEvents << " signal events to process." << std::endl;
    for (int index = 0; index < nEvents; index++)
    {
        chainSig->GetEntry(index);
        PrintProgressBar(index, nEvents);
        if(pt2/myy<0.25 || pt1/myy<0.35) continue;
        if(N_j_central<1)continue;
			
        sig_sumw+=weight;
        weight = weight*(normSig/29879.8);
        norm_sig_sumw+= weight;

			if(metSig>2.5 && metSig<3.5)
				{
					//Norminal selection
					nomSigYields["MetSig25NJ0"]+=weight;
					if(N_j_central==1)nomSigYields["MetSig25NJ1"]+=weight;
					if(N_j_central>=2)nomSigYields["MetSig25NJ2"]+=weight;
					//Norminal selection with SV method
					if(diff_sumpt2>0.0)nomSigYields["MetSig25VtxNJ0"]+=weight;
					if(N_j_central==1&&diff_sumpt2>0.0)nomSigYields["MetSig25VtxNJ1"]+=weight;
					if(N_j_central>=2&&diff_sumpt2>0.0)nomSigYields["MetSig25VtxNJ2"]+=weight;
					std::cout<<"check fill 1: "<<std::endl;
					histo["sigmetsig25noJ"]->Fill(BDTValues[1]);
					if(N_j_central==1)histo["sigmetsig25NJ1"]->Fill(BDTValues[2]);
					if(N_j_central>=2)histo["sigmetsig25NJ2"]->Fill(BDTValues[3]);
					std::cout<<"check fill 2: "<<std::endl;
					for(int cut =0; cut<20;cut++)
					{
						double cutVal = (cut-10)/10.0;
						if(BDTValues[1]>cutVal)
							sigYield["MetSig25NJ0"][1][cut] += weight;
						if(N_j_central==1&&BDTValues[2]>cutVal)
							sigYield["MetSig25NJ1"][2][cut] += weight;
						if(N_j_central>=2&&BDTValues[3]>cutVal)
							sigYield["MetSig25NJ2"][3][cut] += weight;
							
						
						
						if(BDTValues[1]>cutVal&&diff_sumpt2>0.0)
							sigYield["MetSig25VtxNJ0"][1][cut] += weight;
						if(N_j_central==1&&BDTValues[2]>cutVal&&diff_sumpt2>0.0)
							sigYield["MetSig25VtxNJ1"][2][cut] += weight;
						if(N_j_central>=2&&BDTValues[3]>cutVal&&diff_sumpt2>0.0)
							sigYield["MetSig25VtxNJ2"][3][cut] += weight;
						
					}
				}

			if(metSig>3.5 && metSig<5.5)
			{
				//Norminal selection
				nomSigYields["MetSig35NJ0"]+=weight;
				if(N_j_central==1)nomSigYields["MetSig35NJ1"]+=weight;
				if(N_j_central>=2)nomSigYields["MetSig35NJ2"]+=weight;
				//Norminal seletion with SV method
				if(diff_sumpt2>0.0)nomSigYields["MetSig35VtxNJ0"]+=weight;
				if(N_j_central==1&&diff_sumpt2>0.0)nomSigYields["MetSig35VtxNJ1"]+=weight;
				if(N_j_central>=2&&diff_sumpt2>0.0)nomSigYields["MetSig35VtxNJ2"]+=weight;
				
				histo["sigmetsig35noJ"]->Fill(BDTValues[4]);
				if(N_j_central==1)histo["sigmetsig35NJ1"]->Fill(BDTValues[5]);
				if(N_j_central>=2)histo["sigmetsig35NJ2"]->Fill(BDTValues[6]);
				
				for(int cut =0; cut<20;cut++)
				{
					double cutVal = (cut-10)/10.0;
					if(BDTValues[4]>cutVal)
						sigYield["MetSig35NJ0"][4][cut] += weight;
					if(N_j_central==1&&BDTValues[5]>cutVal)
						sigYield["MetSig35NJ1"][5][cut] += weight;
					if(N_j_central>=2&&BDTValues[6]>cutVal)
						sigYield["MetSig35NJ2"][6][cut] += weight;
					
					if(BDTValues[4]>cutVal&&diff_sumpt2>0.0)
						sigYield["MetSig35VtxNJ0"][4][cut] += weight;
					if(N_j_central==1&&BDTValues[5]>cutVal&&diff_sumpt2>0.0)
						sigYield["MetSig35VtxNJ1"][5][cut] += weight;
					if(N_j_central>=2&&BDTValues[6]>cutVal&&diff_sumpt2>0.0)
						sigYield["MetSig35VtxNJ2"][6][cut] += weight;
				}
			}
			if(metSig>5.5)
			{
				//Norminal selection
				nomSigYields["MetSig55NJ0"]+=weight;
				if(N_j_central==1)nomSigYields["MetSig55NJ1"]+=weight;
				if(N_j_central>=2)nomSigYields["MetSig55NJ2"]+=weight;
				//Norminal seletion with SV method
				if(diff_sumpt2>0.0)nomSigYields["MetSig55VtxNJ0"]+=weight;
				if(N_j_central==1&&diff_sumpt2>0.0)nomSigYields["MetSig55VtxNJ1"]+=weight;
				if(N_j_central>=2&&diff_sumpt2>0.0)nomSigYields["MetSig55VtxNJ2"]+=weight;
				
				histo["sigmetsig55noJ"]->Fill(BDTValues[7]);
				if(N_j_central==1)histo["sigmetsig55NJ1"]->Fill(BDTValues[8]);
				if(N_j_central>=2)histo["sigmetsig55NJ2"]->Fill(BDTValues[9]);
				
				for(int cut =0; cut<20;cut++)
				{
					double cutVal = (cut-10)/10.0;
					if(BDTValues[7]>cutVal)
						sigYield["MetSig55NJ0"][7][cut] += weight;
					if(N_j_central==1&&BDTValues[8]>cutVal)
						sigYield["MetSig55NJ1"][8][cut] += weight;
					if(N_j_central>=2&&BDTValues[9]>cutVal)
						sigYield["MetSig55NJ2"][9][cut] += weight;
						
					
					
					if(BDTValues[7]>cutVal&&diff_sumpt2>0.0)
						sigYield["MetSig55VtxNJ0"][7][cut] += weight;
					if(N_j_central==1&&BDTValues[8]>cutVal&&diff_sumpt2>0.0)
						sigYield["MetSig55VtxNJ1"][8][cut] += weight;
					if(N_j_central>=2&&BDTValues[9]>cutVal&&diff_sumpt2>0.0)
						sigYield["MetSig55VtxNJ2"][9][cut] += weight;
					
				}
				
			}

    }

    std::cout<<"Sum of signal weights: "<<sig_sumw<<std::endl;

    nEvents = chainBkg->GetEntries();
    std::cout << "There are "<< nEvents << " background events to process." << std::endl;
    for (int index = 0; index < nEvents; index++)
    {
        chainBkg->GetEntry(index);
        PrintProgressBar(index, nEvents);
			  if(pt2/myy<0.25 || pt1/myy<0.35) continue;
			  if(N_j_central<1)continue;
			
        bkg_sumw+= weight;
        weight = weight*(normBkg/255158);
        norm_bkg_sumw += weight;

			//std::cout<<"check25: "<<index<<std::endl;
			if(metSig>2.5 && metSig<3.5)
			{
				//Nominal selection
				nomBkgYields["MetSig25NJ0"]+=weight;
				if(N_j_central==1)nomBkgYields["MetSig25NJ1"]+=weight;
				if(N_j_central>=2)nomBkgYields["MetSig25NJ2"]+=weight;
				//Norminal seletion with SV method
				if(diff_sumpt2>0.0)nomBkgYields["MetSig25VtxNJ0"]+=weight;
				if(N_j_central==1&&diff_sumpt2>0.0)nomBkgYields["MetSig25VtxNJ1"]+=weight;
				if(N_j_central>=2&&diff_sumpt2>0.0)nomBkgYields["MetSig25VtxNJ2"]+=weight;
				
				
				histo["bkgmetsig25noJ"]->Fill(BDTValues[1]);
				if(N_j_central==1)histo["bkgmetsig25NJ1"]->Fill(BDTValues[2]);
				if(N_j_central>=2)histo["bkgmetsig25NJ2"]->Fill(BDTValues[3]);
				
				for(int cut =0; cut<20;cut++)
				{
					//std::cout<<"check25d: "<<index<<std::endl;
					double cutVal = (cut-10)/10.0;
					if(BDTValues[1]>cutVal)
						bkgYield["MetSig25NJ0"][1][cut] += weight;
					if(N_j_central==1&&BDTValues[2]>cutVal)
						bkgYield["MetSig25NJ1"][2][cut] += weight;
					if(N_j_central>=2&&BDTValues[3]>cutVal)
						bkgYield["MetSig25NJ2"][3][cut] += weight;
					//std::cout<<"check25e: "<<index<<std::endl;
					
					if(BDTValues[1]>cutVal&&diff_sumpt2>0.0)
						bkgYield["MetSig25VtxNJ0"][1][cut] += weight;
					if(N_j_central==1&&BDTValues[2]>cutVal&&diff_sumpt2>0.0)
						bkgYield["MetSig25VtxNJ1"][2][cut] += weight;
					if(N_j_central>=2&&BDTValues[3]>cutVal&&diff_sumpt2>0.0)
						bkgYield["MetSig25VtxNJ2"][3][cut] += weight;
				}
			}
			//std::cout<<"check35: "<<index<<std::endl;
			if(metSig>3.5 && metSig<5.5)
			{
				nomBkgYields["MetSig35NJ0"]+=weight;
				if(N_j_central==1)nomBkgYields["MetSig35NJ1"]+=weight;
				if(N_j_central>=2)nomBkgYields["MetSig35NJ2"]+=weight;
				//Norminal seletion with SV method
				if(diff_sumpt2>0.0)nomBkgYields["MetSig35VtxNJ0"]+=weight;
				if(N_j_central==1&&diff_sumpt2>0.0)nomBkgYields["MetSig35VtxNJ1"]+=weight;
				if(N_j_central>=2&&diff_sumpt2>0.0)nomBkgYields["MetSig35VtxNJ2"]+=weight;
				
				histo["bkgmetsig35noJ"]->Fill(BDTValues[4]);
				if(N_j_central==1)histo["bkgmetsig35NJ1"]->Fill(BDTValues[5]);
				if(N_j_central>=2)histo["bkgmetsig35NJ2"]->Fill(BDTValues[6]);
				
				
				for(int cut =0; cut<20;cut++)
				{
					//std::cout<<"check35d: "<<index<<std::endl;
					double cutVal = (cut-10)/10.0;
					if(BDTValues[4]>cutVal)
						bkgYield["MetSig35NJ0"][4][cut] += weight;
					if(N_j_central==1&&BDTValues[5]>cutVal)
						bkgYield["MetSig35NJ1"][5][cut] += weight;
					if(N_j_central>=2&&BDTValues[6]>cutVal)
						bkgYield["MetSig35NJ2"][6][cut] += weight;

					//std::cout<<"check35e: "<<index<<std::endl;
					
					if(BDTValues[4]>cutVal&&diff_sumpt2>0.0)
						bkgYield["MetSig35VtxNJ0"][4][cut] += weight;
					if(N_j_central==1&&BDTValues[5]>cutVal&&diff_sumpt2>0.0)
						bkgYield["MetSig35VtxNJ1"][5][cut] += weight;
					if(N_j_central>=2&&BDTValues[6]>cutVal&&diff_sumpt2>0.0)
						bkgYield["MetSig35VtxNJ2"][6][cut] += weight;
				}
			}
			//std::cout<<"check55: "<<index<<std::endl;
			if(metSig>5.5)
			{
				//Nominal selection
				nomBkgYields["MetSig55NJ0"]+=weight;
				if(N_j_central==1)nomBkgYields["MetSig55NJ1"]+=weight;
				if(N_j_central>=2)nomBkgYields["MetSig55NJ2"]+=weight;
				//Norminal seletion with SV method
				if(diff_sumpt2>0.0)nomBkgYields["MetSig55VtxNJ0"]+=weight;
				if(N_j_central==1&&diff_sumpt2>0.0)nomBkgYields["MetSig55VtxNJ1"]+=weight;
				if(N_j_central>=2&&diff_sumpt2>0.0)nomBkgYields["MetSig55VtxNJ2"]+=weight;
				
				histo["bkgmetsig55noJ"]->Fill(BDTValues[7]);
				if(N_j_central==1)histo["bkgmetsig55NJ1"]->Fill(BDTValues[8]);
				if(N_j_central>=2)histo["bkgmetsig55NJ2"]->Fill(BDTValues[9]);
				
				for(int cut =0; cut<20;cut++)
				{
					//std::cout<<"check55d: "<<index<<std::endl;
					double cutVal = (cut-10)/10.0;
					if(BDTValues[7]>cutVal)
						bkgYield["MetSig55NJ0"][7][cut] += weight;
					if(N_j_central==1&&BDTValues[8]>cutVal)
						bkgYield["MetSig55NJ1"][8][cut] += weight;
					if(N_j_central>=2&&BDTValues[9]>cutVal)
						bkgYield["MetSig55NJ2"][9][cut] += weight;

					//std::cout<<"check55c: "<<index<<std::endl;
					
					if(BDTValues[7]>cutVal&&diff_sumpt2>0.0)
						bkgYield["MetSig55VtxNJ0"][7][cut] += weight;
					if(N_j_central==1&&BDTValues[8]>cutVal&&diff_sumpt2>0.0)
						bkgYield["MetSig55VtxNJ1"][8][cut] += weight;
					if(N_j_central>=2&&BDTValues[9]>cutVal&&diff_sumpt2>0.0)
						bkgYield["MetSig55VtxNJ2"][9][cut] += weight;
				}
				//std::cout<<"check55e: "<<index<<std::endl;
			}
    }
	
	outfile->Write();
	outfile->Close();
    std::cout<<"Calculating significance from S and B:"<<std::endl;
	
	for(auto set: sets){
		for(auto jetset : jetsets)
				{
					//double sig = nomSigYields[set]/sqrt(nomSigYields[set]+nomBkgYields[set]);
					double sig = nomSigYields[set+jetset]/sqrt(nomBkgYields[set+jetset]);
					std::cout<<set+jetset<<" signal: "<<nomSigYields[set+jetset]<<" bkg: "<<nomBkgYields[set+jetset]<<" sigf = "<<sig<<std::endl;
					sigTotals[set+jetset].push_back(sig);
					
					
				}
    }
	std::cout<<"Calculating efficiency"<<std::endl;
	std::vector<TString> tmpsets = {"MetSig25","MetSig35","MetSig55"};
	std::map<TString, double> Xvals;
	std::map<TString, double> Yvals;
	Xvals.clear();
	Yvals.clear();
	
	for(auto set: tmpsets){
		for(auto jetset : jetsets)
		{
			//double sig = nomSigYields[set]/sqrt(nomSigYields[set]+nomBkgYields[set]);
			
			TString vtxname = Form("%sVtx%s",set.Data(),jetset.Data());
			//std::cout<<vtxname<<std::endl;
			
			double effs = nomSigYields[vtxname]/nomSigYields[set+jetset];
			std::cout<<"Signal: "<<vtxname<<"\\"<<set+jetset<<": "<<effs<<std::endl;
			
			double effb = nomBkgYields[vtxname]/nomBkgYields[set+jetset];
			std::cout<<"Bkg: "<<vtxname<<"\\"<<set+jetset<<": "<<effb<<std::endl;
			Xvals[vtxname]=effs;
			Yvals[vtxname]=1-effb;
		}
	}
	//updated function to make efficiency plots...
	makeEffRoc("metSig25",Xvals,Yvals);
	makeEffRoc("metSig35",Xvals,Yvals);
	makeEffRoc("metSig55",Xvals,Yvals);


    Double_t xVals[100];
    Double_t yVals[100];
    std::vector<Color_t> colors{kBlue+1,kRed+1,kGreen+2,kTeal-1,kOrange-3,kViolet};
	  //std::vector<Color_t> colors{kBlue+1,kRed+1,kGreen+2,kTeal-1};

    TString pdfName = Form("Njet2NewFSOptimise_vyy.pdf");
    TCanvas *can = new TCanvas("c1","Can",0.,0.,800,600);
    can->Print(pdfName+"[");

	for(auto set : sets){
			for(auto jetset : jetsets)
			{
        TLegend* legend = new TLegend(0.2,0.9, 0.45, 0.8);
        legend->SetNColumns(2);
        legend->SetTextFont(42);
        legend->SetTextSize(0.03);
        legend->SetBorderSize(0);
        legend->SetFillStyle(0);
        legend->SetLineColor(0);
        std::vector<TGraph*> tgraphs;
        std::vector<double> bdtMax;
        double graphMax = 0;
        std::cout<<"check1...."<<set+jetset<<std::endl;
        for(int bdtNum = 1; bdtNum <= 9; bdtNum++)
        {
            double xofYmax = -10;
            double yMax = 0;
            std::cout<<"Values for "<<set+jetset<<std::endl;
            for(int index = 0; index < 20; index++)
            {
                double cutVal = (index-10)/10.0;
                xVals[index] = cutVal;
                double irrBkg = 35.51;
							  if(set.Contains("35")) irrBkg = 22.51;
                if(set.Contains("55")) irrBkg = 9.92;
							

                //yVals[index] = sigYield[set][bdtNum][index] / TMath::Sqrt(sigYield[set][bdtNum][index] + bkgYield[set][bdtNum][index]+irrBkg);
							yVals[index] = sigYield[set+jetset][bdtNum][index] / TMath::Sqrt(bkgYield[set+jetset][bdtNum][index]+irrBkg);
              //  std::cout<<"BDTCut="<<cutVal<<" S="<<sigYield[set][bdtNum][index]<<" [] B="<<bkgYield[set][bdtNum][index]<<" S/sqrt(S+B)="<<yVals[index]<<std::endl;
                TString log = Form("BDTCut=%0.2f S=%0.2f [%0.2f%%] B=%0.2f [%0.2f%%] S/sqrt(S+B)=%0.2f",cutVal,sigYield[set+jetset][bdtNum][index],sigYield[set+jetset][bdtNum][index]/norm_sig_sumw*100,bkgYield[set+jetset][bdtNum][index],bkgYield[set+jetset][bdtNum][index]/norm_bkg_sumw*100,yVals[index]);
                std::cout<<log<<std::endl;
                if(yVals[index] > yMax)
                {
                    yMax = yVals[index];
                    xofYmax = cutVal;
                    if(yMax > graphMax)
                        graphMax = yMax;
                }
            }
            std::cout<<"check2...."<<set+jetset<<std::endl;
            sigTotals[set+jetset].push_back(yMax);
            TGraph* gr = new TGraph(20,xVals,yVals);
            gr->SetLineColor(colors[bdtNum]);
            gr->SetLineWidth(2);
            gr->GetYaxis()->SetTitle("Significance #frac{s}{#sqrt{s+b}}");
            gr->GetXaxis()->SetTitle("BDT Cut Value");
            gr->SetTitle(Form("BDT%d",bdtNum));
//            gr->Draw("AC same");
            legend->AddEntry(gr,gr->GetTitle(),"L");
            tgraphs.push_back(gr);
            bdtMax.push_back(xofYmax);
        }
        TLatex l;
        l.SetNDC();
        l.SetTextColor(kBlack);
        l.SetTextAlign(11);
        l.SetTextFont(42);
        l.SetTextSize(0.03);
        std::cout<<"check3...."<<set+jetset<<std::endl;
        std::cout<<"check size: sets - "<<sets.size()<<" tgraphs - "<<tgraphs.size()<<std::endl;
        for(int i = 0; i< tgraphs.size(); i++)
        {
            tgraphs[i]->SetMaximum(graphMax*1.2);
            if( i ==0 )
            tgraphs[i]->Draw("AC");
            else
            tgraphs[i]->Draw("C");
        }

        legend->Draw();
        std::cout<<"check4...."<<set+jetset<<std::endl;
        l.DrawLatex(0.2,0.75, setDisplayName[set+jetset]);
        l.DrawLatex(0.2,0.70, "Maximum at:");
        for(int i = 0; i<= sets.size(); i++)
        {
            l.DrawLatex(0.2,0.70-(i+1)*0.05, Form("BDT%d = %0.2f",i+1,bdtMax[i]));
        }
       std::cout<<"check5...."<<set+jetset<<std::endl;
        can->Print(pdfName);
        can->Clear();
    }
}
    TH2F* hist2D = new TH2F("Name","name2",BDTCount+1,0,BDTCount+1,sets.size()*jetsets.size(),0,sets.size()*jetsets.size());
    std::cout<<"Creating 2D hist..."<<std::endl;
    int yBin = 1;
    for(auto set : sets)
    {
			for(auto jetset : jetsets){
        TString label = setDisplayName[set+jetset];
        //label.ReplaceAll("metSig > 3.5, SV","#splitline{metSig > 3.5}{SV}");
        //label.ReplaceAll("metSig > 5.5, SV","#splitline{metSig > 5.5}{SV}");
        hist2D->GetYaxis()->SetBinLabel(yBin,label);
        yBin++;
			}
    }
    for(int xBin = 1; xBin <= BDTCount+1; xBin++)
    {
        TString label = Form("BDT%d",xBin-1);
        if(xBin == 1) label="No BDT";
        hist2D->GetXaxis()->SetBinLabel(xBin,label);
    }



    for(int xbin = 1; xbin <= BDTCount+1; xbin++)
    {
        int ybin = 1;
        for(auto set : sets)
        {
					for(auto jetset : jetsets){
            TString category = "MetSig25Vtx";
            if(set.Contains("35")) category = "MetSig35Vtx";
						if(set.Contains("55")) category = "MetSig55Vtx";
            double perc = (sigTotals[set+jetset][xbin-1]-sigTotals[category][0])/sigTotals[category][0]*100;
						std::cout<<set+jetset<<" cat1: "<<sigTotals[category][0]<<" xbin"<<sigTotals[set+jetset][xbin-1]<<" per "<<perc<<std::endl;
            hist2D->SetBinContent(xbin,ybin,perc);
            ybin++;
					}
        }
    }
    hist2D->SetEntries(1);
    hist2D->SetMaximum(180);
    hist2D->SetMinimum(0);

    can->SetLeftMargin(0.15);
    can->SetRightMargin(0.15);

    gStyle->SetPaintTextFormat("4.2f %%");
    gStyle->SetHistMinimumZero();
    //gStyle->SetPalette(kInvertedDarkBodyRadiator);
    gStyle->SetPalette(kBird);
    //hist2D->Draw("colz0 text");
	  hist2D->Draw("COLZ0 text");

    TLine *line = new TLine();
    line->SetLineStyle(1);
    line->SetLineWidth(2);
    line->SetLineColor(kBlack);
    line->DrawLine(0.0, 2.0, 6.0, 2.0);

    line->DrawLine(1.0, 0.0, 1.0, 4.0);

    TLatex l;
    l.SetTextColor(kBlack);
    l.SetTextAlign(23);
    l.SetTextSize(0.025);
	std::cout<<std::endl;
    for(double x = 0.5; x<= BDTCount+0.5; x++)
    for(double y = 1.4; y<= sets.size()-0.6; y+=2)
    {
        int setNum = y;
        int xbin = x-0.5;
        TString set = sets[setNum];
        double perc = (sigTotals[set][xbin]-sigTotals[set][0])/sigTotals[set][0]*100;
        double perc2 = hist2D->GetBinContent(hist2D->FindBin(x,y))/hist2D->GetBinContent(hist2D->FindBin(0.5,y))*100;
        //l.DrawLatex(x,y, Form("(%0.2f %%)",perc));
			std::cout<<set<<" cat1: "<<sigTotals[set][0]<<" xbin"<<sigTotals[set][xbin]<<" per "<<perc<<std::endl;
    }


    can->Print(pdfName);
    can->Print(pdfName+"]");
/*
    //---------------------------------------------------------------------------------------
    // Data Improvements

    pdfName = Form("Data-Optimise.pdf");
    can = new TCanvas("c1","Can",0.,0.,800,600);
    can->Print(pdfName+"[");

    TString dataFitsDir = "AllFits";
    //sets = {"MetSig4","MetSig4Vtx","MetSig4Vtx2J","MetSig4","MetSig4Vtx","MetSig4Vtx2J","MetSig55","MetSig55Vtx","MetSig55Vtx2J"};
    sets = {"MetSig35","MetSig35Vtx","MetSig35Vtx2J","MetSig55","MetSig55Vtx","MetSig55Vtx2J"};
    
    setDisplayName["MetSig55"]  = "metSig > 5.5";
    setDisplayName["MetSig55Vtx"]  = "metSig > 5.5, SV";
    setDisplayName["MetSig55Vtx2J"] = "metSig > 5.5, SV, N_{j} #geq 2";
    setDisplayName["MetSig35"]      = "metSig > 3.5";
    setDisplayName["MetSig35Vtx"]   = "metSig > 3.5, SV";
    setDisplayName["MetSig35Vtx2J"] = "metSig > 3.5, SV, N_{j} #geq 2";

    std::map<TString, double> setNom;
    for(auto set : sets)
    {
        sigTotals[set].clear();
        TLegend* legend = new TLegend(0.2,0.9, 0.45, 0.8);
        legend->SetNColumns(2);
        legend->SetTextFont(42);
        legend->SetTextSize(0.03);
        legend->SetBorderSize(0);
        legend->SetFillStyle(0);
        legend->SetLineColor(0);
        std::vector<TGraph*> tgraphs;
        std::vector<double> bdtMax;
        double graphMax = 0;
        double graphMin = 0;

        for(int bdtNum = 1; bdtNum <= BDTCount; bdtNum++)
        {
            TString sigTextFile = Form("%s/%s/%s_MVA%d/BDTCutSig.txt",dataFitsDir.Data(),set.Data(),set.Data(),bdtNum);

            if(bdtNum == 1)
            {
                std::string line;
                double tempSig;
                std::ifstream ifs(sigTextFile.Data());

                std::getline(ifs, line, ' ') >> tempSig;
                setNom[set]=tempSig;
            }

            TGraph* gr = new TGraph(sigTextFile);
            gr->SetLineColor(colors[bdtNum]);
            gr->SetLineWidth(2);
            gr->GetYaxis()->SetTitle("Significance");
            gr->GetXaxis()->SetTitle("BDT Cut Value");
            gr->SetTitle(Form("BDT%d",bdtNum));
//            gr->Draw("AC same");

            legend->AddEntry(gr,gr->GetTitle(),"L");

            double yMax = -100;
            double xMax = 0;
            double yMin = 999;
            for(int i = 0; i < gr->GetN(); i++)
            {
                if(yMax < gr->GetY()[i] )
                {
                    xMax = gr->GetX()[i];
                    yMax = gr->GetY()[i];
                }
                if(yMin > gr->GetY()[i])
                yMin = gr->GetY()[i];
            }

            if(yMax > graphMax)
                graphMax = yMax;
            if(graphMin > yMin)
                graphMin=yMin;

            sigTotals[set].push_back(yMax);

            tgraphs.push_back(gr);
            bdtMax.push_back(xMax);
        }
        TLatex l;
        l.SetNDC();
        l.SetTextColor(kBlack);
        l.SetTextAlign(11);
        l.SetTextFont(42);
        l.SetTextSize(0.03);

        for(int i = 0; i < tgraphs.size(); i++)
        {
        //std::cout<<graphMax<<std::endl;
        tgraphs[i]->SetMaximum(graphMax*1.2);
        tgraphs[i]->SetMinimum(graphMin*1.1);
//        TGraph gr;
//        gr.GetXaxis()->SetRangeUser(-1.5,1.0);
//        gr.GetHistogram()->GetXaxis()->SetLimits(-2.0,1.0);
        tgraphs[i]->GetXaxis()->SetLimits(-1.7,1.0);
        }

        for(int i = 0; i < tgraphs.size(); i++)
        {
            if( i ==0 )
            tgraphs[i]->Draw("AC");
            else
            tgraphs[i]->Draw("C");
        }

        legend->Draw();

        l.DrawLatex(0.2,0.75, setDisplayName[set]);
        l.DrawLatex(0.2,0.70, "Maximum at:");
        for(int i = 0; i< BDTCount; i++)
        {
        l.DrawLatex(0.2,0.70-(i+1)*0.05, Form("BDT%d = %0.2f",i+1,bdtMax[i]));
        }

        can->Print(pdfName);
        can->Clear();
    }

    hist2D = new TH2F("Name","name2",BDTCount+1,0,BDTCount+1,sets.size(),0,sets.size());
    yBin = 1;
    for(auto set : sets)
    {
        TString label = setDisplayName[set];
//        label.ReplaceAll("metSig > 3.5, SV","#splitline{metSig > 3.5}{SV}");
//        label.ReplaceAll("metSig > 4.0, SV","#splitline{metSig > 4.0}{SV}");
//        label.ReplaceAll("metSig > 5.5, SV","#splitline{metSig > 5.5}{SV}");
//        label.ReplaceAll("metSig > 3.5, SV, N_{j} #geq 2","#splitline{metSig > 3.5}{SV, N_{j} #geq 2}");
//        label.ReplaceAll("metSig > 4.0, SV, N_{j} #geq 2","#splitline{metSig > 4.0}{SV, N_{j} #geq 2}");
//        label.ReplaceAll("metSig > 5.5, SV, N_{j} #geq 2","#splitline{metSig > 5.5}{SV, N_{j} #geq 2}");

        hist2D->GetYaxis()->SetBinLabel(yBin,label);
        hist2D->GetYaxis()->SetLabelSize(0.04);
        yBin++;
    }
    for(int xBin = 1; xBin <= BDTCount+1; xBin++)
    {
        TString label = Form("BDT%d",xBin-1);
        if(xBin == 1) label="No BDT";
        hist2D->GetXaxis()->SetBinLabel(xBin,label);
    }



    for(int xbin = 1; xbin <= BDTCount+1; xbin++)
    {
        int ybin = 1;
        for(auto set : sets)
        {
            double sig = 1.0;
            if(xbin==1)
            {
                sig = setNom[set];
            }
            else
            {
                sig = sigTotals[set][xbin-2];
            }
            hist2D->SetBinContent(xbin,ybin,sig);
            ybin++;
        }
    }
    hist2D->SetEntries(1);
    hist2D->SetMaximum(6);
    hist2D->SetMinimum(-1);

    can->SetLeftMargin(0.22);
    can->SetRightMargin(0.15);
    can->SetTopMargin(0.15);

    gStyle->SetPaintTextFormat("4.3f");
    gStyle->SetHistMinimumZero();
    gStyle->SetPalette(kInvertedDarkBodyRadiator);

    hist2D->Draw("colz0 text");

    line = new TLine();
    line->SetLineStyle(1);
    line->SetLineWidth(2);
    line->SetLineColor(kBlack);
    //line->DrawLine(0.0, 1.0, 6.0, 1.0);

    line->DrawLine(0.0, 3.0, 6.0, 3.0);
    line->DrawLine(0.0, 6.0, 6.0, 6.0);

    line->DrawLine(1.0, 0.0, 1.0, sets.size());

    l.SetTextColor(kBlack);
    l.SetTextAlign(23);
    l.SetTextSize(0.022);

    for(double x = 1.5; x<= BDTCount+0.5; x++)
    for(double y = 0.3; y<= sets.size()-0.7; y+=1)
    {
        int setNum = y;
        int xbin = x-0.5;
        TString set = sets[setNum];

        double perc = (sigTotals[set][xbin-1]-setNom[set])/TMath::Abs(setNom[set])*100;
        //double perc2 = hist2D->GetBinContent(hist2D->FindBin(x,y))/hist2D->GetBinContent(hist2D->FindBin(0.5,y))*100;
        l.DrawLatex(x,y, Form("(%0.2f %%)",perc));
    }

    TLatex ltitle;
    ltitle.SetNDC();
    ltitle.SetTextColor(kBlack);
    ltitle.SetTextAlign(22);
    ltitle.SetTextFont(42);
    ltitle.SetTextSize(0.06);
    ltitle.DrawLatex(0.5,0.93,"Significance (Perc. Change)");

    can->Print(pdfName);
    can->Print(pdfName+"]");
*/
    return 1;
}

void PrintProgressBar(int index, int total)
{
    if (index%100 == 0)
    {
        TString print_bar = " [";
        for (int bar = 0; bar < 20; bar++)
        {
            double current_fraction = double(bar) / 20.0;
            if (double(index)/double(total) > current_fraction) print_bar.Append("/");
            else print_bar.Append(".");
        }
        print_bar.Append("] ");
        double percent = 100.0 * (double(index) / double(total));
        TString text = Form("%s %2.2f ", print_bar.Data(), percent);
        std::cout << text << "%\r" << std::flush;
    }
}
TGraph* getNoImp(TH1* hist){
	int n =hist->GetNbinsX();
	double x[n], y[n];
	for(int bin=0; bin < hist->GetNbinsX(); bin++)
	{
		x[bin] = hist->GetXaxis()->GetBinCenter(bin+1);
		y[bin] = 1-(x[bin]*x[bin]);
		//std::cout<<"x["<<bin<<"]: "<<x[bin]<<std::endl;
		//std::cout<<"y["<<bin<<"]: "<<y[bin]<<std::endl;
	}
	TGraph* gr = new TGraph(n,x,y);
	return gr;
}
void makeEffRoc(TString bdtclass,std::map<TString, double> &Xvals,std::map<TString, double> &Yvals){
	TCanvas *can = new TCanvas();
	can->SetTopMargin(0.04); can->SetRightMargin(0.04);
	TString baseDir = "TrainingInfo/";
	TString fileName = Form("Njet_275mx60Sherpayy"+bdtclass+"NJ0.root");
	TString fileDir = baseDir+fileName;
	TFile *file1= new TFile(fileDir);
	TH1 *hist1=(TH1*)file1->Get("Method_BDT/BDTG1/MVA_BDTG1_rejBvsS");
	hist1->SetMarkerSize(0);
	hist1->SetLineColor(kBlue+1);
	hist1->GetYaxis()->SetTitle("Bkg rejection (1-eff)");
	hist1->GetXaxis()->SetTitle("Signal eff");
	hist1->SetMaximum(1.5);
	hist1->SetMinimum(0.0005);
	hist1->Draw();
	
	fileName = Form("Njet_275mx60Sherpayy"+bdtclass+"NJ1.root");
	fileDir = baseDir+fileName;
	TFile *file2=new TFile(fileDir);
	TH1 *hist2=(TH1*)file2->Get("Method_BDT/BDTG1/MVA_BDTG1_rejBvsS");
	hist2->SetMarkerSize(0);
	hist2->SetLineColor(kRed+1);
	hist2->Draw("same");
	fileName = Form("Njet_275mx60Sherpayy"+bdtclass+"NJ2.root");
	fileDir = baseDir+fileName;
	TFile *file3=new TFile(fileDir);
	TH1 *hist3=(TH1*)file3->Get("Method_BDT/BDTG1/MVA_BDTG1_rejBvsS");
	hist3->SetMarkerSize(0);
	hist3->SetLineColor(kGreen+1);
	hist3->Draw("same");
	
	TString names = "MetSig25VtxNJ";
	if(bdtclass=="metSig35")names="MetSig35VtxNJ";
	if(bdtclass=="metSig55")names="MetSig55VtxNJ";
	TGraph *curr1 = new TGraph();
	curr1->SetPoint(0,Xvals[names+"0"],Yvals[names+"0"]);
	std::cout<<Xvals[names+"0"]<<" "<<Yvals[names+"0"]<<std::endl;
	curr1->SetMarkerColor(kBlue+1);
	curr1->SetLineColor(kBlue+1);
	curr1->SetLineWidth(2);
	curr1->SetMarkerSize(2);
	curr1->Draw("same P");
	
	TGraph *curr2 = new TGraph();
	curr2->SetPoint(0,Xvals[names+"1"],Yvals[names+"1"]);
	std::cout<<Xvals[names+"1"]<<" "<<Yvals[names+"1"]<<std::endl;
	curr2->SetMarkerColor(kRed+1);
	curr2->SetLineColor(kRed+1);
	curr2->SetLineWidth(2);
	curr2->SetMarkerSize(2);
	curr2->Draw("same P");
	
	TGraph *curr3 = new TGraph();
	curr3->SetPoint(0,Xvals[names+"2"],Yvals[names+"2"]);
	std::cout<<Xvals[names+"2"]<<" "<<Yvals[names+"2"]<<std::endl;
	curr3->SetMarkerColor(kGreen+1);
	curr3->SetLineColor(kGreen+1);
	curr3->SetLineWidth(2);
	curr3->SetMarkerSize(2);
	curr3->Draw("same P");
	
	
	
	
	
	TGraph* gr35 = getNoImp(hist1);
	gr35->SetLineColor(1);
	gr35->SetLineWidth(2);
	gr35->Draw("same C");
	TLegend* legend = new TLegend(0.25,0.7,0.63,0.9);
	legend->SetNColumns(2);
	legend->SetTextFont(42);
	legend->SetTextSize(0.03);
	legend->SetBorderSize(0);
	legend->SetFillStyle(0);
	legend->SetLineColor(0);
	legend->AddEntry(hist1,bdtclass+"BDT 0", "lep");
	legend->AddEntry(hist2,bdtclass+"BDT 1", "L");
	legend->AddEntry(hist3,bdtclass+"BDT 2", "L");
	legend->AddEntry(gr35,"No Improvement", "L");
	legend->Draw("same");
	can->SetGridx(true);
	can->SetGridy(true);
	can->Print("Plots/"+bdtclass+".pdf");
	

}


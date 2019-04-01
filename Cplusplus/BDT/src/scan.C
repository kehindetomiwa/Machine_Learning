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
void makeEffRoc(TString bdtclass,std::map<TString, double> &Xvals,std::map<TString, double> &Yvals,TString labels);
TGraph* getNoImp(TH1* hist);
void LoopFills(TString file, std::map<TString,double> &normYields, TString kind);
TString getJet(TString jetcat){
	TString ret = "";
	if(jetcat=="NJgt1")return "1";
	if(jetcat=="NJeq1")return "2";
	if(jetcat=="NJgt2")return "3";
}
TString getmetlabel(TString metcat){
	if(metcat=="MetSig25")return "Low";
	if(metcat=="MetSig35")return "Int.";
	if(metcat=="MetSig55")return "High";
	if(metcat=="MetSig25Vtx")return "Low Vxt";
	if(metcat=="MetSig35Vtx")return "Int Vxt";
	if(metcat=="MetSig55Vtx")return "High Vxt";
	
	
}
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
	  plotter->setAtlasLabel("ATLAS #it{work in progress}");
	  TString baseDir = "MVA/Testout/";
	  TString signalfile = baseDir+"out_mr275mx60_jpt20.root";
	  //TString bkgfile = baseDir+"out_SherpaFast_sim_jpt20.root";
	  TString bkgfile = baseDir+"out_background.root";
		std::vector<TString> bdtclasses = {"metsig25noJ","metsig25NJ1","metsig25NJ2","metsig35noJ","metsig35NJ1","metsig35NJ2","metsig55noJ","metsig55NJ1","metsig55NJ2"};
	std::vector<TString> sets = {"MetSig25","MetSig35","MetSig55","MetSig25Vtx","MetSig35Vtx","MetSig55Vtx"};
	std::vector<TString> jetsets = {"NJgt1","NJeq1","NJgt2"};
	std::map<TString,double> nomBkGYields;nomBkGYields.clear();
	std::map<TString,double> nomSiGYields;nomSiGYields.clear();
	std::map<TString,double> sigTotals;sigTotals.clear();
	for(TString set: sets){
		for(TString jetset: jetsets){
			nomSiGYields[set+jetset]=0.0;
			nomBkGYields[set+jetset]=0.0;
			for(int cut =0; cut<20;cut++){
				TString cutname = Form("cut%d",cut);
				nomSiGYields[set+jetset+cutname]=0.0;
				nomBkGYields[set+jetset+cutname]=0.0;
			}
		}
	}
	
	
	LoopFills(signalfile,nomSiGYields,"signal");
	LoopFills(bkgfile,nomBkGYields,"background");
/*	for(std::map<TString,double>::iterator c = nomSiGYields.begin(); c!=nomSiGYields.end(); ++c)
	{
		std::cout<<"signal: "<<(*c).first<<"  :  "<<(*c).second<<std::endl;
	}
	for(std::map<TString,double>::iterator c = nomBkGYields.begin(); c!=nomBkGYields.end(); ++c)
	{
		std::cout<<"background: "<<(*c).first<<"  :  "<<(*c).second<<std::endl;
	}
*/

	
	std::cout<<"Calculating significance from S and B:"<<std::endl;
	for(TString set: sets){
		for(TString jetset: jetsets){
			double sig = nomSiGYields[set+jetset]/sqrt(nomBkGYields[set+jetset]);
			sigTotals[set+jetset]=sig;
			std::cout<<set+jetset<<" signal: "<<nomSiGYields[set+jetset]<<" bkg: "<<nomBkGYields[set+jetset]<<" sigf = "<<sigTotals[set+jetset]<<std::endl;
			
			/*
			for(int cut =0; cut<20;cut++){
				TString cutname = Form("cut%d",cut);
				double irrBkg = 22.51;
				if(set.Contains("55")) irrBkg = 9.92;
				double sigbdt = nomSiGYields[set+jetset+cutname]/sqrt(nomBkGYields[set+jetset+cutname]+irrBkg);
				std::cout<<set+jetset+cutname<<" signal: "<<nomSiGYields[set+jetset+cutname]<<" bkg: "<<nomBkGYields[set+jetset+cutname]<<" sigf = "<<sigbdt<<std::endl;
				
			}*/
			
			
		}
	}
	std::vector<TString> tmpsets = {"MetSig25","MetSig35","MetSig55"};
	std::map<TString, double> Xvals;
	std::map<TString, double> Yvals;
	Xvals.clear();
	Yvals.clear();
	std::cout<<"Calculating efficiency:"<<std::endl;
	for(TString set: tmpsets){
		for(TString jetset : jetsets)
		{
			//double sig = nomSigYields[set]/sqrt(nomSigYields[set]+nomBkgYields[set]);
			
			TString vtxname = Form("%sVtx%s",set.Data(),jetset.Data());
			//std::cout<<vtxname<<std::endl;
			
			double effs = nomSiGYields[vtxname]/nomSiGYields[set+jetset];
			std::cout<<"Signal: "<<vtxname<<"\\"<<set+jetset<<": "<<effs<<std::endl;
			
			double effb = nomBkGYields[vtxname]/nomBkGYields[set+jetset];
			std::cout<<"Bkg: "<<vtxname<<"\\"<<set+jetset<<": "<<effb<<std::endl;
			Xvals[vtxname]=effs;
			Yvals[vtxname]=1-effb;
		}
	}
	//updated function to make efficiency plots...
	makeEffRoc("metsig25",Xvals,Yvals,"Low");
	makeEffRoc("metsig35",Xvals,Yvals,"Int.");
	makeEffRoc("metsig55",Xvals,Yvals,"High");
	
	Double_t xVals[100];
	Double_t yVals[100];
	std::vector<Color_t> colors{kBlue+2,kRed+1,kGreen+2,kTeal,kOrange-3,kViolet,kYellow-1,kBlack,kBlue-8,kGray};
	
	TString plotout = Form("MVA/Plots/");
	TCanvas *can = new TCanvas("c1","Can",0.,0.,800,600);
	
	std::vector<TString> vtxs = {"","Vtx"};
	for(TString vtx : vtxs){
		TCanvas *can = new TCanvas("c1","Can",0.,0.,800,600);
		TLegend* legend = new TLegend(0.55,0.70, 0.95, 0.92);
		legend->SetHeader("ATLAS #it{work in progress}");
		legend->SetNColumns(2);
		legend->SetTextFont(42);
		legend->SetTextSize(0.03);
		legend->SetBorderSize(0);
		legend->SetFillStyle(0);
		legend->SetLineColor(0);
		std::vector<TGraph*> tgraphs;
		std::vector<double> bdtMax;
		double graphMax = 0;
		int color = 0;
		for(TString set: tmpsets){
			for(TString jetset : jetsets)
			{
				
				double xofYmax = -10;
				double yMax = 0;
				
				for(int cut =0; cut<20;cut++){
					TString cutname = Form("cut%d",cut);
					//double irrBkg = 22.51;
					//if(set.Contains("55")) irrBkg = 9.92;
					
					double sigbdt = 0.0;
					if(nomSiGYields[set+vtx+jetset+cutname]==0.0 || nomBkGYields[set+vtx+jetset+cutname]==0.0)
						sigbdt =0.0;
					else
					  sigbdt = nomSiGYields[set+vtx+jetset+cutname]/sqrt(nomBkGYields[set+vtx+jetset+cutname]);
					
					std::cout<<set+vtx+jetset+cutname<<" signal: "<<nomSiGYields[set+vtx+jetset+cutname]<<" bkg: "<<nomBkGYields[set+vtx+jetset+cutname]<<" sigfbdt = "<<sigbdt<<std::endl;
					double cutVal = (cut-10)/10.0;
					xVals[cut] = cutVal;
					yVals[cut] = sigbdt;
					if(yVals[cut] > yMax)
					{
						yMax = yVals[cut];
						xofYmax = cutVal;
						if(yMax > graphMax)
							graphMax = yMax;
					}
					
				}//end of cut
				sigTotals[set+vtx+jetset+"max"] = yMax;
				//sigTotals[set+jetset].push_back(yMax);
				TGraph* gr = new TGraph(20,xVals,yVals);
				//gr->SetLineColor(colors[color]);
				gr->SetLineColor(colors[color]);
				gr->SetLineWidth(2);
				gr->GetYaxis()->SetTitle("Significance #frac{s}{#sqrt{b}}");
				gr->GetXaxis()->SetTitle("BDT Cut Value");
				TString setnew = "Low";
				if(set.Contains("35")) setnew = "Int.";
				if(set.Contains("55")) setnew = "High";
				gr->SetTitle(setnew+" S_{E^{miss}_{T}} "+getJet(jetset)+vtx);
				
				legend->AddEntry(gr,gr->GetTitle(),"L");
				tgraphs.push_back(gr);
				bdtMax.push_back(xofYmax);
				color++;
			}
		}//end of sets loop
		
		TLatex l;
		l.SetNDC();
		l.SetTextColor(kBlack);
		l.SetTextAlign(11);
		l.SetTextFont(42);
		l.SetTextSize(0.03);
		std::cout<<"check size: "<<tgraphs.size()<<std::endl;
		for(int i = 0; i< tgraphs.size(); i++)
		{
			tgraphs[i]->SetMaximum(graphMax*1.5);
			if( i ==0 )
				tgraphs[i]->Draw("AC");
			else
				tgraphs[i]->Draw("C");
		}
		legend->Draw();
		//l.DrawLatex(0.2,0.70, "Maximum at:");
		int kid=0;
		for(TString set: tmpsets){
			for(TString jetset : jetsets)
			{
				TString label = set+vtx+jetset;
				//l.DrawLatex(0.2,0.70-(kid+1)*0.05, Form("%s = %0.2f",label.Data(),bdtMax[kid]));
				kid++;
			}
		}
		can->SaveAs(plotout+"out_met"+vtx+"New.pdf");
		
	}//end of Vxt loop
	
	
	 //for(std::map<TString,double>::iterator c = sigTotals.begin(); c!=sigTotals.end(); ++c)
	// {
	// std::cout<<"total: "<<(*c).first<<"  :  "<<(*c).second<<std::endl;
	// }
	
	std::cout<<"Calculating significance improvement:"<<std::endl;
	for(TString set: sets){
		for(TString jetset: jetsets){
			double perc = ((sigTotals[set+jetset+"max"] - sigTotals[set+jetset])/sigTotals[set+jetset])*100;
		//	std::cout<<set+jetset<<"  "<<sigTotals[set+jetset]<<" "<<set+jetset+"BDT_max"<<" "<<sigTotals[set+jetset+"max"]<<" perc: "<<perc<<std::endl;
			//std::cout<<set+jetset<<"&"<<sigTotals[set+jetset]<<"&"<<sigTotals[set+jetset+"max"]<<" gain: "<<(sigTotals[set+jetset+"max"]-sigTotals[set+jetset])/sigTotals[set+jetset]<<"\\\\"<<std::endl;
			TString n_label = getmetlabel(set)+" $S_{E^{miss}_{T}}$ " +getJet(jetset);
			TString texout = Form("%s&%0.2f&%0.2f&%0.2f\\\\",n_label.Data(),sigTotals[set+jetset],sigTotals[set+jetset+"max"],perc);
			std::cout<<texout<<std::endl;
		}
	}
	
	
	
	 
    return 1;
}


void LoopFills(TString file, std::map<TString,double> &normYields, TString kind){
	TChain *treechain = new TChain("tree");
	treechain->AddFile(file);
	float metSig = 0.0;
	std::vector<float> BDTValues;
	std::vector<TString> bdtclasses = {"metsig25noJ","metsig25NJ1","metsig25NJ2","metsig35noJ","metsig35NJ1","metsig35NJ2","metsig55noJ","metsig55NJ1","metsig55NJ2"};
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
	treechain->SetBranchAddress("dz",&dz);
	treechain->SetBranchAddress("metsig", &metSig);
	treechain->SetBranchAddress("weight", &weight);
	treechain->SetBranchAddress("diff_sumpt2", &diff_sumpt2);
	treechain->SetBranchAddress("m_yy", &myy);
	treechain->SetBranchAddress("phpt1", &pt1);
	treechain->SetBranchAddress("phpt2", &pt2);
	treechain->SetBranchAddress("N_j_central", &N_j_central);
	
	treechain->SetBranchAddress("jets_Rpt1", &jets_Rpt1);
	treechain->SetBranchAddress("jets_JvfCorr1", &jets_JvfCorr1);
	treechain->SetBranchAddress("jets_Rpt2", &jets_Rpt2);
	treechain->SetBranchAddress("jets_JvfCorr2", &jets_JvfCorr2);
	treechain->SetBranchAddress("jets_Rpt3", &jets_Rpt3);
	treechain->SetBranchAddress("jets_JvfCorr3", &jets_JvfCorr3);
	treechain->SetBranchAddress("jets_Rpt4", &jets_Rpt4);
	treechain->SetBranchAddress("jets_JvfCorr4", &jets_JvfCorr4);
	treechain->SetBranchAddress("dphi_yy_j1", &dphi_yy_j1);
	treechain->SetBranchAddress("dphi_FjetV_met", &dphi_FjetV_met);
	for(int i = 0; i <=9; i++)
		BDTValues.push_back(0.0);
	int sigVin=1;
	
	for(TString bdtclass: bdtclasses){
		treechain->SetBranchAddress("met_MVA"+bdtclass,&BDTValues[sigVin]);
		sigVin++;
	}
	UInt_t nEvent = treechain->GetEntries();
	//UInt_t nEvent = 100;
	for(UInt_t i =0; i<nEvent; i++){
		treechain->GetEntry(i);
		if(pt2/myy<0.25 || pt1/myy<0.35) continue;
		if(N_j_central<1)continue;
		double sumw =0.0;
		double norm=0.0;
		double normsumw=0.0;
		sumw+=weight;
		if(kind == "signal"){
			norm=363.81;
			weight=weight*(norm/29879.8);
		}
		else if(kind == "background"){
			norm=41030.0;
			weight=weight*(norm/255158.0);
		}
		else {
			std::cout<<"check the kind flag....exiting..."<<std::endl;
			exit(1);
		}
		normsumw+=weight;
		
		if(metSig>2.5 && metSig<3.5){
			normYields["MetSig25NJgt1"]+=weight;
			if(N_j_central==1)normYields["MetSig25NJeq1"]+=weight;
			if(N_j_central>=2)normYields["MetSig25NJgt2"]+=weight;
			//Norminal selection with SV method
			if(diff_sumpt2>0.0)normYields["MetSig25VtxNJgt1"]+=weight;
			if(N_j_central==1&&diff_sumpt2>0.0)normYields["MetSig25VtxNJeq1"]+=weight;
			if(N_j_central>=2&&diff_sumpt2>0.0)normYields["MetSig25VtxNJgt2"]+=weight;
			for(int cut =0; cut<20;cut++){
				double cutVal = (cut-10)/10.0;
				TString cutname = Form("cut%d",cut);
				//std::cout<<cutname<<" "<<cutVal<<std::endl;
				if(BDTValues[1]>cutVal)
					normYields["MetSig25NJgt1"+cutname] += weight;
				if(N_j_central==1&&BDTValues[2]>cutVal)
					normYields["MetSig25NJeq1"+cutname] += weight;
				if(N_j_central>=2&&BDTValues[3]>cutVal)
					normYields["MetSig25NJgt2"+cutname] += weight;
				
				
				
				if(BDTValues[1]>cutVal&&diff_sumpt2>0.0)
					normYields["MetSig25VtxNJgt1"+cutname] += weight;
				if(N_j_central==1&&BDTValues[2]>cutVal&&diff_sumpt2>0.0)
					normYields["MetSig25VtxNJeq1"+cutname] += weight;
				if(N_j_central>=2&&BDTValues[3]>cutVal&&diff_sumpt2>0.0)
					normYields["MetSig25VtxNJgt2"+cutname] += weight;
			
			}
		}
		
		if(metSig>3.5 && metSig<5.5)
		{
			//Norminal selection
			normYields["MetSig35NJgt1"]+=weight;
			if(N_j_central==1)normYields["MetSig35NJeq1"]+=weight;
			if(N_j_central>=2)normYields["MetSig35NJgt2"]+=weight;
			//Norminal seletion with SV method
			if(diff_sumpt2>0.0)normYields["MetSig35VtxNJgt1"]+=weight;
			if(N_j_central==1&&diff_sumpt2>0.0)normYields["MetSig35VtxNJeq1"]+=weight;
			if(N_j_central>=2&&diff_sumpt2>0.0)normYields["MetSig35VtxNJgt2"]+=weight;

			for(int cut =0; cut<20;cut++){
				double cutVal = (cut-10)/10.0;
				TString cutname = Form("cut%d",cut);
				//std::cout<<cutname<<" "<<cutVal<<std::endl;
				if(BDTValues[1]>cutVal)
					normYields["MetSig35NJgt1"+cutname] += weight;
				if(N_j_central==1&&BDTValues[2]>cutVal)
					normYields["MetSig35NJeq1"+cutname] += weight;
				if(N_j_central>=2&&BDTValues[3]>cutVal)
					normYields["MetSig35NJgt2"+cutname] += weight;
				
				
				
				if(BDTValues[1]>cutVal&&diff_sumpt2>0.0)
					normYields["MetSig35VtxNJgt1"+cutname] += weight;
				if(N_j_central==1&&BDTValues[2]>cutVal&&diff_sumpt2>0.0)
					normYields["MetSig35VtxNJeq1"+cutname] += weight;
				if(N_j_central>=2&&BDTValues[3]>cutVal&&diff_sumpt2>0.0)
					normYields["MetSig35VtxNJgt2"+cutname] += weight;
			
			}
		}
		if(metSig>5.5)
		{
			//Norminal selection
			normYields["MetSig55NJgt1"]+=weight;
			if(N_j_central==1)normYields["MetSig55NJeq1"]+=weight;
			if(N_j_central>=2)normYields["MetSig55NJgt2"]+=weight;
			//Norminal seletion with SV method
			if(diff_sumpt2>0.0)normYields["MetSig55VtxNJgt1"]+=weight;
			if(N_j_central==1&&diff_sumpt2>0.0)normYields["MetSig55VtxNJeq1"]+=weight;
			if(N_j_central>=2&&diff_sumpt2>0.0)normYields["MetSig55VtxNJgt2"]+=weight;
		 
			for(int cut =0; cut<20;cut++){
				double cutVal = (cut-10)/10.0;
				TString cutname = Form("cut%d",cut);
				//std::cout<<cutname<<" "<<cutVal<<std::endl;
				if(BDTValues[1]>cutVal)
					normYields["MetSig55NJgt1"+cutname] += weight;
				if(N_j_central==1&&BDTValues[2]>cutVal)
					normYields["MetSig55NJeq1"+cutname] += weight;
				if(N_j_central>=2&&BDTValues[3]>cutVal)
					normYields["MetSig55NJgt2"+cutname] += weight;
				
				
				
				if(BDTValues[1]>cutVal&&diff_sumpt2>0.0)
					normYields["MetSig55VtxNJgt1"+cutname] += weight;
				if(N_j_central==1&&BDTValues[2]>cutVal&&diff_sumpt2>0.0)
					normYields["MetSig55VtxNJeq1"+cutname] += weight;
				if(N_j_central>=2&&BDTValues[3]>cutVal&&diff_sumpt2>0.0)
					normYields["MetSig55VtxNJgt2"+cutname] += weight;
			
			}
			
		}
		
	}
	
	
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
void makeEffRoc(TString bdtclass,std::map<TString, double> &Xvals,std::map<TString, double> &Yvals,TString labels){
	TCanvas *can = new TCanvas();
	can->SetTopMargin(0.04); can->SetRightMargin(0.04);
	//TString baseDir = "TrainingInfo/";
	TString baseDir = "Testout/";
	//TString fileName = Form("Njet_275mx60Sherpayy"+bdtclass+"NJ0.root");
	TString fileName = Form(bdtclass+"noJtesttrainoutNew"+bdtclass+"noJ.root");
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
	
	//fileName = Form("Njet_275mx60Sherpayy"+bdtclass+"NJ1.root");
	fileName = Form(bdtclass+"NJ1testtrainoutNew"+bdtclass+"NJ1.root");
	fileDir = baseDir+fileName;
	TFile *file2=new TFile(fileDir);
	TH1 *hist2=(TH1*)file2->Get("Method_BDT/BDTG1/MVA_BDTG1_rejBvsS");
	hist2->SetMarkerSize(0);
	hist2->SetLineColor(kRed+1);
	hist2->Draw("same");
	//fileName = Form("Njet_275mx60Sherpayy"+bdtclass+"NJ2.root");
	fileName = Form(bdtclass+"NJ2testtrainoutNew"+bdtclass+"NJ2.root");
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
	curr1->SetPoint(0,Xvals[names+"gt1"],Yvals[names+"gt1"]);
	std::cout<<Xvals[names+"gt1"]<<" "<<Yvals[names+"gt1"]<<std::endl;
	curr1->SetMarkerColor(kBlue+1);
	curr1->SetLineColor(kBlue+1);
	curr1->SetLineWidth(2);
	curr1->SetMarkerSize(2);
	curr1->Draw("same P");
	
	TGraph *curr2 = new TGraph();
	curr2->SetPoint(0,Xvals[names+"eq1"],Yvals[names+"eq1"]);
	std::cout<<Xvals[names+"eq1"]<<" "<<Yvals[names+"eq1"]<<std::endl;
	curr2->SetMarkerColor(kRed+1);
	curr2->SetLineColor(kRed+1);
	curr2->SetLineWidth(2);
	curr2->SetMarkerSize(2);
	curr2->Draw("same P");
	
	TGraph *curr3 = new TGraph();
	curr3->SetPoint(0,Xvals[names+"gt2"],Yvals[names+"gt2"]);
	std::cout<<Xvals[names+"gt2"]<<" "<<Yvals[names+"gt2"]<<std::endl;
	curr3->SetMarkerColor(kGreen+1);
	curr3->SetLineColor(kGreen+1);
	curr3->SetLineWidth(2);
	curr3->SetMarkerSize(2);
	curr3->Draw("same P");
	
	
	
	
	
	TGraph* gr35 = getNoImp(hist1);
	gr35->SetLineColor(1);
	gr35->SetLineWidth(2);
	gr35->Draw("same C");
	TLegend* legend = new TLegend(0.2,0.7,0.43,0.9);
	//legend->SetNColumns(2);
	legend->SetHeader("ATLAS #it{work in progress}");
	legend->SetTextFont(42);
	legend->SetTextSize(0.035);
	//legend->SetBorderSize(0);
	//legend->SetFillStyle(0);
	//legend->SetLineColor(0);
	legend->AddEntry(hist1,labels+" S_{E^{miss}_{T}} 1", "lep");
	legend->AddEntry(hist2,labels+" S_{E^{miss}_{T}} 2", "L");
	legend->AddEntry(hist3,labels+" S_{E^{miss}_{T}} 3", "L");
	legend->AddEntry(gr35,"No Improvement", "L");
	legend->Draw("same");
	can->SetGridx(true);
	can->SetGridy(true);
	can->Print("Plots/"+bdtclass+"New.pdf");
	

}


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
	  TString signalfile = baseDir+"Out_R21_HHDMmr275mx60br50_comb.root";
	  TString bkgfile = baseDir+"Out_R21_background_comb.root";
		std::vector<TString> bdtclasses = {"metsig25","metsig35","metsig55"};
		std::vector<TString> sets = {"MetSig25","MetSig35","MetSig55","MetSig25Vtx","MetSig35Vtx","MetSig55Vtx"};
		//std::vector<TString> sampsets = {"R21_ggZH125","R21_A400_Z_vv_H250_yy","R21_HHDMmr275mx60br50"};
		std::map<TString,double> nomBkGYields;nomBkGYields.clear();
		std::map<TString,double> nomSiGYields;nomSiGYields.clear();
		std::map<TString,double> sigTotals;sigTotals.clear();
	
	  std::vector<TString> signalcats = {"A400Z","275mx60","ggZH125"};
		//for(TString sampset: sampsets){
			for(TString set: sets){
     		nomSiGYields[set]=0.0;
				nomBkGYields[set]=0.0;
				for(TString sig: signalcats){
					for(int cut =0; cut<20;cut++){
						TString cutname = Form("%scut%d",sig.Data(),cut);
						nomSiGYields[set+cutname]=0.0;
						nomBkGYields[set+cutname]=0.0;
					}
				}
			}
	//	}
	
	
	LoopFills(signalfile,nomSiGYields,"signal");
	LoopFills(bkgfile,nomBkGYields,"background");
	
  for(std::map<TString,double>::iterator c = nomSiGYields.begin(); c!=nomSiGYields.end(); ++c){
		std::cout<<"signal: "<<(*c).first<<"  :  "<<(*c).second<<std::endl;
	}
	for(std::map<TString,double>::iterator c = nomBkGYields.begin(); c!=nomBkGYields.end(); ++c){
		std::cout<<"background: "<<(*c).first<<"  :  "<<(*c).second<<std::endl;
	}


	
	std::cout<<"Calculating significance from S and B:"<<std::endl;
	//for(TString sampset: sampsets){
	   for(TString set: sets){
		  double sig = nomSiGYields[set]/sqrt(nomBkGYields[set]);
			sigTotals[set]=sig;
			std::cout<<set<<" signal: "<<nomSiGYields[set]<<" bkg: "<<nomBkGYields[set]<<" sigf = "<<sigTotals[set]<<std::endl;
			
			
			
		}
	//}
	

	std::vector<TString> tmpsets = {"MetSig25","MetSig35","MetSig55"};
	std::map<TString, double> Xvals;
	std::map<TString, double> Yvals;
	Xvals.clear();
	Yvals.clear();
	std::cout<<"Calculating efficiency:"<<std::endl;
	//for(TString sampset: sampsets){
		for(TString set: tmpsets){
			//double sig = nomSigYields[set]/sqrt(nomSigYields[set]+nomBkgYields[set]);
			
			TString vtxname = Form("%sVtx",set.Data());
			//std::cout<<vtxname<<std::endl;
			
			double effs = nomSiGYields[vtxname]/nomSiGYields[set];
			std::cout<<"Signal: "<<vtxname<<"\\"<<set<<": "<<effs<<std::endl;
			
			double effb = nomBkGYields[vtxname]/nomBkGYields[set];
			std::cout<<"Bkg: "<<vtxname<<"\\"<<set<<": "<<1-effb<<std::endl;
			Xvals[vtxname]=effs;
			Yvals[vtxname]=1-effb;
		}
	//}
	
	//updated function to make efficiency plots...
	makeEffRoc("metsig25",Xvals,Yvals,"Low");
	makeEffRoc("metsig35",Xvals,Yvals,"Int");
	makeEffRoc("metsig55",Xvals,Yvals,"High");
	/*
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
			for(TString jetset : sampsets)
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
			for(TString jetset : sampsets)
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
		for(TString jetset: sampsets){
			double perc = ((sigTotals[set+jetset+"max"] - sigTotals[set+jetset])/sigTotals[set+jetset])*100;
		//	std::cout<<set+jetset<<"  "<<sigTotals[set+jetset]<<" "<<set+jetset+"BDT_max"<<" "<<sigTotals[set+jetset+"max"]<<" perc: "<<perc<<std::endl;
			//std::cout<<set+jetset<<"&"<<sigTotals[set+jetset]<<"&"<<sigTotals[set+jetset+"max"]<<" gain: "<<(sigTotals[set+jetset+"max"]-sigTotals[set+jetset])/sigTotals[set+jetset]<<"\\\\"<<std::endl;
			TString n_label = getmetlabel(set)+" $S_{E^{miss}_{T}}$ " +getJet(jetset);
			TString texout = Form("%s&%0.2f&%0.2f&%0.2f\\\\",n_label.Data(),sigTotals[set+jetset],sigTotals[set+jetset+"max"],perc);
			std::cout<<texout<<std::endl;
		}
	}
	
	*/
	
	 
    return 1;
}


void LoopFills(TString file, std::map<TString,double> &normYields, TString kind){
	TChain *treechain = new TChain("tree");
	treechain->AddFile(file);
	float metSig = 0.0;
	std::vector<float> BDTValues;
	std::vector<TString> bdtclasses = {"metsig25","metsig35","metsig55"};
	std::vector<TString> sampsets = {"R21_ggZH125_comb","R21_A400_Z_vv_H250_yy_comb","R21_HHDMmr275mx60br50_comb"};
	float diff_sumpt2 = 0;
	float weight = 0;
	float dz = 100;
	float myy = 0.0;
	float pt1 = 0.0;
	float pt2 = 0.0;
	float N_j_central = 0.0;
	float jets_rpt1 = 0.0;
	float jets_jvf1 = 0.0;
	float jets_rpt2 = 0.0;
	float jets_jvf2 = 0.0;
	float jets_rpt3 = 0.0;
	float jets_jvf3 = 0.0;
	float jets_rpt4 = 0.0;
	float jets_jvf4 = 0.0;
	float dphiyyj1 = 0.0;
	float dphifjmet = 0.0;
	treechain->SetBranchAddress("dz",&dz);
	treechain->SetBranchAddress("metsig", &metSig);
	treechain->SetBranchAddress("weight", &weight);
	treechain->SetBranchAddress("diff_sumpt2", &diff_sumpt2);
	treechain->SetBranchAddress("m_yy", &myy);
	treechain->SetBranchAddress("phpt1", &pt1);
	treechain->SetBranchAddress("phpt2", &pt2);
	treechain->SetBranchAddress("N_j_central", &N_j_central);
	
	treechain->SetBranchAddress("jets_rpt1", &jets_rpt1);
	treechain->SetBranchAddress("jets_jvf1", &jets_jvf1);
	treechain->SetBranchAddress("jets_rpt2", &jets_rpt2);
	treechain->SetBranchAddress("jets_jvf2", &jets_jvf2);
	treechain->SetBranchAddress("jets_rpt3", &jets_rpt3);
	treechain->SetBranchAddress("jets_jvf3", &jets_jvf3);
	treechain->SetBranchAddress("jets_rpt4", &jets_rpt4);
	treechain->SetBranchAddress("jets_jvf4", &jets_jvf4);
	treechain->SetBranchAddress("dphiyyj1", &dphiyyj1);
	treechain->SetBranchAddress("dphifjmet", &dphifjmet);
	for(int i = 0; i <=9; i++)
		BDTValues.push_back(0.0);
	int sigVin=1;
	for(TString samset: sampsets){
		for(TString bdtclass: bdtclasses){
			treechain->SetBranchAddress("met_MVA"+samset+bdtclass,&BDTValues[sigVin]);
			sigVin++;
		}
	}
	UInt_t nEvent = treechain->GetEntries();
	//UInt_t nEvent = 100;
	std::cout<<"number Events: "<<nEvent<<std::endl;
	for(UInt_t i =0; i<nEvent; i++){
		treechain->GetEntry(i);
		if(pt1<40 && pt2<30) continue;
		//std::cout<<"check weight: "<<weight<<std::endl;
		//if(pt2/myy<0.25 || pt1/myy<0.35) continue;
		//if(N_j_central<1)continue;
		/*
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
		*/
		if(metSig>2.5 && metSig<3.5){
			if(N_j_central>=1)normYields["MetSig25"]+=weight;
			if(N_j_central>=1&& diff_sumpt2>0.0)normYields["MetSig25Vtx"]+=weight;
		
			for(int cut = 0; cut<20;cut++){
				double cutVal = (cut-10)/10.0;
				TString cutname = Form("cut%d",cut);
				//std::cout<<cutname<<" "<<cutVal<<std::endl;
				if(BDTValues[1]>cutVal&&N_j_central>=1)
					normYields["MetSig25ggZH125"+cutname]+=weight;
				if(BDTValues[4]>cutVal&&N_j_central>=1)
					normYields["MetSig25A400Z"+cutname]+=weight;
				if(BDTValues[7]>cutVal&&N_j_central>=1)
					normYields["MetSig25275mx60"+cutname]+=weight;
				
				
				
				if(BDTValues[1]>cutVal&&diff_sumpt2>0.0&&N_j_central>=1)
					normYields["MetSig25VtxggZH125"+cutname]+=weight;
				if(BDTValues[4]>cutVal&&diff_sumpt2>0.0&&N_j_central>=1)
					normYields["MetSig25VtxA400Z"+cutname]+=weight;
				if(BDTValues[7]>cutVal&&diff_sumpt2>0.0&&N_j_central>=1)
					normYields["MetSig25Vtx275mx60"+cutname]+=weight;
			
			}
		}
		
		if(metSig>3.5 && metSig<5.5)
		{
			if(N_j_central>=1)normYields["MetSig35"]+=weight;
			if(N_j_central>=1&& diff_sumpt2>0.0)normYields["MetSig35Vtx"]+=weight;
			
			for(int cut =0; cut<20;cut++){
				double cutVal = (cut-10)/10.0;
				TString cutname = Form("cut%d",cut);
				//std::cout<<cutname<<" "<<cutVal<<std::endl;
				if(BDTValues[2]>cutVal&&N_j_central>=1)
					normYields["MetSig35ggZH125"+cutname]+=weight;
				if(BDTValues[5]>cutVal&&N_j_central>=1)
					normYields["MetSig35A400Z"+cutname]+=weight;
				if(BDTValues[8]>cutVal&&N_j_central>=1)
					normYields["MetSig35275mx60"+cutname]+=weight;
				
				
				
				if(BDTValues[2]>cutVal&&diff_sumpt2>0.0&&N_j_central>=1)
					normYields["MetSig35VtxggZH125"+cutname]+=weight;
				if(BDTValues[5]>cutVal&&diff_sumpt2>0.0&&N_j_central>=1)
					normYields["MetSig35VtxA400Z"+cutname]+=weight;
				if(BDTValues[8]>cutVal&&diff_sumpt2>0.0&&N_j_central>=1)
					normYields["MetSig35Vtx275mx60"+cutname]+=weight;
				
			}
			
			
		}
		if(metSig>5.5)
		{
			if(N_j_central>=1)normYields["MetSig55"]+=weight;
			if(N_j_central>=1&& diff_sumpt2>0.0)normYields["MetSig55Vtx"]+=weight;

			for(int cut =0; cut<20;cut++){
				double cutVal = (cut-10)/10.0;
				TString cutname = Form("cut%d",cut);
				//std::cout<<cutname<<" "<<cutVal<<std::endl;
				if(BDTValues[3]>cutVal&&N_j_central>=1)
					normYields["MetSig55ggZH125"+cutname]+=weight;
				if(BDTValues[6]>cutVal&&N_j_central>=1)
					normYields["MetSig55A400Z"+cutname]+=weight;
				if(BDTValues[9]>cutVal&&N_j_central>=1)
					normYields["MetSig55275mx60"+cutname]+=weight;
				
				
				
				if(BDTValues[3]>cutVal&&diff_sumpt2>0.0&&N_j_central>=1)
					normYields["MetSig55VtxggZH125"+cutname]+=weight;
				if(BDTValues[6]>cutVal&&diff_sumpt2>0.0&&N_j_central>=1)
					normYields["MetSig55VtxA400Z"+cutname]+=weight;
				if(BDTValues[9]>cutVal&&diff_sumpt2>0.0&&N_j_central>=1)
					normYields["MetSig55Vtx275mx60"+cutname]+=weight;
				
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
	//TString baseDir = "MVA/TrainingInfo/";
	TString baseDir = "MVA/Testout/";
	
	TString fileName = Form("R21_A400_Z_vv_H250_yy_comb"+bdtclass+".root");
	TString fileDir = baseDir+fileName;
	TFile *file1= new TFile(fileDir);
	TH1 *hist1=(TH1*)file1->Get("Method_BDT/BDTG1/MVA_BDTG1_rejBvsS");
	hist1->SetMarkerSize(0);
	hist1->SetLineColor(kBlue+1);
	hist1->GetYaxis()->SetTitle("Bkg rejection (1-eff)");
	hist1->GetXaxis()->SetTitle("Signal eff");
	hist1->SetMaximum(1.5);
	hist1->SetMinimum(0.0005);
	hist1->Draw("C");
	
	
	fileName = Form("R21_HHDMmr275mx60br50_comb"+bdtclass+".root");
	fileDir = baseDir+fileName;
	TFile *file2=new TFile(fileDir);
	TH1 *hist2=(TH1*)file2->Get("Method_BDT/BDTG1/MVA_BDTG1_rejBvsS");
	hist2->SetMarkerSize(0);
	hist2->SetLineColor(kRed+1);
	hist2->Draw("same C");

	fileName = Form("R21_ggZH125_comb"+bdtclass+".root");
	fileDir = baseDir+fileName;
	TFile *file3=new TFile(fileDir);
	TH1 *hist3=(TH1*)file3->Get("Method_BDT/BDTG1/MVA_BDTG1_rejBvsS");
	hist3->SetMarkerSize(0);
	hist3->SetLineColor(kGreen+1);
	hist3->Draw("same C");
	
	TString names = "MetSig25Vtx";
	if(bdtclass=="metsig35")names="MetSig35Vtx";
	if(bdtclass=="metsig55")names="MetSig55Vtx";
	TGraph *curr1 = new TGraph();
	curr1->SetPoint(0,Xvals[names],Yvals[names]);

	std::cout<<names<<" "<<Xvals[names]<<" "<<Yvals[names]<<std::endl;
	curr1->SetMarkerColor(kBlack);
	curr1->SetLineColor(kBlack);
	curr1->SetLineWidth(2);
	curr1->SetMarkerSize(2);
	curr1->Draw("same P");
	/*
	TGraph *curr2 = new TGraph();
	curr2->SetPoint(0,Xvals[names+"eq1"],Yvals[names+"eq1"]);
	std::cout<<Xvals[names+"eq1"]<<" "<<Yvals[names+"eq1"]<<std::endl;
	curr2->SetMarkerColor(kBlack);
	curr2->SetLineColor(kBlack);
	curr2->SetLineWidth(2);
	curr2->SetMarkerSize(2);
	curr2->Draw("same P");
	
	TGraph *curr3 = new TGraph();
	curr3->SetPoint(0,Xvals[names],Yvals[names]);
	std::cout<<Xvals[names]<<" "<<Yvals[names]<<std::endl;
	curr3->SetMarkerColor(kBlack);
	curr3->SetLineColor(kBlack);
	curr3->SetLineWidth(2);
	curr3->SetMarkerSize(2);
	curr3->Draw("same P");
	*/
	
	
	
	
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
	legend->AddEntry(hist1,labels+" S_{E^{miss}_{T}} A400Z", "lp");
	legend->AddEntry(hist2,labels+" S_{E^{miss}_{T}} 275mx60", "L");
	legend->AddEntry(hist3,labels+" S_{E^{miss}_{T}} ggZH125", "L");
	legend->AddEntry(gr35,"No Improvement", "L");
	legend->Draw("same");
	can->SetGridx(true);
	can->SetGridy(true);
	can->Print("MVA/Plots/"+bdtclass+"R21_comb.pdf");
	

}


/*  Run interface for MVATools
 *  Code uses configuration file on data/plotConfig.cfg
 *  Autors: Xifeng Ruan,  Kehinde Tomiwa
 *         kehinde.tomiwa4@gmail.com
 */

#include "Plotter.h"
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include "TROOT.h"
#include "TEnv.h"
TGraph* getNoImp(TH1* hist){
	int n =hist->GetNbinsX();
	double x[n], y[n];
	for(int bin=0; bin < hist->GetNbinsX(); bin++)
	{
		x[bin] = hist->GetXaxis()->GetBinCenter(bin+1);
		y[bin] = 1-(x[bin]*x[bin]);
		std::cout<<"x["<<bin<<"]: "<<x[bin]<<std::endl;
		std::cout<<"y["<<bin<<"]: "<<y[bin]<<std::endl;
	}
	TGraph* gr = new TGraph(n,x,y);
	return gr;
}

void splitString(const TString& str, std::vector<TString>& splits,const TString& delimiter)
{
   TObjArray* splitsArray = str.Tokenize(delimiter);
   for (int i = 0; i < splitsArray->GetEntries(); i++) splits.push_back(splitsArray->At(i)->GetName());
   delete splitsArray;
 
}


TString getSample(TString sample){
	TString ret = "";
	if(sample=="R21_A400_Z_vv_H250_yy_comb")ret= "A400Z";
	if(sample=="R21_HHDMmr275mx60br50_comb")ret= "275mx60";
	if(sample=="R21_ggZH125_comb")ret= "ggZH125";
        return ret;
}
TString getmetlabel(TString metcat){
        TString ret = "";
	if(metcat=="metSig55")ret= "Low";
	if(metcat=="metSig70")ret= "High";
        return ret;
	
	
}

int main(int argc, char **argv)
{
	Plotter *plotter = new Plotter();
	plotter->initialize();
	plotter->SetAtlasStyle();
	plotter->setLogY(false);
	plotter->setGrid(true,true);
	plotter->dataToMC(false);
	plotter->drawRatio(false);
	plotter->drawRatioErr(false);
	plotter->setDrawOptions("L");
	plotter->setLegNCol(2);
	plotter->setYRange(0.0,0.9);
	plotter->setAtlasLabel("ATLAS #it{work in progress}");
	


        TString config = TString(argv[1]);
        std::cout<<"reading config: "<<config<<std::endl;
        TEnv m_reader(config);

        TString baseDir = m_reader.GetValue("TrainInfoDir","");
        TString outdir  = m_reader.GetValue("plotOutputDir","");
        
        system(Form("mkdir -vp %s", outdir.Data()));

        std::vector<TH1*> histList;
	
   
        std::vector<TString> metsets;
        splitString(m_reader.GetValue("Categories",""),metsets," ");
	/
        std::vector<TString> traintypes;
        splitString(m_reader.GetValue("SignalSamples",""),traintypes," ");
	
	int i = 1;
	for(TString traintype : traintypes)
          for(TString set: metsets){
	  {  
		TString fileName = traintype+set+".root";
		std::cout<<"check1: "<<fileName<<std::endl;
		TString fileDir = baseDir+fileName;
		
		TFile *file=new TFile(fileDir);
		std::cout<<"opened: "<<fileDir<<std::endl;
		TH1 *hist=(TH1*)file->Get("dataset/Method_BDTG/BDTG/MVA_BDTG_rejBvsS");
                //TString label = getmetlabel(set);
                TString label = m_reader.GetValue(set+"Label","");
		hist->SetName(label+" S_{E^{miss}_{T}}");
		std::cout<<"open hist: "<<hist->GetName()<<std::endl;
		histList.push_back(hist);
		std::cout<<"push back: "<<fileDir<<std::endl;
		i++;
	  }
        }
	std::cout<<"check:0"<<std::endl;
	TString pdfName = outdir+"RoC_comb.pdf";
	
	TCanvas *can = new TCanvas("c1","Can",0.,0.,800,600);
	can->Print(pdfName+"[");
	
	std::vector<TString> labels = {};
	
	//Plot the 3 met sig plots.
	can = plotter->getStylishCanvas(histList,labels);
	plotter->setYRange(0.0,1.2);
	//can->Print(pdfName);
	TGraph* gr35 = getNoImp(histList[1]);
	gr35->SetLineColor(1);
	gr35->SetLineWidth(2);
	gr35->Draw("same C");
	TLegend* legend = new TLegend(0.2,0.2, 0.45, 0.45);
	legend->SetNColumns(2);
	legend->SetTextFont(42);
	legend->SetTextSize(0.03);
	legend->SetBorderSize(0);
	legend->SetFillStyle(0);
	legend->SetLineColor(0);
	legend->AddEntry(gr35,"No Improvement", "L");
	legend->Draw("same");
	can->Update();
	can->Print(pdfName);
	std::cout<<"check:1"<<std::endl;
	for(auto hist : histList)
		hist->GetXaxis()->SetRangeUser(0.7,0.9);
	
	plotter->setYRange(0.0,0.9);
	can = plotter->getStylishCanvas(histList,labels);
	plotter->setYRange(0.0,1.2);
	can->Print(pdfName);
	
	can->Print(pdfName+"]");
	
	
	
	std::cout<<"check: Training"<<std::endl;
	
	//----------------------------------------------------------------------------------------
	// Training plots
	std::vector<TH1*> sigTrain;
	std::vector<TH1*> bkgTrain;
        std::vector<TH1*> sigTrainT;
        std::vector<TH1*> bkgTrainT;
	for(TString traintype : traintypes)
		for(TString set : metsets)
		{
			TString fileName = traintype+set+".root";
			TString fileDir = baseDir+fileName;
			
			TFile *file=new TFile(fileDir);
			TH1 *hist=(TH1*)file->Get("dataset/Method_BDTG/BDTG/MVA_BDTG_S");
                        TString label = m_reader.GetValue(set+"Label","");
			hist->SetName(label+" S_{E^{miss}_{T}} "+getSample(traintype)+" S Test");
			hist->SetFillColor(kBlue+1);
			hist->SetFillColorAlpha(kBlue+1,0.5);
			sigTrain.push_back(hist);
			

            TH1 *histT=(TH1*)file->Get("dataset/Method_BDTG/BDTG/MVA_BDTG_Train_S");
            histT->SetName(label+" S_{E^{miss}_{T}} "+getSample(traintype)+" S Train");
            histT->SetMarkerStyle(20);
            histT->SetMarkerSize(.75);
            histT->SetMarkerColor(kBlue+1);
            sigTrainT.push_back(histT);

			
			TH1 *hist2=(TH1*)file->Get("dataset/Method_BDTG/BDTG/MVA_BDTG_B");
			hist2->SetName(label+" S_{E^{miss}_{T}} "+getSample(traintype)+" B Test");
			//hist2->SetName(set+getJet(jetset)+"Background");
			hist2->SetFillColor(kRed+1);
			hist2->SetFillColorAlpha(kRed+1,0.5);
			bkgTrain.push_back(hist2);

        	TH1 *hist2T=(TH1*)file->Get("dataset/Method_BDTG/BDTG/MVA_BDTG_Train_B");
            hist2T->SetName(label+" S_{E^{miss}_{T}} "+getSample(traintype)+" B Train");
            hist2T->SetMarkerStyle(20);
            hist2T->SetMarkerSize(.75);
            hist2T->SetMarkerColor(kRed+1);
            bkgTrainT.push_back(hist2T);
		}
	
	plotter->reset();
	plotter->setLogY(false);
	plotter->setGrid(false,false);
	plotter->dataToMC(false);
	plotter->drawRatio(false);
	plotter->drawRatioErr(false);
	plotter->setAtlasLabel("ATLAS #it{work in progress}");
	plotter->setDrawOptions("Hist");
	
	pdfName = outdir+"SBTrain-Plots_comb.pdf";
	
	can = new TCanvas("c1","Can",0.,0.,800,600);
	can->Print(pdfName+"[");
	
	for(int histNum = 0; histNum < sigTrain.size(); histNum++)
	{
		std::vector<TH1*> temp;
		std::vector<TString> labels = {};
		temp.push_back(sigTrain[histNum]);
		temp.push_back(bkgTrain[histNum]);
        temp.push_back(sigTrainT[histNum]);
        temp.push_back(bkgTrainT[histNum]);
		sigTrain[histNum]->GetXaxis()->SetTitle("BDT Output");
		sigTrain[histNum]->GetYaxis()->SetTitle("Events (Norm)");
		//TH1 hist; hist.GetXaxis
		
		can = plotter->getStylishCanvas(temp,labels);
		can->Print(pdfName);
	}
	can->Print(pdfName+"]");
	
	
	//---------------------------------------------------------------------------------------
	// Correlations
	
	std::vector<TH2*> sigCorr;
	std::vector<TH2*> bkgCorr;
	
	for(TString traintype : traintypes)
		for(TString set : metsets)
		{
			
			TString fileName = traintype+set+".root";
			TString fileDir = baseDir+fileName;
			TString label = m_reader.GetValue(set+"Label","");
			TFile *file=new TFile(fileDir);
			TH2 *hist=(TH2*)file->Get("dataset/CorrelationMatrixS");
			//hist->SetName(set+getJet(jetset)+" Signal");
			hist->SetName(label+" "+getSample(traintype)+" Sig");
			sigCorr.push_back(hist);
			
			
			TH2 *hist2=(TH2*)file->Get("dataset/CorrelationMatrixB");
			//hist2->SetName(set+getJet(jetset)+" Background");
			hist2->SetName(label+" "+getSample(traintype)+" Bkg");
			bkgCorr.push_back(hist2);
			std::cout<<"Hist2D: "<<set<<" BDT: "<<i<<std::endl;
		}
	
	pdfName = outdir+"Correlation-Plots_comb.pdf";
	
	can = new TCanvas("c2","Can2",0.,0.,1800,1000);
	can->SetTopMargin(0.1);
	can->SetRightMargin(0.15);
	gStyle->SetPaintTextFormat("0.0f %%");
	gStyle->SetHistMinimumZero();
	gStyle->SetPalette(kRainBow);
	
	
	TLatex l;
	l.SetTextColor(kBlack);
	l.SetTextSize(0.035);
	l.SetTextAlign(31);
	
	can->Print(pdfName+"[");
	for(int histNum = 0; histNum < sigCorr.size(); histNum++)
	{
		std::cout<<"Iteration: "<<histNum<<std::endl;
		//padSig->cd();
		sigCorr[histNum]->Draw("colz text");
		TString label = sigCorr[histNum]->GetName();
		label.ReplaceAll("Signal","");
		std::cout<<"label: "<<label<<std::endl;
		l.DrawLatex(0,sigCorr[histNum]->GetNbinsY(), Form("%s",label.Data()));
		can->Print(pdfName);
		//padbkg->cd();
		bkgCorr[histNum]->Draw("colz text");
		label = bkgCorr[histNum]->GetName();
		label.ReplaceAll("Background","");
		std::cout<<"label: "<<label<<std::endl;
		l.DrawLatex(0,bkgCorr[histNum]->GetNbinsY(), Form("%s",label.Data()));
		can->Print(pdfName);
		
	}
	can->Print(pdfName+"]");
	can->Clear();
	
	//---------------------------------------------------------------------------------------
	// Overtraining
	
	std::vector<TH1*> testHist;
	std::vector<TH1*> trainHist;
	
	for(TString traintype : traintypes)
		for(TString set : metsets)
		{
			TString fileName = traintype+set+".root";
			TString fileDir = baseDir+fileName;
			
			TFile *file=new TFile(fileDir);
			TTree *testtree = (TTree*)file->Get("dataset/TestTree");
			float BDT =0.0;
			testtree->SetBranchAddress("BDTG",&BDT);
			
			TH1F *hist1   = new TH1F("test","",50,-1,1);
			Long64_t nentries = testtree->GetEntries();
			for (Long64_t i=0;i<nentries;i++)
			{
				testtree->GetEntry(i);
				hist1->Fill(BDT);
			}
			//hist1->SetName(set+getJet(jetset)+" Test");
                        TString label = m_reader.GetValue(set+"Label","");
			hist1->SetName(label+" S_{E^{miss}_{T}} "+getSample(traintype)+" Test");
			hist1->SetFillColor(kBlue+1);
			hist1->SetFillColorAlpha(kBlue+1,0.5);
			//hist1->Scale(20.0/hist1->Integral());
			testHist.push_back(hist1);
			
			
			TTree *traintree = (TTree*)file->Get("dataset/TrainTree");
			traintree->SetBranchAddress("BDTG",&BDT);
			
			TH1F *hist2   = new TH1F("train","",50,-1,1);
			nentries = traintree->GetEntries();
			for (Long64_t i=0;i<nentries;i++)
			{
				traintree->GetEntry(i);
				hist2->Fill(BDT);
			}
			
			hist2->SetName(label+" S_{E^{miss}_{T}} "+getSample(traintype)+" Train");
			hist2->SetFillColor(kRed+1);
			hist2->SetFillColorAlpha(kRed+1,0.5);
			trainHist.push_back(hist2);
		}
	
	
	plotter->reset();
	plotter->setLogY(false);
	plotter->setGrid(false,false);
	plotter->dataToMC(false);
	plotter->drawRatio(false);
	plotter->drawRatioErr(false);
	plotter->setAtlasLabel("ATLAS #it{work in progress}");
	plotter->setDrawOptions("Hist");
	
	pdfName = outdir+"TrainTest-Plots_comb.pdf";
	
	can = new TCanvas("c1","Can",0.,0.,800,600);
	can->Print(pdfName+"[");
	
	for(int histNum = 0; histNum < sigTrain.size(); histNum++)
	{
		std::vector<TH1*> temp;
		double ks = testHist[histNum]->Chi2Test(trainHist[histNum],"WW CHI2/NDF");
		std::vector<TString> labels = {Form("#chi^{2}/NDF: %0.2f",ks)};
		temp.push_back(testHist[histNum]);
		temp.push_back(trainHist[histNum]);
		testHist[histNum]->GetXaxis()->SetTitle("BDT Output");
		testHist[histNum]->GetYaxis()->SetTitle("Events (Norm)");
		//TH1 hist; hist.GetXaxis
		
		can = plotter->getStylishCanvas(temp,labels);
		can->Print(pdfName);
	}
	can->Print(pdfName+"]");
      
	
	return 1;
}




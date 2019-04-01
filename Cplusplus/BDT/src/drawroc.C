TString filename="HS_";

void drawcompare(TString f1, TString f2, TString picname, TString leg1, TString leg2);
void drawgroup(TString filename, TString samplename);
void drawpic(TString picname);
void drawratiopic(TString picname_numerator, TString picname_denominator);
void drawsimple(TString filename, TString picname, TString samplename);
void drawoverlay(std::vector<TString> files, TString picname, std::vector<TString> legs,TString tag);
void drawgroup2(std::vector<TString> filenames, std::vector<TString> legs, TString tag);
void drawoverlaysamefile(std::vector<TString> picnames, TString filename, std::vector<TString> legs, TString tag);

void drawroc(){
std::vector<TString> filenames1,legs1,filenames2,legs2, filenames3, legs3,filenames4, legs4;
filenames1.push_back("/home/roreed/workspace/Frameworks/h015FW/TwoFifty/MVA/TrainingWeights/fullggh.root"),legs1.push_back("Full set");
//filenames1.push_back("mvaweights/independantstrongggh.root"),legs1.push_back("IStrong set");
//filenames1.push_back("mvaweights/independantweakggh.root"),legs1.push_back("IWeak set");
//filenames1.push_back("mvaweights/testggh.root"),legs1.push_back("Only ld vtx set");
//filenames1.push_back("mvaweights/test2ggh.root"),legs1.push_back("dSumpt2 & SumEt set");
//filenames2.push_back("mvaweights/fullzh.root"),legs2.push_back("Full set");
//filenames2.push_back("mvaweights/independantstrongzh.root"),legs2.push_back("IStrong set");
//filenames2.push_back("mvaweights/independantweakzh.root"),legs2.push_back("IWeak set");
//filenames2.push_back("mvaweights/testzh.root"),legs2.push_back("Only ld vtx set");
//filenames2.push_back("mvaweights/test2zh.root"),legs2.push_back("dSumpt2 & SumEt set");

std::vector<TString> filelist = {"Method_BDT/BDTG1/MVA_BDTG1_rejBvsS",
"Method_BDT/BDTG2/MVA_BDTG2_rejBvsS","Method_BDT/BDTG3/MVA_BDTG3_rejBvsS",
"Method_BDT/BDTG4/MVA_BDTG4_rejBvsS","Method_BDT/BDTG5/MVA_BDTG5_rejBvsS",
"Method_BDT/BDTG6/MVA_BDTG6_rejBvsS","Method_BDT/BDTG7/MVA_BDTG7_rejBvsS",
"Method_BDT/BDTG8/MVA_BDTG8_rejBvsS","Method_BDT/BDTG9/MVA_BDTG9_rejBvsS"};
drawoverlaysamefile(filelist,filenames1[0],legs1,"ggh");
drawgroup2(filenames1,legs1,"ggh");
//drawgroup2(filenames2,legs2,"zh");
//drawgroup2(filenames3,legs3,"");
//drawgroup2(filenames4,legs4,"");

}
void drawgroup2(std::vector<TString> filenames, std::vector<TString> legs, TString tag){
drawoverlay(filenames, "Method_BDT/BDTG1/MVA_BDTG1_rejBvsS", legs ,tag);

}

void drawgroup(TString filename, TString samplename){
TFile *file=new TFile(filename);
int size= file->GetListOfKeys()->GetSize();
for(int i=0;i<size;i++){
TString figname= file->GetListOfKeys()->At(i)->GetName();
if(figname.Contains("fromZ"))drawsimple(filename, figname, samplename);
}

}

void drawpic(TString picname){
gStyle->SetOptStat(0);
TCanvas *c1=new TCanvas("plot","plot",800,600);
TFile *file1=new TFile(filename+"full/hist-sample.root");
TFile *file2=new TFile(filename+"fast/hist-sample.root");
TH1F *pic1=(TH1F*)file1->Get(picname);
TH1F *pic2=(TH1F*)file2->Get(picname);
TString xlabel=pic1->GetXaxis()->GetTitle();

pic1->Scale(1./pic1->Integral());
pic2->Scale(1./pic2->Integral());
pic1->Draw();
pic2->Draw("same");

pic1->SetLineWidth(2);
pic1->SetLineColor(2);
pic2->SetLineWidth(2);
pic2->SetLineColor(4);
pic1->GetYaxis()->SetRangeUser(0,(pic1->GetMaximum()>pic2->GetMaximum())?pic1->GetMaximum()*1.5:pic2->GetMaximum()*1.5 );
pic1->GetXaxis()->SetTitleOffset(1.2);
TLegend *leg=new TLegend(0.75,0.75,0.95,0.95,"","brNDC");

leg->AddEntry(pic1,"FullSim", "fl");
leg->AddEntry(pic2,"FastSim", "fl");
leg->Draw();


//c1->Print("figures/Compare"+picname+".pdf");
c1->Print("figures/Compare"+picname+".png");

}

void drawsimple(TString filename, TString picname, TString samplename){
gStyle->SetOptStat(0);
TCanvas *c1=new TCanvas("plot","plot",800,600);
TFile *file1=new TFile(filename);
TH1F *pic1=(TH1F*)file1->Get(picname);
TString xlabel=pic1->GetXaxis()->GetTitle();
pic1->Draw();

pic1->SetLineWidth(2);
pic1->SetLineColor(2);
pic1->GetYaxis()->SetRangeUser(0,pic1->GetMaximum()*1.5 );
pic1->GetXaxis()->SetTitleOffset(1.2);
//TLegend *leg=new TLegend(0.75,0.75,0.95,0.95,"","brNDC");
//
//leg->AddEntry(pic1,"FullSim", "fl");
//leg->AddEntry(pic2,"FastSim", "fl");
//leg->Draw();


c1->Print("validation/figures/val_"+samplename+"_"+picname+".pdf");
c1->Print("validation/figures/val_"+samplename+"_"+picname+".png");

}
void drawoverlay(std::vector<TString> files, TString picname, std::vector<TString> legs, TString tag){
gStyle->SetOptStat(0);
TCanvas *c1=new TCanvas("plot","plot",800,600);
TLegend *leg=new TLegend(0.75,0.75,0.95,0.95,"","brNDC");
c1->SetTitle("");
for(int i=0;i<files.size();i++){
TFile *file=new TFile(files[i],"open");
TH1F *pic=(TH1F*)file->Get(picname);
pic->Draw("samehist");
pic->SetLineWidth(2);
pic->SetLineColor(2+i*1);
//pic->Scale(1/pic->Integral());
pic->GetYaxis()->SetRangeUser(0.,1 );
pic->GetXaxis()->SetTitle(pic->GetTitle());
pic->SetTitle("");
pic->GetXaxis()->SetTitleOffset(1.2);
//gPad->SetLogy(5);
leg->AddEntry(pic,legs[i], "fl");

}
leg->Draw();
c1->Print("/home/roreed/workspace/Frameworks/h015FW/TwoFifty/MVA/Plots/ROC_"+tag+".pdf");


}
void drawoverlaysamefile(std::vector<TString> picnames, TString filename, std::vector<TString> legs, TString tag){
gStyle->SetOptStat(0);
TCanvas *c1=new TCanvas("plot","plot",800,600);
TLegend *leg=new TLegend(0.75,0.75,0.95,0.95,"","brNDC");
c1->SetTitle("");
TFile *file=new TFile(filename,"open");

for(int i=0;i<picnames.size();i++){
TH1F *pic=(TH1F*)file->Get(picnames[i]);
pic->Draw("samehist");
pic->SetLineWidth(2);
pic->SetLineColor(2+i*2);
pic->Scale(1./pic->Integral());
pic->GetYaxis()->SetRangeUser(0.00001,pic->GetMaximum()*1.5 );
pic->GetXaxis()->SetTitle(pic->GetTitle());
pic->SetTitle("");
pic->GetXaxis()->SetTitleOffset(1.2);
//gPad->SetLogy(5);
leg->AddEntry(pic,legs[i], "fl");

}
leg->Draw();
c1->Print("/home/roreed/workspace/Frameworks/h015FW/TwoFifty/MVA/Plots/cmp_"+tag+".pdf");


}

void drawcompare(TString f1, TString f2, TString picname, TString leg1, TString leg2){
gStyle->SetOptStat(0);
TCanvas *c1=new TCanvas("plot","plot",800,600);
TFile *file1=new TFile(f1);
TH1F *pic1=(TH1F*)file1->Get(picname);
TFile *file2=new TFile(f2);
TH1F *pic2=(TH1F*)file2->Get(picname);

//pic1->Rebin(5);
//pic2->Rebin(5);
pic1->Draw();
pic1->SetLineWidth(2);
pic1->SetLineColor(2);
pic2->Draw("same");
pic2->SetLineWidth(4);
pic2->SetLineColor(4);
pic1->Scale(1/pic1->Integral());
pic2->Scale(1/pic2->Integral());
pic1->GetYaxis()->SetRangeUser(0.001,pic1->GetMaximum()*1.5 );
pic1->GetXaxis()->SetTitle(pic1->GetTitle());

pic1->GetXaxis()->SetTitleOffset(1.2);
cout<<pic1->Integral(13,40)<<endl;
cout<<pic2->Integral(13,40)<<endl;
gPad->SetLogy(5);
TLegend *leg=new TLegend(0.75,0.75,0.95,0.95,"","brNDC");
leg->AddEntry(pic1,leg1, "fl");
leg->AddEntry(pic2,leg2, "fl");
leg->Draw();
c1->Print("figures/val_"+picname+".pdf");


}
void drawratiopic(TString picname_numerator, TString picname_denominator){
gStyle->SetOptStat(0);
TCanvas *c1=new TCanvas("plot","plot",800,600);
TFile *file1=new TFile(filename+"full/hist-sample.root");
TFile *file2=new TFile(filename+"fast/hist-sample.root");
TH1F *pic1=(TH1F*)file1->Get(picname_numerator);
TH1F *pic2=(TH1F*)file2->Get(picname_numerator);
TH1F *pic1d=(TH1F*)file1->Get(picname_denominator);
TH1F *pic2d=(TH1F*)file2->Get(picname_denominator);
TString xlabel=pic1->GetXaxis()->GetTitle();

pic1->Divide(pic1d);
pic2->Divide(pic2d);

//pic1->Scale(1./pic1->Integral());
//pic2->Scale(1./pic2->Integral());
pic1->Draw();
pic2->Draw("same");

pic1->SetLineWidth(2);
pic1->SetLineColor(2);
pic2->SetLineWidth(2);
pic2->SetLineColor(4);
pic1->GetYaxis()->SetRangeUser(0,(pic1->GetMaximum()>pic2->GetMaximum())?pic1->GetMaximum()*1.5:pic2->GetMaximum()*1.5 );
pic1->GetXaxis()->SetTitleOffset(1.2);
TLegend *leg=new TLegend(0.75,0.75,0.95,0.95,"Ratio Plot","brNDC");

leg->AddEntry(pic1,"FullSim", "fl");
leg->AddEntry(pic2,"FastSim", "fl");
leg->Draw();


c1->Print("validation/figures/CompareRatio"+picname_numerator+".pdf");
c1->Print("validation/figures/CompareRatio"+picname_numerator+".png");

}


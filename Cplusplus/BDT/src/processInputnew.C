#define readernew_cxx
#include "src/readernew.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <iostream>
#include <TProfile.h>
#include <fstream>
using namespace std;
double lumi=44.0;
std::map<TString, TH1F*> pics;
TString output;
//bool dosideband=false;
TString fileName;
void PrintProgressBar(int index, int total);
TString dpd;
TString blind;
int main (int argc, char** argv)
{
    TFile *file=new TFile(argv[1]);
    TChain *chain=new TChain("ntuple");
    chain->Add(argv[1]);

    fileName = TString(argv[2]);
    dpd = TString(argv[3]);
    blind = TString(argv[4]);
    std::cout<<"data period: "<<dpd<<endl;
    //fileName.ReplaceAll("Datasets/","");
    output="MVAInputDatasets/"+fileName;
    //dosideband=atoi(argv[3]);
    readernew a(chain);
    a.Loop();

    return 0;
}
void readernew::Loop()
{
    TFile *outfile=new TFile(output, "recreate");

    TTree * tree=new TTree("tree","tree");
    float m_ldvtxsumpt2, m_subvtxsumpt2, m_orgldvtxsumpt2, m_npvx, m_weight, m_part, m_met, m_metsig, m_sumet, m_sample, m_dz, m_dsumpt2, m_m_yy, m_N_j_central, m_hardvtxsumpt2;
    float m_N_j_btag,m_N_leps,m_phpt1,m_phpt2,m_pheta1,m_pheta2, m_mjj, m_myyjj, m_jetpt1, m_jetpt2,m_jeteta1,m_jeteta2,m_N_j,m_ptyy,m_pthard,m_etayy,m_dphiyymet,m_phphi1,m_phphi2;
    float m_jets_rpt1, m_jets_jvf1,m_jets_rpt2, m_jets_jvf2, m_jets_rpt3, m_jets_jvf3,m_jets_rpt4, m_jets_jvf4;
    float m_dphiyy, m_dphiyyj1,m_dphij1met, m_dphisjmet, m_dphifjmet, m_refjet, m_ptjj,m_dphimetj1,m_fjpt,m_sjpt,m_diffrjptjj,m_mu;
    float m_met_sig_hardVertex,m_met_hardVertexTST,m_diff_methd_met,m_diff_methd_metsig;
    

    tree->Branch("diff_methd_met",&m_diff_methd_met);
    tree->Branch("diff_methd_metsig",&m_diff_methd_metsig);

    tree->Branch("met_sig_hardVertex",&m_met_sig_hardVertex);
    tree->Branch("met_hardVertexTST", &m_met_hardVertexTST);
    tree->Branch("Vertex_sumpt2_php", &m_ldvtxsumpt2);
    tree->Branch("Vertex_sumpt2_Pileup", &m_subvtxsumpt2);
    tree->Branch("diff_sumpt2", &m_dsumpt2);
    tree->Branch("Vertex_sumpt2_hard", &m_hardvtxsumpt2);
    tree->Branch("orgldvtxsumpt2", &m_orgldvtxsumpt2);
    tree->Branch("npvx", &m_npvx);
    tree->Branch("weight", &m_weight);
    tree->Branch("part", &m_part);
    tree->Branch("metsig", &m_metsig);
    tree->Branch("met", &m_met);
    tree->Branch("myyjj", &m_myyjj);
    tree->Branch("mjj", &m_mjj);
    tree->Branch("mu", &m_mu);

    tree->Branch("sumet", &m_sumet);
    tree->Branch("sample", &m_sample);
    tree->Branch("dz", &m_dz);
    tree->Branch("m_yy", &m_m_yy);
    tree->Branch("N_j_central", &m_N_j_central);
    tree->Branch("N_j_btag", &m_N_j_btag);
    tree->Branch("N_leps", &m_N_leps);
    tree->Branch("phpt1", &m_phpt1);
    tree->Branch("phpt2", &m_phpt2);
    tree->Branch("pheta1", &m_pheta1);
    tree->Branch("pheta2", &m_pheta2);
    tree->Branch("jetpt1", &m_jetpt1);
    tree->Branch("jets_rpt1", &m_jets_rpt1);
    tree->Branch("jets_jvf1", &m_jets_jvf1);
    tree->Branch("jets_rpt2", &m_jets_rpt2);
    tree->Branch("jets_jvf2", &m_jets_jvf2);
    
    tree->Branch("jets_rpt3", &m_jets_rpt3);
    tree->Branch("jets_jvf3", &m_jets_jvf3);
    tree->Branch("jets_rpt4", &m_jets_rpt4);
    tree->Branch("jets_jvf4", &m_jets_jvf4);
    tree->Branch("jetpt2", &m_jetpt2);
    tree->Branch("jeteta1", &m_jeteta1);
    tree->Branch("jeteta2", &m_jeteta2);
    tree->Branch("ptyy", &m_ptyy);

    tree->Branch("phphi1", &m_phphi1);
    tree->Branch("phphi2", &m_phphi2);

    tree->Branch("etayy", &m_etayy);
    tree->Branch("N_j", &m_N_j);
    tree->Branch("dphiyymet", &m_dphiyymet);
    tree->Branch("pthard", &m_pthard);
    tree->Branch("dphiyy", &m_dphiyy);
    tree->Branch("dphiyyj1", &m_dphiyyj1);
    tree->Branch("dphisjmet", &m_dphisjmet);
    tree->Branch("dphifjmet", &m_dphifjmet);
    tree->Branch("refjet", &m_refjet);
    tree->Branch("ptjj", &m_ptjj);
    tree->Branch("dphij1met", &m_dphimetj1);
    tree->Branch("fjpt", &m_fjpt);
    tree->Branch("sjpt", &m_sjpt);
    tree->Branch("diffrjptjj", &m_diffrjptjj);
    
    TH1F *hist_dphi_php_j1=new TH1F("dphi_php_j1","dphi_php_j1", 50, 0, 3.5);
    TH1F *hist_dphi_pu_j1=new TH1F("dphi_pu_j1","dphi_pu_j1", 50, 0, 3.5);
    TH1F *hist_dphi_php_met=new TH1F("dphi_php_met","dphi_php_met", 50, 0, 3.5);
    TH1F *hist_dphi_pu_met=new TH1F("dphi_pu_met","dphi_pu_met", 50, 0, 3.5);

    TH1F *hist_dR_php_met=new TH1F("dR_php_met","dR_php_met", 50, 0, 10);
    TH1F *hist_dR_pu_met=new TH1F("dR_pu_met","dR_pu_met", 50, 0, 10);

    if (fChain == 0) return;
    Long64_t nentries = fChain->GetEntries();

    std::cout<<"Opening file: "<<fileName<<" and there are: "<<nentries<<" entries.."<<std::endl;
    for (Long64_t jentry=0; jentry<nentries; jentry++)
    {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        fChain->GetEntry(jentry);
       // PrintProgressBar(jentry,nentries);
        //if(dosideband)
	//if(!(m_yy<160&&m_yy>105))continue;
	//if(!(phpt1>40&&phpt2>30))continue;
        TString sname = fChain->GetCurrentFile()->GetName();
        TString filename = sname;
        TString datastr = dpd;
        TString gamjet = "";
        if(filename.Contains("AntiID"))gamjet= "ID";
        if(filename.Contains("AntiISO"))gamjet= "ISO";
        bool passIsoY1 =(*photons_passiso)[0];
        bool passIsoY2 =(*photons_passiso)[1];
        bool passIDY1  =(*photons_passid)[0];
        bool passIDY2  =(*photons_passid)[1];
        if(filename.Contains("AntiISO") && passIsoY2) continue;
        if(filename.Contains("AntiID") && passIDY2) continue;
        //std::cout<<"sample name: "<< sname<<endl;
        
        if(sample==345318 &&met_truth_TST<10.0 )continue;
        //if(sname.Contains("AntiID.root"))lumi=1.0;

        double SF = 1.0;
        if(dpd == "data16")lumi=36.1;     
        if(dpd == "data17")lumi=44.0;
        if(sname.Contains("data") || sname.Contains("Anti"))lumi=1.0;




        double pheta1=(*photons_eta)[0];
        double pheta2=(*photons_eta)[1];
        double phpt1=(*photons_pT)[0];
        double phpt2=(*photons_pT)[1];
        double met=met_TST;
        //TString filename = sname;
        #include "rwt.h"
        
        
        if(!(m_yy<400&&m_yy>120)) continue;
        if(!((*photons_pT)[0]>40&&(*photons_pT)[1]>30))continue;

        //Inv mass cut on Data and ANtiID samples
        //if(sname.Contains("data") && !(m_yy<400&&m_yy>120)) continue;
        //if(sname.Contains("AntiID.root") && !(m_yy<400&&m_yy>120)) continue;
        
        if(sname.Contains("data") && !(sname.Contains("Anti"))&&(m_yy<255&&m_yy>235)&&blind=="1") continue;
        //if(sname.Contains("data") && !(sname.Contains("Anti"))&&(m_yy<122&&m_yy>128)) continue;
        // if(m_yy<255&&m_yy>235) continue;
        if(Vertex_SumPt2_php>100000 || Vertex_SumPt2_php<0)continue;
        if(Vertex_SumPt2_Pileup>100000 || Vertex_SumPt2_Pileup<0)continue;
        if(TMath::IsNaN(Vertex_SumPt2_Pileup)) continue;
        if(TMath::IsNaN(Vertex_SumPt2_php)) continue;
        if(jets_pT->size()>=1){if(jets_rpt->at(0)>10)continue;}
        if(jets_pT->size()>=2){if(jets_rpt->at(1)>10)continue;}
        if(jets_pT->size()>=3){if(jets_rpt->at(2)>10)continue;}
        if(jets_pT->size()>=4){if(jets_rpt->at(3)>10)continue;}
        
        int indff = 1000;
        if(sname.Contains("Anti"))weight_yy_combPRW = 1; 
        if(filename.Contains("sherpa"))SF=SF/lumi;
        m_weight=weight_yy_combPRW*lumi*lumiXsecWeight*SF;
       // if(filename.Contains("sherpa")){
         //  SF=SF/lumi;
           //m_weight=weight_yy_combPRW*lumiXsecWeight*SF;
       // }

        if(sname.Contains("data") && !(sname.Contains("Anti")))m_weight=1;
        if(TMath::IsNaN(m_weight))continue;       
        if(jentry%indff == 0) cout<<filename<<" SF "<<SF<<"  weight: "<<m_weight<<" wcPRW: "<<weight_yy_combPRW<<" lumi: "<<lumi<< " lumXsec: "<<lumiXsecWeight<<endl;
        m_npvx=N_pv;
        m_mu=mu;
        m_ldvtxsumpt2=Vertex_SumPt2_php;

        m_subvtxsumpt2=Vertex_SumPt2_Pileup;
        m_dz=fabs(Vertex_Z_php-Vertex_Z_Pileup);
        m_met_hardVertexTST = met_hardVertexTST;
        m_met_sig_hardVertex = met_sig_hardVertex;
        m_part=jentry%2;
        m_met=met_TST;
        m_metsig=met_sig;

        m_sumet=sumet_TST;
        m_dsumpt2=m_ldvtxsumpt2-m_subvtxsumpt2;
        m_diff_methd_metsig = met_sig_hardVertex-met_sig;
        m_diff_methd_met = met_sig_hardVertex - met_TST;
        m_m_yy=m_yy;
        m_N_j_central=N_j_central;
        m_N_j_btag=N_j_btag;
        m_N_leps=N_leps;
        m_phpt1=(*photons_pT)[0];
        m_phpt2=(*photons_pT)[1];
        m_pheta1=(*photons_eta)[0];
        m_pheta2=(*photons_eta)[1];
        m_phphi1=(*photons_phi)[0];
        m_phphi2=(*photons_phi)[1];
        
        int added1=0;
        int added2=0;
        TLorentzVector metv;
        metv.SetPtEtaPhiM(met_TST, 0, phi_TST,0);

        TLorentzVector ph1,ph2,myyjj,myyjjjj,myy,mjj,mjjorg,mtyyjjmet, mtyymet,mtjjmet;
        ph1.SetPtEtaPhiM((*photons_pT)[0],(*photons_eta)[0],(*photons_phi)[0],0);
        ph2.SetPtEtaPhiM((*photons_pT)[1],(*photons_eta)[1],(*photons_phi)[1],0);
	
        TLorentzVector jet20,forwardjets, softjets, dijet,dijet20, j1;
        std::vector<TLorentzVector> vec;
        int ncj = 0;
        for(int i=0;i<jets_pT->size();i++){
          TLorentzVector jettmp,tmptrackjet;
          jettmp.SetPtEtaPhiM(jets_pT->at(i),jets_eta->at(i), jets_phi->at(i),jets_m->at(i));
          vec.push_back(jettmp);
          if(jets_pT->at(i)>20)jet20+=jettmp;
          if(jets_pT->at(i)>20&&fabs(jets_eta->at(i))>2.5){forwardjets+=jettmp;}
          if(jets_pT->at(i)>20&&fabs(jets_eta->at(i))<2.5&&jets_pT->at(i)<30){softjets+=jettmp;}
          if(jets_pT->at(i)>25){
            if(fabs(jets_eta->at(i))<2.5){
              ncj++;
              if(ncj<=2)dijet+=jettmp;
            }
         }
         if(fabs(jets_eta->at(i))<2.5&&added1<2){
           myyjj+=jettmp;added1++;mjj+=jettmp;mtyyjjmet+=jettmp;mtjjmet+=jettmp;
        }
        if(fabs(jets_eta->at(i))<2.5&&added2<4){myyjjjj+=jettmp;added2++;}
      }
      if(vec.size()>0)j1=vec[0];

        


     m_ptyy=pT_yy;
     m_etayy=(ph1+ph2).Eta();
     m_N_j=N_j;
     m_dphiyymet=fabs((ph1+ph2).DeltaPhi(metv));
     m_pthard=pT_hard;
     m_dphiyy=fabs(ph1.DeltaPhi(ph2));

     if(forwardjets.Pt()>0){m_dphifjmet=fabs(forwardjets.DeltaPhi(metv));}else{m_dphifjmet=-1;}
     if(softjets.Pt()>0){m_dphisjmet=fabs(softjets.DeltaPhi(metv));}else{m_dphisjmet=-1;}
     m_fjpt=forwardjets.Pt();
     m_sjpt=softjets.Pt();
     if(j1.Pt()>=0){
      m_dphimetj1=fabs(metv.DeltaPhi(j1));
      m_dphiyyj1=fabs(j1.DeltaPhi(ph1+ph2));
    }
    else {m_dphimetj1=-1;m_dphiyyj1=-1;}


     m_refjet=RefJets;
     m_ptjj=dijet.Pt();
     m_diffrjptjj = RefJets -dijet.Pt();
     if(jets_pT->size()>=1)m_jetpt1=jets_pT->at(0);
     else m_jetpt1=0;
     if(jets_pT->size()>=2)m_jetpt2=jets_pT->at(1);
     else m_jetpt2=0;
     if(jets_pT->size()>=1)m_jeteta1=jets_eta->at(0);
     else m_jeteta1=0;
     if(jets_pT->size()>=2)m_jeteta2=jets_eta->at(1);
     else m_jeteta2=0;
     
     //if(jets_rpt->at(0)<10||jets_rpt->at(1)<10||jets_rpt->at(2)<10||jets_rpt->at(3)<<10)continue;

     if(jets_pT->size()>=1)m_jets_rpt1=jets_rpt->at(0);
     else m_jets_rpt1=-1;
     if(jets_pT->size()>=2)m_jets_rpt2=jets_rpt->at(1);
     else m_jets_rpt2=-1;
     if(jets_pT->size()>=3)m_jets_rpt3=jets_rpt->at(2);
     else m_jets_rpt3=-1;
     if(jets_pT->size()>=4)m_jets_rpt4=jets_rpt->at(3);
     else m_jets_rpt4=-1;
			
     if(jets_pT->size()>=1)m_jets_jvf1=jets_jvf->at(0);
     else m_jets_jvf1=-1;
     if(jets_pT->size()>=2)m_jets_jvf2=jets_jvf->at(1);
     else m_jets_jvf2=-1;
     if(jets_pT->size()>=3)m_jets_jvf3=jets_jvf->at(2);
     else m_jets_jvf3=-1;
     if(jets_pT->size()>=4)m_jets_jvf4=jets_jvf->at(3);
     else m_jets_jvf4=-1;


        tree->Fill();
    }

    outfile->Write();
    outfile->Close();
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

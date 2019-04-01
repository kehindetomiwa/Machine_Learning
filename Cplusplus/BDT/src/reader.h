//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jan  1 15:10:48 2017 by ROOT version 6.04/16
// from TTree ntuple/ntuple
// found on file: /afs/cern.ch/user/r/ruanxf/work/public/V14/gghjvt.root
//////////////////////////////////////////////////////////

#ifndef reader_h
#define reader_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
using namespace std;
class reader {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           lb;
   Int_t           mu;
   Int_t           N_pv;
   Int_t           run;
   Int_t           event;
   Int_t           sample;
   Double_t        Vertex_Z_php;
   Double_t        Vertex_Z_hard;
   Double_t        Vertex_Z_truth;
   Double_t        Vertex_SumPt2_php;
   Double_t        Vertex_SumPt2_hard;
   Double_t        Vertex_Z_PU;
   Double_t        Vertex_SumPt2_PU;
   Double_t        Vertex_pT_php;
   Double_t        Vertex_eta_php;
   Double_t        Vertex_phi_php;
   Double_t        Vertex_pT_Pileup;
   Double_t        Vertex_eta_Pileup;
   Double_t        Vertex_phi_Pileup;

   Double_t        met_x;
   Double_t        met_y;
   Double_t        pT_hard;
   Double_t        met_TST;
   Double_t        RefGamma;
   Double_t        RefEle;
   Double_t        RefMuons;
   Double_t        RefJets;
   Double_t        pvSoftTrack_TST;
   Double_t        phi_TST;
   Double_t        phi_PVSoftTrk;
   Double_t        sumet_TST;
   Double_t        met_sig;
   Double_t        met_truth_TST;
   Double_t        met_truth_sig;
   Double_t        met_hardVertexTST;
   Double_t        RefGamma_hardVertexTST;
   Double_t        RefEle_hardVertexTST;
   Double_t        RefMuons_hardVertexTST;
   Double_t        RefJets_hardVertexTST;
   Double_t        pvSoftTrack_hardVertexTST;
   Double_t        phi_hardVertexTST;
   Double_t        sumet_hardVertexTST;
   Double_t        met_sig_hardVertex;
   Double_t        phi_PVSoftTrk_hardVertexTST;
   vector<double>  *photons_pT;
   vector<double>  *photons_eta;
   vector<double>  *photons_phi;
   vector<double>  *photons_ptCone20;
   vector<double>  *photons_ptCone40;
   vector<double>  *photons_topoEtCone20;
   vector<double>  *photons_topoEtCone40;
   vector<double>  *photons_Rhad;
   vector<double>  *photons_Reta;
   vector<double>  *photons_weta2;
   vector<double>  *photons_Rphi;
   vector<double>  *photons_wtots1;
   vector<double>  *photons_DeltaE;
   vector<double>  *photons_Eratio;
   vector<double>  *photons_f1;
   vector<double>  *photons_cl_Es0;
   vector<double>  *photons_cl_Es1;
   vector<double>  *photons_cl_Es2;
   vector<double>  *photons_cl_Es3;
   vector<double>  *photons_cl_E;
   vector<double>  *photons_cl_etaCalo;
   vector<double>  *photons_cl_phiCalo;
   vector<double>  *photons_cl_eta;
   Double_t        m_yy;
   Double_t        pT_yy;
   Int_t           convType_y1;
   Int_t           convType_y2;
   vector<double>  *jets_pT;
   vector<double>  *jets_eta;
   vector<double>  *jets_phi;
   vector<double>  *jets_m;
   vector<double>  *jets_jvt;
   Int_t           N_j;
   Int_t           N_j_central;
   Int_t           N_j_btag;
   Int_t           N_leps;
   Double_t        weight_EightSet;
   Double_t        weight_MoriondSet;
   Double_t        weight_ThreeJetSet;
   Int_t           cat_EightSet;
   Int_t           cat_MoriondSet;
   Int_t           cat_ThreeJetSet;
   Double_t        lumiXsecWeight;
   Double_t        generatedMass;
   Double_t        weight_yy_combPRW;
   Double_t        weight_yy_combPRWnoTor;
   Double_t        weight_yy_15PRW;
   Double_t        weight_yy_16PRW;
   Char_t          isShellySR;
   Char_t          isHiggsSR;
   Char_t          isInclSR;
   Char_t          isSideBand;

   // List of branches
   TBranch        *b_lb;   //!
   TBranch        *b_mu;   //!
   TBranch        *b_N_pv;   //!
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_sample;   //!
   TBranch        *b_Vertex_Z_php;   //!
   TBranch        *b_Vertex_Z_hard;   //!
   TBranch        *b_Vertex_Z_truth;   //!
   TBranch        *b_Vertex_SumPt2_php;   //!
   TBranch        *b_Vertex_SumPt2_hard;   //!
   TBranch        *b_Vertex_Z_PU;   //!
   TBranch        *b_Vertex_SumPt2_PU;   //!
   TBranch        *b_Vertex_pT_php; //!
   TBranch        *b_Vertex_eta_php; //!
   TBranch        *b_Vertex_phi_php; //!
   TBranch        *b_Vertex_pT_Pileup; //!
   TBranch        *b_Vertex_eta_Pileup; //!
   TBranch        *b_Vertex_phi_Pileup; //!

   TBranch        *b_met_x;   //!
   TBranch        *b_met_y;   //!
   TBranch        *b_pT_hard;   //!
   TBranch        *b_met_TST;   //!
   TBranch        *b_RefGamma;   //!
   TBranch        *b_RefEle;   //!
   TBranch        *b_RefMuons;   //!
   TBranch        *b_RefJets;   //!
   TBranch        *b_pvSoftTrack_TST;   //!
   TBranch        *b_phi_TST;   //!
   TBranch        *b_phi_PVSoftTrk;   //!
   TBranch        *b_sumet_TST;   //!
   TBranch        *b_met_sig;   //!
   TBranch        *b_met_truth_TST;   //!
   TBranch        *b_met_truth_sig;   //!
   TBranch        *b_met_hardVertexTST;   //!
   TBranch        *b_RefGamma_hardVertexTST;   //!
   TBranch        *b_RefEle_hardVertexTST;   //!
   TBranch        *b_RefMuons_hardVertexTST;   //!
   TBranch        *b_RefJets_hardVertexTST;   //!
   TBranch        *b_pvSoftTrack_hardVertexTST;   //!
   TBranch        *b_phi_hardVertexTST;   //!
   TBranch        *b_sumet_hardVertexTST;   //!
   TBranch        *b_met_sig_hardVertex;   //!
   TBranch        *b_phi_PVSoftTrk_hardVertexTST;   //!
   TBranch        *b_photons_pT;   //!
   TBranch        *b_photons_eta;   //!
   TBranch        *b_photons_phi;   //!
   TBranch        *b_photons_ptCone20;   //!
   TBranch        *b_photons_ptCone40;   //!
   TBranch        *b_photons_topoEtCone20;   //!
   TBranch        *b_photons_topoEtCone40;   //!
   TBranch        *b_photons_Rhad;   //!
   TBranch        *b_photons_Reta;   //!
   TBranch        *b_photons_weta2;   //!
   TBranch        *b_photons_Rphi;   //!
   TBranch        *b_photons_wtots1;   //!
   TBranch        *b_photons_DeltaE;   //!
   TBranch        *b_photons_Eratio;   //!
   TBranch        *b_photons_f1;   //!
   TBranch        *b_photons_cl_Es0;   //!
   TBranch        *b_photons_cl_Es1;   //!
   TBranch        *b_photons_cl_Es2;   //!
   TBranch        *b_photons_cl_Es3;   //!
   TBranch        *b_photons_cl_E;   //!
   TBranch        *b_photons_cl_etaCalo;   //!
   TBranch        *b_photons_cl_phiCalo;   //!
   TBranch        *b_photons_cl_eta;   //!
   TBranch        *b_m_yy;   //!
   TBranch        *b_pT_yy;   //!
   TBranch        *b_convType_y1;   //!
   TBranch        *b_convType_y2;   //!
   TBranch        *b_jets_pT;   //!
   TBranch        *b_jets_eta;   //!
   TBranch        *b_jets_phi;   //!
   TBranch        *b_jets_m;   //!
   TBranch        *b_jets_jvt;   //!
   TBranch        *b_N_j;   //!
   TBranch        *b_N_j_central;   //!
   TBranch        *b_N_j_btag;   //!
   TBranch        *b_N_leps;   //!
   TBranch        *b_weight_EightSet;   //!
   TBranch        *b_weight_MoriondSet;   //!
   TBranch        *b_weight_ThreeJetSet;   //!
   TBranch        *b_cat_EightSet;   //!
   TBranch        *b_cat_MoriondSet;   //!
   TBranch        *b_cat_ThreeJetSet;   //!
   TBranch        *b_lumiXsecWeight;   //!
   TBranch        *b_generatedMass;   //!
   TBranch        *b_weight_yy_combPRW;   //!
   TBranch        *b_weight_yy_combPRWnoTor;   //!
   TBranch        *b_weight_yy_15PRW;   //!
   TBranch        *b_weight_yy_16PRW;   //!
   TBranch        *b_isShellySR;   //!
   TBranch        *b_isHiggsSR;   //!
   TBranch        *b_isInclSR;   //!
   TBranch        *b_isSideBand;   //!

   reader(TTree *tree=0);
   virtual ~reader();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef reader_cxx
reader::reader(TTree *tree) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/afs/cern.ch/user/r/ruanxf/work/public/V14/gghjvt.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/afs/cern.ch/user/r/ruanxf/work/public/V14/gghjvt.root");
      }
      f->GetObject("ntuple",tree);

   }
   Init(tree);
}

reader::~reader()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t reader::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t reader::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void reader::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   photons_pT = 0;
   photons_eta = 0;
   photons_phi = 0;
   photons_ptCone20 = 0;
   photons_ptCone40 = 0;
   photons_topoEtCone20 = 0;
   photons_topoEtCone40 = 0;
   photons_Rhad = 0;
   photons_Reta = 0;
   photons_weta2 = 0;
   photons_Rphi = 0;
   photons_wtots1 = 0;
   photons_DeltaE = 0;
   photons_Eratio = 0;
   photons_f1 = 0;
   photons_cl_Es0 = 0;
   photons_cl_Es1 = 0;
   photons_cl_Es2 = 0;
   photons_cl_Es3 = 0;
   photons_cl_E = 0;
   photons_cl_etaCalo = 0;
   photons_cl_phiCalo = 0;
   photons_cl_eta = 0;
   jets_pT = 0;
   jets_eta = 0;
   jets_phi = 0;
   jets_m = 0;
   jets_jvt = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("lb", &lb, &b_lb);
   fChain->SetBranchAddress("mu", &mu, &b_mu);
   fChain->SetBranchAddress("N_pv", &N_pv, &b_N_pv);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("sample", &sample, &b_sample);
   fChain->SetBranchAddress("Vertex_Z_php", &Vertex_Z_php, &b_Vertex_Z_php);
   fChain->SetBranchAddress("Vertex_Z_hard", &Vertex_Z_hard, &b_Vertex_Z_hard);
   fChain->SetBranchAddress("Vertex_Z_truth", &Vertex_Z_truth, &b_Vertex_Z_truth);
   fChain->SetBranchAddress("Vertex_SumPt2_php", &Vertex_SumPt2_php, &b_Vertex_SumPt2_php);
   fChain->SetBranchAddress("Vertex_SumPt2_hard", &Vertex_SumPt2_hard, &b_Vertex_SumPt2_hard);
   fChain->SetBranchAddress("Vertex_Z_Pileup", &Vertex_Z_PU, &b_Vertex_Z_PU);
   fChain->SetBranchAddress("Vertex_SumPt2_Pileup", &Vertex_SumPt2_PU, &b_Vertex_SumPt2_PU);
   fChain->SetBranchAddress("Vertex_pT_php", &Vertex_pT_php, &b_Vertex_pT_php);
   fChain->SetBranchAddress("Vertex_eta_php", &Vertex_eta_php, &b_Vertex_eta_php);
   fChain->SetBranchAddress("Vertex_phi_php", &Vertex_phi_php, &b_Vertex_phi_php);
   fChain->SetBranchAddress("Vertex_pT_Pileup", &Vertex_pT_Pileup, &b_Vertex_pT_Pileup);
   fChain->SetBranchAddress("Vertex_eta_Pileup", &Vertex_eta_Pileup, &b_Vertex_eta_Pileup);
   fChain->SetBranchAddress("Vertex_phi_Pileup", &Vertex_phi_Pileup, &b_Vertex_phi_Pileup);
   fChain->SetBranchAddress("met_x", &met_x, &b_met_x);
   fChain->SetBranchAddress("met_y", &met_y, &b_met_y);
   fChain->SetBranchAddress("pT_hard", &pT_hard, &b_pT_hard);
   fChain->SetBranchAddress("met_TST", &met_TST, &b_met_TST);
   fChain->SetBranchAddress("RefGamma", &RefGamma, &b_RefGamma);
   fChain->SetBranchAddress("RefEle", &RefEle, &b_RefEle);
   fChain->SetBranchAddress("RefMuons", &RefMuons, &b_RefMuons);
   fChain->SetBranchAddress("RefJets", &RefJets, &b_RefJets);
   fChain->SetBranchAddress("pvSoftTrack_TST", &pvSoftTrack_TST, &b_pvSoftTrack_TST);
   fChain->SetBranchAddress("phi_TST", &phi_TST, &b_phi_TST);
   fChain->SetBranchAddress("phi_PVSoftTrk", &phi_PVSoftTrk, &b_phi_PVSoftTrk);
   fChain->SetBranchAddress("sumet_TST", &sumet_TST, &b_sumet_TST);
   fChain->SetBranchAddress("met_sig", &met_sig, &b_met_sig);
   fChain->SetBranchAddress("met_truth_TST", &met_truth_TST, &b_met_truth_TST);
   fChain->SetBranchAddress("met_truth_sig", &met_truth_sig, &b_met_truth_sig);
   fChain->SetBranchAddress("met_hardVertexTST", &met_hardVertexTST, &b_met_hardVertexTST);
   fChain->SetBranchAddress("RefGamma_hardVertexTST", &RefGamma_hardVertexTST, &b_RefGamma_hardVertexTST);
   fChain->SetBranchAddress("RefEle_hardVertexTST", &RefEle_hardVertexTST, &b_RefEle_hardVertexTST);
   fChain->SetBranchAddress("RefMuons_hardVertexTST", &RefMuons_hardVertexTST, &b_RefMuons_hardVertexTST);
   fChain->SetBranchAddress("RefJets_hardVertexTST", &RefJets_hardVertexTST, &b_RefJets_hardVertexTST);
   fChain->SetBranchAddress("pvSoftTrack_hardVertexTST", &pvSoftTrack_hardVertexTST, &b_pvSoftTrack_hardVertexTST);
   fChain->SetBranchAddress("phi_hardVertexTST", &phi_hardVertexTST, &b_phi_hardVertexTST);
   fChain->SetBranchAddress("sumet_hardVertexTST", &sumet_hardVertexTST, &b_sumet_hardVertexTST);
   fChain->SetBranchAddress("met_sig_hardVertex", &met_sig_hardVertex, &b_met_sig_hardVertex);
   fChain->SetBranchAddress("phi_PVSoftTrk_hardVertexTST", &phi_PVSoftTrk_hardVertexTST, &b_phi_PVSoftTrk_hardVertexTST);
   fChain->SetBranchAddress("photons_pT", &photons_pT, &b_photons_pT);
   fChain->SetBranchAddress("photons_eta", &photons_eta, &b_photons_eta);
   fChain->SetBranchAddress("photons_phi", &photons_phi, &b_photons_phi);
   fChain->SetBranchAddress("photons_ptCone20", &photons_ptCone20, &b_photons_ptCone20);
   fChain->SetBranchAddress("photons_ptCone40", &photons_ptCone40, &b_photons_ptCone40);
   fChain->SetBranchAddress("photons_topoEtCone20", &photons_topoEtCone20, &b_photons_topoEtCone20);
   fChain->SetBranchAddress("photons_topoEtCone40", &photons_topoEtCone40, &b_photons_topoEtCone40);
   fChain->SetBranchAddress("photons_Rhad", &photons_Rhad, &b_photons_Rhad);
   fChain->SetBranchAddress("photons_Reta", &photons_Reta, &b_photons_Reta);
   fChain->SetBranchAddress("photons_weta2", &photons_weta2, &b_photons_weta2);
   fChain->SetBranchAddress("photons_Rphi", &photons_Rphi, &b_photons_Rphi);
   fChain->SetBranchAddress("photons_wtots1", &photons_wtots1, &b_photons_wtots1);
   fChain->SetBranchAddress("photons_DeltaE", &photons_DeltaE, &b_photons_DeltaE);
   fChain->SetBranchAddress("photons_Eratio", &photons_Eratio, &b_photons_Eratio);
   fChain->SetBranchAddress("photons_f1", &photons_f1, &b_photons_f1);
   fChain->SetBranchAddress("photons_cl_Es0", &photons_cl_Es0, &b_photons_cl_Es0);
   fChain->SetBranchAddress("photons_cl_Es1", &photons_cl_Es1, &b_photons_cl_Es1);
   fChain->SetBranchAddress("photons_cl_Es2", &photons_cl_Es2, &b_photons_cl_Es2);
   fChain->SetBranchAddress("photons_cl_Es3", &photons_cl_Es3, &b_photons_cl_Es3);
   fChain->SetBranchAddress("photons_cl_E", &photons_cl_E, &b_photons_cl_E);
   fChain->SetBranchAddress("photons_cl_etaCalo", &photons_cl_etaCalo, &b_photons_cl_etaCalo);
   fChain->SetBranchAddress("photons_cl_phiCalo", &photons_cl_phiCalo, &b_photons_cl_phiCalo);
   fChain->SetBranchAddress("photons_cl_eta", &photons_cl_eta, &b_photons_cl_eta);
   fChain->SetBranchAddress("m_yy", &m_yy, &b_m_yy);
   fChain->SetBranchAddress("pT_yy", &pT_yy, &b_pT_yy);
   fChain->SetBranchAddress("convType_y1", &convType_y1, &b_convType_y1);
   fChain->SetBranchAddress("convType_y2", &convType_y2, &b_convType_y2);
   fChain->SetBranchAddress("jets_pT", &jets_pT, &b_jets_pT);
   fChain->SetBranchAddress("jets_eta", &jets_eta, &b_jets_eta);
   fChain->SetBranchAddress("jets_phi", &jets_phi, &b_jets_phi);
   fChain->SetBranchAddress("jets_m", &jets_m, &b_jets_m);
   //fChain->SetBranchAddress("jets_jvt", &jets_jvt, &b_jets_jvt);
   fChain->SetBranchAddress("N_j", &N_j, &b_N_j);
   fChain->SetBranchAddress("N_j_central", &N_j_central, &b_N_j_central);
   fChain->SetBranchAddress("N_j_btag", &N_j_btag, &b_N_j_btag);
   fChain->SetBranchAddress("N_leps", &N_leps, &b_N_leps);
   fChain->SetBranchAddress("weight_EightSet", &weight_EightSet, &b_weight_EightSet);
   fChain->SetBranchAddress("weight_MoriondSet", &weight_MoriondSet, &b_weight_MoriondSet);
   fChain->SetBranchAddress("weight_ThreeJetSet", &weight_ThreeJetSet, &b_weight_ThreeJetSet);
   fChain->SetBranchAddress("cat_EightSet", &cat_EightSet, &b_cat_EightSet);
   fChain->SetBranchAddress("cat_MoriondSet", &cat_MoriondSet, &b_cat_MoriondSet);
   fChain->SetBranchAddress("cat_ThreeJetSet", &cat_ThreeJetSet, &b_cat_ThreeJetSet);
   fChain->SetBranchAddress("lumiXsecWeight", &lumiXsecWeight, &b_lumiXsecWeight);
   fChain->SetBranchAddress("generatedMass", &generatedMass, &b_generatedMass);
   fChain->SetBranchAddress("weight_yy_combPRW", &weight_yy_combPRW, &b_weight_yy_combPRW);
   fChain->SetBranchAddress("weight_yy_combPRWnoTor", &weight_yy_combPRWnoTor, &b_weight_yy_combPRWnoTor);
   fChain->SetBranchAddress("weight_yy_15PRW", &weight_yy_15PRW, &b_weight_yy_15PRW);
   fChain->SetBranchAddress("weight_yy_16PRW", &weight_yy_16PRW, &b_weight_yy_16PRW);
   fChain->SetBranchAddress("isShellySR", &isShellySR, &b_isShellySR);
   fChain->SetBranchAddress("isHiggsSR", &isHiggsSR, &b_isHiggsSR);
   fChain->SetBranchAddress("isInclSR", &isInclSR, &b_isInclSR);
   fChain->SetBranchAddress("isSideBand", &isSideBand, &b_isSideBand);
   Notify();
}

Bool_t reader::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void reader::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t reader::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef reader_cxx

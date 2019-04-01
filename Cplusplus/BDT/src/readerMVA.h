//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Aug 25 11:12:29 2017 by ROOT version 5.34/36
// from TTree tree/tree
// found on file: /afs/cern.ch/work/k/ktomiwa/public/PostICEP/BDT/NewVer/MVA/MVAInputDatasets/data.root
//////////////////////////////////////////////////////////

#ifndef readerMVA_h
#define readerMVA_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class readerMVA {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         dphi_php_j1;
   Float_t         dphi_php_j2;
   Float_t         dphi_pu_j1;
   Float_t         dphi_pu_j2;
   Float_t         dphi_php_met;
   Float_t         dphi_pu_met;
   Float_t         dR_php_met;
   Float_t         dR_pu_met;
   Float_t         dR_php_j1;
   Float_t         dR_php_j2;
   Float_t         dR_pu_j1;
   Float_t         dR_pu_j2;
   Float_t         Vertex_sumpt2_php;
   Float_t         Vertex_sumpt2_Pileup;
   Float_t         diff_sumpt2;
   Float_t         Vertex_sumpt2_hard;
   Double_t        Vertex_pT_php;
   Double_t        Vertex_eta_php;
   Double_t        Vertex_phi_php;
   Double_t        Vertex_pT_Pileup;
   Double_t        Vertex_eta_Pileup;
   Double_t        Vertex_phi_Pileup;
   Float_t         orgldvtxsumpt2;
   Float_t         npvx;
   Float_t         weight;
   Float_t         part;
   Float_t         metsig;
   Float_t         met;
   Float_t         met2;
   Float_t         myyjj;
   Float_t         mjj;
   Float_t         sumet;
   Float_t         sample;
   Float_t         dz;
   Float_t         m_yy;
   Float_t         N_j_central;
   Float_t         N_j_btag;
   Float_t         N_leps;
   Float_t         phpt1;
   Float_t         phpt2;
   Float_t         pheta1;
   Float_t         pheta2;
   Float_t         jetpt1;
   Float_t         jetpt2;
   Float_t         jeteta1;
   Float_t         jeteta2;
   Float_t         ptyy;
   Float_t         etayy;
   Float_t         N_j;
   Float_t         dphiyymet;
   Float_t         pthard;

   // List of branches
   TBranch        *b_dphi_php_j1;   //!
   TBranch        *b_dphi_php_j2;   //!
   TBranch        *b_dphi_pu_j1;   //!
   TBranch        *b_dphi_pu_j2;   //!
   TBranch        *b_dphi_php_met;   //!
   TBranch        *b_dphi_pu_met;   //!
   TBranch        *b_dR_php_met;   //!
   TBranch        *b_dR_pu_met;   //!
   TBranch        *b_dR_php_j1;   //!
   TBranch        *b_dR_php_j2;   //!
   TBranch        *b_dR_pu_j1;   //!
   TBranch        *b_dR_pu_j2;   //!
   TBranch        *b_Vertex_sumpt2_php;   //!
   TBranch        *b_Vertex_sumpt2_Pileup;   //!
   TBranch        *b_diff_sumpt2;   //!
   TBranch        *b_Vertex_sumpt2_hard;   //!
   TBranch        *b_Vertex_pT_php;   //!
   TBranch        *b_Vertex_eta_php;   //!
   TBranch        *b_Vertex_phi_php;   //!
   TBranch        *b_Vertex_pT_Pileup;   //!
   TBranch        *b_Vertex_eta_Pileup;   //!
   TBranch        *b_Vertex_phi_Pileup;   //!
   TBranch        *b_orgldvtxsumpt2;   //!
   TBranch        *b_npvx;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_part;   //!
   TBranch        *b_metsig;   //!
   TBranch        *b_met;   //!
   TBranch        *b_met2;   //!
   TBranch        *b_myyjj;   //!
   TBranch        *b_mjj;   //!
   TBranch        *b_sumet;   //!
   TBranch        *b_sample;   //!
   TBranch        *b_dz;   //!
   TBranch        *b_m_yy;   //!
   TBranch        *b_N_j_central;   //!
   TBranch        *b_N_j_btag;   //!
   TBranch        *b_N_leps;   //!
   TBranch        *b_phpt1;   //!
   TBranch        *b_phpt2;   //!
   TBranch        *b_pheta1;   //!
   TBranch        *b_pheta2;   //!
   TBranch        *b_jetpt1;   //!
   TBranch        *b_jetpt2;   //!
   TBranch        *b_jeteta1;   //!
   TBranch        *b_jeteta2;   //!
   TBranch        *b_ptyy;   //!
   TBranch        *b_etayy;   //!
   TBranch        *b_N_j;   //!
   TBranch        *b_dphiyymet;   //!
   TBranch        *b_pthard;   //!

   readerMVA(TTree *tree=0);
   virtual ~readerMVA();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef readerMVA_cxx
readerMVA::readerMVA(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/afs/cern.ch/work/k/ktomiwa/public/PostICEP/BDT/NewVer/MVA/MVAInputDatasets/data.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/afs/cern.ch/work/k/ktomiwa/public/PostICEP/BDT/NewVer/MVA/MVAInputDatasets/data.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

readerMVA::~readerMVA()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t readerMVA::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t readerMVA::LoadTree(Long64_t entry)
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

void readerMVA::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("dphi_php_j1", &dphi_php_j1, &b_dphi_php_j1);
   fChain->SetBranchAddress("dphi_php_j2", &dphi_php_j2, &b_dphi_php_j2);
   fChain->SetBranchAddress("dphi_pu_j1", &dphi_pu_j1, &b_dphi_pu_j1);
   fChain->SetBranchAddress("dphi_pu_j2", &dphi_pu_j2, &b_dphi_pu_j2);
   fChain->SetBranchAddress("dphi_php_met", &dphi_php_met, &b_dphi_php_met);
   fChain->SetBranchAddress("dphi_pu_met", &dphi_pu_met, &b_dphi_pu_met);
   fChain->SetBranchAddress("dR_php_met", &dR_php_met, &b_dR_php_met);
   fChain->SetBranchAddress("dR_pu_met", &dR_pu_met, &b_dR_pu_met);
   fChain->SetBranchAddress("dR_php_j1", &dR_php_j1, &b_dR_php_j1);
   fChain->SetBranchAddress("dR_php_j2", &dR_php_j2, &b_dR_php_j2);
   fChain->SetBranchAddress("dR_pu_j1", &dR_pu_j1, &b_dR_pu_j1);
   fChain->SetBranchAddress("dR_pu_j2", &dR_pu_j2, &b_dR_pu_j2);
   fChain->SetBranchAddress("Vertex_sumpt2_php", &Vertex_sumpt2_php, &b_Vertex_sumpt2_php);
   fChain->SetBranchAddress("Vertex_sumpt2_Pileup", &Vertex_sumpt2_Pileup, &b_Vertex_sumpt2_Pileup);
   fChain->SetBranchAddress("diff_sumpt2", &diff_sumpt2, &b_diff_sumpt2);
   fChain->SetBranchAddress("Vertex_sumpt2_hard", &Vertex_sumpt2_hard, &b_Vertex_sumpt2_hard);
   fChain->SetBranchAddress("Vertex_pT_php", &Vertex_pT_php, &b_Vertex_pT_php);
   fChain->SetBranchAddress("Vertex_eta_php", &Vertex_eta_php, &b_Vertex_eta_php);
   fChain->SetBranchAddress("Vertex_phi_php", &Vertex_phi_php, &b_Vertex_phi_php);
   fChain->SetBranchAddress("Vertex_pT_Pileup", &Vertex_pT_Pileup, &b_Vertex_pT_Pileup);
   fChain->SetBranchAddress("Vertex_eta_Pileup", &Vertex_eta_Pileup, &b_Vertex_eta_Pileup);
   fChain->SetBranchAddress("Vertex_phi_Pileup", &Vertex_phi_Pileup, &b_Vertex_phi_Pileup);
   fChain->SetBranchAddress("orgldvtxsumpt2", &orgldvtxsumpt2, &b_orgldvtxsumpt2);
   fChain->SetBranchAddress("npvx", &npvx, &b_npvx);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("part", &part, &b_part);
   fChain->SetBranchAddress("metsig", &metsig, &b_metsig);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("met2", &met2, &b_met2);
   fChain->SetBranchAddress("myyjj", &myyjj, &b_myyjj);
   fChain->SetBranchAddress("mjj", &mjj, &b_mjj);
   fChain->SetBranchAddress("sumet", &sumet, &b_sumet);
   fChain->SetBranchAddress("sample", &sample, &b_sample);
   fChain->SetBranchAddress("dz", &dz, &b_dz);
   fChain->SetBranchAddress("m_yy", &m_yy, &b_m_yy);
   fChain->SetBranchAddress("N_j_central", &N_j_central, &b_N_j_central);
   fChain->SetBranchAddress("N_j_btag", &N_j_btag, &b_N_j_btag);
   fChain->SetBranchAddress("N_leps", &N_leps, &b_N_leps);
   fChain->SetBranchAddress("phpt1", &phpt1, &b_phpt1);
   fChain->SetBranchAddress("phpt2", &phpt2, &b_phpt2);
   fChain->SetBranchAddress("pheta1", &pheta1, &b_pheta1);
   fChain->SetBranchAddress("pheta2", &pheta2, &b_pheta2);
   fChain->SetBranchAddress("jetpt1", &jetpt1, &b_jetpt1);
   fChain->SetBranchAddress("jetpt2", &jetpt2, &b_jetpt2);
   fChain->SetBranchAddress("jeteta1", &jeteta1, &b_jeteta1);
   fChain->SetBranchAddress("jeteta2", &jeteta2, &b_jeteta2);
   fChain->SetBranchAddress("ptyy", &ptyy, &b_ptyy);
   fChain->SetBranchAddress("etayy", &etayy, &b_etayy);
   fChain->SetBranchAddress("N_j", &N_j, &b_N_j);
   fChain->SetBranchAddress("dphiyymet", &dphiyymet, &b_dphiyymet);
   fChain->SetBranchAddress("pthard", &pthard, &b_pthard);
   Notify();
}

Bool_t readerMVA::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void readerMVA::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t readerMVA::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef readerMVA_cxx

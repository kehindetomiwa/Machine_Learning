#define readerMVA_cxx
//#include "src/reader.h"
#include "src/readerMVA.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <iostream>
#include <TProfile.h>
#include <fstream>
using namespace std;
double lumi=36.5;
std::map<TString, TH1F*> pics;
TString output;
bool dojvt=false;
std::vector<TString> mvaNames;
std::map<TString,float> m_mvaValues;
TString fileName;
std::map<TString, std::ofstream*> m_yieldFiles; //!

void PrintProgressBar(int index, int total);

int main (int argc, char** argv)
{
    std::cout<<"Starting data yields..."<<std::endl;
    TFile *file=new TFile(argv[1]);
    TChain *chain=new TChain("tree");
    chain->Add(argv[1]);
    std::cout<<"Added file to chain..."<<std::endl;

    std::cout<<"Creating reader..."<<std::endl;
    readerMVA a(chain);

    std::cout<<"Searching for BDT variables with met_MVA suffix:"<<std::endl;

    TIter next(chain->GetListOfBranches());
    TBranch *branch;


    while ((branch = (TBranch*)next()))
    {
        TString branchName = branch->GetName();

        if(branchName.Contains("met_MVA"))
        {
            std::cout<<"Found "<<branchName<<std::endl;
            mvaNames.push_back(branchName);
            m_mvaValues[branchName] = -99;
            chain->SetBranchAddress(branchName,&m_mvaValues[branchName]);
        }
    }

    std::cout<<"Creating yield files for each BDT..."<<std::endl;
    TString outputDir = "/afs/cern.ch/work/k/ktomiwa/public/PostICEP/BDT/NewVer/MVA/YieldFiles/";
    system(Form("mkdir -vp %s", outputDir.Data()));

    std::vector<TString> sets = {"MetSig35","MetSig35Vtx","MetSig35Vtx2J","MetSig55","MetSig55Vtx","MetSig55Vtx2J"};
    int BDTCount = 5;

    //m_yieldFiles["met_sig4_nom"] = new std::ofstream(Form("%s/met_sig4_nom.txt",outputDir.Data()));
    //m_yieldFiles["met_sig55_nom"] = new std::ofstream(Form("%s/met_sig55_nom.txt",outputDir.Data()));
    for(int BDTNum = 1; BDTNum <= BDTCount; BDTNum++)
            for(double bdtCut = -1.0; bdtCut < 1.0; bdtCut+=0.1)
                for(auto set : sets)
                {
                    TString fileName = Form("%s_MVA%d_BDTCut%.0f",set.Data(),BDTNum,(bdtCut+1)*10);
                    system(Form("mkdir -vp %s/%s", outputDir.Data(),set.Data()));
                    m_yieldFiles[fileName] = new std::ofstream(Form("%s/%s/%s.txt",outputDir.Data(),set.Data(),fileName.Data()));
                }



    std::cout<<"Starting loop..."<<std::endl;
    a.Loop();

    for(int BDTNum = 1; BDTNum <= BDTCount; BDTNum++)
            for(double bdtCut = -1.0; bdtCut < 1.0; bdtCut+=0.1)
                for(auto set : sets)
                {
                TString fileName = Form("%s_MVA%d_BDTCut%.0f",set.Data(),BDTNum,(bdtCut+1)*10);
                m_yieldFiles[fileName]->close();
                }

    //m_yieldFiles["met_sig4_nom"]->close();
    //m_yieldFiles["met_sig55_nom"]->close();

    return 0;
}
void readerMVA::Loop()
{
    if (fChain == 0) return;
    Long64_t nentries = fChain->GetEntries();
    double counter = 0;

    std::cout<<"Opening file: "<<fileName<<" and there are: "<<nentries<<" entries.."<<std::endl;
    for (Long64_t jentry=0; jentry<nentries; jentry++)
    {
//        Long64_t ientry = LoadTree(jentry);
//        if (ientry < 0) break;
        fChain->GetEntry(jentry);
        PrintProgressBar(jentry,nentries);

        if(phpt1 < 40.0 || phpt2 < 30.0 ) continue; //change variables name
        if(m_yy < 160.0 || m_yy > 400.0 )   continue;

        //m_weight=1.0;
        //if(diff_sumpt2<0)continue;

        if(metsig > 3.5)//change variable name
        {
            counter++;
            //if(diff_sumpt2>0)
            //(*m_yieldFiles["met_sig4_nom"])<<Form("%.2f %.1f\n",m_yy,1.0);
            for(double bdtCut = -1.0; bdtCut < 1.0; bdtCut+=0.1)
            {
                int mvaCount = 0;
                for(auto mvaName : mvaNames)
                {
                    mvaCount++;
                    if(mvaCount > 5) continue;
                    if(m_mvaValues[mvaName] < bdtCut) continue;

                    TString fileName = Form("%s_MVA%d_BDTCut%.0f","MetSig35",mvaCount,(bdtCut+1)*10);
                    (*m_yieldFiles[fileName])<<Form("%.2f %.1f\n",m_yy,1.0);

                    if(diff_sumpt2<0) continue;
                    fileName = Form("%s_MVA%d_BDTCut%.0f","MetSig35Vtx",mvaCount,(bdtCut+1)*10);
                    (*m_yieldFiles[fileName])<<Form("%.2f %.1f\n",m_yy,1.0);

                    if(N_j_central < 2) continue;
                    fileName = Form("%s_MVA%d_BDTCut%.0f","MetSig35Vtx2J",mvaCount,(bdtCut+1)*10);
                    (*m_yieldFiles[fileName])<<Form("%.2f %.1f\n",m_yy,1.0);
                }
            }
        }
        if(metsig > 5.5)//change variable name
        {
            //if(diff_sumpt2>0)
            //(*m_yieldFiles["met_sig55_nom"])<<Form("%.2f %.1f\n",m_yy,1.0);
            for(double bdtCut = -1.0; bdtCut < 1.0; bdtCut+=0.1)
            {
                int mvaCount = 0;
                for(auto mvaName : mvaNames)
                {
                    mvaCount++;
                    if(mvaCount <= 5) continue;
                    if(m_mvaValues[mvaName] < bdtCut) continue;

                    TString fileName = Form("%s_MVA%d_BDTCut%.0f","MetSig55",mvaCount-5,(bdtCut+1)*10);
                    (*m_yieldFiles[fileName])<<Form("%.2f %.1f\n",m_yy,1.0);

                    if(diff_sumpt2<0) continue;
                    fileName = Form("%s_MVA%d_BDTCut%.0f","MetSig55Vtx",mvaCount-5,(bdtCut+1)*10);
                    (*m_yieldFiles[fileName])<<Form("%.2f %.1f\n",m_yy,1.0);

                    if(N_j_central < 2) continue;
                    fileName = Form("%s_MVA%d_BDTCut%.0f","MetSig55Vtx2J",mvaCount-5,(bdtCut+1)*10);
                    (*m_yieldFiles[fileName])<<Form("%.2f %.1f\n",m_yy,1.0);
                }
            }
        }
    }
    std::cout<<"Processed: "<<counter<<std::endl;
    cout<<"Done!"<<endl;
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

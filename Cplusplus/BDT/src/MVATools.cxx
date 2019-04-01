/*  Run interface for MVATools
 *  Code uses configuration file on data/MVATool.cfg
 *  Autors: Xifeng Ruan, Rob Reed, Kehinde Tomiwa
 *         kehinde.tomiwa4@gmail.com
 */

#include "MVATools.h"
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,8,0)
#include "TMVA/DataLoader.h"
#endif
MVATools::MVATools(TString filesig, TString filebkg, TString fileout,TString strvar,TString sigTree,TString bkgTree, TString eventweight)
{
    TMVA::Tools::Instance();
    mvaoutname=fileout;
    TChain *chains=new TChain(sigTree);
    TChain *chainb=new TChain(bkgTree);
    cout<<filesig<<" and "<<filebkg<<endl;
    chains->Add(filesig);
    chainb->Add(filebkg);
    signal=chains;
    background=chainb;
    weight=eventweight;
    addvarlist(strvar);

}

MVATools::MVATools(TString filesigtr, TString filesigte, TString filebkgtr, TString filebkgte, TString fileout,TString strvar, TString Stree,TString Btree,TString eventweight)
{
    mvaoutname=fileout;
    TChain *chaintests=new TChain(Stree);
    TChain *chaintestb=new TChain(Btree);
    TChain *chaintrains=new TChain(Stree);
    TChain *chaintrainb=new TChain(Btree);
    chaintests->Add(filesigte);
    chaintestb->Add(filebkgte);
    chaintrains->Add(filesigtr);
    chaintrainb->Add(filebkgtr);
    signaltr=chaintrains;
    backgroundtr=chaintrainb;
    signalte=chaintests;
    backgroundte=chaintestb;
    weight=eventweight;
    addvarlist(strvar);
}


MVATools::MVATools() {}
void MVATools::Loop()
{


}


void MVATools::Training_setupFactory(TString factConfig)
{
    std::cout << "==> Start TMVAClassification" << std::endl;
    setdir("TrainingInfo");
    ////inputing, outfile, varname, input treeS, treeB, cutS, cutB, weightname
    outputFile = TFile::Open( trainingDir+"/"+mvaoutname+".root", "RECREATE" );
	//outputFile = TFile::Open( mvaoutname+".root", "RECREATE" );
    //factory = new TMVA::Factory( mvaoutname, outputFile,
    //                           "!V:!Silent:Color:DrawProgressBar:Transformations=I;P;G,D:AnalysisType=Classification" );
	factory = new TMVA::Factory( mvaoutname, outputFile,factConfig);
    #if ROOT_VERSION_CODE >= ROOT_VERSION(6,8,0)
    dataloader = new TMVA::DataLoader("dataset");
    #else
    dataloader = factory;
    #endif
    for(int i=0; i<mvaVars.size(); i++)
    {
	    std::cout<<"adding: "<<mvaVars.at(i)<<std::endl;
		//factory->AddVariable( mvaVars.at(i),mvaVars.at(i), 'F' );
		dataloader->AddVariable( mvaVars.at(i),mvaVars.at(i), 'F' );
    }
	for(int i=0; i<spectatorVars.size();i++){
	    std::cout<<"adding spectator: "<<spectatorVars.at(i)<<std::endl;
		//factory->AddSpectator(spectatorVars.at(i),spectatorVars.at(i), 'F');
		dataloader->AddSpectator(spectatorVars.at(i),spectatorVars.at(i), 'F');
    }
    cout<<"Finished training initialisation..."<<endl;
}

void MVATools::Training_setInputs(TString traingCfg, TString mvaalgo, TString algocofig)
{
    std::cout<<"Setting signal and background inputs..."<<std::endl;
    Double_t signalWeight     = 1.0;
    Double_t backgroundWeight = 1.0;
    dataloader->AddSignalTree    ( signal,     signalWeight     );
    dataloader->AddBackgroundTree( background, backgroundWeight );
    dataloader->SetSignalWeightExpression    (weight);
    dataloader->SetBackgroundWeightExpression(weight);
    m_trainconfig	=   traingCfg;
    m_mvaAlgo	        =   mvaalgo;
    m_algoConfig        =   algocofig;
    std::cout<<"Finished setting inputs and weights..."<<std::endl;
		
}

void MVATools::Training_execute(TCut mycuts, TCut mycutb )
{
    std::cout<<"Starting the training run..."<<std::endl;

    //dataloader->PrepareTrainingAndTestTree(mycuts,mycutb, 
    //"nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=EqualNumEvents:!V");
	dataloader->PrepareTrainingAndTestTree(mycuts,mycutb,m_trainconfig);
    #if ROOT_VERSION_CODE >= ROOT_VERSION(6,8,0)
	if(m_mvaAlgo == "BDTG")
    //factory->BookMethod(dataloader,TMVA::Types::kBDT, "BDTG1",
    //"!H:!V:NTrees=800:MinNodeSize=1:BoostType=Grad:Shrinkage=0.06:UseBaggedBoost:BaggedSampleFraction=0.6:nCuts=20:MaxDepth=3");
	factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTG",m_algoConfig);
    #else
	factory->BookMethod( TMVA::Types::kBDT, "BDTG",m_algoConfig);
    // factory->BookMethod(TMVA::Types::kBDT, "BDTG1",
    //"!H:!V:NTrees=800:MinNodeSize=1:BoostType=Grad:Shrinkage=0.06:UseBaggedBoost:BaggedSampleFraction=0.6:nCuts=20:MaxDepth=3");
    #endif
 
    std::cout<<"Training all methods..."<<std::endl;
    factory->TrainAllMethods();

    // ---- Evaluate all MVAs using the set of test events
    std::cout<<"Testing all methods..."<<std::endl;
    factory->TestAllMethods();

    // ----- Evaluate and compare performance of all configured MVAs
    std::cout<<"Evaluate all methods..."<<std::endl;
    factory->EvaluateAllMethods();

    outputFile->Close();
    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;

    std::cout<<"Finished the training run..."<<std::endl;
    delete factory;
    delete dataloader;
}


void MVATools::Evaluate_setMVAReader(TString file, std::vector<TString> xml,  TString fileout, std::vector<TString> strvar)
{
    readerchain=new TChain("tree");
    readerchain->Add(file);
    readeroutputfile=fileout;
    UInt_t xmlSize = xml.size();
    for(int BDTCount=0; BDTCount<xmlSize; BDTCount++)
    {
        std::cout<<"The strvar for xml value: "<<strvar[BDTCount]<<std::endl;
        addvarlist(strvar[BDTCount]);
        reader.push_back(new TMVA::Reader( "!Color:!Silent" ));
        for (UInt_t ivar=0; ivar<mvaVars.size(); ivar++)
        {
		    std::cout<<"adding to reader: "<<mvaVars.at(ivar)<<std::endl;
			reader[BDTCount]->AddVariable(mvaVars.at(ivar) , &mvaVarsHolder[ivar]);
        }
		for (UInt_t ivar=0; ivar<spectatorVars.size(); ivar++)
		{
		    std::cout<<"adding to spec reader: "<<spectatorVars.at(ivar)<<std::endl;
			reader[BDTCount]->AddSpectator(spectatorVars.at(ivar) , &mvaVarsSpecHolder[ivar]);
		}
        stringset.push_back(mvaVars);
        reader[BDTCount]->BookMVA(  Form( "BDT%i", BDTCount+1 ) , xml[BDTCount]);
    }
}

void MVATools::Evaluate_saveBDTOutput()
{
    std::cout << "==> Recalculating and adding BDT values into file..." << std::endl;

    TFile *outf=new TFile(readeroutputfile,"recreate");

    TString tmpstr="met_MVA";
    Float_t  outvars[reader.size()];
    Float_t  treevars[mvaVars.size()];

    //for (UInt_t ivar=0; ivar<reader.size(); ivar++)
    //{
    //readerchain->SetBranchAddress( Form( tmpstr+"%i", ivar+1 ) , &outvars[ivar] );
    //}
    for (UInt_t ivar=0; ivar<mvaVars.size(); ivar++)
    {
        readerchain->SetBranchAddress(mvaVars[ivar] , &(treevars[ivar]));
    }
        
    outputtree = readerchain->CloneTree(0);
    for (UInt_t ivar=0; ivar<reader.size(); ivar++)
    {
        outputtree->Branch( Form( tmpstr+"%i", ivar+1 ) , &outvars[ivar] );
    }


    UInt_t totalSize = readerchain->GetEntries();
    std::cout<<"Running over file: "<<readerchain->GetFile()->GetName()<<std::endl;
    for (UInt_t i=0; i<totalSize; i++)
    {
        readerchain->GetEntry(i);
        PrintProgressBar(i,totalSize);

        for (UInt_t ivar=0; ivar<mvaVars.size(); ivar++)
        {
            mvaVarsHolder[ivar] = treevars[ivar];
        }

        for (int k=0; k<reader.size(); k++)
        {
            outvars[k]=Float_t(reader[k]->EvaluateMVA(  Form( "BDT%i", k+1 )  ));
        }

        outputtree->Fill();
    }

    outf->Write();
    outf->Close();

//    std::vector<Double_t> vars; // vector has size of number of input variables
//    Float_t  treevars[mvaVars.size()];
//    Float_t  treespec[spectatorVars.size()];
//
//    Float_t weighttmp;
//    TFile *outf=new TFile(readeroutputfile,"recreate");
//    outputtree=new TTree("tree","tree");
//    TString tmpstr="met_MVA";
//
//    for (UInt_t ivar=0; ivar<reader.size(); ivar++)
//    {
//        outputtree->Branch( Form( tmpstr+"%i", ivar+1 ) , &outvars[ivar] );
//    }
//    for (UInt_t ivar=0; ivar<spectatorVars.size(); ivar++)
//    {
//        outputtree->Branch(spectatorVars[ivar] , &(treespec[ivar]));
//    }
//    for (UInt_t ivar=0; ivar<mvaVars.size(); ivar++)
//    {
//        outputtree->Branch(mvaVars[ivar] , &(treevars[ivar]));
//    }

//    UInt_t totalSize = readerchain->GetEntries();
//    std::cout<<"Running over file: "<<readerchain->GetFile()->GetName()<<std::endl;
//    for (UInt_t i=0; i<totalSize; i++)
//    {
//        readerchain->GetEntry(i);
//        PrintProgressBar(i,totalSize);
//
//
//        for (int j=0; j<stringset.size(); j++)
//        {
//            for(int k=0; k<stringset.at(j).size(); k++)
//            {
//                arrayvar2d[j][k]=Float_t(readerchain->GetLeaf(stringset.at(j).at(k))->GetValue());
//            }
//        }
//
//        for (UInt_t ivar=0; ivar<spectatorVars.size(); ivar++)
//        {
//            treespec[ivar]=Float_t(readerchain->GetLeaf(spectatorVars[ivar])->GetValue());
//        }
//
//        for (UInt_t ivar=0; ivar<mvaVars.size(); ivar++)
//        {
//            treevars[ivar]=Float_t(readerchain->GetLeaf(mvaVars[ivar])->GetValue());
//        }
//
//        for (int k=0; k<reader.size(); k++)
//        {
//            outvars[k]=Float_t(reader[k]->EvaluateMVA(  Form( "BDT%i", k+1 )  ));
//        }
//
//        outputtree->Fill();
//    }
//
//    outf->Write();
//    outf->Close();
}

void MVATools::addvarlist(TString strvar)
{
    int count=0;
    ifstream inputlist(strvar);
    string s;

    while( getline(inputlist,s))
    {
        if(count==0)mvaVars=splitstr(s," ");
        if(count==1)spectatorVars=splitstr(s," ");
        count++;
    }
}

std::vector<TString> MVATools::splitstr(string input, const char *sign)
{
    std::vector<TString> outstring;
    char tmpstr[2000];
    sprintf(tmpstr,input.c_str());
    char * token = strtok(tmpstr,sign);
    while( token != NULL )
    {
        outstring.push_back(token);
        //cout<<token<<endl;
        token = strtok( NULL, sign);
    }
    return outstring;
}

void MVATools::PrintProgressBar(int index, int total)
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
void MVATools::setdir(TString dir)
{
    trainingDir = dir;
    system(Form("mkdir -vp %s", trainingDir.Data()));
}

/*  Run interface for MVATools
 *  Code uses configuration file in data/MVATool.cfg
 *  Main class: MVATools
 *  Compatible with gcc and ROOT 6.12/06 (https://root.cern.ch/)
 *  Autors: Xifeng Ruan, Rob Reed, Kehinde Tomiwa
 *         kehinde.tomiwa4@gmail.com
 */
#define MVATools_cxx
#include "MVATools.h"
#include "TStopwatch.h"
#include "TEnv.h"

using namespace std;
TString traindir = "";
bool doTrain = false;
bool doEval = false;
void createfile(TString mvavar, TString optvar,TString common);
void createfile(std::vector<TString> &vbfname,std::vector<TString> &mvalist,TString mvavar, TString optvar, TString common);
TString createdumpfile(TString label,TString infile, TString outfile, int index, TString invar, TString optvar,TString outvar);
void splitString(const TString& str, std::vector<TString>& splits,const TString& delimiter);
int main(int argc, char **argv)
{
		
  if(argc<3){
    std::cout<<"Check arguement provided"<<std::endl;
    std::cout<<"For training "<<argv[0]<<" -t <MVAConfig file"<<std::endl;
    exit(1);
  }
  if(strcmp(argv[1],"-t")==0)doTrain = true;
  if(strcmp(argv[1],"-e")==0)doEval = true;
  if(!doTrain and !doEval)
  {
    std::cout<<"Please specify a command to train and/or evaluate..."<<std::endl;
    std::cout<<argv[0]<<"-t -- Trains the MVA"<<std::endl;
    std::cout<<argv[0]<<"-e -- Evaluates the MVA"<<std::endl;
    return 1;
  }
  TString config = TString(argv[2]);
  std::cout<<"reading config: "<<config<<std::endl;
  TEnv m_reader(config);
   
  //m_reader->ReadFile(config);
  //TString test = m_reader.GetValue("BDTGConfig","");
  //std::cout<<"Test: "<<test<<std::endl;

 


 
  //TString IdName  = settings->getStr("IDname");
  std::vector<TString>  IdNames;
  splitString(m_reader.GetValue("IDname",""),IdNames," ");
  for(TString IdName: IdNames ){
    std::cout<<"chcek split: "<<IdName<<std::endl;
  }
  std::vector<TString> cates;
  splitString(m_reader.GetValue("Categories",""),cates," ");
  

  TString baseDir = m_reader.GetValue("BASEdirectory","");
  TString mvaInputdir = m_reader.GetValue("MVAInputdir","");
    
    traindir = m_reader.GetValue("Trainingdir","");
    system(Form("mkdir -vp %s", traindir.Data()));
    std::vector<TString> label;
    std::vector<TString> filelist;

    std::vector<TString> vbfname;
    std::vector<TString> mvalist;
    std::vector<TString> mvavars;
    std::vector<TString> vbfnamezh;
    std::vector<TString> mvalistzh;

    std::cout<<"Done..."<<std::endl;

    if(doTrain)
    {
      //create xml file and vectors
      for(TString IdName: IdNames ){
	      for(TString cate : cates){
	        TString trainvarables = m_reader.GetValue("MVAVariables","");
          std::cout<<"check var: "<<trainvarables<<std::endl;
	        TString spectatorVars = m_reader.GetValue("MVASpecVariables","");
          std::cout<<"check spec var: "<<spectatorVars<<std::endl;
	        TString sampleIdentifier = IdName+cate;
	        std::cout<<"check: sampleID: "<<sampleIdentifier<<std::endl;
	        //createfile(vbfname,mvalist,trainvarables,spectatorVars,sampleIdentifier);
	        createfile(trainvarables,spectatorVars,sampleIdentifier);
	        TString cut   = m_reader.GetValue(cate+"Cut","");
	        TCut cutS     = TCut(cut);
	        TCut cutB     = TCut(cut);
	        std::cout<<"Starting traning on "<<IdName<<std::endl;
	        std::cout<<"check: "<<cut<<std::endl;
	        TString bkgSample = m_reader.GetValue("Commonbackground","");
	        TString samplepath 		= baseDir+mvaInputdir;
	        TString signalfile 		=samplepath+"/"+IdName+".root";
	        TString bkgsamplefile =samplepath+"/"+bkgSample+".root";
	        TString mvatxtfile   =    traindir+"/MVA-Vars-"+sampleIdentifier+".txt";
	        TString config			= m_reader.GetValue("TrainConfig","");
	        TString mvaalgo 		= m_reader.GetValue("MVAalgo","");
	        TString algoConfig 	= m_reader.GetValue(mvaalgo+"Config","");
	        TString factConfig	= m_reader.GetValue("FactoryConfig","");
          TString signalTree        = m_reader.GetValue("SignalTree","");
          TString bkgTree        = m_reader.GetValue("BackgroundTree","");
          TString weight        = m_reader.GetValue("WeightEvent","");
	        //timer
          TStopwatch sw;
	        sw.Start();
	        MVATools mva(signalfile,bkgsamplefile,sampleIdentifier,mvatxtfile,signalTree,bkgTree,weight);
	        mva.Training_setupFactory(factConfig);
	        mva.Training_setInputs(config,mvaalgo,algoConfig);
	        mva.Training_execute(cutS,cutB);
	        sw.Stop();
	        std::cout << "--- Training time on "<<IdName<<std::endl;; sw.Print();
      }
		}
  }

    if(doEval)
    {
				
      TString runDir 	 = m_reader.GetValue("RunDir","");
      TString MVAoutput = m_reader.GetValue("MVAOutputdir","");
      TString outputPath =runDir+MVAoutput;
      system(Form("mkdir -vp %s", outputPath.Data()));
      std::cout<<"Starting evaluation..."<<std::endl;
      std::vector<TString> xml;
      std::vector<TString> list;
      for(TString IdName: IdNames ){
	      for(TString cate : cates){
	        TString weightfile = runDir + "dataset/weights/" +IdName+cate+"_BDTG.weights.xml";
	        TString trainVarfile	= runDir + "TrainingInfo/MVA-Vars-"+IdName+cate+".txt";
	        xml.push_back(weightfile);
	        list.push_back(trainVarfile);
	      }
      }
			std::vector<TString> SampleEva;
      splitString(m_reader.GetValue("EvaluationSamples",""),SampleEva," ");
       
      for(TString sample : SampleEva){
	      TStopwatch sw;
	      sw.Start();
	      MVATools Trainner;
	      std::cout<<"Processing "<<sample<<std::endl;
	      TString samplepath 			=	baseDir+mvaInputdir+"/"+sample;
	      TString evaOutputsmple 	= outputPath+"/Out"+sample;
	      Trainner.Evaluate_setMVAReader(samplepath,xml,evaOutputsmple,list);
	      Trainner.Evaluate_saveBDTOutput();
	      sw.Stop();
	      std::cout << "--- Evaluation time: "; sw.Print();
      }
    }
    return 0;
}

void createfile(std::vector<TString> &vbfname,std::vector<TString> &mvalist,TString mvavar, TString optvar,TString common)
{
    int size=vbfname.size();
    TString xml=Form("%s",common.Data());
    TString file=Form(traindir+"/MVA-Vars-%s.txt",common.Data());
    cout<<" creating file "<<file<<endl;
    vbfname.push_back(xml);
    mvalist.push_back(file);
    ofstream fout(file.Data());
    fout<<mvavar<<endl;
    fout<<optvar<<endl;
    fout.close();
}
void createfile(TString mvavar, TString optvar,TString common)
{
		TString file=Form(traindir+"/MVA-Vars-%s.txt",common.Data());
		cout<<" creating file "<<file<<endl;
		ofstream fout(file.Data());
		fout<<mvavar<<endl;
		fout<<optvar<<endl;
		fout.close();
}

void splitString(const TString& str, std::vector<TString>& splits,const TString& delimiter)
{
  TObjArray* splitsArray = str.Tokenize(delimiter);
  for (int i = 0; i < splitsArray->GetEntries(); i++) splits.push_back(splitsArray->At(i)->GetName());
  delete splitsArray;

}
TString createdumpfile(TString label,TString infile, TString outfile, int index, TString invar, TString optvar,TString outvar)
{
    TString file=Form("InputCards/%s.txt",label.Data());
    cout<<" creating file "<<file<<endl;
    ofstream fout(file.Data());
    fout<<invar<<endl;
    fout<<optvar<<endl;
    fout<<outvar<<endl;
    fout<<infile<<endl;
    fout<<outfile<<endl;
    fout<<index<<endl;
    fout.close();
    return file;
}

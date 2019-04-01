/*  Run interface for MVATools
 *  Code uses configuration file on data/MVATool.cfg
 *  Autors:  Rob Reed, Kehinde Tomiwa
 *         kehinde.tomiwa4@gmail.com
 */

#ifndef Plotter_H
#define Plotter_H

#include "TString.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLatex.h"
#include "TPaveLabel.h"
#include "TLine.h"
#include "TH1D.h"
#include "TGraphAsymmErrors.h"
#include "TEfficiency.h"
#include "TAxis.h"
#include "TGaxis.h"
#include <deque>
#include "TStyle.h"

#include <TColor.h>
#include <TLegend.h>

class Plotter {
private:

//General variables
bool m_useLogY = false;
bool m_dataToMC = false;
bool m_doRatio = false;
bool m_useGridX = false;
bool m_useGridY = false;
double m_yMax = 0;
double m_yMin = 99999999;

//Labels
TString m_ratioLabel = "Data / MC";
int m_legendNCols = 0;
	TString AtlasLabel = "ATLAS #it{Internal}";

//Legend Position
double m_legX1 = 0.2;
double m_legX2 = 0.4;
double m_legY2 = 0.92;
double m_legY1 = 0.90;

//Canvas and pad sizes
double m_canY2                  = 600;
double m_padY2                  = 0.12;
double m_padScaling		        = 1.0;
double m_ratioPadScaling		= 1.0;

TString m_drawOption            = "EH";

//Draw options
bool m_drawRatioErr = true;

public:
    Plotter();
    ~Plotter();
    void initialize();

    TCanvas* getStylishCanvas(std::vector<TH1*> histos, std::vector<TString> addLatexLabels = {""});
    TCanvas* getStylishCanvas(std::deque<TH1*> histos, std::vector<TString> addLatexLabels = {""});

    TStyle* AtlasStyle();
    void SetAtlasStyle();

    void reset();

    void setDrawOptions(TString value)          {m_drawOption  = value;}

    void setLegNCol(int value)        {m_legendNCols = value;}
    void setLogY(bool value)          {m_useLogY  = value;}
	  void setGrid(bool valueX, bool valueY)          {m_useGridX = valueX;m_useGridY = valueY;}
    void dataToMC(bool value)         {m_dataToMC = value;}
    void drawRatio(bool value)        {m_doRatio = value;}
    void setRatioLabel(TString label) {m_ratioLabel = label;}
    void drawRatioErr(bool value)     {m_drawRatioErr = value;}
    void setYRange(double yMin, double yMax) {m_yMin = yMin ; m_yMax = yMax; }
    void setLegXY(double legX1, double legX2, double legY1, double legY2) {m_legX1 = legX1; m_legX2 = legX2; m_legY1 = legY1; m_legY2 = legY2; }
	void setAtlasLabel(TString label){AtlasLabel = label;}


};
#endif // Plotter_H

/*  Run interface for MVATools
 *  Code uses configuration file on data/MVATool.cfg
 *  Autors: Rob Reed, Kehinde Tomiwa
 *         kehinde.tomiwa4@gmail.com
 */

#include "Plotter.h"
#include "TMath.h"
#include "TStyle.h"
#include "TROOT.h"
//______________________________________________________________________________
Plotter::Plotter()
{}

//______________________________________________________________________________
Plotter::~Plotter(){
}

//______________________________________________________________________________
void Plotter::initialize()
{
    SetAtlasStyle();
    //General variables
    m_useLogY  = false;
    m_dataToMC = false;
    m_doRatio  = false;
	  m_useGridX  = false;
	  m_useGridY  = false;
    m_yMax = 0;
    m_yMin = 99999999;
    m_legendNCols=0;

    //Labels
    m_ratioLabel = "Data / MC";
	  AtlasLabel = "ATLAS #it{Internal}";

    //Legend Position
    m_legX1 = 0.18;
    m_legX2 = 0.6;
    m_legY2 = 0.92;
    m_legY1 = 0.90;

    //Canvas and pad sizes
    m_canY2           = 600;
    m_padY2           = 0.12;
    m_padScaling	  = 1.0;
    m_ratioPadScaling = 1.0;

    //Draw options
    m_drawRatioErr = true;
    m_drawOption   = "EH";

}

void Plotter::reset()
{
    //General variables
    m_useLogY = false;
    m_dataToMC = false;
	  m_useGridX  = false;
	  m_useGridY  = false;
    m_doRatio = false;
    m_yMax = 0;
    m_yMin = 99999999;

    //Labels
    m_ratioLabel = "Data / MC";
    m_legendNCols=0;

    //Legend Position
    m_legX1 = 0.2;
    m_legX2 = 0.4;
    m_legY2 = 0.92;
    m_legY1 = 0.90;

    //Canvas and pad sizes
    m_canY2           = 600;
    m_padY2           = 0.12;
    m_padScaling	  = 1.0;
    m_ratioPadScaling = 1.0;

    //Draw options
    m_drawRatioErr = true;
    m_drawOption = "EH";
}

//______________________________________________________________________________
TCanvas* Plotter::getStylishCanvas(std::deque<TH1*> histos, std::vector<TString> addLatexLabels)
{
    std::vector<TH1*> hists;
    for(auto hist : histos) hists.push_back(hist);

    return getStylishCanvas(hists, addLatexLabels);
}

//______________________________________________________________________________
TCanvas* Plotter::getStylishCanvas(std::vector<TH1*> histos, std::vector<TString> addLatexLabels)
{
    if(m_dataToMC && histos.size() > 2)
        std::cout<<"Data to MC comparison with more than two histos. Assuming all histograms except 1st to be MC."<<std::endl;

    //Set default variables
    bool firstPlot = true;
    int colorNum = 0;

    //Colors
    std::vector<Color_t> colors{kBlue+1,kRed+1,kGreen+2,kTeal-1,kOrange-3,kViolet};

    //Legend Position
    double m_legY1 = m_legY2-(histos.size()*0.055);

    TPad *plotPad  = 0;
    TPad *ratioPad = 0;
    TCanvas *can   = 0;

    if(m_doRatio)
    {
        //m_canY2 = 800;
        m_padY2 = 0.35;

        can = new TCanvas("c1","Can",0.,0.,800,m_canY2);
        m_padScaling 	= 1. / (1. - m_padY2) / 8. * 6.;
        m_ratioPadScaling	= (1. / m_padY2) / 8. * 6.;

        plotPad = new TPad( "plotPad", "plotPad", 0.0, m_padY2, 1.00, 1.00 );
        ratioPad = new TPad( "ratioPad", "ratioPad", 0.0, 0.0, 1.00, m_padY2 );

        plotPad->SetBottomMargin(.035);
        ratioPad->SetTopMargin(.035);
        ratioPad->SetBottomMargin(m_ratioPadScaling * ratioPad->GetBottomMargin());

        plotPad->SetBottomMargin(0.00001);
        plotPad->SetBorderMode(0);

        ratioPad->Draw();
        plotPad->Draw();

        plotPad->cd();
    }
    else
    {
        can = new TCanvas("c1","Can",0.,0.,800,m_canY2);
        can->cd();
    }

    m_legY1 = m_legY2 - (m_legY2 - m_legY1) * 1.5*m_padScaling;
    m_legY1=0.8;
    //Create blank legend
    TLegend* legend = new TLegend(m_legX1, m_legY1, m_legX2, m_legY2);
    if(m_legendNCols > 0)
        legend->SetNColumns(m_legendNCols);
    legend->SetTextFont(42);
    legend->SetTextSize(0.03);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->SetLineColor(0);

    //Create latex printer
    TLatex l;
    l.SetNDC();
    l.SetTextColor(kBlack);
    l.SetTextFont(72);
    l.SetTextSize(gStyle->GetTextSize() * 1.0 * m_padScaling);
    l.SetTextAlign(33);
    double dy = 0.06*m_padScaling;
    double startExtraTex = 0.92;

    //Relative height to stop plots at
    double latexInfoHeight = startExtraTex-(addLatexLabels.size()+1)*dy;
    double relMax = (m_legY1 > latexInfoHeight) ? latexInfoHeight-dy : m_legY1-dy;

    if(m_doRatio)
        relMax += 0.06;
    else
        relMax -= 0.06;

    //Set log scale if desired
    if(m_useLogY)
        can->SetLogy(true);
    //Set grid if desire
	  if(m_useGridX)
			can->SetGridx(true);
	if(m_useGridY)
		can->SetGridy(true);
	//if(m_useGridX && m_useGridX)
	//	can->SetGrid(true,true);
    //Get the maximum in plots
    if(m_yMax == 0)
        for(auto hist : histos)
        {
            double currYMax = hist->GetMaximum() + hist->GetBinError(hist->GetMaximumBin());
            if(currYMax > m_yMax)
                m_yMax = currYMax;
        }

    //Get the minimum in plots
    if(m_yMin == 99999999)
        for(auto hist : histos)
        {
            double currYMin = hist->GetMinimum() - hist->GetBinError(hist->GetMinimumBin());
            if(currYMin < m_yMin)
                m_yMin = currYMin;
        }

    for(auto hist : histos)
    {
        TString histName = hist->GetName();
        //histName.ReplaceAll("_"," ");

        hist->GetYaxis()->SetLabelSize	(gStyle->GetLabelSize("y")* m_padScaling);
        hist->GetYaxis()->SetTitleSize	(gStyle->GetTitleSize("y")* m_padScaling);
        hist->GetYaxis()->SetTitleOffset(gStyle->GetTitleOffset("y")/ m_padScaling);
        hist->GetXaxis()->SetTitleOffset(gStyle->GetTitleOffset("x"));
        hist->GetXaxis()->SetLabelSize	(m_doRatio ? 0. : gStyle->GetLabelSize("x"));
        hist->GetXaxis()->SetTitleSize	(m_doRatio ? 0. : gStyle->GetTitleSize("x"));
        //hist->GetYaxis()->SetTitle("Events");
        hist->GetYaxis()->SetDecimals();

        if(m_useLogY)
        {
            if(m_yMin == 0)m_yMin = 0.005;
            double yRange = (m_yMax-m_yMin);
            double logMax = yRange*yRange/relMax - yRange*yRange;
            hist->SetMaximum(logMax);
            hist->SetMinimum(m_yMin);
        }
        else
        {
            if(m_yMin > 0.01) m_yMin = 0.01;
            hist->SetMaximum((m_yMax-m_yMin)/relMax);
            //hist->SetMaximum(1.3);
            hist->SetMinimum(m_yMin);
            //hist->SetMinimum(0.0);

            if(m_doRatio)
            {
                if(m_yMin > 0.01) m_yMin = 0.01;
                hist->SetMinimum(m_yMin);
                double binWidth = (m_yMax-m_yMin)/hist->GetNbinsY();

                m_yMin = m_yMin-binWidth/6;
                hist->SetMaximum((m_yMax-m_yMin)/relMax);
                hist->SetMinimum(m_yMin);
            }
        }

        if(m_dataToMC)
        {
            if(firstPlot)
            {
                hist->SetLineColor(kBlack);
                legend->AddEntry(hist,histName, "lep");
                hist->Draw();
            }
            else
            {
                hist->SetMarkerSize(0);
                hist->SetLineColor(colors[colorNum]);
                legend->AddEntry(hist,histName, "f");
                hist->Draw(m_drawOption+" same");
            }
        }
        else
        {
            hist->SetLineColor(colors[colorNum]);
            hist->SetMarkerColor(colors[colorNum]);
            legend->AddEntry(hist,histName, "L");
            hist->Draw(m_drawOption+" same");
        }

        if((m_dataToMC && not firstPlot) || not m_dataToMC)
            colorNum++;

        firstPlot = false;
    }
    //Draw first plot over everything else
    //histos[0]->Draw("same L");
    legend->Draw();
	  l.SetTextFont(42);
	  l.SetTextSize((gStyle->GetTextSize()-0.008)*m_padScaling);
    l.DrawLatex(0.92, startExtraTex, AtlasLabel);
    l.SetTextFont(42);
    l.SetTextSize((gStyle->GetTextSize()-0.008)*m_padScaling);
    l.DrawLatex(0.9, startExtraTex-dy, "#sqrt{s} = 13 TeV");
    int latexCount = 2;
    for(auto addLatexLabel : addLatexLabels)
    {
        l.DrawLatex(0.9, startExtraTex-latexCount*dy, addLatexLabel.Data());
        latexCount++;
    }



    if (m_doRatio)
    {
        ratioPad->cd();
        //ratioPad->Draw();

        int nBins = histos[1]->GetNbinsX();

        /* calculate the number of valid ratio points: ratio points are considered
         * valid if they have a finite value (MC prediction != 0) (--> nPoints)
         * and the observed data is greater than zero (--> nRatioPoints) */
        int nPoints	= 0;
        int nRatioPoints = 0;
        for (int i = 1; i <= nBins; i++)
        {
            if (histos[1]->GetBinContent(i) != 0.)
            {
                nPoints++;
                if (histos[0]->GetBinContent(i) > 0)
                    nRatioPoints++;
            }
        }

        /* the graph used to draw the ratio points */
        TGraphErrors * ratioGraph = new TGraphErrors(nRatioPoints);
        /* the graph used to draw the error band on the ratio */
        TGraphAsymmErrors * ratioErrorGraph = new TGraphAsymmErrors(nPoints);

        int iPoint 		= 0;
        int iRatioPoint 	= 0;

        bool forceRatioLimits = false;
        double	ratioMax	  = 5.;
        double  ratioMin	  = 0.;

        /* actual minimum and maximum ratio */
        double actualRatioMin = 1.;
        double actualRatioMax = 1.;

        /* loop over all bins of the histogram */
        for (int iBin = 1; iBin <= nBins; iBin++)
        {

            /* get the values and errors of data and MC for this bin */
            double data		= histos[0]->GetBinContent	(iBin);
            double MC		= histos[1]->GetBinContent	(iBin);
            double dataErr	= histos[0]->GetBinError	(iBin);
            double MCErr	= histos[1]->GetBinError	(iBin);

            if (MC == 0.)
                continue;

            /* the value shown in the ratio plot is the ratio
             * of oberserved data and expected MC background */
            double ratio			= data / MC;
            /* the error on the ratio points is calculated from
             * the statistical uncertainty of the observed data */
            double ratioError		= ratio * dataErr / data;
            /* the width of the error band is calculated from the
             * total uncertainty of the expected MC background */
            double ratioBandError	= MCErr / MC;

            if (not isnan(ratioBandError))
            {
            /* set the position and the width of the error band */
            ratioErrorGraph->SetPoint		(iPoint, histos[1]->GetBinCenter(iBin), 1.);
            ratioErrorGraph->SetPointError	(iPoint,
                                             histos[1]->GetBinWidth(iBin) / 2., histos[1]->GetBinWidth(iBin) / 2.,
                                             ratioBandError, ratioBandError);
            }
            iPoint++;

            if (not isnan(ratio))
            {
                /* set the position and the error of the ratio point */
                ratioGraph->SetPoint		(iRatioPoint, histos[1]->GetBinCenter(iBin),	ratio);
                ratioGraph->SetPointError	(iRatioPoint, 0., 						ratioError);

                /* update the actual minimum and maximum ratio */
                if ((ratio - ratioError) < actualRatioMin)
                    actualRatioMin = ratio - ratioError;
                if (ratio + ratioError > actualRatioMax)
                    actualRatioMax = ratio + ratioError;

                iRatioPoint++;

            }

        }

        /* set the fill style of the ratio error band graph */
        ratioErrorGraph->SetFillColor(kOrange - 2);

        /* use the data marker style for the ratio markers */
        ratioGraph->SetMarkerSize(1);//histos[0]->GetMarkerSize());
        ratioGraph->SetMarkerStyle(20);//histos[0]->GetMarkerStyle());

        /* set the x range of the ratio graph to
         * match the one of the histograms above */
        double xLowerLimit = histos[1]->GetXaxis()->GetXmin();
        double xUpperLimit = histos[1]->GetXaxis()->GetXmax();
        ratioGraph->GetXaxis()->SetLimits(xLowerLimit, xUpperLimit);

        /*  */
        ratioGraph->GetXaxis()->SetTitleSize(gStyle->GetTitleSize("x") * m_ratioPadScaling);
        ratioGraph->GetXaxis()->SetLabelSize(gStyle->GetLabelSize("x") * m_ratioPadScaling);
        ratioGraph->GetYaxis()->SetTitleSize(gStyle->GetTitleSize("y") * m_ratioPadScaling);
        ratioGraph->GetYaxis()->SetLabelSize(gStyle->GetLabelSize("y") * m_ratioPadScaling);
        ratioGraph->GetXaxis()->SetTickLength(ratioGraph->GetXaxis()->GetTickLength() * m_ratioPadScaling);
        //Pere

        //ratioGraph->GetYaxis()->SetNdivisions(-306);

        /* set the titles of the axis of the ratio graph */
        ratioGraph->GetXaxis()->SetTitle(histos[1]->GetXaxis()->GetTitle());
        ratioGraph->GetYaxis()->SetTitle(m_ratioLabel.Data());

        //ratioGraph->GetYaxis()->SetRangeUser(actualRatioMin-0.2,actualRatioMax+0.2);
        ratioGraph->GetYaxis()->SetNdivisions(204);

        /* confine the y axis of the ratio plot */
        if (forceRatioLimits || actualRatioMin < ratioMin)
        {
            ratioGraph->GetHistogram()->SetMinimum(ratioMin);
            actualRatioMin = ratioMin;
        }

        if (forceRatioLimits || actualRatioMax > ratioMax)
        {
            ratioGraph->GetHistogram()->SetMaximum(ratioMax);
            actualRatioMax = ratioMax;
        }

        double ratioPlotMax = TMath::CeilNint(actualRatioMax);
        double ratioPlotMin = TMath::FloorNint(actualRatioMin);
        ratioPlotMax += (ratioPlotMax-ratioPlotMin)/6;
        ratioPlotMin -= (ratioPlotMax-ratioPlotMin)/6;
        //double errBinWidth = (actualRatioMax-actualRatioMin)/4; //ratioGraph->GetHistogram()->GetYaxis()->GetBinWidth(1);
        ratioGraph->GetHistogram()->SetMaximum(ratioPlotMax);
        ratioGraph->GetHistogram()->SetMinimum(ratioPlotMin);

        ratioGraph->SetLineColor(histos[0]->GetLineColor());
        ratioGraph->SetMarkerColor(histos[0]->GetMarkerColor());

        /* draw the ratio graph */
        ratioGraph->Draw("AP");
        if(m_drawRatioErr)
            ratioErrorGraph->Draw("2 SAME");
        ratioGraph->Draw("P SAME");

        /* is 1. is included in the range of the y axis of the ratio plot... */
        if (ratioGraph->GetHistogram()->GetMinimum() <= 1. && ratioGraph->GetHistogram()->GetMaximum() >= 1.)
        {

            /* draw the red line indicating 1 in the ratio plot */

            TLine *line = new TLine();
            line->SetLineStyle(1);
            line->SetLineWidth(2);
            line->SetLineColor(kRed);
            line->DrawLine(xLowerLimit, 1.0, xUpperLimit, 1.0);
            line->SetLineWidth(1);
            line->SetLineStyle(2);
            line->DrawLine(xLowerLimit,((1.0+ratioPlotMin)/2.0),xUpperLimit,((1.0+ratioPlotMin)/2.0));
            line->DrawLine(xLowerLimit,((1.0+ratioPlotMax)/2.0),xUpperLimit,((1.0+ratioPlotMax)/2.0));

            //TLine * line = new TLine(xLowerLimit, 1., xUpperLimit, 1.);
            //line->SetLineColor(kRed);
            //line->Draw();
        }

        ratioPad->RedrawAxis();
        ratioPad->Update();

        /*------------------------------*/
        //Pere
        can->cd();
        TLatex * ratioLabel = new TLatex();
        ratioLabel->SetNDC();
        ratioLabel->SetTextFont(42);
        ratioLabel->SetTextSize(gStyle->GetTitleSize("y")*0.75);

        ratioLabel->SetTextAlign(21);
        ratioLabel->SetTextColor(1);
        ratioLabel->SetTextAngle(90);
        ratioLabel->DrawLatex(0.063, 0.225, m_ratioLabel.Data());//Pere
        /*------------------------------*/

    }
    m_yMax = 0;
    m_yMin = 99999999;
    return can;
}

TStyle* Plotter::AtlasStyle()
{
    TStyle *atlasStyle = new TStyle("ATLAS","Atlas style");

    // use plain black on white colors
    int icol=0; // WHITE
    atlasStyle->SetFrameBorderMode(icol);
    atlasStyle->SetFrameFillColor(icol);
    atlasStyle->SetCanvasBorderMode(icol);
    atlasStyle->SetCanvasColor(icol);
    atlasStyle->SetPadBorderMode(icol);
    atlasStyle->SetPadColor(icol);
    atlasStyle->SetStatColor(icol);
    //atlasStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

    // set the paper & margin sizes
    atlasStyle->SetPaperSize(20,26);

    // set margin sizes
    atlasStyle->SetPadTopMargin(0.05);
    atlasStyle->SetPadRightMargin(0.05);
    atlasStyle->SetPadBottomMargin(0.16);
    atlasStyle->SetPadLeftMargin(0.16);

    // set title offsets (for axis label)
    atlasStyle->SetTitleXOffset(1.1);
    atlasStyle->SetTitleYOffset(1.3);

    // use large fonts
    //int font=72; // Helvetica italics
    int font=42; // Helvetica
    double tsize=0.05; // originally 0.05
    atlasStyle->SetTextFont(font);

    TGaxis::SetMaxDigits(4);

    atlasStyle->SetTextSize(tsize);
    atlasStyle->SetLabelFont(font,"x");
    atlasStyle->SetTitleFont(font,"x");
    atlasStyle->SetLabelFont(font,"y");
    atlasStyle->SetTitleFont(font,"y");
    atlasStyle->SetLabelFont(font,"z");
    atlasStyle->SetTitleFont(font,"z");

    atlasStyle->SetLabelSize(tsize,"x");
    atlasStyle->SetTitleSize(tsize,"x");
    atlasStyle->SetLabelSize(tsize,"y");
    atlasStyle->SetTitleSize(tsize,"y");
    atlasStyle->SetLabelSize(tsize,"z");
    atlasStyle->SetTitleSize(tsize,"z");

    // use bold lines and markers
    atlasStyle->SetMarkerStyle(20);
    atlasStyle->SetMarkerSize(1.2);
    atlasStyle->SetHistLineWidth((Width_t)3.0);
    atlasStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

    // get rid of X error bars
    //atlasStyle->SetErrorX(0.001);
    // get rid of error bar caps
    atlasStyle->SetEndErrorSize(0.);

    // do not display any of the standard histogram decorations
    atlasStyle->SetOptTitle(0);
    //atlasStyle->SetOptStat(1111);
    atlasStyle->SetOptStat(0);
    //atlasStyle->SetOptFit(1111);
    atlasStyle->SetOptFit(0);

    // put tick marks on top and RHS of plots
    atlasStyle->SetPadTickX(1);
    atlasStyle->SetPadTickY(1);

    return atlasStyle;

}

void Plotter::SetAtlasStyle()
{
    std::cout << "\nApplying ATLAS style settings...\n" << std::endl ;
    TStyle* atlasStyle = AtlasStyle();
    gROOT->SetStyle("ATLAS");
    gROOT->ForceStyle();
}

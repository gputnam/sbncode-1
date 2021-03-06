#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/OscCalcSterileApprox.h"
#include "CAFAna/Vars/FitVarsSterileApprox.h"
#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Experiment/MultiExperimentSBN.h"
#include "CAFAna/Experiment/CountingExperiment.h"
#include "CAFAna/Analysis/ExpInfo.h"
#include "CAFAna/Analysis/Surface.h"
#include "CAFAna/Systs/SBNWeightSysts.h"
using namespace ana;

#include "OscLib/IOscCalculator.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH2.h"
#include "TLegend.h"
#include "TString.h"
#include "TGraph.h"

#include <fstream>
#include <vector>

const double sbndPOT = kPOTnominal;
const double icarusPOT = kPOTnominal;
const double uboonePOT = 1.3e21;

const std::string numuStr = "numu";
const std::string nueStr = "nue";

void exclusion(const std::string anatype = numuStr)
{
  const char* name_in;
  const char* name_out;
  if (anatype == numuStr) {
    name_in = "surfaces_numu.root";
    name_out = "exclusion_numu.pdf";
  }
  else if (anatype == nueStr) {
    name_in = "surfaces_nue.root";
    name_out = "exclusion_nue.pdf";
  }
  else {
    std::cout << "Must specifiy nue or numu" << std::endl;
    return;
  }

  TFile fin(name_in);  
  TFile fprop("sterile_3p1_limits.root");

  //Surface& surf_syst_nd = *ana::LoadFrom<Surface>(fin.GetDirectory("exclusion/nd_prop_systs")).release(); 
  //Surface& surf_syst_fd = *ana::LoadFrom<Surface>(fin.GetDirectory("exclusion/fd_prop_systs")).release(); 
  //Surface& surf_syst_ub = *ana::LoadFrom<Surface>(fin.GetDirectory("exclusion/ub_prop_systs")).release(); 
  Surface& surf_syst_nd_fd = *ana::LoadFrom<Surface>(fin.GetDirectory("exclusion/nd_fd_prop_systs")).release();
  //Surface& surf_syst_all = *ana::LoadFrom<Surface>(fin.GetDirectory("exclusion/allexpt_prop_systs")).release(); 

  //Surface& surf_nom_nd = *ana::LoadFrom<Surface>(fin.GetDirectory("exclusion/nom_nd")).release(); 
  //Surface& surf_nom_ub = *ana::LoadFrom<Surface>(fin.GetDirectory("exclusion/nom_ub")).release(); 
  //Surface& surf_nom_fd = *ana::LoadFrom<Surface>(fin.GetDirectory("exclusion/nom_fd")).release(); 
  Surface& surf_nom_nd_fd = *ana::LoadFrom<Surface>(fin.GetDirectory("exclusion/nom_nd_fd")).release(); 
  //Surface& surf_nom = *ana::LoadFrom<Surface>(fin.GetDirectory("exclusion/nom")).release(); 

  TGraph * proposal_90pctCL  = (TGraph *) fprop.Get( "lim_dis_3p1_sbnproposal_90pctCL"  );
  TGraph * proposal_3sigCL   = (TGraph *) fprop.Get( "lim_dis_3p1_sbnproposal_3sigCL"   );
  TGraph * proposal_5sigCL   = (TGraph *) fprop.Get( "lim_dis_3p1_sbnproposal_5sigCL"   );
  TGraph * minosp_90pctCL       = (TGraph *) fprop.Get( "lim_dis_3p1_minosp_90pctCL"       );
  TGraph * minisciboone_90pctCL = (TGraph *) fprop.Get( "lim_dis_3p1_minisciboone_90pctCL" );

  TH2* crit5sig = Gaussian5Sigma1D1Sided(surf_nom_nd_fd);
  TH2* crit3sig = Gaussian3Sigma1D1Sided(surf_nom_nd_fd);
  TH2* crit90 = Gaussian90Percent1D1Sided(surf_nom_nd_fd);
  TH2* crit95 = Gaussian95Percent1D1Sided(surf_nom_nd_fd);
  TH2* crit99 = Gaussian99Percent1D1Sided(surf_nom_nd_fd);

  //surf_nom.SetTitle("90% Exclusion");
 
  //surf_nom.DrawContour(crit90, 7, kBlack);
  surf_nom_nd_fd.DrawContour(crit90, 7, kMagenta);
  //surf_nom_nd.DrawContour(crit90,7,kRed);
  //surf_nom_ub.DrawContour(crit90,7,kGreen+3);
  //surf_nom_fd.DrawContour(crit90, 7, kBlue);

  //surf_syst_all.DrawContour(crit90, kSolid, kBlack);
  //surf_syst_nd.DrawContour(crit90, kSolid, kRed);
  //surf_syst_ub.DrawContour(crit90, kSolid, kGreen+3);
  //surf_syst_fd.DrawContour(crit90, kSolid, kBlue);
  surf_syst_nd_fd.DrawContour(crit90, kSolid, kMagenta);
    
  //proposal_90pctCL->SetLineColor(kGreen);
  //proposal_90pctCL->Draw("l");
  //minosp_90pctCL->SetLineColor(kCyan);
  //minosp_90pctCL->Draw("l");
  //minisciboone_90pctCL->SetLineColor(kOrange);
  //minisciboone_90pctCL->Draw("l");

  TLegend * lgdis = new TLegend(0.11,0.11,0.40,0.40);
  lgdis->SetFillColor(0);
  lgdis->SetBorderSize(0);
  //lgdis->AddEntry(proposal_90pctCL, "Proposal 90%");
  //lgdis->AddEntry(minosp_90pctCL, "Minos/Minos+ 90%");
  //lgdis->AddEntry(minisciboone_90pctCL, "MiniBoone/SciBoone 90%");
  TH1* dummy = new TH1F("", "", 1, 0, 1);
  dummy->SetLineColor(kRed);
  lgdis->AddEntry(dummy->Clone(), "SBND");
  dummy->SetLineColor(kGreen+3);
  lgdis->AddEntry(dummy->Clone(), "MicroBoone");
  dummy->SetLineColor(kBlue);
  lgdis->AddEntry(dummy->Clone(), "Icarus");
  dummy->SetLineColor(kMagenta);
  lgdis->AddEntry(dummy->Clone(), "SBND + Icarus");
  dummy->SetLineColor(kBlack);
  lgdis->AddEntry(dummy->Clone(), "All");
  dummy->SetLineStyle(7);
  lgdis->AddEntry(dummy->Clone(), "Stats Only");

  lgdis->Draw("same");
  gPad->Print(name_out);

//  TFile fout("exclusion_graphs.root", "RECREATE");
//  
//  std::vector<TGraph*> graph_nd_fd_90 = surf_nom_nd_fd.GetGraphs(crit90);
//  std::vector<TGraph*> graph_nd_fd_95 = surf_nom_nd_fd.GetGraphs(crit95);
//  std::vector<TGraph*> graph_nd_fd_99 = surf_nom_nd_fd.GetGraphs(crit99);
//  std::vector<TGraph*> graph_nd_fd_3s = surf_nom_nd_fd.GetGraphs(crit3sig);
//  std::vector<TGraph*> graph_nd_fd_5s = surf_nom_nd_fd.GetGraphs(crit5sig);
//  
//  std::vector<Surface> surfaces {surf_nom, surf_nom_nd, surf_nom_ub, surf_nom_fd, surf_nom_nd_fd, surf_syst_all, surf_syst_nd, surf_syst_ub, surf_syst_fd, surf_syst_nd_fd};
//
//  std::vector<TH2*> con_lev {crit90, crit95, crit99, crit3sig, crit5sig};
//
//  std::vector<std::string> s_name {"all_stat", "nd_stat", "ub_stat", "fd_stat", "nd_fd_stat", "all_syst", "nd_syst", "ub_syst", "fd_syst", "nd_fd_syst"};
//
//  std::vector<std::string> c_name {"_90pct", "_95pct", "_99pct", "_3sig", "_5sig"};
//  
//  std::cout<<"Before Loop"<<std::endl;
// 
//  ofstream txtfile;
//  txtfile.open("CAFAna_exclusion.txt");
//
//  //std::vector<double> x_v;
//  //std::vector<double> y_v;
//
//  for (int i = 0; i < 10; ++i) {
//    txtfile<<s_name[i]<<"\n"<<"=========="<<"\n";
//    for (int j = 0; j < 5; ++j) {
//      std::vector<TGraph*> graphs = surfaces[i].GetGraphs(con_lev[j]);
//      std::cout<<"Got Graph"<<std::endl;
//      txtfile<<c_name[j]<<"\n"<<"------"<<"\n";
//      for (auto g : graphs) {
//        g->Write((s_name[i]+c_name[j]).c_str());
//        int n = g->GetN();
//        double *x = g->GetX();
//        double *y = g->GetY();
//        for (int k = 0; k < n; ++k) {
//	  //x_v.push_back(x[k]);
//	  //y_v.push_back(y[k]);
//          txtfile<<x[k]<<" "<<y[k]<<"\n";
//	}
//      }
//      txtfile<<"\n";
//    }
//    txtfile<<"\n\n";
//  } 
//
//  txtfile.close();

}

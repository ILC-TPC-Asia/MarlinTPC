#define CHECK_DIST
#include <iomanip>
#include "Runinfo.h+"

#define __MODULE3__
//#define __MODULE0__

#ifdef __MODULE3__
void HitEfficiency(Int_t module = 3, Int_t row = 16)
#endif
#ifdef __MODULE0__
void HitEfficiency(Int_t module = 0, Int_t row = 23)
#endif
{
  //const Int_t kNp = 11;
  //const Int_t kNp = 9;
  const Int_t kNp = 100; //10;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp], dlen[kNp];
#ifdef __MODULE3__
  //Int_t kRun[] = {19982, 19983, 19984, 19985, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972};
  //Int_t kRun[] = { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}; // B=1T
  //Int_t kRun[] = {19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972};
  //Int_t kRun[] = {19982, 19983, 19984, 19985, 19981, 19980, 19975, 19974, 19973, 19972};
  //Int_t kRun[] = {19981, 19980};
  //Int_t kRun[] = { 19676, 19671, 19672, 19675, 19668, 19669, 19670, 19667, 19674}; // only module3
  //
  //w/ field shaper data set phi=0
 // Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // B=1T
  //Int_t kRun[] = {20042, 20041, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054};
  Int_t kRun[] = { 19895, 19894, 19893, 19892, 19891, 19890, 19889, 19888, 19887, 19886, 19885, 19884, 19883, 19882, 19881}; // B=1T,with gate phi=-20
  
  //w/ field shaper data set phi = 20
  //Int_t kRun[] = {20146, 20147, 20148, 20149, 20150, 20151, 20152, 20153, 20154, 20155, 20156, 20157, 20158, 20159};
#endif
#ifdef __MODULE0__
  //Int_t kRun[] = { 19676, 19677, 19675,19674, 19673, 19671, 19670, 19669, 19668}; // only module0
  Int_t kRun[] = { 19677, 19676, 19675, 19674, 19673,19672 19671, 19670, 19669, 19668}; // only module0
#endif
  Int_t nRun = sizeof(kRun)/sizeof(int);

  Int_t layer = (module < 2 ? 0 : module < 5 ? 28 : 56) + row;

  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  Double_t x, y, dx, dy;
  Int_t n = 0;
  for (Int_t i=0; i<nRun; i++) {
      Int_t run = kRun[i];
      Process(run, layer, x, y, dx, dy);
#ifndef TIME_DEP
      xdata[n] = x; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy; n++; 
#else
      xdata[n] = i; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy;
      dlen[n] = rinfo.GetDlength(kRun[i]);
      n++; 
#endif
  }

  TCanvas *c1 = new TCanvas("c1","",600,500);
  c1->cd();

  // ---------
  // Graph
  // ---------
  TGraphErrors *grp = new TGraphErrors(n,xdata,ydata,dxdata,dydata);
  stringstream titlestr;
  titlestr << "Hit efficiency (Module" << module << " Row" << row << ")" << ends;
  grp->SetTitle(titlestr.str().data());
#ifndef TIME_DEP
  Double_t xmin = 0.;
  Double_t xmax = 600.;
  grp->GetHistogram()->GetXaxis()->SetTitle("Drift Length [mm]");
#else
  Double_t xmin = -1;
  Double_t xmax = 10;
  grp->GetHistogram()->GetXaxis()->SetTitle("Sequence");
#endif
  grp->GetHistogram()->GetXaxis()->SetLimits(xmin,xmax);
  grp->GetHistogram()->GetYaxis()->SetTitle("Hit efficiency");
  grp->GetHistogram()->GetYaxis()->SetTitleOffset(1.3);
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  Double_t ymin = 0.949;
  Double_t ymax = 1.01;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);
 // stringstream titlestr;
 // titlestr << "NDF Row" << layer << ends;
 // grp->SetTitle(titlestr.str().data());

  // ---------
  // Fit
  // ---------
#if 1
  TF1 fun("fun","[0]+[1]*x");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  grp->Fit(&fun,"","",xmin,xmax);
  Double_t y0    = fun.GetParameter(0);
  Double_t dy0   = fun.GetParError (0);
  Double_t dydx  = fun.GetParameter(1);
  Double_t ddydx = fun.GetParError (1);
  Double_t chi2  = fun.GetChisquare();
  Int_t    ndf   = fun.GetNDF();

  TPaveText *pt = new TPaveText(xmin+6*(xmax-xmin)/10,ymin+4*(ymax-ymin)/5, 
		                xmin+9.8*(xmax-xmin)/10,ymin+4.9*(ymax-ymin)/5);
  pt->SetTextFont(132);
  pt->SetTextAlign(12);
#endif
  stringstream stext;
#if 0
  stext << "#chi^{2}/ndf = " << setprecision(3) << chi2 << "/" << ndf << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  pt->Draw();
  stext << "y0      = " << setw(6) << setprecision(3) << y0
        << " #pm "      << setw(4) << setprecision(2) << dy0
	<< ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  stext << "dy/dx = " << setw(6) << setprecision(3) << dydx
        << " #pm "    << setw(4) << setprecision(2) << ddydx
        << ends;
  pt->AddText(stext.str().data());
#endif

  // ---------
  // Info.
  // ---------
  for (Int_t i=0; i<nRun; i++) {
      stext.str("");
      stext.clear();
#ifndef TIME_DEP 
      stext << "r" << kRun[i] << ends;
#else
      stext << "z=" << dlen[i] << "cm" << ends;
#endif
      TLatex *runno = new TLatex;
      runno->SetTextFont(132);
      runno->SetTextAlign(12);
      Double_t yoff = (2*(i%2)-1)*(ymax-ymin)/7;
      runno->DrawLatex(xdata[i],ydata[i]+yoff,stext.str().data());
      Double_t xoff = (xmax-xmin)/nRun/10;
      TLine *line = new TLine(xdata[i]+xoff,ydata[i]+yoff,xdata[i],ydata[i]);
      line->SetLineStyle(3);
      line->Draw();
  }
#if 0
  // save plot as pdf file
  stringstream ofile;
  ofile << "Charge_Row" << layer << "_B" << rinfo.GetBfield(run) << "T"
        << "_P" << rinfo.GetMomentum(run) << "GeV.pdf"<< ends; 
  c1->Print(ofile.str().data());
#endif
}

static int lastrun = -1;

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  if (run != lastrun) {
    stringstream finstr;
    //finstr << "../Data/r" << run << ".root" << ends;
#ifdef __MODULE0__
    //finstr << "../module0/r" << run << ".root" << ends;
    finstr << "../Data_module0/r" << run << ".root" << ends;
#endif
#ifdef __MODULE3__
    //finstr << "../r" << run << ".root" << ends;
    //finstr << "../Data/r" << run << ".root" << ends;
    finstr << "../Data_module3/r" << run << ".root" << ends;

    //finstr << "../module3/zcog_trunc306_wo_hitflag/r" << run << ".root" << ends;
    //finstr << "../module3/19972-19985_bad_rows_removed/r" << run << ".root" << ends;
#endif
    TFile *hfp = new TFile(finstr.str().data());
  }
  //TNtupleD *hResXin = static_cast<TNtupleD*>(hfp->Get("MyRootFileProcessor/hits"));
  //TNtupleD *hResXin = static_cast<TNtupleD*>(hfp->Get("MyRootFileProcessor/trks"));
  TNtupleD *hitmap = static_cast<TNtupleD*>(hfp->Get("MyRootFileProcessor/hitmap"));
#ifdef CHECK_DIST
  stringstream hstr;
  hstr << "c" << "_" << rinfo.GetDlength(run) << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),"",400,400);
  cp->cd();
#endif

  ///// Cuts ////////////////////////
  const Int_t    kNdfCut    = 40;
  const Double_t kCpaMinCut = -8.;
  const Double_t kCpaMaxCut =  0.;
  const Double_t kChi2Cut   = 40000.;
#if 1 // phi = 0
  //const Double_t kTanfMinCut = -0.05;
  //const Double_t kTanfMaxCut =  0.02;
  const Double_t kPhi0MinCut = -9999.;
  const Double_t kPhi0MaxCut = +9999.;
  const Double_t kTanlMinCut = -9999.;
  const Double_t kTanlMaxCut = +9999.;
  const Int_t knTrksCut = 1;
  //const Double_t kfi0locMinCut = -9999;
  //const Double_t kfi0locMaxCut = +9999;
  //const Double_t kfi0locMinCut = 4.64;
  //const Double_t kfi0locMaxCut = 4.72;
  
  const Double_t kfi0locMinCut = 4.36;//for 19895
  const Double_t kfi0locMaxCut = 4.45;

#endif
#if 0 // phi = 10
  // loose cut
  const Double_t kTanfMinCut = -0.25;
  const Double_t kTanfMaxCut = -0.15;
  // tight cut
  //const Double_t kTanfMinCut = -0.19;
#endif
#if 0 // phi = 20
  // loose 
  const Double_t kTanfMinCut = -0.45;
  const Double_t kTanfMaxCut = -0.35;
  // tight
  //const Double_t kTanfMinCut = -0.39;
  //const Double_t kTanfMaxCut = -0.37;
#endif
#if 0 // phi = -20
  const Double_t kTanfMinCut = 0.25;
  const Double_t kTanfMaxCut = 0.35;
#endif
  ///////////////////////////////////
  stringstream cut;
  //cut << "nhits_pre==1&&nhits_pos==1&&"
  //cut << "nhits_pos==1&&";
  cut << "nTrks==1&&";
  cut    << "fi0>"  << kPhi0MinCut << "&&";
  cut    << "fi0<"  << kPhi0MaxCut << "&&";
   cut   << "fi0loc>"  << kfi0locMinCut << "&&";
   cut   << "fi0loc<"  << kfi0locMaxCut << "&&";
   cut   << "chi2<" << kChi2Cut   << "&&";
  //cut << "tanf<" << kTanfMaxCut << "&&tanf>" << kTanfMinCut << "&&";
  //cut << "nhits_pos==1&&";
   cut << "serialrow==" << layer;
#if 0
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
#endif
  cut << ends;

  hitmap->Draw("nhits_mid", cut.str().data());
  TH1D* htemp = static_cast<TH1D* >(gROOT->FindObject("htemp"));
  //cerr << "bin 0 : " << htemp->GetBinContent(0) << endl;
  //cerr << "bin 1 : " << htemp->GetBinContent(1) << endl;
  //cerr << "bin 2 : " << htemp->GetBinContent(2) << endl;
  Int_t nentries_0 = htemp->GetBinContent(1);
  Int_t nentries_1 = htemp->GetBinContent(2);
  Float_t sum = Float_t(nentries_0 + nentries_1);
  Float_t efficiency = Float_t(nentries_1)/sum;
  //Float_t efferror = TMath::Sqrt(TMath::Power((TMath::Sqrt(Float_t(nentries_1))/sum),2.) +
  Float_t efferror = TMath::Sqrt(efficiency*(1-efficiency)/sum);
  //cerr << TMath::Sqrt(Float_t(nentries_1))/sum << endl;
  //cerr << (Float_t(nentries_1))/(sum*sum)*TMath::Sqrt(sum) << endl;
  x  = rinfo.GetDlength(run);
  y  = efficiency;
  dx = 0;
  dy = efferror; 
  x  *= 10.;
  dx *= 10.;
#ifdef CHECK_DIST
  htemp->SetMinimum(0.);
  htemp->SetMarkerStyle(4);
  htemp->SetMarkerSize(0.7);
  htemp->SetMarkerColor(2);
  htemp->SetLineColor(2);
  htemp->Draw("pe");
#endif
}

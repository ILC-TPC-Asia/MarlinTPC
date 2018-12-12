#define CHECK_DIST
#include <iomanip>
#include "Runinfo.h+"

#define __MODULE3__
//#define __MODULE0__

#ifdef __MODULE3__
void Tanl(Int_t module = 3, Int_t row = 16)
#endif
#ifdef __MODULE0__
void Tanl(Int_t module = 0, Int_t row = 23)
#endif
{
  //const Int_t kNp = 11;
  //const Int_t kNp = 9;
  const Int_t kNp = 100; //10;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp], dlen[kNp];
#ifdef __MODULE3__
  //Int_t kRun[] = {19982, 19983, 19984, 19985, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972};
  //Int_t kRun[] = { 19676, 19671, 19672, 19675, 19668, 19669, 19670, 19667, 19674}; // only module3
  //w/ Gate data set phi=0
  //Int_t kRun[] = {19982, 19983, 19984, 19985, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972};
  //w/ Gate data set phi = 10
  //Int_t kRun[] = {19740, 19741, 19742, 19743, 19739, 19738, 19737, 19736, 19735, 19734, 19733, 19732};
  
  //w/ Gate data set phi = 20
  //Int_t kRun[] = {19746, 19747, 19748, 19749, 19750, 19752, 19753, 19754, 19755, 19756, 19757, 19758};
  //Int_t kRun[] = {19746, 19747, 19748, 19749};
  //Int_t kRun[] = {19746};
 
  //w/ Gate data set phi = -20
  //Int_t kRun[] = {19881, 19882 , 19883 , 19884 , 19885 , 19886 , 19887 , 19888 , 19889 , 19890 , 19891 , 19892 , 19893 , 19894 , 19895};

  //w/ field shaper data set phi=0
  Int_t kRun[] = {20042, 20041, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054};
  
  //w/ field shaper data set phi=10
  //Int_t kRun[] = {20130, 20131, 20132, 20133, 20134, 20135, 20136, 20137, 20138, 20139, 20140, 20141, 20142, 20143}
  
  //w/ field shaper data set phi = 20
  //Int_t kRun[] = {20146, 20147, 20148, 20149, 20150, 20151, 20152, 20153, 20154, 20155, 20156, 20157, 20158, 20159};
  
  //w/ field shaper data set phi = -20
  //Int_t kRun[] = {20162, 20163, 20164, 20165, 20166, 20167, 20168, 20169, 20170, 20171, 20172, 20173, 20174, 20175, 20176};
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
  titlestr << "tan#theta (Module" << module << " Row" << row << ")" << ends;
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
  grp->GetHistogram()->GetYaxis()->SetTitle("tnl");
  grp->GetHistogram()->GetYaxis()->SetTitleOffset(1.3);
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  Double_t ymin = -0.02;
  Double_t ymax = 0.02;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);
 // stringstream titlestr;
 // titlestr << "NDF Row" << layer << ends;
 // grp->SetTitle(titlestr.str().data());

  // ---------
  // Fit
  // ---------
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
  stringstream stext;
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
    finstr << "../module0/r" << run << ".root" << ends;
#endif
#ifdef __MODULE3__
    //finstr << "../module3/r" << run << ".root" << ends;
    finstr << "../module3/zcog_trunc306/r" << run << ".root" << ends;
#endif
    TFile *hfp = new TFile(finstr.str().data());
  }
  TNtupleD *hits = static_cast<TNtupleD*>(hfp->Get("MyRootFileProcessor/hits"));
  //TNtupleD *hResXin = static_cast<TNtupleD*>(hfp->Get("MyRootFileProcessor/trks"));
#ifdef CHECK_DIST
  stringstream hstr;
  hstr << "c" << "_" << rinfo.GetDlength(run) << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),"",400,400);
  cp->cd();
#endif

  ///// Cuts ////////////////////////
  const Int_t    kNdfCut    = 10;
  const Double_t kCpaMinCut = -10.;
  const Double_t kCpaMaxCut =  10.;
#if 0 // phi = 0
  const Double_t kTanfMinCut = -0.05;
  const Double_t kTanfMaxCut =  0.02;
#endif
#if 0 // phi = 10
  // loose cut
  const Double_t kTanfMinCut = -0.25;
  const Double_t kTanfMaxCut = -0.15;
  // tight cut
  //const Double_t kTanfMinCut = -0.19;
#endif
#if 1 // phi = 20
  // very loose 
  const Double_t kTanfMinCut = -1.;
  const Double_t kTanfMaxCut = +1.;
  // loose 
  //const Double_t kTanfMinCut = -0.45;
  //const Double_t kTanfMaxCut = -0.35;
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
  cut << "ndf>"  << kNdfCut;
  cut << "&&ntrks==1";
  cut << "&&serialrow=="  << layer;
  cut << "&&tanf<" << kTanfMaxCut << "&&tanf>" << kTanfMinCut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;

  hits->Draw("tnl", cut.str().data());
  TH1D* htemp = static_cast<TH1D* >(gROOT->FindObject("htemp"));
  htemp->Rebin(3);
  htemp->Fit("gaus","","",-0.02,0.02);
  Double_t mu  = htemp->GetFunction("gaus")->GetParameter(1);
  Double_t sg  = htemp->GetFunction("gaus")->GetParameter(2);
  //Double_t mu = htemp->GetMean(); 
  cerr << "mu = " << mu << endl; 
  //Double_t sg  = htemp->GetRMS();

  x  = rinfo.GetDlength(run);
  y  = mu;
  dx = 0;
  dy = sg; 
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

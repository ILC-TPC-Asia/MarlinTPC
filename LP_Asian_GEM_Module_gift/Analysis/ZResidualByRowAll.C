//#define CHECK_DIST
#include <iomanip>
#include "Runinfo.h"

  const Int_t kNdl = 15; // 10
  //    dlen [cm]      5     10     15     20     25     30     35     40     45     50
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17375, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}; // B=1T
  //Int_t kRun[] = { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}; // B=1T
  //Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // B=1T
  Int_t kRun[] = { 20042, 20041, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054, 20055}; // B=1T
  //Int_t kRun[] = { 19932, 19928, 19929, 19931, 19930, 19933, 19934, 19935, 19936,19937,19938, 19939, 19940,19941, 19942}; // B=0T
  //
void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy);

void ZResidualByRowAll()
{
//--Set Style
  // Axis
  gStyle->SetLabelFont(22,"X");
  gStyle->SetLabelFont(22,"Y");
  gStyle->SetLabelSize(0.04,"X");
  gStyle->SetLabelSize(0.04,"Y");
  // Title offset
  gStyle->SetTitleOffset(1.0, "X");
  gStyle->SetTitleOffset(0.8, "Y");
  // Pad grid
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  // Title
  gStyle->SetTitleFont(22, "X");
  gStyle->SetTitleFont(22, "Y");
  
  gStyle->SetTitleSize(0.05, "X");
  gStyle->SetTitleSize(0.05, "Y");

  // ---------------
  // Get Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  //ofstream outfile("deltaz.temp.dat");
  //
  // ---------------
  // Create Canvas
  // ---------------
  stringstream canvasstr;
  canvasstr << "c" << "_delz" << ends;
  TCanvas *c1 = new TCanvas(canvasstr.str().data(),"",800,500);
 
  // ------------------
  // Set a graph frame
  // ------------------
  auto mgrp = new TMultiGraph();
  stringstream titlestr;
  titlestr << "Yoka Z Residual" << ends;
  mgrp->SetTitle(titlestr.str().data());
  mgrp->GetHistogram()->GetXaxis()->SetLimits(0.,84.);
  mgrp->GetHistogram()->GetXaxis()->SetTitle("Layer Serial No.");
  mgrp->GetHistogram()->GetYaxis()->SetTitle("#Delta z [mm]");
#if 1
  Double_t ymin = -2.0;
  Double_t ymax = +2.0;
#else
  Double_t ymin = -1.0;
  Double_t ymax = +1.0;
#endif
  mgrp->SetMinimum(ymin);
  mgrp->SetMaximum(ymax);

  // ----------------------
  // Loop over drift length
  // ----------------------
 //cerr << "start looping over idl" << endl;
  Int_t color = 0;
  for (Int_t idl=0; idl<kNdl; idl++) {
    const Int_t kNp = 84;
    Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
    Int_t n = 0;
    // -------------------
    // Loop over pad rows
    // -------------------
    for (Int_t layer=0; layer<kNp; layer++) {
#if 0
      int slayer = layer/28;
      int row    = layer%28;
      if (slayer != 1 || row < 5 || row > 22) continue;
#endif
// cerr << "start going to call process : idl = "<< idl << ", layer = " << layer << endl;
      Double_t x, y, dx, dy;
      Process(kRun[idl], layer, x, y, dx, dy);
      xdata[n] = x; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy; n++; 
      //outfile << x << " " << y << endl;
// cerr << "end of process : x = "<< x << ", y = " << y << endl;
    }
    //outfile.close();
  
    //--
    // Create a graph of residuals vs pad rows
    //--
    TGraphErrors *grp = new TGraphErrors(n,xdata,ydata,dxdata,dydata);
    grp->SetMarkerColor(color++);
    grp->SetMarkerStyle(21);
    mgrp->Add(grp);
// cerr << "added grp = " << grp << endl;
  }
// cerr << "end loop of idl"  << endl;
  // -------------------------------------------------------------
  // Draw a graph of residuals vs pad rows for all drift lengths
  // -------------------------------------------------------------
  c1->cd();
  mgrp->Draw("ap");

#if 0
  // save plot as pdf file
  stringstream ofile;
  ofile << "ZResidualByRow.pdf"<< ends; 
  c1->Print(ofile.str().data());
#endif
}

TFile *hfp = 0;
static int lastrun = -1;
void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

#ifdef CHECK_DIST
  stringstream hstr2;
  hstr2 << "c" << "_" << layer << ends;
  TCanvas *cp = new TCanvas(hstr2.str().data(),"",400,400);
  cp->cd();
#endif

  if (lastrun != run) {
    stringstream finstr;
    finstr << "../Data_module3/r" << run << ".root" << ends;
    hfp = new TFile(finstr.str().data());
  }

  TNtupleD *hResXin = static_cast<TNtupleD *>(hfp->Get("MyRootFileProcessor/hits"));

  stringstream cut;
  ///// Cuts ////////////////////////
#if 1
  const Double_t kResZmin   =  -3.;
  const Double_t kResZmax   =  +3.;
#if 0
  const Int_t    kNdfCut    =  155;
  const Double_t kChi2Cut   = 10000.;
#else
  const Int_t    kNdfCut    =  40;//100
  const Double_t kChi2Cut   = 40000.;//10000
#endif
  const Double_t kCpaMinCut = -8;//-0.2
  const Double_t kCpaMaxCut =  2;//0.8
  const Double_t kPhi0MinCut = -9999.;
  const Double_t kPhi0MaxCut = +9999.;
#else
  const Double_t kResZmin   = -1.0;
  const Double_t kResZmax   = +1.0;
  //const Int_t    kNdfCut    = 20;
  const Int_t    kNdfCut    = 24;
  //const Double_t kChi2Cut   = 300.;
  const Double_t kChi2Cut   = 100.;
  //const Double_t kCpaMinCut = -0.2;
  //const Double_t kCpaMaxCut =  4.65;
  const Double_t kCpaMinCut = -4.0;
  const Double_t kCpaMaxCut =  4.0;
  //const Double_t kPhi0MinCut = 4.68;
  //const Double_t kPhi0MaxCut = 6.27;
  const Double_t kPhi0MinCut = 4.64;
  const Double_t kPhi0MaxCut = 4.70;
#endif
  ///////////////////////////////////
  cut << "serialrow==" << layer << "&&"
      << "dzin>" << kResZmin << "&&"
      << "dzin<" << kResZmax << "&&"
      << "ndf>"  << kNdfCut   << "&&"
      << "chi2<" << kChi2Cut   << "&&"
      << "fi0>"  << kPhi0MinCut << "&&"
      << "fi0<"  << kPhi0MaxCut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;

  stringstream hstr;
  hstr << "delx" << setw(2) << setfill('0') << layer << ends;
  stringstream target;
  target << "dzin>>" << hstr.str().data() << "(100," << kResZmin << ", " << kResZmax << ")" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  TH1D *hp = new TH1D(*static_cast<TH1D *>(gROOT->FindObject(hstr.str().data())));
  //Double_t y, dy;
  if (hp && hp->GetEntries()) {
    hp->Rebin(1);
    hp->Fit("gaus","","",kResZmin,kResZmax);
    Double_t mu  = hp->GetFunction("gaus")->GetParameter(1);
    Double_t sg  = hp->GetFunction("gaus")->GetParameter(2);
    hp->Fit("gaus","","",mu-2.5*sg,mu+2.5*sg);

    y  = hp->GetFunction("gaus")->GetParameter(1);
    dy = hp->GetFunction("gaus")->GetParError(1);
#ifdef CHECK_DIST
    hp->SetMinimum(-1.);
    hp->SetMarkerStyle(4);
    hp->SetMarkerSize(0.7);
    hp->SetMarkerColor(2);
    hp->SetLineColor(2);
    hp->Draw("pe");
#endif
  } else {
    y  = 0;
    dy = 0;
#ifdef CHECK_DIST
    delete cp;
#endif
  }
  x  = layer;
  dx = 0;
  cerr << x << "+/-" << dx << " " << y << "+/-" << dy << endl;
  lastrun = run;
}

/*
void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();
#ifdef CHECK_DIST
  stringstream hstr;
  hstr << "c" << "_" << rinfo.GetDlength(run) << "_" << layer << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),hstr.str().data(),400,400);
  cp->cd();
#endif

  if (lastrun != run) {
    stringstream finstr;
    finstr << "../Data_module3/r" << run << ".root" << ends;
    hfp = new TFile(finstr.str().data());
 cerr << "file name = "<< finstr.str()  << endl;
  }

  TNtupleD *hResXin = static_cast<TNtupleD *>(gROOT->FindObject("hResXin"));
  stringstream item;
  item << "dzin" << setw(2) << setfill('0') << layer << ends;
  stringstream cut;

  ///// Cuts ////////////////////////
#if 0
  const Double_t kResZmin   = -0.1;
  const Double_t kResZmax   = +0.1;
  const Int_t    kNdfCut    =  28;
  const Double_t kChi2Cut   = 1000.;
  const Double_t kCpaMinCut =  0.2;
  const Double_t kCpaMaxCut =  0.8;
#else
#if 0
  const Double_t kResZmin   = -0.3;
  const Double_t kResZmax   = +0.3;
  //const Int_t    kNdfCut    =  140;
  //const Int_t    kNdfCut    =  32;
  const Int_t    kNdfCut    =  20;
  //const Double_t kChi2Cut   = 10000.;
  const Double_t kChi2Cut   = 300.;
  //const Double_t kCpaMinCut = -0.2;
  //const Double_t kCpaMaxCut =  0.8;
  const Double_t kCpaMinCut = -4.;
  const Double_t kCpaMaxCut =  4.;
  const Double_t kPhi0MinCut = 6.22;
  const Double_t kPhi0MaxCut = 6.28;
  const Double_t kTanlMinCut = -0.02;
  const Double_t kTanlMaxCut = +0.02;
#else
  //const Double_t kResZmin   = -0.3;
  //const Double_t kResZmax   = +0.3;
  const Double_t kResZmin   = -1;
  const Double_t kResZmax   = +1;
  //const Int_t    kNdfCut    =  80;
  const Int_t    kNdfCut    =  40;
  //const Int_t    kNdfCut    =  34;
  //const Int_t    kNdfCut    =  30;
  const Double_t kChi2Cut   = 10000.;
  const Double_t kCpaMinCut = -8.;
  const Double_t kCpaMaxCut =  2.;
  //const Double_t kPhi0MinCut = 6.22;
  //const Double_t kPhi0MaxCut = 6.28;
  const Double_t kPhi0MinCut = -0.03;
  const Double_t kPhi0MaxCut =  0.06;
  const Double_t kTanlMinCut = -0.04;
  const Double_t kTanlMaxCut = +0.04;
#endif
#endif
  ///////////////////////////////////
  cut << item.str().data() << ">" << kResZmin << "&&"
      << item.str().data() << "<" << kResZmax << "&&"
      << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut   << "&&"
      << "fi0>"  << kPhi0MinCut << "&&"
      << "fi0<"  << kPhi0MaxCut << "&&"
      << "tnl>"  << kTanlMinCut << "&&"
      << "tnl<"  << kTanlMaxCut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;

  stringstream target;
  target << item.str().data() << ">>htemp(100," << kResZmin << ", " << kResZmax << ")" << ends;

  TNtupleD *hResZin = static_cast<TNtupleD *>(gROOT->FindObject("hResZin"));
  hResZin->Draw(target.str().data(), cut.str().data());
  TH1 *htemp = static_cast<TH1 *>(gROOT->FindObject("htemp"));
  const Int_t kNcut = 10;
  if (htemp && (htemp->GetEntries() > kNcut)) {
    htemp->Rebin(1);
    htemp->Fit("gaus","","",-0.3,0.3);
    Double_t mu  = htemp->GetFunction("gaus")->GetParameter(1);
    Double_t sg  = htemp->GetFunction("gaus")->GetParameter(2);
    htemp->Fit("gaus","","",mu-2.5*sg,mu+2.5*sg);

    y  = htemp->GetFunction("gaus")->GetParameter(1);
    dy = htemp->GetFunction("gaus")->GetParError(1);
    y  *= 10.;
    dy *= 10.;

#ifdef CHECK_DIST
    htemp->SetMinimum(-0.1);
    htemp->SetMarkerStyle(4);
    htemp->SetMarkerSize(0.7);
    htemp->SetMarkerColor(2);
    htemp->SetLineColor(2);
    htemp->DrawCopy("pe");
#endif
  } else {
    y  = 0;
    dy = 0;
#ifdef CHECK_DIST
    delete cp;
#endif
  }
  x  = layer;
  dx = 0;
  cerr << x << "+/-" << dx << " " << y << "+/-" << dy << endl;
  lastrun = run;
}
*/

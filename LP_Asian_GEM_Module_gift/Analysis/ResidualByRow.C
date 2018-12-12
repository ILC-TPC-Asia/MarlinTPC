#define CHECK_DIST
#include <iomanip>
#include "Runinfo.h+"

  const Int_t kNdl = 11; // 10
  //    dlen [cm]      5     10     15     20     25     30     35     40     45     50
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17375, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}; // B=1T
 // Int_t kRun[] = { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}; // B=1T
 // Int_t kRun[] = { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}; // B=1T
 // Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // B=1T
 //Int_t kRun[] = { 20041, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054, 20055}; // B=1T
  Int_t kRun[] = { 19895, 19894, 19893, 19892, 19891, 19890, 19889, 19888, 19887, 19886, 19885, 19884, 19883, 19882, 19881}; // B=1T,with gate phi=-20


void Process(Int_t, Int_t, Double_t &, Double_t &, Double_t &, Double_t &);

void ResidualByRow(Int_t idl = 0)
{
//--Set Style
  // Axis
  gStyle->SetLabelFont(22,"X");
  gStyle->SetLabelFont(22,"Y");
  // Title offset
  gStyle->SetTitleOffset(1.0, "X");
  gStyle->SetTitleOffset(1.2, "Y");
  // Pad grid
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  // Title
  gStyle->SetTitleFont(22, "X");
  gStyle->SetTitleFont(22, "Y");
  gStyle->SetTitleFont(22, "");

  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  const Int_t kNp = 84;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];

  ofstream outfile("deltax.temp.dat");
  Double_t x, y, dx, dy;
  Int_t n = 0;
  for (Int_t layer=0; layer<kNp; layer++) {
#if 1
    int slayer = layer/28;
    int row    = layer%28;
    if (slayer != 1 || row < 5 || row > 22) continue;
#endif
    Process(kRun[idl], layer, x, y, dx, dy);
    xdata[n] = x; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy; n++; 
    outfile << x << " " << y << endl;
  }
  outfile.close();

  TCanvas *c1 = new TCanvas("c1","",800,500);
  c1->cd();

  TGraphErrors *grp = new TGraphErrors(n,xdata,ydata,dxdata,dydata);
  stringstream titlestr;
  titlestr << "Residual (Drift Length " << rinfo.GetDlength(kRun[idl]) << "[cm])" << ends;
  grp->SetTitle(titlestr.str().data());
  grp->GetHistogram()->GetXaxis()->SetLimits(0.,84.);
  grp->GetHistogram()->GetXaxis()->SetTitle("Layer Serial No.");
  grp->GetHistogram()->GetYaxis()->SetTitle("#Delta x [mm]");
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  Double_t ymin = -3.0;
  Double_t ymax = +3.0;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);

  TPaveText *pt = new TPaveText(330,4*ymax/5, 530.,(4.9*ymax)/5);

  pt->SetTextFont(132);
  pt->SetTextAlign(12);

#if 0
  // save plot as pdf file
  stringstream ofile;
  ofile << "ResidualByRow_DL" << rinfo.GetDlength(kRun[idl]) << ".pdf"<< ends; 
  //ofile << "ResidualByRow_DL" << rinfo.GetDlength(kRun[idl]) << ".C"<< ends; 
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
  const Double_t kResXmin   =  -3.;
  const Double_t kResXmax   =  +3.;
  const Double_t kfi0locMinCut = 4.36;//for 19895
  const Double_t kfi0locMaxCut = 4.45;
#if 0
  const Int_t    kNdfCut    =  155;
  const Double_t kChi2Cut   = 10000.;
  const Double_t kCpaMinCut = -0.2;
  const Double_t kCpaMaxCut =  0.8;
#else
  //const Int_t    kNdfCut    =  80;
  const Int_t    kNdfCut    =  40;
  const Double_t kChi2Cut   = 40000.;
  //const Double_t kCpaMinCut = -2.;
  //const Double_t kCpaMaxCut =  8.;
  const Double_t kCpaMinCut = -8.;
  const Double_t kCpaMaxCut =  2.;
#endif
  const Double_t kPhi0MinCut = -9999.;
  const Double_t kPhi0MaxCut = +9999.;
#else
  const Double_t kResXmin   = -1.0;
  const Double_t kResXmax   = +1.0;
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
      << "dxin>" << kResXmin << "&&"
      << "dxin<" << kResXmax << "&&"
      << "ndf>"  << kNdfCut   << "&&"
      << "chi2<" << kChi2Cut   << "&&"
      << "fi0>"  << kPhi0MinCut << "&&"
      << "fi0<"  << kPhi0MaxCut << "&&"
      << "fi0loc<"  << kfi0locMaxCut <<"&&"
      << "fi0loc>"  << kfi0locMinCut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;

  stringstream hstr;
  hstr << "delx" << setw(2) << setfill('0') << layer << ends;
  stringstream target;
  target << "dxin>>" << hstr.str().data() << "(100," << kResXmin << ", " << kResXmax << ")" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  TH1D *hp = new TH1D(*static_cast<TH1D *>(gROOT->FindObject(hstr.str().data())));
  //Double_t y, dy;
  if (hp && hp->GetEntries()) {
    hp->Rebin(1);
    hp->Fit("gaus","","",kResXmin,kResXmax);
    Double_t mu  = hp->GetFunction("gaus")->GetParameter(1);
    Double_t sg  = hp->GetFunction("gaus")->GetParameter(2);
    hp->Fit("gaus","","",mu-2.5*sg,mu+2.5*sg);

    y  = hp->GetFunction("gaus")->GetParameter(1);
    dy = hp->GetFunction("gaus")->GetParError(1);
    cerr << x << "+/-" << dx << " " << y << "+/-" << dy << endl;
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
  }
  x  = layer;
  dx = 0;
  lastrun = run;
}

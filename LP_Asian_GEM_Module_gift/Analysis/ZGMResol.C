#define CHECK_DIST
#include <iomanip>
#include "Runinfo.h+"

void Process(Int_t run, Int_t layer, Double_t &, Double_t &, Double_t &, Double_t &, const Char_t *inout);

void ZGMResol(Int_t module = 3, Int_t row = 16)
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

  const Int_t kNp = 14; //11;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
  //    dlen [cm]      5     10     15     20     25     30     35     40     45     50
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17375, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}; // B=1T
 // Int_t kRun[] = { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}; // B=1T
  //Int_t kRun[] = { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}; // B=1T
 Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // B=1T

  Int_t layer = (module < 2 ? 0 : module < 5 ? 28 : 56) + row;

  // ---------------
  // Loop over runs
  // ---------------
  Int_t n = 0;
  for (Int_t i=0; i<kNp; i++) {
    Int_t run = kRun[i];
    Double_t xin, yin, dxin, dyin;
    Process(run, layer, xin, yin, dxin, dyin, "in");

    Double_t xot, yot, dxot, dyot;
    Process(run, layer, xot, yot, dxot, dyot, "ot");

    Double_t y  = TMath::Sqrt(yin*yot);
    Double_t dy = (dyin+dyot)/2;

    xdata[n] = xin; ydata[n] = y; dxdata[n] = dxin; dydata[n] = dy; n++;
    cerr << " sigma = " << y << "+/-" << dy << endl;
  }
  // ------------------------
  // Fill z v.s. sigma_x plot
  // ------------------------
  TCanvas *c1 = new TCanvas("c1","",800,500);
  c1->cd();

  TGraphErrors *grp = new TGraphErrors(n,xdata,ydata,dxdata,dydata);
  stringstream titlestr;
  titlestr << "GM Resolutin (Module" << module << " Row" << row << ")" << ends;
  grp->SetTitle(titlestr.str().data());
  grp->GetHistogram()->GetXaxis()->SetLimits(0.,550.);
  grp->GetHistogram()->GetXaxis()->SetTitle("Drift Length: z [mm]");
  grp->GetHistogram()->GetYaxis()->SetTitle("#sigma_{z} [mm]");
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  double ymin = 0.0;
  double ymax = 1.0;
  //double ymax = 0.5;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);
  TF1 fun("fun","sqrt([0]**2+[1]**2*x)");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  grp->Fit(&fun,"","",100.,600.);
  Double_t sigma0        = fun.GetParameter(0) * 1.e3;             // [#mum]
  Double_t dsigma0       = fun.GetParError (0) * 1.e3;             // [#mum]
  Double_t cdbyrootneff  = fun.GetParameter(1) * sqrt(10.) * 1e3;  // [#mum/#sqrt{cm}]
  Double_t dcdbyrootneff = fun.GetParError (1) * sqrt(10.) * 1e3;  // [#mum/#sqrt{cm}]
  Double_t chi2          = fun.GetChisquare();
  Int_t    ndf           = fun.GetNDF();

  TPaveText *pt = new TPaveText(330,3.9*ymax/5, 530.,4.9*ymax/5);

  pt->SetFillColor(0);
  pt->SetTextFont(132);
  pt->SetTextAlign(12);
  stringstream stext;
  stext << "#chi^{2}/ndf = " << setprecision(3) << chi2 << "/" << ndf << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  pt->Draw();
  stext << "#sigma_{0} = " << setw(6) << setprecision(3) << sigma0
        << " #pm " << setw(4) << setprecision(2) << dsigma0
        << " [#mum]" << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  stext << "C_{D}/#sqrt{N_{eff}} = " << setw(6) << setprecision(3) << cdbyrootneff 
        << " #pm " <<  setw(4) << setprecision(2) << dcdbyrootneff 
        << " [#mum/#sqrt{cm}]" << ends;
  pt->AddText(stext.str().data());
  pt->Draw();

  TLatex *fitfun = new TLatex;
  fitfun->SetTextFont(132);
  fitfun->SetTextAlign(12);
  fitfun->DrawLatex(100,4*ymax/5, "#sigma_{z} = #sqrt{#sigma_{0}^{2}+(C_{D}^{2}/N_{eff}) z}");

#if 0
  // save plot as pdf  file
  Runinfo &rinfo = *Runinfo::GetInstancePtr();
  stringstream ofile;
  ofile << "GMResol_Row" << layer << "_B" << rinfo.GetBfield(run) << "T"
        << "_P" << rinfo.GetMomentum(run) << "GeV.pdf"<< ends; 
  c1->Print(ofile.str().data());
#endif
}

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, const Char_t *inout)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

#ifdef CHECK_DIST
  stringstream hstr;
  hstr << "c" << "_" << rinfo.GetDlength(run) << "_" << inout << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),hstr.str().data(),400,400);
  cp->cd();
#endif

  // ---------------
  // Open input file
  // ---------------
  stringstream finstr;
  finstr << "../Data_module3/r" << run << ".root" << ends;
  TFile *hfp = new TFile(finstr.str().data());

  // -----------------
  // Get residual data
  // -----------------
  TNtupleD *hResXin = static_cast<TNtupleD *>(hfp->Get("MyRootFileProcessor/hits"));

  stringstream item;
  item << "dz" << inout << ends;

  stringstream cut;

  ///// Cuts ////////////////////////
#if 1
  const Double_t kResZmin   = -2.;
  const Double_t kResZmax   = +2.;
#if 0
  const Int_t    kNdfCut    = 155;
  const Double_t kChi2Cut   = 4000.;
  const Double_t kCpaMinCut = -0.2;
  const Double_t kCpaMaxCut =  0.8;
#else
  const Int_t    kNdfCut    = 40;//80
  const Double_t kChi2Cut   = 40000.;
  const Double_t kCpaMinCut = -8.0;//-2
  const Double_t kCpaMaxCut =  2.0;//4
#endif
  const Double_t kPhi0MinCut = -9999.;
  const Double_t kPhi0MaxCut = +9999.;
  const Double_t kTanlMinCut = -9999.;
  const Double_t kTanlMaxCut = +9999.;
#else
  const Double_t kResZmin   = -5.;
  const Double_t kResZmax   = +5.;
  const Int_t    kNdfCut    = 24;
  //const Double_t kChi2Cut   = 300.;
  const Double_t kChi2Cut   = 100.;
  //const Double_t kCpaMinCut = -0.2;
  //const Double_t kCpaMaxCut =  0.8;
  //const Double_t kCpaMinCut = -4.0;
  //const Double_t kCpaMaxCut =  4.0;
  const Double_t kCpaMinCut = -0.6;
  const Double_t kCpaMaxCut =  1.0;
  //const Double_t kPhi0MinCut = 4.64;
  //const Double_t kPhi0MaxCut = 4.70;
  const Double_t kPhi0MinCut = 4.66;
  const Double_t kPhi0MaxCut = 4.695;
  //const Double_t kPhi0MinCut = 4.655;
  //const Double_t kPhi0MaxCut = 4.685;
  const Double_t kTanlMinCut = -0.008;
  const Double_t kTanlMaxCut =  0.005;
#endif
  ///////////////////////////////////
  cut << "serialrow==" << layer << "&&"
      << "nhits==1" << "&&"
      << item.str().data() << ">" << kResZmin << "&&"
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
  //target << item.str().data() << ">>htemp(100," << kResZmin << ", " << kResZmax << ")" << ends;
  target << item.str().data() << ">>htemp(50," << kResZmin << ", " << kResZmax << ")" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  // -----------------
  // Fit residuals
  // -----------------
  TH1F *htemp = static_cast<TH1F*> (gROOT->FindObject("htemp"));
  htemp->Rebin(1);
  //htemp->Fit("gaus","","",-0.3,0.3);
  htemp->Fit("gaus","RI","",-1.3,1.3);
  Double_t mu  = htemp->GetFunction("gaus")->GetParameter(1);
  Double_t sg  = htemp->GetFunction("gaus")->GetParameter(2);
  htemp->Fit("gaus","RI","",mu-2.5*sg,mu+2.5*sg);

  x  = rinfo.GetDlength(run) * 10; // [cm] to [mm]
  y  = htemp->GetFunction("gaus")->GetParameter(2); // sigma
  dx = 0;
  dy = htemp->GetFunction("gaus")->GetParError(2);  // its error
#ifdef CHECK_DIST
  htemp->SetMinimum(-0.1);
  htemp->SetMarkerStyle(4);
  htemp->SetMarkerSize(0.7);
  htemp->SetMarkerColor(2);
  htemp->SetLineColor(2);
  htemp->Draw("pe");
#endif
}

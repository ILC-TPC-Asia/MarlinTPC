#define CHECK_DIST
#include <iomanip>
#include "Runinfo.h+"

void ZResidual(Int_t module = 3, Int_t row = 16)
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

  const Int_t kNp = 14; // 10
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
  //    dlen [cm]      5     10     15     20     25     30     35     40     45     50
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17375, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}; // B=1T
  //Int_t kRun[] = { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}; // B=1T
 Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // B=1T

  Int_t layer = (module < 2 ? 0 : module < 5 ? 28 : 56) + row;

  Double_t x, y, dx, dy;
  Int_t n = 0;
  n = 0;
  for (Int_t i=0; i<kNp; i++) {
    Process(kRun[i], layer, x, y, dx, dy);
    xdata[n] = x; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy; n++; 
  }

  TCanvas *c1 = new TCanvas("c1","",800,500);
  c1->cd();

  TGraphErrors *grp = new TGraphErrors(n,xdata,ydata,dxdata,dydata);
  stringstream titlestr;
  titlestr << "Residual (Row" << layer << ")" << ends;
  grp->SetTitle(titlestr.str().data());
  grp->GetHistogram()->GetXaxis()->SetLimits(0.,550.);
  grp->GetHistogram()->GetXaxis()->SetTitle("Drift Length: z [mm]");
  grp->GetHistogram()->GetYaxis()->SetTitle("#Delta z [mm]");
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  Double_t ymin = -3.0;
  Double_t ymax = +3.0;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);
  TF1 fun("fun","[0]+[1]*x");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  grp->Fit(&fun,"","",10.,600.);
  grp->Draw("same");
  Double_t p0   = fun.GetParameter(0) * 1.e3; // [#mum]
  Double_t dp0  = fun.GetParError (0) * 1.e3; // [#mum]
  Double_t p1   = fun.GetParameter(1) * 1.e4; // [#mum/cm]
  Double_t dp1  = fun.GetParError (1) * 1.e4; // [#mum/cm]
  Double_t chi2 = fun.GetChisquare();
  Int_t    ndf  = fun.GetNDF();

  TPaveText *pt = new TPaveText(330,3.*ymax/5, 540.,(4.8*ymax)/5);

  pt->SetTextFont(132);
  pt->SetTextAlign(12);
  stringstream stext;
  stext << "#chi^{2}/ndf = " << setprecision(3) << chi2 << "/" << ndf << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  stext << "#Delta z_{0} = " << setw(6) << setprecision(3) << p0 
        << " #pm " << setw(4) << setprecision(2) << dp0
        << " [#mum]" << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  stext << "Slope = " << setw(6) << setprecision(3) << p1
        << " #pm " <<  setw(4) << setprecision(2) << dp1 
        << " [#mum/cm]" << ends;
  pt->AddText(stext.str().data());
  pt->Draw();

  TLatex *fitfun = new TLatex;
  fitfun->SetTextFont(132);
  fitfun->SetTextAlign(12);
  fitfun->DrawLatex(100,ymax/5, "#Delta z = #Delta z_{0}+Slope * z");

#if 0
  // save plot as pdf file
  stringstream ofile;
  ofile << "Residual_Row" << layer << ".pdf"<< ends; 
  c1->Print(ofile.str().data());
#endif
}

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

#ifdef CHECK_DIST
  stringstream hstr;
  hstr << "c" << "_" << layer << "_" << rinfo.GetDlength(run) << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),"",400,400);
  cp->cd();
#endif

  stringstream finstr;
  finstr << "../Data/r" << run << ".root" << ends;
  TFile *hfp = new TFile(finstr.str().data());

  TNtupleD *hResXin = static_cast<TNtupleD *>(hfp->Get("MyRootFileProcessor/hits"));

  stringstream cut;
  ///// Cuts ////////////////////////
  const Double_t kResZmin   =  -3.;
  const Double_t kResZmax   =  +3.;
#if 0
  const Int_t    kNdfCut    =  155;
  const Double_t kChi2Cut   = 10000.;
#else
  const Int_t    kNdfCut    =  80;
  const Double_t kChi2Cut   = 40000.;
#endif
  const Double_t kCpaMinCut = -0.2;
  const Double_t kCpaMaxCut =  0.8;
  ///////////////////////////////////
  cut << "serialrow==" << layer << "&&"
      << "dzin>" << kResZmin << "&&"
      << "dzin<" << kResZmax << "&&"
      << "ndf>"  << kNdfCut   << "&&"
      << "chi2<" << kChi2Cut;
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
  hp->Rebin(1);
  hp->Fit("gaus","","",kResZmin,kResZmax);
  Double_t mu  = hp->GetFunction("gaus")->GetParameter(1);
  Double_t sg  = hp->GetFunction("gaus")->GetParameter(2);
  hp->Fit("gaus","","",mu-2.5*sg,mu+2.5*sg);

  x  = rinfo.GetDlength(run) * 10; // [cm] to [mm]
  y  = hp->GetFunction("gaus")->GetParameter(1);
  dx = 0;
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
}

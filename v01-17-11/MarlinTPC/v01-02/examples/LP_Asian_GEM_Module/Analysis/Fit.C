#define CHECK_DIST
#include <iomanip>
#include "Runinfo.h+"

void Fit(Int_t run = 19985, Int_t layer = 43)
{
    Double_t xin, yin, dxin, dyin;
    Process(run, layer, xin, yin, dxin, dyin, "in");

    Double_t xot, yot, dxot, dyot;
    Process(run, layer, xot, yot, dxot, dyot, "ot");

    Double_t y  = TMath::Sqrt(yin*yot);
    Double_t dy = (dyin+dyot)/2;

    cerr << " sigma = " << y << "+/-" << dy << endl;
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
  finstr << "../Data/r" << run << ".root" << ends;
  TFile *hfp = new TFile(finstr.str().data());
  TDirectoryFile *dirf = static_cast<TDirectoryFile *>(gROOT->FindObject("MyRootFileProcessor"));
  dirf->cd();

  // -----------------
  // Get residual data
  // -----------------
  TNtupleD *hResXin = static_cast<TNtupleD *>(gROOT->FindObject("hits"));

  stringstream item;
  item << "dx" << inout << ends;

  stringstream cut;

  ///// Cuts ////////////////////////
  const Double_t kResXmin   = -9999.;
  const Double_t kResXmax   = +9999.;
  const Int_t    kNdfCut    = 40;
  const Double_t kChi2Cut   = 4000.;
  const Double_t kCpaMinCut = -8;
  const Double_t kCpaMaxCut =  2;
  ///////////////////////////////////
  cut << "serialrow==" << layer << "&&"
      << item.str().data() << ">" << kResXmin << "&&"
      << item.str().data() << "<" << kResXmax << "&&"
      << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;
  hResXin->Draw(item.str().data(), cut.str().data());

  // -----------------
  // Fit residuals
  // -----------------
  htemp->Rebin(1);
  htemp->Fit("gaus","","",-0.3,0.3);
  Double_t mu  = htemp->GetFunction("gaus")->GetParameter(1);
  Double_t sg  = htemp->GetFunction("gaus")->GetParameter(2);
  htemp->Fit("gaus","","",mu-2.5*sg,mu+2.5*sg);

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

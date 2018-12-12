#define CHECK_DIST
//#define TIME_DEP
#include <iomanip>
#include "Runinfo.h+"

void Process(Int_t , Int_t , Double_t &, Double_t &, Double_t &, Double_t &);
void Charge(Int_t module = 3, Int_t row = 16)
{
  const Int_t kNp = 14;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp], dlen[kNp];
#ifndef TIME_DEP
  //   dlen [cm]      5     10     15     20     25     30     35     40     45     50
  //Int_t kRun[] = {17319, 17367, 17324, 17364, 17327, 17359, 17375, 17356, 17340, 17352}; // B=1T
 // Int_t kRun[] = {17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}; // B=1T
  Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // B=1T
 //Int_t kRun[] = { 20041, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054, 20055}; // B=1T
 // Int_t kRun[] = {19895, 19894, 19893, 19892, 19891, 19890, 19889, 19888, 19887, 19886, 19885, 19884, 19883, 19882, 19881}; // B=1T,with gate phi=-20
#else
  //Int_t kRun[] = {17319, 17324, 17327, 17340, 17352, 17356, 17359, 17364, 17367, 17375}; // B=1T
  //Int_t kRun[] = {17319, 17324, 17327, 17340, 17352, 17356, 17359, 17364, 17367, 17331,1111, 1112 ,1113,1114}; // B=1T
  //Int_t kRun[] = {19895, 19894, 19893, 19892, 19891, 19890, 19889, 19888, 19887, 19886, 19885, 19884, 19883, 19882, 19881}; // B=1T,with gate phi=-20
#endif

  Int_t layer = (module < 2 ? 0 : module < 5 ? 28 : 56) + row;

  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  Double_t x, y, dx, dy;
  Int_t n = 0;
  for (Int_t i=0; i<kNp; i++) {
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
  titlestr << "Gain Variation (Module" << module << " Row" << row << ")" << ends;
  grp->SetTitle(titlestr.str().data());
#ifndef TIME_DEP
  Double_t xmin =   0.;
  Double_t xmax = 600.;
  grp->GetHistogram()->GetXaxis()->SetTitle("Drift Length [mm]");
#else
  Double_t xmin = -1;
  Double_t xmax = 10;
  grp->GetHistogram()->GetXaxis()->SetTitle("Sequence");
#endif
  grp->GetHistogram()->GetXaxis()->SetLimits(xmin,xmax);
  grp->GetHistogram()->GetYaxis()->SetTitle("Charge: MP");
  grp->GetHistogram()->GetYaxis()->SetTitleOffset(1.3);
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  Double_t ymin = 0.;
  Double_t ymax = 2000.;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);
  //stringstream titlestr;
  titlestr << "Charge_Row" << layer << ends;
  grp->SetTitle(titlestr.str().data());

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
  for (Int_t i=0; i<kNp; i++) {
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
      Double_t xoff = (xmax-xmin)/kNp/10;
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

//static int lastrun = -1;
static int lastrun = 0;

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

    TFile *hfp ;
  if (run != lastrun) {
    stringstream finstr;
    //finstr << "../Data/r" << run << ".root" << ends;
    finstr << "../Data_module3/r" << run << ".root" << ends;
    //finstr << "../Data_module0/r" << run << ".root" << ends;
    hfp = new TFile(finstr.str().data());
  }
  TNtupleD *hResXin = static_cast<TNtupleD*>(hfp->Get("MyRootFileProcessor/hits"));
#ifdef CHECK_DIST
  stringstream hstr;
  hstr << "c" << "_" << rinfo.GetDlength(run) << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),"",400,400);
  cp->cd();
#endif
  stringstream dxstr;
  dxstr << "dxin" << setw(2) << setfill('0') << layer << ends;

  ///// Cuts ////////////////////////
  const Int_t    kNdfCut    =  40;
  const Double_t kChi2Cut   = 40000.;
  const Double_t kCpaMinCut = -8;
  const Double_t kCpaMaxCut =  2;
  const Double_t kQMaxCut   = 7000.;
  ///////////////////////////////////
  stringstream cut;
  cut << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut   << "&&"
      << "chg <" << kQMaxCut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;

  hResXin->Draw("chg", cut.str().data());
  //hResXin->Draw("chg");

  TH1F *htemp = static_cast<TH1F*> (gROOT->FindObject("htemp"));

  htemp->Rebin(1);
  htemp->Fit("landau","","",0.0,10000.);
  htemp->Fit("landau","","",0.0,10000.);
  Double_t mu  = htemp->GetFunction("landau")->GetParameter(1);
  Double_t sg  = htemp->GetFunction("landau")->GetParameter(2);

  x  = rinfo.GetDlength(run);
  y  = htemp->GetFunction("landau")->GetParameter(1);
  dx = 0;
  dy = htemp->GetFunction("landau")->GetParError(1);
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

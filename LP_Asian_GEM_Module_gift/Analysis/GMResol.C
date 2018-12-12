#define CHECK_DIST
#include <iomanip>
#include "Runinfo.h+"

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, const Char_t *inout);

void GMResol(Int_t module = 3, Int_t row = 16)
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
  //Int_t kRun[] = { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}; // B=1T
  //Int_t kRun[] = { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}; // B=1T
 Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // B=1T
 //Int_t kRun[] = { 20041, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054, 20055}; // B=1T
  //Int_t kRun[] = { 19868, 19867, 19866, 19865, 19869, 19864, 19870, 19863, 19862, 19861, 19871, 19860, 19872, 19873, 19874}; // B=1T,with gate(not recomend)
//  Int_t kRun[] = { 19895, 19894, 19893, 19892, 19891, 19890, 19889, 19888, 19887, 19886, 19885, 19884, 19883, 19882, 19881}; // B=1T,with gate(not recomend)
//  Int_t kRun[] = { 19743, 19742, 19741, 19740, 19739, 19738,19737, 19736, 19735, 19734, 19733, 19732}; // B=1T,with gate phi=10
  //Int_t kRun[] = { 19895, 19894, 19893, 19892, 19891, 19890, 19889, 19888, 19887, 19886, 19885, 19884, 19883, 19882, 19881}; // B=1T,with gate phi=-20
  //Int_t kRun[] = { 19746, 19747, 19748, 19749, 19750, 19752, 19753, 19754, 19755, 19756, 19757, 19758}; // B=1T,with gate phi=20


  Int_t layer = (module < 2 ? 0 : module < 5 ? 28 : 56) + row;

  // ---------------
  // Loop over runs
  // ---------------
  Int_t n = 0;
  for (Int_t i=0; i<kNp; i++) {
    Int_t run = kRun[i];

    cerr << "********Run=" << run << endl;

    Double_t xin, yin, dxin, dyin;
    Process(run, layer, xin, yin, dxin, dyin, "in");

    Double_t xot, yot, dxot, dyot;
    Process(run, layer, xot, yot, dxot, dyot, "ot");

    Double_t y  = TMath::Sqrt(yin*yot);
    Double_t dy = (dyin+dyot)/2;

    xdata[n] = xin; ydata[n] = y; dxdata[n] = dxin; dydata[n] = dy; n++;
    cerr << " sigma = " << y << "+/-" << dy << endl;
    // ofstream outf("result_module3/sigmax_module3.dat");
    // outf << y << " " << cd << " " << dcd <<endl;

    
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
  grp->GetHistogram()->GetYaxis()->SetTitle("#sigma_{x} [mm]");
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  double ymin = 0.0;
  //double ymin = 0.15;
  //double ymax = 0.8;
  //double ymax = 0.3;
  double ymax = 0.4;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);
  TF1 fun("fun","sqrt([0]**2+[1]**2*x)");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  grp->Fit(&fun,"","",200.,600.);
  //grp->Fit(&fun,"","",0.,600.);
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
  fitfun->DrawLatex(100,4*ymax/5, "#sigma_{x} = #sqrt{#sigma_{0}^{2}+(C_{D}^{2}/N_{eff}) z}");

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
  //finstr << "../Data_module3/r" << run << ".root" << ends;
  finstr << "../Data_test/r" << run << ".root" << ends;
  TFile *hfp = new TFile(finstr.str().data());

  // -----------------
  // Get residual data
  // -----------------
  TNtupleD *hResXin = static_cast<TNtupleD *>(hfp->Get("MyRootFileProcessor/hits"));

  stringstream item;
  item << "dx" << inout << ends;

  stringstream cut;

  ///// Cuts ////////////////////////
#if 1
  const Double_t kResXmin   = -1.5;
  const Double_t kResXmax   = +1.5;
  //const Double_t kResXmin   = -0.8;
  //const Double_t kResXmax   = +0.8;
#if 0
  const Int_t    kNdfCut    = 155;
  const Double_t kChi2Cut   = 4000.;
  const Double_t kCpaMinCut = -0.2;
  const Double_t kCpaMaxCut =  0.8;
#else
  const Int_t    kNdfCut    = 40;//80
  const Double_t kChi2Cut   = 40000.;
  const Double_t kCpaMinCut = -8;//-2
  const Double_t kCpaMaxCut =  2;//4
#endif
  const Double_t kPhi0MinCut = 4.37;
  const Double_t kPhi0MaxCut = 4.44;
  const Double_t kTanlMinCut = -9999.;
  const Double_t kTanlMaxCut = +9999.;
  const Int_t knTrksCut = 1;
  //const Double_t kfi0locMinCut = -9999;
  //const Double_t kfi0locMaxCut = +9999;
 //const Double_t kfi0locMinCut = 4.64;
  //const Double_t kfi0locMaxCut = 4.72;

  const Double_t kfi0locMinCut = 4.69;
  const Double_t kfi0locMaxCut = 4.76;
 
  //const Double_t kfi0locMinCut = 4.36;//for 19895
  //const Double_t kfi0locMaxCut = 4.43;

  //const Double_t kfi0locMinCut = 4.8;//for 19743
  //const Double_t kfi0locMaxCut = 5.1;
  
 // const Double_t kfi0locMinCut = 4.36;//for 19895
 // const Double_t kfi0locMaxCut = 4.40;
 
 // const Double_t kfi0locMinCut = 5.01;//for 19746
 // const Double_t kfi0locMaxCut = 5.15;
#else
  const Double_t kResXmin   = -1.0;
  const Double_t kResXmax   = +1.0;
  const Int_t    kNdfCut    = 5;//100
  //const Double_t kChi2Cut   = 300.;
  const Double_t kChi2Cut   = 10000.;
  //const Double_t kCpaMinCut = -0.2;
  //const Double_t kCpaMaxCut =  0.8;
  //const Double_t kCpaMinCut = -4.0;
  //const Double_t kCpaMaxCut =  4.0;
  const Double_t kCpaMinCut = -8; //-0.6
  const Double_t kCpaMaxCut =  2;//1.0
  //const Double_t kPhi0MinCut = 4.64;
  //const Double_t kPhi0MaxCut = 4.70;
  const Double_t kPhi0MinCut = -99999; //4.66
  const Double_t kPhi0MaxCut = 99999; //4.695
  //const Double_t kPhi0MinCut = 4.655;
  //const Double_t kPhi0MaxCut = 4.685;
  //const Double_t kTanlMinCut = -0.008;
  const Double_t kTanlMinCut = -99999;
  //const Double_t kTanlMaxCut =  0.005;
  const Double_t kTanlMaxCut =  99999;
#endif
  ///////////////////////////////////
  cut << "serialrow==" << layer << "&&"
      << "nhits==1" << "&&"
      << item.str().data() << ">" << kResXmin << "&&"
      << item.str().data() << "<" << kResXmax << "&&"
    //  << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut   << "&&"
    //  << "fi0>"  << kPhi0MinCut << "&&"
    //  << "fi0<"  << kPhi0MaxCut << "&&"
      << "fi0loc>"  << kfi0locMinCut << "&&"
      << "fi0loc<"  << kfi0locMaxCut << "&&"
      << "tnl>"  << kTanlMinCut << "&&"
      << "tnl<"  << kTanlMaxCut << "&&"
     << "nTrks<=" << knTrksCut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;
  stringstream target;
  //target << item.str().data() << ">>htemp(100," << kResXmin << ", " << kResXmax << ")" << ends;
  target << item.str().data() << ">>htemp(50," << kResXmin << ", " << kResXmax << ")" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  //hResXin->Draw(target.str().data());
  

  // -----------------
  // Fit residuals
  // -----------------
  TH1F *htemp = static_cast<TH1F*> (gROOT->FindObject("htemp"));
  htemp->Rebin(1);
 htemp->Fit("gaus","","",-0.3,0.3);
 //htemp->Fit("gaus","RI","",-1.3,1.3);
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

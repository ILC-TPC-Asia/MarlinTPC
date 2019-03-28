#define CHECK_DIST
#include <iomanip>
#include "Runinfo.h+"
//#include <string>

void Process(Int_t run, Int_t module, Int_t row, 
             Double_t &x, Double_t &y, Double_t &dx, Double_t &dy);

void Padres_rowloop_module3(Int_t module = 3)
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
  // -----------
  // Input data
  // -----------
  const Int_t kNp = 14; //11;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
  //    dlen [cm]      5     10     15     20     25     30     35     40     45     50
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17375, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}; // B=1T
  Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // B=1T
 //Int_t kRun[] = { 20041, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054, 20055}; // B=1T
 // Int_t kRun[] = { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}; // B=1T
 // Int_t kRun[] = { 19868, 19867, 19866, 19865, 19869, 19864, 19870, 19863, 19862, 19861, 19871, 19860, 19872, 19873, 19874}; // B=1T,with gate(not recomend)
   //Int_t kRun[] = { 19816, 19817, 19818, 19819, 19820, 19821, 19822, 19824, 19825, 19826, 19827, 19828};//non reco2 
   //Int_t kRun[] = { 19717, 19718, 19719, 19720, 19721, 19722, 19723, 19724, 19725, 19726, 19727, 19728};//non reco3
  //Int_t kRun[] = { 19743, 19742, 19741, 19740, 19739, 19738,19737, 19736, 19735, 19734, 19733, 19732}; // B=1T,with gate(not recomend) phi=10
  
  
     ofstream outf("result_module3/Cd_by_Row_gate_module3_19985.dat");
     ofstream outf2("result_module3/sigmaPR0_by_Row_gate_module3_19985.dat");
  // ---------------
  // Loop over rows
  // ---------------
#if 1
  Int_t k = 0;
  Int_t krow = 26;
  for (Int_t j=1; j<krow; j++) { Int_t row = j;
  //for (Int_t j=16; j<17; j++) { Int_t row = j;
    //Int_t row = j;
     row = j;
#endif    

  // ---------------
  // Loop over runs
  // ---------------
  Int_t n = 0;
  for (Int_t i=1; i<kNp; i++) {
    Int_t run = kRun[i];
    Double_t x, y, dx, dy;
    Process(run, module, row, x, y, dx, dy);
    
    xdata[i] = x; ydata[i] = y*y; dxdata[i] = dx; dydata[i] = 2*y*dy; 
  }
  
  // ---------------
  // Plot PRF vs z
  // ---------------
  TCanvas *c1 = new TCanvas("c1","",800,500);
  c1->cd();

  TGraphErrors *grp = new TGraphErrors(kNp,xdata,ydata,dxdata,dydata);
  stringstream titlestr;
  titlestr << "Pad Response (Module" << module << " Row" << row << ")" << ends;
  grp->SetTitle(titlestr.str().data());
  grp->GetHistogram()->GetXaxis()->SetLimits(0.,550.);
  grp->GetHistogram()->GetXaxis()->SetTitle("Drift Length: z [mm]");
  grp->GetHistogram()->GetYaxis()->SetTitle("#sigma_{PR}(0)^{2} [mm^{2}]");
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  grp->SetMinimum(0.);
  Double_t ymax = 1.5;
  grp->SetMaximum(ymax);
  TF1 fun("fun","[0]**2+[1]**2*x");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);

  fun.SetParameter(0,0.1);
  fun.SetParameter(1,0.01);
  grp->Fit(&fun,"","",200.,600.);
  //grp->Fit(&fun,"","",350.,600.);
  Double_t sigma0  = fun.GetParameter(0) * 1.e3;             // [#mum]
  Double_t dsigma0 = fun.GetParError (0) * 1.e3;             // [#mum]
  Double_t cd      = fun.GetParameter(1) * sqrt(10.) * 1e3;  // [#mum/#sqrt{cm}]
  Double_t dcd     = fun.GetParError (1) * sqrt(10.) * 1e3;  // [#mum/#sqrt{cm}]
  Double_t chi2    = fun.GetChisquare();
  Int_t    ndf     = fun.GetNDF();

  TPaveText *pt = new TPaveText(330,8*ymax/10, 530.,8*ymax/10+1.8*ymax/10);

  pt->SetTextFont(132);
  pt->SetTextAlign(12);
  stringstream stext;
  stext << "#chi^{2}/ndf = " << setprecision(3) << chi2 << "/" << ndf << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  stext << "#sigma_{PR}(0) = " << setw(6) << setprecision(4) << sigma0 
        << " #pm "             << setw(3) << setprecision(2) << dsigma0 
        << " [#mum]"           << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  stext << "C_{D} = " << setw(6) << setprecision(4) << cd
        << " #pm "    << setw(3) << setprecision(2) << dcd
        << " [#mum/#sqrt{cm}]" << ends;
  pt->AddText(stext.str().data());
  pt->Draw();

  TLatex *fitfun = new TLatex;
  fitfun->SetTextFont(132);
  fitfun->SetTextAlign(12);
  fitfun->DrawLatex(50.,ymax/2, "#sigma_{PR}^{2} = #sigma_{PR}(0)^{2}+(C_{D}^{2}) z");

#if 1
  // save plot as pdf file
  Runinfo &rinfo = *Runinfo::GetInstancePtr();
  stringstream ofile;
  ofile << "result_module3/PadRes_Module" << module << "_Row" << row << "_withGate_19985.pdf"<< ends;
  c1->Print(ofile.str().data());
#endif
#if 1
 //stringstream datname
     //datname << "result/Cd_by_row_field.dat" << ends;
     //string datname(result/Cd_by_row_field.dat) ;
     //cerr << "dat name: " << datname << endl;
     //ofstream outf("datname");
     //ofstream outf("result/Cd_by_Row_field.dat");
     outf << row << " " << cd << " " << dcd <<endl;
     //ofstream outf2("result/sigmaPR0_by_Row_field.dat");
     outf2 << row << " " << sigma0 << " " << dsigma0 <<endl;
     cerr << row <<" "<< cd << " " << dcd << endl;
     cerr << row <<" "<< sigma0 << " " << dsigma0 << endl;
#endif
}
}
void Process(Int_t run, Int_t module, Int_t row, 
             Double_t &x, Double_t &y, Double_t &dx, Double_t &dy)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();
  Double_t dlen  = rinfo.GetDlength(run);

#ifdef CHECK_DIST
  TDirectory *last = gDirectory;
  stringstream hstr;
  hstr << "c" << "_" << dlen << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),"",400,400);
  cp->cd();
#endif

  // ---------------
  // Open input file
  // ---------------
  stringstream finstr;
  finstr << "../Data_module3/r" << run << ".root" << ends;
  TFile *hfp = new TFile(finstr.str().data());

  ///// Cuts2 ////////////////////////
#if 0
  const Int_t    kNdfCut    = 20;
  const Double_t kChi2Cut   = 300.;
  const Double_t kCpaMinCut = -0.2;
  const Double_t kCpaMaxCut =  0.8;
  const Double_t kPhi0MinCut = 4.64;
  const Double_t kPhi0MaxCut = 6.27;
#else
  //const Int_t    kNdfCut    = 155;
  const Int_t    kNdfCut    = 30;
  const Double_t kChi2Cut   = 10000.;
  const Double_t kCpaMinCut = -8.0;
  const Double_t kCpaMaxCut =  2.0;
  //const Double_t kCpaMinCut = -10.0;
  //const Double_t kCpaMaxCut =  10.0;
  //const Double_t kPhi0MinCut = 4.64;
  //const Double_t kPhi0MaxCut = 4.70;
  const Int_t knTrksCut = 1;
  const Double_t kPhi0MinCut = -9999;
  const Double_t kPhi0MaxCut = 9999;
  const Double_t kfi0locMinCut = 4.64;//for recomendeed data
  const Double_t kfi0locMaxCut = 4.72;
  //const Double_t kfi0locMinCut = 0;
  //const Double_t kfi0locMaxCut = 10;
  
  //const Double_t kfi0locMinCut = 4.70;//for 19717
  //const Double_t kfi0locMaxCut = 4.76;

  //const Double_t kfi0locMinCut = 4.8;//for 19743
  //const Double_t kfi0locMaxCut = 5.1;

  //const Double_t kfi0locMinCut = 4.70;//for 19868
  //const Double_t kfi0locMaxCut = 4.80;
  //const Double_t kchgsumCut = 200;
#endif
  ///////////////////////////////////
  TNtupleD* hPadRes = dynamic_cast<TNtupleD*>(hfp->Get("MyRootFileProcessor/clrs"));
  stringstream cut;
  cut << "module==" << module   << "&&" 
      << "row=="    << row      << "&&"
      << "ndf>"  << kNdfCut     << "&&"
      << "chi2<" << kChi2Cut    << "&&"
      << "fi0loc>"  << kfi0locMinCut << "&&"
      << "fi0loc<"  << kfi0locMaxCut << "&&"
  //    << "fi0>"  << kPhi0MinCut << "&&"
  //    << "fi0<"  << kPhi0MaxCut << "&&"
      << "cpa>"  << kCpaMinCut  << "&&"
      << "cpa<"  << kCpaMaxCut  << "&&"
     // << "nTrks<=" << knTrksCut << "&&"
      << "nTrks<=" << knTrksCut ;
    //  << "chgsum>" << kchgsumCut ;
  hPadRes->Draw("normchg:drphi>>htmp(100,-4,4,440,0,1.1)",cut.str().data(),"goff");
  //hPadRes->Draw("normchg:drphi>>htmp(100,-4,4,440,0,1.1)",cut.str().data());
  //hPadRes->Draw("normchg:drphi>>htmp(100,-4,4,440,0,1.1)");
  TH2D *hq = static_cast<TH2D *>(gROOT->FindObject("htmp"));
  
  //hq->FitSlicesY();
  hq->FitSlicesY(0,0,-1,0,"QNRI");
  stringstream hslicestr;
  hslicestr << "htmp_1" << ends;
  TH1D *hslice = static_cast<TH1D *>(gROOT->FindObject(hslicestr.str().data()));

  TF1 func("user","[0]*exp(-pow((x-[1])/[2],2)/2.)/sqrt(2*3.141592*[2])+[3]",-8.,+8.);
  //TF1 func("user","[0]*exp(-pow((x-[1])/[2],2)/2.)/sqrt(2*3.141592*[2])",-8.,+8.);
  func.SetParameter(0,1.);
  func.SetParameter(1,0.);
  func.SetParameter(2,1.);
  //func.SetParameter(3,0.);
  hslice->Rebin(1);
  hslice->Fit("user","","",-2.,+2.);
  y  = hslice->GetFunction("user")->GetParameter(2);
  //hslice->Fit("user","","",-5*y,5*y);
  hslice->Fit("user","","",-2.5*y,2.5*y);
  y  = hslice->GetFunction("user")->GetParameter(2);
  //hslice->Fit("user","","",-5*y,5*y);
  hslice->Fit("user","","",-3.0*y,3.0*y);
  //hslice->Fit("user","","",-2.5*y,2.5*y);
  //hslice->Fit("user","","",-3.5*y,3.5*y);
  //hslice->Fit("user","","",-2.0*y,2.0*y);

  x  = dlen;
  y  = hslice->GetFunction("user")->GetParameter(2);
  dx = 0.1; //FIXME
  dy = hslice->GetFunction("user")->GetParError(2);
  const Int_t kcm2mm = 10;
  x  *= kcm2mm;
  dx *= kcm2mm;
#ifdef CHECK_DIST
  hslice->SetMinimum(-0.1);
  hslice->SetMaximum(1.2);
  hslice->SetMarkerStyle(4);
  hslice->SetMarkerSize(0.7);
  hslice->SetMarkerColor(2);
  hslice->SetLineColor(2);
  hq->Draw();
  hslice->Draw("pe same");
  func.Draw("same");
  last->cd();
#endif

  hfp->Close();
}

class dataFile : public TObject
{
  public: 
	  dataFile (Char_t* name, Int_t len) {
	    fname = name;
            flen  = len;
	  }

	  Char_t* getFileName()    { return fname; }
	  Int_t   getDriftLength() { return flen;  }

  private:
	  Char_t* fname; // input file name
	  Int_t    flen; // drift length [cm]
};

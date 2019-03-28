#define CHECK_DIST
#include <iomanip>
#include "Runinfo.h+"

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, const Char_t *inout);//GMResol
void Process(Int_t run, Int_t module, Int_t row, 
             Double_t &x, Double_t &y, Double_t &dx, Double_t &dy);//PadRes

void sigmax_sigmaPR(Int_t module = 3)
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
  Double_t xdata2[kNp], ydata2[kNp], dxdata2[kNp], dydata2[kNp];
  //    dlen [cm]      5     10     15     20     25     30     35     40     45     50
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17375, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}; // B=1T
  //Int_t kRun[] = { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}; // B=1T
 // Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // B=1T
 Int_t kRun[] = { 20041, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054, 20055}; // B=1T


   // ---------------
   // Loop over rows 
   // ---------------
 #if 1
     Int_t k = 0;
     Int_t krow = 26;
     for (Int_t j=21; j<krow; j++) { 
     Int_t row = j;
#endif


  Int_t layer = (module < 2 ? 0 : module < 5 ? 28 : 56) + row;
  // ---------------
  // Loop over runs
  // ---------------
  Int_t n = 0;
  for (Int_t i=0; i<kNp; i++) {
    Int_t run = kRun[i];
    Double_t xin, yin, dxin, dyin;
  cerr << "break3" << endl;
    Process(run, layer, xin, yin, dxin, dyin, "in");

    Double_t xot, yot, dxot, dyot;
    Process(run, layer, xot, yot, dxot, dyot, "ot");

    Double_t x2, y2, dx2, dy2;
    Process(run, layer, row, x2, y2, dx2, dy2);

    Double_t y  = TMath::Sqrt(yin*yot);
    Double_t dy = (dyin+dyot)/2;

    xdata[n] = xin; ydata[n] = y*y; dxdata[n] = dxin; dydata[n] = 2*y*dy; 
    cerr << " sigmax = " << y << "+/-" << dy << endl;

    xdata2[n] = x2; ydata2[n] = y2*y2; dxdata2[n] = dx2; dydata2[n] = 2*y2*dy2;
    cerr << " sigmaPR = " << y << "+/-" << dy << endl;
   n++; 
  }
  // ------------------------
  // Fill z v.s. sigma_x plot
  // ------------------------
  /*
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
  //double ymax = 0.8;
  double ymax = 0.3;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);
  TF1 fun("fun","sqrt([0]**2+[1]**2*x)");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  grp->Fit(&fun,"","",200.,600.);
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
  ofile << "result_module3/GMResol_Module" << module << "_Row" << row << "_withoutGate.pdf" << ends; 
  c1->Print(ofile.str().data());
#endif
#if 0
  outf << row << " " << cdbyrootneff << " " << dcdbyrootneff <<endl;
            outf2 << row << " " << sigma0 << " " << dsigma0 <<endl;
                 cerr << row <<" "<< cdbyrootneff << " " << dcdbyrootneff << endl;
                      cerr << row <<" "<< sigma0 << " " << dsigma0 << endl;
                      #endif
  */
}
}
void Process(Int_t run, Int_t layer, Int_t module,Int_t row,  Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, const Char_t *inout)
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

 //finstr.Close();      
  // -----------------
  // Get residual data
  // -----------------
  TNtupleD *hResXin = static_cast<TNtupleD *>(hfp->Get("MyRootFileProcessor/hits"));
  cerr << "hfp" << hfp<< endl;

  stringstream item;
  item << "dx" << inout << ends;

  stringstream cut;

  ///// Cuts ////////////////////////
#if 1
  //const Double_t kResXmin   = -1.5;
  //const Double_t kResXmax   = +1.5;
  const Double_t kResXmin   = -0.8;
  const Double_t kResXmax   = +0.8;
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
  const Double_t kPhi0MinCut = -9999.;
  const Double_t kPhi0MaxCut = +9999.;
  const Double_t kTanlMinCut = -9999.;
  const Double_t kTanlMaxCut = +9999.;
  const Int_t knTrksCut = 1;
  //const Double_t kfi0locMinCut = -9999;
  //const Double_t kfi0locMaxCut = +9999;
  const Double_t kfi0locMinCut = 4.64;
  const Double_t kfi0locMaxCut = 4.72;
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
      << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut   << "&&"      
      << "fi0>"  << kPhi0MinCut << "&&"
      << "fi0<"  << kPhi0MaxCut << "&&"
      << "fi0loc>"  << kfi0locMinCut << "&&"
      << "fi0loc<"  << kfi0locMaxCut << "&&"
      << "tnl>"  << kTanlMinCut << "&&"
      << "tnl<"  << kTanlMaxCut << "&&"
      << "nTrks<=" << knTrksCut ;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;
  
  stringstream cut2;
  cut2 << "module==" << module   << "&&" 
      << "row=="    << row      << "&&"
      << "ndf>"  << kNdfCut     << "&&"
      << "chi2<" << kChi2Cut    << "&&"
      << "fi0loc>"  << kfi0locMinCut << "&&"
      << "fi0loc<"  << kfi0locMaxCut << "&&"
  //    << "fi0>"  << kPhi0MinCut << "&&"
  //    << "fi0<"  << kPhi0MaxCut << "&&"
      << "cpa>"  << kCpaMinCut  << "&&"
      << "cpa<"  << kCpaMaxCut  << "&&"
      << "nTrks<=" << knTrksCut ;
cerr << "layer = " << layer << endl;

  stringstream target;
  //target << item.str().data() << ">>htemp(100," << kResXmin << ", " << kResXmax << ")" << ends;
  target << item.str().data() << ">>htemp(50," << kResXmin << ", " << kResXmax << ")" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  //hResXin->Draw(target.str().data());
  
//Padres
  TNtupleD* hPadRes = dynamic_cast<TNtupleD*>(hfp->Get("MyRootFileProcessor/clrs"));
  //hPadRes->Draw("normchg:drphi>>htmp(100,-4,4,440,0,1.1)",cut.str().data(),"goff");
  cerr << "hPadRes=" << hPadRes << endl;
  hPadRes->Draw("normchg:drphi>>htmp(100,-4,4,440,0,1.1)",cut2.str().data());
  //hPadRes->Draw("normchg:drphi>>htmp(100,-4,4,440,0,1.1)");

  // -----------------
  // Fit residuals
  // -----------------
  TH1F *htemp = static_cast<TH1F*> (gROOT->FindObject("htemp"));
  cerr << "gROOT" << gROOT <<endl;
  cerr << "htemp" << htemp <<endl;
  htemp->Rebin(1);
  //htemp->Fit("gaus","","",-0.3,0.3);
  htemp->Fit("gaus","RI","",-1.3,1.3);
  Double_t mu  = htemp->GetFunction("gaus")->GetParameter(1);
  Double_t sg  = htemp->GetFunction("gaus")->GetParameter(2);
   cerr <<"mu" << mu << endl; 
   cerr <<"sg" << sg << endl; 
    
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

  x  = rinfo.GetDlength(run) ; // [cm]
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











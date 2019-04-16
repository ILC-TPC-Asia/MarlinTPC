#define CHECK_DIST
#include <iomanip>
#include "Runinfo.h+"
void Process(Int_t run, Int_t module, Int_t row, 
             Double_t &x, Double_t &y, Double_t &dx, Double_t &dy);

void Padres_auto(Int_t module = 3, Int_t row = 18)
//void Padres(Int_t module = 0, Int_t row = 17)
{

//-Set Style
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
  const Int_t kNp = 15; //11;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
  //    dlen [cm]      5     10     15     20     25     30     35     40     45     50
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17375, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}; // B=1T
  //Int_t kRun[] = { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}; // B=1T

  //2016
    //straight 
    //Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // Set1 (http://www-jlc.kek.jp/jlc/ja/elog) kNp =14
    //Int_t kRun[] = { 19868, 19867, 19866, 19865, 19869, 19864, 19870, 19863, 19862, 19861, 19871, 19860, 19872, 19873, 19874}; // set2(not recomend) kNp =15
    //Int_t kRun[] = { 19717, 19718, 19719, 19720, 19721, 19722, 19723, 19724, 19725, 19726, 19727, 19728};//Set13 kNp =12 
    //Int_t kRun[] = { 20041, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054, 20055}; //Set16 FS kNp =14
    //Int_t kRun[] = { 20074, 20073, 20072, 20071, 20070, 20069, 20068, 20067, 20066, 20065, 20064, 20063, 20062, 20061, 20059, 20058 };//Set17 kNp=16, FS, noemal gain
   
 
    //angled  
    //Int_t kRun[] = { 19743, 19742, 19741, 19740, 19739, 19738,19737, 19736, 19735, 19734, 19733, 19732}; // B=1T,with gate phi=10
    //Int_t kRun[] = { 19895, 19894, 19893, 19892, 19891, 19890, 19889, 19888, 19887, 19886, 19885, 19884, 19883, 19882, 19881}; // B=1T,with gate phi=-20
    //Int_t kRun[] = { 19746, 19747, 19748, 19749, 19750, 19752, 19753, 19754, 19755, 19756, 19757, 19758}; // B=1T,with gate phi=20


//  Int_t kRun[] = { 19895, 19894, 19893, 19892, 19891, 19890, 19889, 19888, 19887, 19886, 19885, 19884, 19883, 19882, 19881}; // B=1T,with gate(not recomend)
 
 //?
 // Int_t kRun[] = { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}; // B=1T
  // Int_t kRun[] = { 19816, 19817, 19818, 19819, 19820, 19821, 19822, 19824, 19825, 19826, 19827, 19828};//non reco(0T) 

  // ---------------
  // Loop over runs
  // ---------------
  Int_t n = 0;
  for (Int_t i=0; i<kNp; i++) {
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
  grp->GetHistogram()->GetYaxis()->SetTitle("#sigma_{PR}^{2} [mm^{2}]");
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
  //grp->Fit(&fun,"","",250.,500.);
  grp->Fit(&fun,"","",200.,500.);
  //grp->Fit(&fun,"","",0.,500.);
  fun.SetParameter(0,fun.GetParameter(0));
  fun.SetParameter(1,fun.GetParameter(1));
  grp->Fit(&fun,"","",200.,550.);
  //grp->Fit(&fun,"","",0.,550.);
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

#if 0
  // save plot as pdf file
  Runinfo &rinfo = *Runinfo::GetInstancePtr();
  stringstream ofile;
  ofile << "PadRes_Module" << module << "_Row" << row << "_B" << rinfo.GetBfield(run) << "T" 
        << "_P" << rinfo.GetMomentum(run) << "GeV.pdf"<< ends;
  c1->Print(ofile.str().data());
#endif

}

void Process(Int_t run, Int_t module, Int_t row, 
             Double_t &x, Double_t &y, Double_t &dx, Double_t &dy)
{
  int year =2016;
  cerr<< "year=" << year <<endl;

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
  if(year==2016 && module==3){
      finstr << "../Data_module3/r" << run << ".root" << ends;}
  if(year==2012){
      finstr << "../Data_2012/r" << run << ".root" << ends;}
  else{finstr << "../Data_tmp/r" << run << ".root" << ends;}
  TFile *hfp = new TFile(finstr.str().data());
  //cerr << "Break1" <<endl;
  cerr << "hfp " << hfp <<endl;
  ///// Cuts ////////////////////////
#if 0
  const Int_t    kNdfCut    = 20;
  const Double_t kChi2Cut   = 300.;
  const Double_t kCpaMinCut = -0.2;
  const Double_t kCpaMaxCut =  0.8;
  const Double_t kPhi0MinCut = 4.64;
  const Double_t kPhi0MaxCut = 6.27;
#else
  //const Int_t    kNdfCut    = 155;
  const Int_t    kNdfCut    = 40;
  //const Int_t    kNdfCut    = 30;
  const Double_t kChi2Cut   = 10000.;
  const Double_t kCpaMinCut = -8.0;
  const Double_t kCpaMaxCut =  2.0;
  //const Double_t kPhi0MinCut = 4.64;
  //const Double_t kPhi0MaxCut = 4.70;
  const Int_t knTrksCut = 1;

   Double_t kfi0locMinCut = 0;
   Double_t kfi0locMaxCut = 0;//19985
  
   cerr << "Run=" << run << endl;

  if(run<=19985 && run>=19972){
     kfi0locMinCut = 4.64;
     kfi0locMaxCut = 4.72;//19985
  }
  if(run==20041){
     kfi0locMinCut = 4.69;
     kfi0locMaxCut = 4.76;//20041
  }
  //const Double_t kchgsumMinCut = 200;
  //const Double_t kchgsumMaxCut = 1000;
 // if(kRun[0]==19985){ 
     //  const Double_t kfi0locMinCut = 4.70;//for 19717
     //  const Double_t kfi0locMaxCut = 4.76;
  //}

  if(run<=20074 && run>=20058){
      kfi0locMinCut = 4.67;//for 20074
      kfi0locMaxCut = 4.75;
  }
  //else
  //    kPhi0MinCut = 4.37;
  //    kPhi0MaxCut = 4.44;
  //}

  //const Double_t kfi0locMinCut = 4.70;//for 19868
  //const Double_t kfi0locMaxCut = 4.80;
  
    //const Double_t kfi0locMinCut = 4.68;//for 19932
    //const Double_t kfi0locMaxCut = 4.75;

  //const Double_t kfi0locMinCut = 4.36;//for 19895
  //const Double_t kfi0locMaxCut = 4.45;
  //const Double_t kfi0locMinCut = 4.36;//for 19895
  //const Double_t kfi0locMaxCut = 4.40;

  //const Double_t kfi0locMinCut = 4.8;//for 19743
  //const Double_t kfi0locMaxCut = 5.1;

  //const Double_t kfi0locMinCut = 4;//for 19746
  //const Double_t kfi0locMaxCut = 5.5;

  //const Double_t kfi0locMinCut = 3;//for 19746
  //const Double_t kfi0locMaxCut = 7;
  
  //const Double_t kfi0locMinCut = 4.45;//for 18609
  //const Double_t kfi0locMaxCut = 4.7;

#endif
  ///////////////////////////////////
  //TNtupleD* hPadRes = dynamic_cast<TNtupleD*>(hfp->Get("MyRootFileProcessor/clrs"));
  TNtupleD* hPadRes = static_cast<TNtupleD*>(hfp->Get("MyRootFileProcessor/clrs"));
  cerr << "Break3" <<endl;
  stringstream cut;
  cut << "module==" << module   << "&&" 
      << "row=="    << row      << "&&"
      << "ndf>"  << kNdfCut << "&&"
      << "chi2<" << kChi2Cut    << "&&"
   //   << "fi0>"  << kPhi0MinCut << "&&"
   //   << "fi0<"  << kPhi0MaxCut << "&&"
      << "fi0loc>"  << kfi0locMinCut << "&&"
      << "fi0loc<"  << kfi0locMaxCut << "&&"
      << "cpa>"  << kCpaMinCut  << "&&"
      << "cpa<"  << kCpaMaxCut  << "&&"
      << "nTrks<=" << knTrksCut ;
  //    << "chgsum>" << kchgsumMinCut << "&&"
   //   << "chgsum<=" << kchgsumMaxCut ;
  
  //
   //  << "ndf>"  << kNdfCut << "&&"
    //  << "chi2<" << kChi2Cut;
   //  << "fi0loc>"  << kfi0locMinCut << "&&"
    //  << "fi0loc<"  << kfi0locMaxCut;
    //  << "fi0>"  << kPhi0MinCut;
  //    << "fi0<"  << kPhi0MaxCut << "&&"
   //   << "cpa>"  << kCpaMinCut << "&&"
    //  << "cpa<"  << kCpaMaxCut;
    //  << "nTrks<=" << knTrksCut ;
  cerr << "cut" << cut.str().data() <<endl;
  cerr << "hPadRes" << hPadRes <<endl;
 hPadRes->Draw("normchg:drphi>>htmp(100,-4,4,440,0,1.1)",cut.str().data(),"goff");
  cerr << "Break2" <<endl;
 // hPadRes->Draw("normchg:drphi>>htmp(100,-4,4,440,0,1.1)");
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

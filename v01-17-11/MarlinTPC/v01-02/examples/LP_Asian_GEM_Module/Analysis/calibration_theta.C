//#define __DEBUG__
#include <iomanip>
//#include "TFile.h"
//#include "TH1D.h"
//#include "TROOT.h"
//#include "TCanvas.h"
//#include "TNtupleD.h"
//#include "TH2D.h"
//#include "JCluster.h"
//#include "TGraph.h"
//#include "JREvent.h"
//#include "TSystem.h"
//#include <sstream> 
//#include "T.h"
//#include <cstdlib>
#include "Runinfo.h+"

const int kNlayers = 56;
const float kTimeBin = 50; //ns
const int kMaxWidth  = 50;
const int kMinWidth  =  4;
const int kThreshold = 3;

const int maxevent = 20000;

void calibration_theta(int module=3)
{
  const int kNp = 28;
  //int rnum[npoints];
  float dlen;
  float time;
  float ddlen;
  float dtime;

  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
    //Int_t kRun[] = { 19902, 19904, 19906};//Set5 kNp=3
    //Int_t kRun[] = { 19903, 19905, 19907};//Set5 kNp=3
    Int_t run = { 19907 };//Set5 kNp=3

    stringstream ofile;
    ofile << "result/dtcalibration_" << run << ".dat" << ends;
    ofstream outf(ofile.str().data());
   outf << "run" << " " << "irow" << " " << "deltadt" << " " << "deltal" <<endl;

  // ---------------
  // Loop over rows
  // ---------------
  Int_t nrows = 28;
  for (Int_t irow = 0; irow < nrows; irow++) {

  Runinfo &rinfo = *Runinfo::GetInstancePtr();
  //float d0 = 0.;

 // gSystem->Load("libPhysics");
 // gSystem->Load("libGraf3d");
 // gSystem->Load("libJREvent"); //for yokarowmon

  TFile *fp;
    dlen = rinfo.GetDlength(run);
    ddlen = 0;
    
    stringstream fname;
    fname << "../Data/r" << run << ".root" << ends;

    fp = new TFile(fname.str().data());
  TNtupleD *pulse = static_cast<TNtupleD*>(fp->Get("MyRootFileProcessor/clrs"));
 
  //   JREvent *event = 0;
  //JREvent *event = static_cast<JREvent *>(gROOT->FindObject("Event"));
  //JREvent *event = JREvent::GetInstance();
  //T->SetBranchAddress("Event", &event);
#if 0
  const Int_t nentries = (const Int_t)pulse->GetEntries();
  cerr << "nentries = " << nentries << endl;
  if (nentries>maxevent) {
    cerr << "Set higher maxevent because now we're going to process more than that!" << endl;
    cerr << "Quit." << endl;
    return;
  }
#endif
  Int_t layer = (module < 2 ? 0 : module < 5 ? 28 : 56) + irow;

  stringstream histname;
	histname << "hdt" << irow ;

stringstream item;
	item << "dt>>" << histname.str().data()  <<"(500,0,10000)";

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
#endif
  const Double_t kTanlMinCut = -9999.;
  const Double_t kTanlMaxCut = +9999.;
  const Int_t knTrksCut = 1;
  //const Double_t kfi0locMinCut = -9999;
  //const Double_t kfi0locMaxCut = +9999;
   
  // Double_t kfi0locMinCut = rinfo.GetPhi0locMinCut(run);
  // Double_t kfi0locMaxCut = rinfo.GetPhi0locMaxCut(run);

  Double_t kCpaMinCut = 0;
  Double_t kCpaMaxCut = 0;
  Double_t kIncidentphiMinCut = 0;
  Double_t kIncidentphiMaxCut = 0;
  
  if( run>=20041 && run<=20055 ){
   kCpaMinCut = -4;
   kCpaMaxCut =  3;
   kIncidentphiMinCut = -0.026;
   kIncidentphiMaxCut = 0.018;
   }
   else if( run>=19667 && run<=19678 ){
   kCpaMinCut = -1.28;
   kCpaMaxCut =  0.80;
   kIncidentphiMinCut = -0.046;
   kIncidentphiMaxCut = -0.011;
   }
   else if( run>=19972 && run<=19985 ){
   kCpaMinCut = -4;//Fixed
   kCpaMaxCut =  5;//Fixed
   //kIncidentphiMinCut = -0.0225;//fixed 3 sigma
   //kIncidentphiMaxCut = 0.0225;
   }
   else if( run>=19881 && run<=19895 ){
   kCpaMinCut = -5;
   kCpaMaxCut =  5;
   kIncidentphiMinCut = 0.222;
   kIncidentphiMaxCut = 0.378;
   }
  else{
   kCpaMinCut = -10;
   kCpaMaxCut =  10;
   kIncidentphiMinCut = -10;
   kIncidentphiMaxCut = 10;
  }

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
stringstream cut;
  cut << "module==" << module << "&&"
      << "row==" << irow << "&&"
      << "nTrks<=" << knTrksCut << "&&"
     // << "nhits==1" << "&&"
     // << item.str().data() << ">" << kResXmin << "&&"
     // << item.str().data() << "<" << kResXmax << "&&"
      << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut   << "&&"
    //  << "fi0>"  << kPhi0MinCut << "&&"
    //  << "fi0<"  << kPhi0MaxCut << "&&"
    //    << "cpa>"  << kCpaMinCut << "&&"
    //    << "cpa<"  << kCpaMaxCut << "&&"
    //  << "incidentphi>"  << kIncidentphiMinCut  << "&&"
    //  << "incidentphi<"  << kIncidentphiMaxCut  << "&&"
      << "tnl>"  << kTanlMinCut << "&&"
      << "tnl<"  << kTanlMaxCut ;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;
 
  //      cut << "module == " << module << "&&row==" << row << ends;

stringstream canvasname;
	canvasname << "c" << histname.str().data();
    TCanvas* c = new TCanvas(canvasname.str().data(),"");
    c->cd();
    pulse->Draw(item.str().data(), cut.str().data());
  TH1F *htmp = static_cast<TH1F*> (gROOT->FindObject(histname.str().data()));
 // htmp->Rebin(1);
// htmp->Fit("gaus","","",-0.3,0.3);
    int maximumbin = htmp->GetBinCenter(htmp->GetMaximumBin());
  //cerr << htmp->GetRMS() << endl;
    TF1 fitfun("fitfun","[0]+[1]*TMath::Exp(-(x-[2])*(x-[2])/(2*[3]*[3]))",0,10000);
    fitfun.SetParameter(2,maximumbin);
    fitfun.SetParameter(3,100);
    //htmp.Fit("fitfun","r");
    htmp->Fit("fitfun","r");
    double dt=fitfun.GetParameter(2);
    double ddt=fitfun.GetParameter(3);
    htmp->Fit("fitfun","r","",dt-3*ddt,dt+3*ddt);
    time = fitfun.GetParameter(2);
    dtime = fitfun.GetParError(2);
    cerr << "time[" << irow << "] = " << time << endl;

    double y = 446.3+133.1*dlen;
    double deltadt = y - time; //ns
    double deltal = 75.1 * deltadt / 1000; //um to mm


  outf << run << " " << irow << " " << deltadt << " " << deltal <<endl;

/*
    pulse->Draw("time>>htemp");
  TH1 *htemp = static_cast<TH1 *>(gROOT->FindObject("htemp"));
    int maximumbin = htemp->GetBinCenter(htemp->GetMaximumBin());
  //cerr << htemp->GetMean() << endl;
  //cerr << htemp->GetRMS() << endl;
    //TF1 fitfun("fitfun","[0]+[1]*TMath::Exp(-(x-[2])*(x-[2])/(2*[3]*[3]))",0,10000);
    //fitfun.SetParameter(2,maximumbin);
    //fitfun.SetParameter(3,100);
   // htemp->Fit("fitfun","r");
    
    stringstream cname;
    cname << "c" << rnum[i] << ends;
    TCanvas* c = new TCanvas(cname.str().data(),"");
    c->cd();

    Int_t maxpeaks = 1;
    TSpectrum *s = new TSpectrum(maxpeaks);
    s->Search(htemp, 1, "new");
    Double_t *xpeaks = s->GetPositionX();
    cerr << xpeaks[0] << endl;
    TF1 fitfun("fitfun","[0]+[1]*TMath::Exp(-(x-[2])*(x-[2])/(2*[3]*[3]))",xpeaks[0]-1000,xpeaks[0]+1000);
    fitfun.SetParameter(2,maximumbin);
    fitfun.SetParameter(3,100);
    htemp->Fit("fitfun","r");



    htemp->Draw("ap");

    time[i] = fitfun.GetParameter(2);
    cerr << "time[" << i << "] = " << time[i] << endl;
 */



/*
  TGraphErrors* gr = new TGraphErrors(kNp,dlen,time,ddlen,dtime);
  gr->SetMarkerStyle(22);
  gr->SetMarkerSize(1);
    TCanvas* c2 = new TCanvas("driftvelocity","");
    c2->cd();
    gr->Fit("pol1","");
  gr->Draw("ap e1");
  */
}

}

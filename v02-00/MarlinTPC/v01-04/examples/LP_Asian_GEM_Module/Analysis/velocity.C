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

void velocity()
{
  const int npoints = 12;
  //const int npoints = 10;
  //int rnum[npoints];
  float dlen[npoints];
  float time[npoints];
  float ddlen[npoints];
  float dtime[npoints];

  const Int_t kNp = 12; //11;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
  //Int_t kRun[] = { 19743, 19742, 19741, 19740, 19739, 19738,19737, 19736, 19735, 19734, 19733, 19732}; // B=1T,with gate phi=10
 //Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // 14   B=1T
 //Int_t kRun[] = { 20041, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054, 20055}; // B=1T
  //Int_t kRun[] = { 19868, 19867, 19866, 19865, 19869, 19864, 19870, 19863, 19862, 19861, 19871, 19860, 19872, 19873, 19874}; // B=1T,with gate(not recomend)
   Int_t kRun[] = { 19717, 19718, 19719, 19720, 19721, 19722, 19723, 19724, 19725, 19726, 19727, 19728};//non reco2

  Runinfo &rinfo = *Runinfo::GetInstancePtr();
  //float d0 = 0.;
#if 0
  rnum[0] = 19985;
  rnum[1] = 19984;
  rnum[2] = 19983;
  rnum[3] = 19982;
  rnum[4] = 19981;
  rnum[5] = 19980;
  rnum[6] = 19979;
  rnum[7] = 19978;
  rnum[8] = 19977;
  rnum[9] = 19976;
  rnum[10] = 19975;
  rnum[11] = 19974;
  rnum[12] = 19973;
  rnum[13] = 19972;
  
  dlen[0] = 275 + d0;
  dlen[1] = 325 + d0;
  dlen[2] = 375 + d0;
  dlen[3] = 425 + d0;
  dlen[4] = 475 + d0;
  dlen[5] = 525 + d0;
  dlen[6] = 225 + d0;
  dlen[7] = 205 + d0;
  dlen[8] = 185 + d0;
  dlen[9] = 725 + d0;
  dlen[10] = 675 + d0;
  dlen[11] = 625 + d0;
  dlen[12] = 525 + d0;
  dlen[13] = 575 + d0;
#else
#if 0
  dlen[13] = 0 + d0;
  dlen[12] = 26 + d0;
  dlen[11] = 46 + d0;
  dlen[10] = 96 + d0;
  dlen[9]  = 146 + d0;
  dlen[8]  = 196 + d0;
  dlen[7]  = 250 + d0;
  dlen[6]  = 300 + d0;
  dlen[5]  = 350 + d0;
  dlen[4]  = 400 + d0;
  dlen[3]  = 450 + d0;
  dlen[2]  = 500 + d0;
  dlen[1]  = 550 + d0;
  dlen[0]  = 568 + d0;
  rnum[0] = 19666; 
  rnum[1] = 19667;
  rnum[2] = 19668;
  rnum[3] = 19669;
  rnum[4] = 19670;
  rnum[5] = 19671;
  rnum[6] = 19672;
  rnum[7] = 19673;
  rnum[8] = 19674;
  rnum[9] = 19675;
  rnum[10] = 19676;
  rnum[11] = 19677;
  rnum[12] = 19678;
  rnum[13] = 19680;
#endif
#endif

  gSystem->Load("libPhysics");
  gSystem->Load("libGraf3d");
 // gSystem->Load("libJREvent"); //for yokarowmon

  TFile *fp;
  for (int i = 0; i < kNp; i++) {
    int run = kRun[i];
    dlen[i] = rinfo.GetDlength(run);
    ddlen[i] = 0;
    
    stringstream fname;
    //fname << "root/quality.Run" << rnum[i] << ".root" << ends;
    fname << "../Data_module3/r" << run << ".root" << ends;

    fp = new TFile(fname.str().data());
   // TNtupleD *pulse  = static_cast<TNtupleD *>(fp->Get("clrs"));
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

  stringstream histname;
	histname << "hdt" << i ;

stringstream item;
	item << "dt>>" << histname.str().data()  <<"(500,0,10000)";

stringstream canvasname;
	canvasname << "c" << histname.str().data();
    TCanvas* c = new TCanvas(canvasname.str().data(),"");
    c->cd();
    pulse->Draw(item.str().data());
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
    time[i] = fitfun.GetParameter(2);
    dtime[i] = fitfun.GetParError(2);
    cerr << "time[" << i << "] = " << time[i] << endl;



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

  }


  TGraphErrors* gr = new TGraphErrors(npoints,dlen,time,ddlen,dtime);
  gr->SetMarkerStyle(22);
  gr->SetMarkerSize(1);
    TCanvas* c2 = new TCanvas("driftvelocity","");
    c2->cd();
    gr->Fit("pol1","");
  gr->Draw("ap e1");
#if 0
  JCluster::SetSortBy(JCluster::kDet); // set sorting policy

  const int ndata = 100;
  double data_val[ndata];
  TNtupleD* tup = new TNtupleD("pulse","","evt:module:row:pad:charge:length:height:time:x:y");
  TH2D* hTrackPos = new TH2D("hTrackPos","Charge-weighted hit position;x;y",2600,-260,260,100,-268,268);
  TGraph* gChargeSumByEvent;
  int chargeSum[maxevent];
  int eventAt[maxevent];

  for (Int_t ev=0; ev<nentries; ev++) {
      T->GetEntry(ev);
 // cerr << "test : " << event->GetEntries() << endl;
      if (!(ev%100)) {
        cerr << "ev: " << ev << " evno: " << event->GetEventNumber() << " #cls: " << event->GetEntries() << endl;
      }
      event->Sort(); // sort clusters in ascending order of layers

      int npads     = 0;
      int chargeSumByEvent = 0;
      int layer     = 0;
      JCluster *cp;
      TIter next(event);
      while ((cp = static_cast<JCluster *>(next()))) {
	//cp->DebugPrint("detail");
#if 1 
        int phmax = cp->GetMaximum();
	int width = cp->GetLength();
        if (phmax < kThreshold || width < kMinWidth || width > kMaxWidth) continue;
#endif
        int module = cp->GetModuleID();
        int llayer = cp->GetLayerID();
        switch (module) {
          case 0:
            layer = llayer;
	    break;
          case 1:
            layer = llayer;
	    break;
          case 2:
            layer = llayer + 28;
	    break;
          case 3:
            layer = llayer + 28;
	    break;
          case 4:
            layer = llayer + 28;
	    break;
          case 5:
            layer = llayer + 56;
	    break;
          case 6:
            layer = llayer + 56;
	    break;
	  default:
	    continue;
	}
	double x = cp->GetPosition().X(); 
	double y = cp->GetPosition().Y(); 
	double charge = double(cp->GetCharge());
	chargeSumByEvent += charge;

	//data_val[0] = double(event->GetEventNumber()); // = ev + 1 and this is inconvenient for loops.
	data_val[0] = double(ev);
	data_val[1] = double(module);
	data_val[2] = double(layer);
	data_val[3] = double(cp->GetPadID());
	data_val[4] = charge;
	data_val[5] = double(width);
	data_val[6] = double(phmax);
	data_val[7] = cp->GetTime() * kTimeBin; // ns
	data_val[8] = x; 
	data_val[9] = y; 
        tup->Fill(data_val);

	hTrackPos->Fill(x,y,charge);
      }	
      
      chargeSum[ev] = chargeSumByEvent;
      eventAt[ev] = event->GetEventNumber(); 
  }
  gChargeSumByEvent = new TGraph(nentries,eventAt,chargeSum);
  gChargeSumByEvent->SetTitle("Total charges by event;event;ADC Sum");
  TCanvas* c_trk_pos = new TCanvas("c_trk_pos","");
  hTrackPos->Draw("COLZ");

  TCanvas* c_chargeSumByEvent = new TCanvas("c_chargeSumByEvent","");
  gChargeSumByEvent->Draw("al");

  TCanvas* c = new TCanvas("c","");
  //tup->Draw("charge","module==3&&row==1");
  double _charge;
  double _evt;
  double _row;
  tup->SetBranchAddress("charge",&_charge);
  tup->SetBranchAddress("evt"   ,&_evt);
  tup->SetBranchAddress("row"   ,&_row);
  Int_t ntupentries = tup->GetEntries();
  double hitcharges[maxevent][kNlayers];
  for (int i=0; i < maxevent; i++ ) {
    for (int j=0; j < kNlayers; j++ ) {
       hitcharges[i][j] = 0.;
    }
  }
  for (int i = 0; i < ntupentries; i++) {
    tup->GetEntry(i);
    hitcharges[int(_evt)][int(_row)] += _charge;
  }
  TH1D* hhitcharges[kNlayers];
  TCanvas* chitcharges[kNlayers];
  for (int i = 0; i < kNlayers; i++) {
    stringstream hname;
    hname << "hitcharge_row" << i << ends;
    hhitcharges[i] = new TH1D(hname.str().data(),"",100,0,10000);
    chitcharges[i] = new TCanvas(hname.str().data(),"");
    
    for (int j =0; j < maxevent; j++) {
      if (hitcharges[j][i]>0) hhitcharges[i]->Fill(hitcharges[j][i]); 
      //if (hitcharges[j][i]>100) hhitcharges[i]->Fill(hitcharges[j][i]); 
    }
    
    //hhitcharges[i]->Draw();
    hhitcharges[i]->Fit("landau");
  }

  stringstream outpdf, outpdf_open, outpdf_close;
  outpdf << "Run" << runid << ".pdf" << ends;
  outpdf_open  << "Run" << runid << ".pdf(" << ends;
  outpdf_close << "Run" << runid << ".pdf)" << ends;
  c_trk_pos->Print(outpdf_open.str().data(),"pdf");
  for (int i = 0; i < kNlayers; i++ ) {
    chitcharges[i]->Print(outpdf.str().data(),"pdf");
  }
  c_chargeSumByEvent->Print(outpdf_close.str().data(),"pdf");
#endif
}

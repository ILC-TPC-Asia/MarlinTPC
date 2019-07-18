//Script for comparison of row difference

void kappa()
{
  const Int_t kNp = 14; //12;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
   // Int_t kRun[] = { 20041, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054, 20055}; //Set16 FS kNp =14
  //Int_t kRun[] = {19678, 19677, 19676, 19675, 19674, 19673, 19672, 19671, 19670, 19669, 19668, 19667};//for calibration
 Int_t kRun[] = { 19985, 19984, 19983, 19982, 19981, 19980, 19979, 19978, 19977, 19976, 19975, 19974, 19973, 19972}; // 14   B=1T
 

  //TCanvas *c0 = new TCanvas("c0");
    ofstream outf("result/cpa_19985_withcut.dat");
  
  // ---------------
  // Loop over runs
  // ---------------
  Int_t n = 0;
  for (Int_t i=0; i<kNp; i++) {
    Int_t run = kRun[i];

    cerr << "********Run=" << run << endl;

    stringstream runn;
    runn << "../Data_new/r" << run << ".root" << ends;
   TFile *_file0 = TFile::Open(runn.str().data());
 //  TFile *_file1 = TFile::Open("../Data/r19971.root");
   
   TCanvas *c1 = new TCanvas("c1");
   
   _file0->cd();
   TDirectoryFile *myrootfp0 = static_cast<TDirectoryFile *>(gROOT->FindObject("MyRootFileProcessor"));
   myrootfp0->cd();
   TNtupleD *hits0 = static_cast<TNtupleD *>(gROOT->FindObject("hits"));
   //hits0->Draw("cpa>>h0(200,-10.,10.)","nTrks==1 && ndf>40","goff");
   hits0->Draw("cpa>>h0(150,-2.,2.)","nTrks==1 && ndf>40","goff");
   //stringstream hist;
   //hist << "h_" << run << ends;
   TH1D *h0 = static_cast<TH1D *>(gROOT->FindObject("h0"));
   //h0->Scale(1./h0->Integral());
   //h0->Draw("EHIST same");

 h0->Fit("gaus","","",-0.5,0.3);
  Double_t mu  = h0->GetFunction("gaus")->GetParameter(1);
  Double_t sg  = h0->GetFunction("gaus")->GetParameter(2);
  h0->Fit("gaus","RI","",mu-sg ,mu+2*sg);
   
  outf << run << " " << mu << " " << sg  <<" " << mu-sg*4 << " " << mu+sg*4 <<endl;
#if 1
  // save plot as pdf  file
 // Runinfo &rinfo = *Runinfo::GetInstancePtr();
  stringstream ofile;
  ofile << "Result/cpa_withcut_run" << run << ".pdf"<< ends; 
  c1->Print(ofile.str().data());
 #endif
  /* _file1->cd();
   TDirectoryFile *myrootfp1 = static_cast<TDirectoryFile *>(gROOT->FindObject("MyRootFileProcessor"));
   myrootfp1->cd();
   TNtupleD *clrs1 = static_cast<TNtupleD *>(gROOT->FindObject("clrs"));
   clrs1->Draw("dt>>h1(100,7000.,9000)","module==3 && row==3 && (ndf+5)/2<50","goff");
   TH1D *h1 = static_cast<TH1D *>(gROOT->FindObject("h1"));
   h1->Scale(1./h1->Integral());
   h1->Draw("EHIST same");
*/
  }  
   }

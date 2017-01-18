#include "Plot.C"

void Runinfo()
{
	map<int, string> runlist;
	//Recomended Data Set (Gate GEM) 
	  runlist[19987] = "7. Nov. 2016 23:42:00.0 0";
	  runlist[19985] = "7. Nov. 2016 23:25:00.0 0";
	  runlist[19984] = "7. Nov. 2016 23:16:00.0 0";
	  runlist[19983] = "7. Nov. 2016 23:08:00.0 0";
	  runlist[19982] = "7. Nov. 2016 23:00:00.0 0";
	  runlist[19981] = "7. Nov. 2016 22:52:00.0 0";
	  runlist[19980] = "7. Nov. 2016 22:44:00.0 0";
	  runlist[19979] = "7. Nov. 2016 22:35:00.0 0";
	  runlist[19978] = "7. Nov. 2016 22:27:00.0 0";
	  runlist[19977] = "7. Nov. 2016 22:19:00.0 0";
	  runlist[19976] = "7. Nov. 2016 22:10:00.0 0";
	  runlist[19975] = "7. Nov. 2016 22:02:00.0 0";
	  runlist[19974] = "7. Nov. 2016 21:53:00.0 0";
	  runlist[19973] = "7. Nov. 2016 21:45:00.0 0";
	  runlist[19972] = "7. Nov. 2016 21:37:00.0 0";

	//Recomended Data Set (Field Shaper) 
          runlist[20042] = "10. Nov. 2016 13:53:00.0 0";
          runlist[20041] = "10. Nov. 2016 13:46:00.0 0";
          runlist[20043] = "10. Nov. 2016 14:00:00.0 0";
          runlist[20044] = "10. Nov. 2016 14:07:00.0 0";
          runlist[20045] = "10. Nov. 2016 14:14:00.0 0";
          runlist[20046] = "10. Nov. 2016 14:21:00.0 0";
          runlist[20047] = "10. Nov. 2016 14:28:00.0 0";
          runlist[20048] = "10. Nov. 2016 14:35:00.0 0";
          runlist[20049] = "10. Nov. 2016 14:43:00.0 0";
          runlist[20050] = "10. Nov. 2016 14:51:00.0 0";
          runlist[20051] = "10. Nov. 2016 15:01:00.0 0";
          runlist[20052] = "10. Nov. 2016 15:08:00.0 0";
          runlist[20053] = "10. Nov. 2016 15:16:00.0 0";
          runlist[20054] = "10. Nov. 2016 15:24:00.0 0";
          runlist[20055] = "10. Nov. 2016 15:32:00.0 0";

	int runno[] = { 19987, 19985, 19984, 19983, 19982,
	       		19981, 19980, 19979, 19978, 19977,
		       	19976, 19975, 19974, 19973, 19972,
	       		20042, 20041, 20043, 20044, 20045,
		       	20046, 20047, 20048, 20049, 20050,
		       	20051, 20052, 20053, 20054, 20055};

	//Environmental Data
	TGraph &grt   = *Plot();
	TGraph &grap  = *Plot("../TxtData/ILC.EXP_LPTPC.PRESSURE_PRESSURE1(AtmosphericPressure)_PRESS.20161026-20161112.txt");
	TGraph &grsp  = *Plot("../TxtData/ILC.EXP_LPTPC.PRESSURE_PRESSURE2(SystemPressureCalib)_PRESS.20161026-20161112.txt");
	TGraph &grgbp = *Plot("../TxtData/ILC.EXP_LPTPC.PRESSURE_PRESSURE4(GasBottlePressure)_PRESS.20161026-20161112.txt");
	TGraph &grdp  = *Plot("../TxtData/ILC.EXP_LPTPC.DEWPOINT_DEWPOINT1_H2OPPM.20161026-20161112.txt");
	TGraph &gro2  = *Plot("../TxtData/ILC.EXP_LPTPC.OXYOA1500E_OXYGEN1_OXYPPM.20161026-20161112.txt");

	DateTimeUtil &dtu = *DateTimeUtil::GetInstancePtr();

	int nRuns = 30; //Number of Runs

	//Output Data File
	ofstream fout("outDATA.dat");
	cerr << " Run   day    T['C]  T[K]   AP[hPa]  P/T    SPR[hPa]  H2O[ppm]  O2[ppm] "<< endl; 
	fout << " Run   day    T['C]  T[K]   AP[hPa]  P/T    SPR[hPa]  H2O[ppm]  O2[ppm] "<< endl; 

	//Loop over Runs
	for (int iRun = 0; iRun < nRuns; iRun++) {
		vector<double> dt = dtu.DateTime(runlist[runno[iRun]]);
		dtu.Print(dt,false);

		double day = dt[6]/24./3600.;       //Day
		double val1 = grt.Eval(day);	    //Temperature['C]
		double val2 = grt.Eval(day) + 273;  //Temperature[K]
		double val3 = grap.Eval(day);	    //Atomospheric Pressure[hPa]
		double val3x = val3/val2;	    //Atomospheric Pressure[hPa]/Temperature[K]
		double val4 = grsp.Eval(day);	    //SystemPressureCalib[hPa]
		double val6 = grdp.Eval(day);	    //H20[ppm]
		double val7 = gro2.Eval(day);	    //O2[ppm]

		cerr << setw(6) << runno[iRun]
		     << setw(7) << setprecision(5) << showpoint << day
		     << setw(7) << setprecision(5) << showpoint << val1
		     << setw(7) << setprecision(5) << showpoint << val2
		     << setw(9) << setprecision(7) << showpoint << val3
		     << setw(6) << setprecision(4) << showpoint << val3x
		     << setw(10) << setprecision(7) << showpoint << val4
		     << setw(10) << setprecision(7) << showpoint << val6
		     << setw(9) << setprecision(6) << showpoint << val7 << endl;

		fout << setw(6) << runno[iRun]
		     << setw(7) << setprecision(5) << showpoint << day
		     << setw(7) << setprecision(5) << showpoint << val1
		     << setw(7) << setprecision(5) << showpoint << val2
		     << setw(9) << setprecision(7) << showpoint << val3
		     << setw(6) << setprecision(4) << showpoint << val3x
		     << setw(10) << setprecision(7) << showpoint << val4
		     << setw(10) << setprecision(7) << showpoint << val6
		     << setw(9) << setprecision(6) << showpoint << val7 << endl;

	}
}
		 

#include <map>
#include <algorithm>
#include "DateTime.C"

TGraph *Plot(const char *fname = "../TxtData/ILC.EXP_LPTPC.TEMPERATURE_TEMPERATURE1_TEMPC.20161026-20161112.txt")
{
	ifstream fin(fname);

  	if (fin.fail()) {
    		cerr << "Opening the input file failed..." << endl;
    		exit(1);
  	}
	
	class EnvData {
	  public:
	    EnvData(int yy, int mm, int dd, int h, int m, double s, double t)
		: _year(yy), _month(mm), _day(dd), _hour(h), _min(m), _sec(s), _value(t)
 	   {  
	     tm ctim;
	     ctim.tm_year = yy - 1900;
	     ctim.tm_mon  = mm - 1;
	     ctim.tm_mday = dd;
	     ctim.tm_hour = h;
	     ctim.tm_min  = m;
	     ctim.tm_sec  = s;
	     ctim.tm_isdst = 0;
	     _time = mktime(&ctim);
	    
	     tm ctim0;
	     ctim0.tm_year = 2016 - 1900;
	     ctim0.tm_mon  = 10 - 1;
	     ctim0.tm_mday = 26;
	     ctim0.tm_hour = 0;
	     ctim0.tm_min  = 0;
	     ctim0.tm_sec  = 0;
	     ctim0.tm_isdst = 0;
	     time_t tim0 = mktime(&ctim0);
	     _time -= tim0;
	   }	  

	  int    GetYear () const { return _year;  }
	  int    GetMonth() const { return _month; }
	  int    GetDay  () const { return _day;   }
	  int    GetHour () const { return _hour;  }
	  int    GetMin  () const { return _min;   }
	  int    GetSec  () const { return _sec;   }
	  double GetTime () const { return _time;  }
	  double GetValue() const { return _value; }

	  void Print()
	  {
	    double dayt = GetTime()/24./3600.;

	    cerr << " " << _year << "/" << _month << "/" << _day
		 << " " << _hour << ":" << _min   << ":" << _sec
		 << " sec= " << _time << " day= " << dayt << " " << _value << endl;
	  }

	  private:
	    int    _year;
	    int    _month;
	    int    _day;
	    int    _hour;
	    int    _min;
	    double _sec;
	    time_t _time;
	    double _value;
	 };		
	
	vector<EnvData> edataarray;

	const int kMaxLen = 1024;
	char temp[kMaxLen];

	while (fin.getline(temp,kMaxLen)) {
	  if (temp[0] == '#') continue;
	  cerr << temp << endl;
	 
	  string sline(temp);
	  string::size_type pos1 = sline.find('"');
	  string::size_type pos2 = sline.find('"', pos1+1);
	  string sdatetime = sline.substr(pos1+1, pos2-pos1-1);         // date&time
	  string svalue    = sline.substr(pos2+1, sline.size()-pos2-1); // value

	  DateTimeUtil &dtu = *DateTimeUtil::GetInstancePtr();

	  vector<double> datetime = dtu.DateTime(sdatetime);
	  int    yy    = datetime[0];
	  int    mm    = datetime[1];
	  int    dd    = datetime[2];
	  int    h     = datetime[3];
	  int    m     = datetime[4];
	  double s     = datetime[5];
	  int    isdst = datetime[7];

	  stringstream ssvalue(svalue);
	  double t;
	  string dummy;
	  if (isdst) ssvalue >> dummy; //In the case of O2 Data
	  ssvalue >> t;

	  EnvData edat(yy, mm, dd, h, m, s, t);		  
	  edataarray.push_back(edat);

	  edat.Print(); 
	}

	//Sort data chronologically
	sort(edataarray.begin(),edataarray.end(),
			[](EnvData &e1, EnvData &e2){return e1.GetTime()<e2.GetTime();});

	size_t npts = edataarray.size();
	double *xdata = new double[npts];
	double *ydata = new double[npts];

	size_t ipt = 0;

	auto itr = edataarray.begin();
	for (auto itr = edataarray.begin(); itr != edataarray.end(); ++itr) {
		xdata[ipt] = itr->GetTime()/24./3600.;
		ydata[ipt] = itr->GetValue();
		ipt++;
	}

	TGraph *grp = new TGraph(npts,xdata,ydata);
	return grp;
}

#include <map>

class DateTimeUtil {
  private:
	 DateTimeUtil() {}
  
  public:
	static DateTimeUtil *GetInstancePtr()
	{
		if (!_theInstance) {
		  _theInstance = new DateTimeUtil;
 		}
     		return _theInstance;
	}		

	vector<double> DateTime(const string &sdatetime)
	{
	  stringstream ssdatetime(sdatetime);
	  string st2, st4, st5;
 	  double    dd, yy;
 	  ssdatetime >> dd >> st2 >> yy >> st4 >> st5;
	  int isdst = 0;
	  if (!st5.size()) isdst = 9999;
		
	  static bool firstcall = true;

	  static map<string, int> months;
	  if (firstcall) {
	    firstcall = false;
	    months["Jan."] = 1;
	    months["Feb."] = 2;
	    months["Mar."] = 3;
	    months["Apr."] = 4;
	    months["May."] = 5;
	    months["Jun."] = 6;
	    months["Jul."] = 7;
	    months["Aug."] = 8;
	    months["Sep."] = 9;
	    months["Oct."] = 10;
	    months["Nov."] = 11;
	    months["Dec."] = 12;
	  }
  
	  double mm = months[st2];
	 
	  string::size_type pos1 = st4.find(':');
	  string::size_type pos2 = st4.find(':', pos1+1);
	  string sh = st4.substr(0, pos1);                   // hour
	  string sm = st4.substr(pos1+1, pos2-pos1-1);       // min
	  string ss = st4.substr(pos2+1, st4.size()-pos2-1); // sec

	  double h, m, s;

	  stringstream temps(sh);
	  temps >> h;
	 	 
	  temps.clear(); 
	  temps.str(sm);
	  temps >> m;
	
	  temps.clear(); 
	  temps.str(ss);
	  temps >> s;

	  tm ctim;
	  ctim.tm_year = yy - 1900;
	  ctim.tm_mon  = mm - 1;
	  ctim.tm_mday = dd;
	  ctim.tm_hour = h;
	  ctim.tm_min  = m;
	  ctim.tm_sec  = s;
	  ctim.tm_isdst = 0;
	  double time = mktime(&ctim);

	  tm ctim0;
	  ctim0.tm_year = 2016 - 1900;
	  ctim0.tm_mon  = 10 - 1;
	  ctim0.tm_mday = 26;
	  ctim0.tm_hour = 0;
	  ctim0.tm_min  = 0;
	  ctim0.tm_sec  = 0;
	  ctim0.tm_isdst = 0;
	  double time0 = mktime(&ctim0);
	  time -= time0;

	  vector<double> ans;
	  ans.push_back(yy);
	  ans.push_back(mm);
	  ans.push_back(dd);
	  ans.push_back(h);
	  ans.push_back(m);
	  ans.push_back(s);
	  ans.push_back(time);
	  ans.push_back(isdst);

	  return ans;
	}

	void Print(vector<double> dt, bool withendl = true)
	{
	  if (dt.size() == 7) {
	    cerr << dt[0] << "/" << dt[1] << "/" << dt[2] << " " 
		 << dt[3] << ":" << dt[4] << ":" << dt[5] << " "
		 << dt[6] << "[sec] " << dt[6]/24./3600. << "[day] ";
	    if (withendl) cerr << endl;
	  }
  	}

private:
  static DateTimeUtil *_theInstance;
};

DateTimeUtil *DateTimeUtil::_theInstance = 0;

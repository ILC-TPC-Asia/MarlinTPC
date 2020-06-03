#include "Runinfo.h"

void testRuninfo(int run = 19906)
{
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  for (Int_t irow = 0; irow < 28; irow++) {
      double x = rinfo.GetDlength(run, irow);
      cerr << "Row = " << irow << ", Dlen = " << x << endl;
  }


}

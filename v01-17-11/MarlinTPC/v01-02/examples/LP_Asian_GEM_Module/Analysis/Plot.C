//TGraph *Plot(const char* filename = "p5.dat")
TGraph *Plot(const char* filename = "t2k.dat", int ncol = 3)
{
    int n=0;
    double xdat[10000], ydat[10000], dummy;
    ifstream in(filename);
    if (ncol == 3) while ((in >> dummy >> xdat[n] >> ydat[n])) n++;
    else           while ((in >> xdat[n] >> ydat[n])) n++;
    TGraph *gr = new TGraph(n,xdat,ydat);
    return gr;
}

//---------------------------------------------------------------------------

#ifndef FilteringUnitH
#define FilteringUnitH

#include <Series.hpp>
#include <math.h>
#include <vector>
#include <algorithm>

double Tr_Filter(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh);
long double TrForMassiveFilter(long double *inB,long double *inY,long double* outB,long double *outY,
int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh);

long double TrForMassiveFilter(std::vector<long double> & inB,
std::vector<long double> & inY,std::vector<long double> & outB,
std::vector<long double> & outY,
int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh);

long double medianFilter(std::vector <long double> & in);

//---------------------------------------------------------------------------
#endif

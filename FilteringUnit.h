//---------------------------------------------------------------------------

#ifndef FilteringUnitH
#define FilteringUnitH

#include <VCLTee.Series.hpp>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include "UsedTypes.h"
#include "commonFunctions.h"
#pragma hdrstop



//#include <boost\math\tools\remez.hpp>

MyDataType TrForMassiveFilter(TSignal & inB,
TSignal & inY,TSignal & outB,
TSignal & outY,
int lengthFilter,MyDataType Fdisk,MyDataType Fpropysk,MyDataType Fzatyh);

double Filter (const std::vector<long double> &in,
std::vector<long double> & out, int length,
double Fdisk, double Fpropysk,double Fzatyh);

long double TrForMassiveFilter(std::vector<long double> & inB,
std::vector<long double> & inY,std::vector<long double> & outB,
std::vector<long double> & outY,int lengthFilter,long double Fdisk,
long double Fpropysk,long double Fzatyh);

double Tr_Filter(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh);


//---------------------------------------------------------------------------
#endif

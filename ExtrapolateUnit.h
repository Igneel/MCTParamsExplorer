//---------------------------------------------------------------------------

#ifndef ExtrapolateUnitH
#define ExtrapolateUnitH

#include <math.h>
#include <vector>
#include <Chart.hpp>
#include <Series.hpp>
int extrapolate5Degree(TLineSeries * Series, long double minX, long double maxX, long double hX,TLineSeries *out,const int powPolinom);
int extrapolate2Degree(TLineSeries * Series, long double minX, long double maxX, long double hX,TLineSeries *out);

int curveFittingUniversal(long double * inX, long double *inY, const int length,long double *outKoef,const int powPolinom);
int curveFittingUniversal(std::vector<long double> * inX, std::vector<long double> *inY,
  std::vector<long double> *outKoef,const int powPolinom);


long double determinant (std::vector<std::vector<long double> > & Arr,int size);
long double determinant (long double ** Arr,int size);

//---------------------------------------------------------------------------
#endif

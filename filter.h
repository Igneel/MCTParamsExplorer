#ifndef FILTER_H
#define FILTER_H

#include "filter_global.h"

FILTERSHARED_EXPORT long double  TrForMassiveFilter(std::vector<long double> & inB,
std::vector<long double> & inY,std::vector<long double> & outB,
std::vector<long double> & outY,
int lengthFilter,long double Fdisk,long double Fpropysk,long double Fzatyh);

FILTERSHARED_EXPORT double TrForMassiveFilter(long double *inB,long double *inY,long double* outB,long double *outY,
int lengthMassive,int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh);

FILTERSHARED_EXPORT double OptimFilter(std::vector<double> in, std::vector<double> out,
        long double deltaP, long double deltaS, long double FPropusk, long double Fpodavl, long double FDiskr);


template <class T>
FILTERSHARED_EXPORT T medianFilter(std::vector <T> & in);

template <class T>
FILTERSHARED_EXPORT inline T max_elem(const std::vector<T> & in);

template <class T>
FILTERSHARED_EXPORT inline T min_elem(const std::vector<T> & in);

#endif // FILTER_H

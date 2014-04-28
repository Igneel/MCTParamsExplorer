//---------------------------------------------------------------------------

#ifndef FilteringUnitH
#define FilteringUnitH

#include <Series.hpp>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include "UsedTypes.h"
#pragma hdrstop
#pragma package(smart_init)

class FilterLowBand
{
public:
    FilterLowBand(unsigned int length,long double Fdisk, long double Fpropysk, long double Fzatyh);
    ~FilterLowBand();

    void FilterDataWithAutoShift(DataTypeInContainer & inB,
    DataTypeInContainer & inY,DataTypeInContainer & outB,
    DataTypeInContainer & outY);

    double FilterData (const std::vector<long double> &in,
    std::vector<long double> & out);

    void setFilterParams(unsigned int length,long double Fdisk, long double Fpropysk, long double Fzatyh);
    
private:

    void calculateImpulseResponse(unsigned int length,long double Fdisk, long double Fpropysk, long double Fzatyh);

    unsigned int N; //Длина фильтра и ведь она же должна считаться из частот)))
    long double Fd; //Частота дискретизации входных данных
    long double Fs; //Частота конца полосы пропускания 
    long double Fx; //Частота начала полосы затухания

    std::vector<long double> H;  //Импульсная характеристика фильтра
    std::vector<long double> H_id; //Идеальная импульсная характеристика
    std::vector<long double> W;   //Весовая функция
};

MyDataType TrForMassiveFilter(DataTypeInContainer & inB,
DataTypeInContainer & inY,DataTypeInContainer & outB,
DataTypeInContainer & outY,
int lengthFilter,MyDataType Fdisk,MyDataType Fpropysk,MyDataType Fzatyh);

double Filter (const std::vector<long double> &in,
std::vector<long double> & out, int length,
double Fdisk, double Fpropysk,double Fzatyh);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*
Медианный фильтр.
Просто возвращает медиану массива.
*/
template <class T>
T medianFilter(std::vector <T> & in)
{
    std::sort(in.begin(),in.end());
    return in[in.size()/2];
}

template <class T>
inline T max_elem(const std::vector<T> & in)
{
    if(in.size()==0)
    {
        //"Сложно найти максимальный элемент, когда нет ни одного");
        return 0;
    }
    std::vector<T>::const_iterator pos;
    T max=*in.begin();
    for(pos=in.begin();pos!=in.end();++pos)
    {
        if(*pos>max)
        max=*pos;
    }
    return max;
}

template <class T>
inline T min_elem(const std::vector<T> & in)
{
    if(in.size()==0)
    {
        //"Сложно найти максимальный элемент, когда нет ни одного");
        return 0;
    }
    std::vector<T>::const_iterator pos;
    T min=*in.begin();
    for(pos=in.begin();pos!=in.end();++pos)
    {
        if(*pos<min)
        min=*pos;
    }
    return min;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



//---------------------------------------------------------------------------
#endif

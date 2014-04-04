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

MyDataType TrForMassiveFilter(DataTypeInContainer & inB,
DataTypeInContainer & inY,DataTypeInContainer & outB,
DataTypeInContainer & outY,
int lengthFilter,MyDataType Fdisk,MyDataType Fpropysk,MyDataType Fzatyh);

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

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



//---------------------------------------------------------------------------
#endif

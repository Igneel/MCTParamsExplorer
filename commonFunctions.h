#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H 

#include "UsedTypes.h"
#include <math>
inline MyDataType dist(MyDataType x1, MyDataType x2);

bool thiningSignal(DataTypeInContainer & inB, DataTypeInContainer & inDependence, DataTypeInContainer & outB, DataTypeInContainer & outDependence, 
    MyDataType left, MyDataType right, size_t NewLength);

//---------------------------------------------------------------------------
// Округление с заданной точностью.
template <class T>
void roundM(T *pos, T* endPos, unsigned int NumberOfDecimalPlaces )
{
    int S=pow(10,NumberOfDecimalPlaces);
    for(;pos!=endPos;++pos)
    {
        int n=(int)(*pos*S*10)%10;
        if(n<5)
            *pos=floorl(*pos*S)/S;
        else
            *pos=ceill(*pos*S)/S;
    }
}
//-------------------------------------------------------------------------------
template <class T>
T roundM(T x, unsigned int NumberOfDecimalPlaces )
{
    int S=pow(10,NumberOfDecimalPlaces);
        T n=static_cast<int>(x*S*10.0)%10;
        if(n<5.0)
            x=floorl(x*S)/S;
        else
            x=ceill(x*S)/S;
    return x;
}
//-------------------------------------------------------------------------------

#endif
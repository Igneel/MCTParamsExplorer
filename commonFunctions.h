#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H 

#include "UsedTypes.h"
#include <math>
#include <algorithm>
inline MyDataType dist(MyDataType x1, MyDataType x2);

bool thiningSignal(TSignal & inB, TSignal & inDependence, TSignal & outB, TSignal & outDependence, 
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
template <class T>
size_t indexOfElemClosestToValue(const std::vector<T> & in,MyDataType value)
{
    MyDataType dist=fabs(in[0]-value);
    size_t index=0;
    for (int i = 1; i < in.size(); ++i)
    {
        if (dist>fabs(in[i]-value))
        {
            dist=fabs(in[i]-value);
            index=i;
        }
    }
    return index;
}

//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
template <class T>
inline T max_abs_elem(const std::vector<T> & in)
{
    if(in.size()==0)
    {
        //"Сложно найти максимальный элемент, когда нет ни одного");
        return 0;
    }
    std::vector<T>::const_iterator pos;
    T max=fabs(*in.begin());
    for(pos=in.begin();pos!=in.end();++pos)
    {
        if(fabs(*pos)>max)
        max=fabs(*pos);
    }
    return max;
}
//-------------------------------------------------------------------------------

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
//-------------------------------------------------------------------------------
template <class T>
int sign(T in)
{
    if (in>=0)
    {
        return 1;
    }
    else 
        return -1;
}
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
T medianFilter(std::vector <T>::iterator &beg, std::vector <T>::iterator &end)
{
    std::sort(beg,end);
    return *(beg+std::distance(beg,end)/2);
}

template <class T>
void medianFilter(std::vector <T> & in,std::vector <T> & out,size_t size)
{
    for(std::vector <T>::iterator i=in.begin();i<in.end() && i!=in.end() ;i+=size)
    {
    if(std::distance(i,in.end())<size)
    {
    std::sort(i,in.end());
    out.push_back(*(i+std::distance(i,in.end())/2));
    }
        std::sort(i,i+size);
        out.push_back(*(i+size/2));
    }
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


#endif
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
    long long S=pow(10,NumberOfDecimalPlaces);
    for(;pos!=endPos;++pos)
    {
        int n=static_cast<int>(*pos*S*10)%10;
        if(n<5.0)
            *pos=static_cast<long double>(floorl(*pos*S))/S;
        else
            *pos=static_cast<long double>(ceill(*pos*S))/S;
    }
}
//-------------------------------------------------------------------------------
template <class T>
T roundM(T x, unsigned int NumberOfDecimalPlaces )
{
    long double S=pow(10,NumberOfDecimalPlaces);
        T n=static_cast<int>(x*S*10.0)%10;
        if(n<5.0)
            x=static_cast<long double>(floorl(x*S))/S;
        else
            x=static_cast<long double>(ceill(x*S))/S;
    return x;
}
//-------------------------------------------------------------------------------
template <class T>
size_t indexOfElemClosestToValue(const std::vector<T> & in,MyDataType value)
{
    MyDataType dist=fabs(in[0]-value);
    size_t index=0;
    for (size_t i = 1; i < in.size(); ++i)
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
size_t indexOfElemClosestToValueFromLeft(const std::vector<T> & in,MyDataType value)
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
size_t indexOfElemClosestToValueFromRight(const std::vector<T> & in,MyDataType value)
{
    MyDataType dist=fabs(in[0]-value);
    size_t index=0;
    for (int i = in.size()-1; i >= 0; --i)
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
inline T min_abs_elem(const std::vector<T> & in)
{
    if(in.size()==0)
    {
        //"Сложно найти максимальный элемент, когда нет ни одного");
        return 0;
    }
    std::vector<T>::const_iterator pos;
    T min=fabs(*in.begin());
    for(pos=in.begin();pos!=in.end();++pos)
    {
        if(fabs(*pos)<min)
        min=fabs(*pos);
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
    if (in.size()<size)
    {
        out.push_back(medianFilter(in));
    }
    for(unsigned int i=size;i<in.size() ;i+=size)
    {
        std::sort(&in[i-size],&in[i]);
        out.push_back(in[i-size/2]);
    }
}

bool testCommonFunctions()
{
    long double e=10e-6;
    long double x=1.123456;
    long double y=1.123454;
    unsigned int NumberOfDecimalPlaces=5;
    if(fabs(roundM(x,NumberOfDecimalPlaces)-1.12346)>e)
    {
        return false;
    }
    if (fabs(roundM(y,NumberOfDecimalPlaces)-1.12345)>e)
    {
        return false;
    }

    TSignal s;
    s.push_back(1.123456);
    s.push_back(1.123454);
    roundM(s.begin(), s.end(),NumberOfDecimalPlaces);
    if (fabs(s[0]-1.12346)>e || fabs(s[1]-1.12345)>e)
    {
        return false;
    }
    return true;
}
//-------------------------------------------------------------------
template <class T>
std::vector<T> calculateSecondDerivative(std::vector<T> & y, T h)
{
  std::vector<T> d2Y(y.size()-2);

      for(int i =0;i<y.size()-2;i++)
      {
        //d2Y[i]=1.0/(fabs(dY[i+1]-dY[i]))*(2.0*dY[i+1]-dY[i+2]/2.0-3.0/2.0*dY[i]);
        // формула f(x-h)-2f(x)+f(x+h)/h^2
        d2Y[i]=(y[i]-2*y[i+1]+y[i+2])/h/h;
        //d2Y[i]=(dY[i+2]-dY[i])/2.0/h;
      }
  return d2Y;
}

//-------------------------------------------------------------------
template <class T>
std::vector<T> calculateFirstDerivative(std::vector<T> & y, T h)
{
  std::vector<T> dY(y.size()-2);
  // Посчитаем производную методом конечных разностей
  // формула df/dx=(f(x+h)-f(x-h))/2/h;
  for(int i =0;i<y.size()-2;i++)
      {
        //dY[i]=1.0/(fabs(y[i+1]-y[i]))*(2.0*y[i+1]-y[i+2]/2.0-3.0/2.0*y[i]);
        dY[i]=(y[i+2]-y[i])/2.0/h;
      }
  return dY;
}

//-------------------------------------------------------------------
TSignal calculateSecondDerivative(TSignal & y, MyDataType h)
{
  TSignal d2Y(y.size()-2);

      for(int i =0;i<y.size()-2;i++)
      {
        //d2Y[i]=1.0/(fabs(dY[i+1]-dY[i]))*(2.0*dY[i+1]-dY[i+2]/2.0-3.0/2.0*dY[i]);
        // формула f(x-h)-2f(x)+f(x+h)/h^2
        d2Y[i]=(y[i]-2*y[i+1]+y[i+2])/h/h;
        //d2Y[i]=(dY[i+2]-dY[i])/2.0/h;
      }
  return d2Y;
}

//-------------------------------------------------------------------
TSignal calculateFirstDerivative(TSignal & y, MyDataType h)
{
  TSignal dY(y.size()-2);
  // Посчитаем производную методом конечных разностей
  // формула df/dx=(f(x+h)-f(x-h))/2/h;
  for(int i =0;i<y.size()-2;i++)
      {
        //dY[i]=1.0/(fabs(y[i+1]-y[i]))*(2.0*y[i+1]-y[i+2]/2.0-3.0/2.0*y[i]);
        dY[i]=(y[i+2]-y[i])/2.0/h;
      }
  return dY;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


#endif
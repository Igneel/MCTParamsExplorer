//---------------------------------------------------------------------------


#pragma hdrstop

#include "FilteringUnit.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// ВНИМАНИЕ!!! Напрямую не вызывать!!! Пользоваться трамплином!!!---------------
double Filter (const std::vector<long double> &in, std::vector<long double> & out, int length, double Fd, double Fs,double Fx)
{
  int N = length; //Длина фильтра
  //Fd - Частота дискретизации входных данных
  //Fs - Частота конца полосы пропускания 
  //Fx - Частота начала полосы затухания

  std::vector<long double> H(N);  //Импульсная характеристика фильтра
  std::vector<long double> H_id(N); //Идеальная импульсная характеристика
  std::vector<long double> W(N);   //Весовая функция

  //Расчет импульсной характеристики фильтра
  long double Fc = (Fs + Fx) / (2.0 * Fd);

  for (int i=0;i<N;i++)
  {
    if (i==0) H_id[i] = 2.0*M_PI*Fc;
    else H_id[i] = sinl(2.0*M_PI*Fc*i )/(M_PI*i);
    // весовая функция Блекмена
    W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( N-1)) + 0.08 * cosl((4*M_PI*i) /( N-1.0));
    H [i] = H_id[i] * W[i];
  }

  //Нормировка импульсной характеристики
  long double SUM=0;
  for (int i=0; i<N; i++) SUM +=H[i];
  for (int i=0; i<N; i++) H[i]/=SUM; //сумма коэффициентов равна 1

  //Фильтрация входных данных
  for (int i=0; i<in.size(); i++)
  {
    out[i]=0.0;
    for (int j=0; j<(i>N-1?N-1:i); j++)
      out[i]+= H[j]*in[i-j];
  }
  return (N-1)/2.0;
}


template <class T>
T medianFilter(std::vector <T> & in)
{
  std::sort(in.begin(),in.end());
  return in[in.size()/2];
}


long double TrForMassiveFilter(std::vector<long double> & inB,
std::vector<long double> & inY,std::vector<long double> & outB,
std::vector<long double> & outY,int lengthFilter,long double Fdisk,
long double Fpropysk,long double Fzatyh)
{
  int lengthMassive=inY.size();
  if(lengthMassive==0)
  {
    return 0;
  }
  std::vector<long double> in(inY);
  std::vector<long double> out(lengthMassive);
  double k=Filter(in,out,lengthFilter,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
  k*=(max_elem(inB)-min_elem(inB))/(double)lengthMassive;// вычисляем сдвиг фаз
  // разность максимума и минимума на длину массива
  outB.resize(lengthMassive);
  outY=out;
  for(int i=0;i<lengthMassive;i++) 
  {
    outB[i]=inB[i]-k;
  }
  return k;
}

#pragma package(smart_init)

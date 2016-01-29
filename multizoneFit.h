#ifndef MULTIZONEFIT_H
#define MULTIZONEFIT_H

#include <vector>
#include "math.h"
#include "stdlib.h"

typedef std::vector<long double> Data_spektr;
typedef std::vector< std::vector <long double> > DataValue;
typedef std::vector<long double> Image;


typedef std::vector<std::vector<long double> > TStatistic;
typedef std::vector<long double> InDataSpectr;
typedef std::vector<long double> MyData_spektr;


class MultiZoneFit
{
public:
    MultiZoneFit() {
    epsilon=2e-6;
    eps1=0.15;

     MaxPoints=100;
    PointPerInt=50;
    MaxParameters=8;    // Максимальное число параметров
   MaxRepeat=100;      // Максимальное число повторов
    ElectronCharge=1.60217656535E-19;// Кл

    SIZE=8; // Очень очень очень странный размер.
      // Параметров-то шесть.
      // Нужно проверить используются где-нибудь индексы больше.
      // Благо есть проверка диапазонов.
      for(int i=0;i<9;++i)
      {
        a[i]=1.1;
      }

      Nstep=SIZE;

    }
    int RunMultizoneFeat (long double VesGxx, long double VesGxy,
      std::vector<long double> LowBound, std::vector<long double> UpBound,
      InDataSpectr MagSpectr, InDataSpectr GxxIn, InDataSpectr GxyIn,
      MyData_spektr &outGxx, MyData_spektr &outGxy,
      TStatistic &outValues,
      int GxxSize,
      int numberOfCarrierTypes);

private:
    void BegRand(int N_make, long double VesGxx, long double VesGxy);
    void Hook(long double VesGxx, long double VesGxy);

    void memoryAlloc();
     void InitVar();

     void CheckLimits();
     void Research(long double VesGxx, long double VesGxy);

     void Optimiz_hall8(long double VesGxx, long double VesGxy);
     void btnFeatMultiClick(long double & VesGxx, long double & VesGxy);
     void Statistic(DataValue & mass, std::vector<long double> & ResulitsForStatistic, int m,int n);
    long double someTestFunc(long double VesGxx);


    long double func_hall8(Image & Data, Data_spektr & Magfield_spektr, Data_spektr &  Gxx,
      Data_spektr &  Gxy, Data_spektr &  GxxExp, Data_spektr &  GxyExp,
     int NPoint, long double Ves1, long double Ves2);


    long double epsilon;
    long double eps1;

    int  MaxPoints;
    int  PointPerInt;
    int  MaxParameters;    // Максимальное число параметров
    int  MaxRepeat;      // Максимальное число повторов
    long double  ElectronCharge;// Кл

    int  SIZE; // Очень очень очень странный размер.
      // Параметров-то шесть.
      // Нужно проверить используются где-нибудь индексы больше.
      // Благо есть проверка диапазонов.

    Data_spektr MagField_spektr;
    Data_spektr Gxx;
    Data_spektr Gxy;
    Data_spektr GxxExp;
    Data_spektr GxyExp;
    int NPoint;

    Image Data,Data0,Min_Value,Max_Value,D_Step,
    D_StepOld,DataOld,DataBef;
    int N_Data;
    int PP,NP;

    bool FlagEnd,FlagDipl;
    //long double eps1,epsilon;

    long double Fold,Fnew,Fbefore;

    long double a[9];

    int Nstep;        // количество допустимых неудачных попыток

    DataValue d1;


};




    // То что вношу я.
 /*    Dat1=array[1..MaxPoints] of long double;
    Data_spektr=array [0..MaxPoints] of long double;
    DataValue = array[1..MaxParameters, 1..MaxRepeat] of long double;
    Image=array[1..SIZE] of long double;

   Bounds= array [1..6] of long double;
    TStatistic = array of array of long double;
    InDataSpectr=array [0..10]of long double;

    MyData_spektr=array [0..10] of long double;
 */




#endif // MULTIZONEFIT_H


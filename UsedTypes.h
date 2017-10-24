#ifndef UsedTypesH
#define UsedTypesH

#include <vector>
#include <utility>
#include <string>
#include <set>

typedef long double MyDataType;
typedef std::vector<MyDataType> TSignal;
typedef TSignal::iterator TSignalIt;
typedef TSignal::const_iterator TSignalconstIt;
typedef std::vector<TSignal> TwoDimensionalContainer;
typedef std::vector<std::vector<MyDataType> > TMatrix;

typedef std::vector<long double> InDataSpectr;
typedef std::vector<std::vector<long double> > TStatistic;
typedef std::vector<long double> MyData_spektr;

struct PeaksCriteria
{
  MyDataType peakHeigh;
  MyDataType peakWidth;
  MyDataType peakWidthOrd;
  MyDataType peakVelocityR;
  MyDataType peakVelocity2R;
  MyDataType symmetri;
  MyDataType peakVelocityL;
  MyDataType peakVelocity2L;
  MyDataType peakLeftBorderFirstDerivative;
  MyDataType peakFirstDerivative;
  MyDataType peakRightBorderFirstDerivative;
  MyDataType peakLeftMiddleFirstDerivative;
  MyDataType peakRightMiddleFirstDerivative;
  MyDataType peakLeftBorderSecondDerivative;
  MyDataType peakSecondDerivative;
  MyDataType peakRightBorderSecondDerivative;
  MyDataType peakLeftMiddleSecondDerivative;
  MyDataType peakRightMiddleSecondDerivative;
};

struct AdditionalData
{
  MyDataType leftPointElectronConductivity;
  MyDataType leftPointHoleConductivity;
  MyDataType rightPointElectronConductivity;
  MyDataType rightPointHoleConductivity;
  MyDataType leftPointElectronConductivityLog;
  MyDataType leftPointHoleConductivityLog;
  MyDataType rightPointElectronConductivityLog;
  MyDataType rightPointHoleConductivityLog;
  MyDataType leftPointMobility;
  MyDataType rightPointMobility;
};

/*
Давайте поиграем в продумывание архитектуры
Вот есть у нас завивимости:
Поле
и Какие-то сигналы
Поле в общем-то тоже сигнал

Как результат наш набор данных должен быть массивом сигналов?
С явными обозначениями, так что видимо это действительно будет
либо ассоциативный массив map либо набор пар

Получается мне нужно реализовать отдельно класс сигнала в принципе
а потом наследовать его и переопределять некоторые функции согласно тому, как должен обрабатываться конкретный сигнал...
я в шоке... профессиональная деформация началась.

*/
typedef std::set<std::pair<std::string, TSignal> >DataSet;

typedef std::vector< std::pair<int, int> > channelsInfo;
typedef std::vector< std::pair<std::string, std::string> > optionDescription;

enum DataKind {CURRENT_DATA=0, FILTERED_DATA=1, EXTRAPOLATED_DATA=2, AVERAGED_DATA=3, TENZOR_DATA=4, MOBILITY_DATA=5, FIT_DATA=6};

enum SignalType {MAGNETIC_FIELD=0, HALL_EFFECT=1, MAGNETORESISTANCE=2, PHOTOCONDUCTIVITY_FARADEY=3, PHOTOCONDICTIVITY_FOYGHT=4,MAGNETIC_FIELD_F=5, SXX=6,SXY=7};
//enum PlotType {MAGNETIC_FIELD, HALL_EFFECT, MAGNETORESISTANCE,MAGNETIC_FIELD_F, SXX,SXY};
enum ParamsType {DIRECT=0,REVERSE=1,COMBINE=2};

const long double THEALMOSTZERO = 0.000001;




#endif


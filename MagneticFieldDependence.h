#ifndef MagneticFieldDependenceH
#define MagneticFieldDependenceH

#include <vector>
#include <algorithm>
#include <System.hpp>
#include "FilteringUnit.h"
#include "LCard.h"

#include <vcl.h>
#include "DataSaver.h"
#include "FilterParams.h"
#include "UsedTypes.h"
#include "ExtrapolateUnit.h"
#include "commonFunctions.h"
//#include "Signal.h"
//#include "MagneticField.h"
//#include "hallEffect.h"


/*
На данный момент:
1. Архитектура не была продумана до конца, так что придется кое-что править.

По поводу продолжения рефакторинга.

Думаю нужен класс Loader/Saver - убрать туда все функции связанные с сохранением, чтобы они не усложняли этот класс.

Также можно разнести данные Холла и магнитосопротивления по разным классам и хранить их как составные части этого класса.

class MagneticFieldDependence
{
	HallEffect * hallEffect;
	MagnetoResistance * magnetoResistance;

	Saver * saver;
	Loader * loader;

} 

*/
/*
class Loader
{
public:
	Loader(AnsiString FileName);
	~Loader();

	TSignal getLoadedData();

private:
	void loadData(TStringList * tsl);	
	inline void ReplaceDotsToComma(std::string &in, std::string & out);
};


*/






class MagneticFieldDependence
{
public:

TSignal * MagneticFieldDependence::getPointerSxx(DataKind dataKind);
//-------------------------------------------------------------------------
TSignal * MagneticFieldDependence::getPointerSxy(DataKind dataKind);

	enum FeatType {ODD_FEAT, EVEN_FEAT};

	MagneticFieldDependence(AnsiString current, AnsiString temperature, AnsiString SampleInventoryNumber,
		AnsiString length, AnsiString width, AnsiString Thickness);

	~MagneticFieldDependence();

    TSignal const * getB();

	TSignal const * getBHall();
	TSignal const * getBMagnetoResistance();
    TSignal const * getHallEffect();
    TSignal const * getMagnetoResistance();
  
    TSignal const * getFilteredB();

	TSignal const * getFilteredBHall();
	TSignal const * getFilteredBMagnetoResistance();
    TSignal const * getFilteredHallEffect();
    TSignal const * getFilteredMagnetoResistance();
    TSignal const * getExtrapolatedB();

	TSignal const * getExtrapolatedBHall();
	TSignal const * getExtrapolatedBMagnetoResistance();
    TSignal const * getExtrapolatedHallEffect();
    TSignal const * getExtrapolatedMagnetoResistance();   

    TSignal const * getSxx();
    TSignal const * getSxy();
    TSignal const * getAveragedB();
    TSignal const * getRh_eff();
    TSignal const * getS_eff();
	

	//(получение, фильтрация, экстраполяция, увеличение/уменьшение, вырезка и т.п.,
	//    построение графиков, сохранение результатов)

	void getSplittedDataFromADC();

	void loadData(TStringList * tsl);
	void loadSampleDescription(TStringList *Names,TStringList *Values,AnsiString FileName);

	void setDependence(TSignal::iterator beginB, 
		TSignal::iterator endB, TSignal::iterator beginHall, 
		TSignal::iterator beginResistance);
	// Сохранение данных.
	void setSampleDescription(AnsiString Temperature, AnsiString Current, AnsiString SampleInventoryNumber, AnsiString length, AnsiString width, AnsiString Thickness);

	void SaveAllData(AnsiString FileName,bool isCombinedParams=false);

	//-------Построение графиков-------------------------------------- 
	bool constructPlotFromTwoMassive(SignalType pt, DataKind dk,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(SignalType p,TLineSeries* s,TColor color);

	void shiftCurve(DataKind dataKind,SignalType dependenceType,MyDataType shiftValue,MyDataType leftBound, MyDataType rightBound);

	//-----Расчет тензора проводимости---------------------------------
	
	void calculateTenzor(DataKind dataKind);

	//------Фильтрация результатов-------------------------------------
	bool setFilterParamsHall(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length);
	bool setFilterParamsHall(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);
	bool setFilterParamsResistance(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length);
	bool setFilterParamsResistance(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);

	FilterParams const * getFilterParamsHall();
	FilterParams const * getFilterParamsResistance();
	// дискретизации, пропускания, затухания
	void filterData(FilterParams &fPHall, FilterParams &fPResistance);
	void filterData();
        void blockfilterData();

	// Экстраполяция результатов

    void setExtrapolateParams(int powPolinowHall,int powPolinomRes);

	bool extrapolateData(const int polinomPowForMagnetoResistance, const int polinomPowForHallEffect);
	
	// просто трамплин на сейвер. возможно он него можно как-нибудь избавиться.
	void setRoundNeeded(bool needRound);

	void setParamsType(ParamsType pt);

    void setChannelsInfo(channelsInfo & cI);

    void multiplySignal(SignalType s, MyDataType x);
    void rearrangeSignal();



    // Сохранение результатов------------------------------------------
	DataSaver * saver;

private:

	ParamsType paramsType;
	TSignal leftBound;
	TSignal rightBound;

	channelsInfo chanInfo;

	
    //bool ChangeChannels;
	//-------Построение графиков--------------------------------------- 
	void plotData();

		

	//------Фильтрация результатов-------------------------------------
	FilterParams *filterParamsHall;
	FilterParams *filterParamsResistance;

    int PowPolinomRes;
    int PowPolinomHall;

    void filterDataHelper(FilterParams &fP,
    SignalType dependenceType);

    //------Загрузка данных-------------------------------------------- 
	void loadDataHelper(TSignal &temp, String AnsiS,const std::string delimiter);
	inline void ReplaceDotsToComma(std::string &in, std::string & out);

	

	//---------Обработка данных----------------------------------------
	void multiplyB(DataKind dataKind);

    void GetEqualNumberOfPoints(TSignal & B,
TSignal & BHall,TSignal & BRes, TSignal & Hall,
TSignal & Res);

	void calculateEffectiveParamsFromSignals();
	void calculateTenzorFromEffectiveParams();
	void featData(DataKind dataKind); // усреднение зависимостей, вызывать эту.
	void MagneticFieldDependence::averageData(TSignal & inY, TSignal &outY, FeatType featType, int centerOfSimmetry);
	
	void cutData(DataKind dataKind); // оставляет только положительные значения магнитного поля

	void clearCurrentParams();
	void clearFilteredParams();

	TSignal * getPointerB(DataKind dataKind);
	TSignal * getPointerHall(DataKind dataKind);
	TSignal * getPointerMagnetoResistance(DataKind dataKind);

	//---------------Много переменных----------------------------------
	// Текущие магнитное поле и эффект Холла/магнитосопротивление,
	// после всяческих преобразований (вырезка, увеличение и т.п.).

    DataSet currentData;
    //Signal * testB;
    //Signal * testHall;


	TSignal B;
	TSignal BHall;
	TSignal BMagnetoResistance;
	TSignal HallEffect;
    TSignal MagnetoResistance;
	// Первоначальные значения, полученные от АЦП сохраняются в текущие.
	// Фильтрованные значения.
	TSignal FilteredB;
	TSignal FilteredBHall;
	TSignal FilteredBMagnetoResistance;
    TSignal FilteredHallEffect;
    TSignal FilteredMagnetoResistance;

	// Экстраполированные значения.
	TSignal ExtrapolatedB;
	TSignal ExtrapolatedBHall;
	TSignal ExtrapolatedBMagnetoResistance;
    TSignal ExtrapolatedHallEffect;
    TSignal ExtrapolatedMagnetoResistance;

    // Усредненные значения.
    TSignal AveragedB;
	TSignal AveragedBHall;
	TSignal AveragedBMagnetoResistance;
	TSignal AveragedHallEffect;
	TSignal AveragedMagnetoResistance;

    // И ещё мы кушаем память для усредненных значений, которые будем
    // кормить спектру подвижности.
    /*
    TSignal AveragedB;
    TSignal sxx;
	TSignal sxy; */

	TSignal s_eff;
	TSignal Rh_eff;
	TSignal sxx;
	TSignal sxy;
    
};


#endif
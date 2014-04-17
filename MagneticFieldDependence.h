#ifndef MagneticFieldDependenceH
#define MagneticFieldDependenceH

#include <vector>
#include <algorithm>
#include <System.hpp>
#include "FilteringUnit.h"
#include "LCard.h"

#include <vcl.h>

#include "FilterParams.h"
#include "UsedTypes.h"
#include "ExtrapolateUnit.h"

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

	DataTypeInContainer getLoadedData();

private:
	void loadData(TStringList * tsl);	
	inline void ReplaceDotsToComma(std::string &in, std::string & out);
};


enum SaveType {ALL_POINTS,SOME_POINTS};
class Saver
{
public:
	Saver();
	~Saver();

	void SaveData(DataKind dataKind,SaveType saveType,AnsiString FileName);
	void SaveAllData(AnsiString FileName);
	void setRoundNeeded(bool needRound);
private:

	// Сохранение результатов и т.д.----------------------------------
	unsigned int NumberOfDecimalPlaces;
	bool isRoundNeeded;
	
    template <class T>
    void MagneticFieldDependence::RoundM(T *pos, T* endPos);

    AnsiString defaultExtension;
    void MagneticFieldDependence::SaveDataHelper(DataTypeInContainer &saveB,
	DataTypeInContainer & saveHall,
	DataTypeInContainer & saveResistance,SaveType mode,
	AnsiString FileName);
	inline void ReplaceCommaToDots(std::string &in, std::string & out);
	
};*/



enum PlotType {MAGNETIC_FIELD, HALL_EFFECT, MAGNETORESISTANCE};
enum SaveType {ALL_POINTS,SOME_POINTS};
enum DataKind {CURRENT_DATA, FILTERED_DATA, EXTRAPOLATED_DATA, ORIGINAL_DATA};



class MagneticFieldDependence
{
public:

	enum FeatType {ODD_FEAT, EVEN_FEAT};

	MagneticFieldDependence::MagneticFieldDependence(MyDataType current);

    DataTypeInContainer const & getB();
    DataTypeInContainer const & getHallEffect();
    DataTypeInContainer const & getMagnetoResistance();
    DataTypeInContainer const & getOriginalB();
    DataTypeInContainer const & getOriginalHallEffect();
    DataTypeInContainer const & getOriginalMagnetoResistance();
    DataTypeInContainer const & getFilteredB();
    DataTypeInContainer const & getFilteredHallEffect();
    DataTypeInContainer const & getFilteredMagnetoResistance();
    DataTypeInContainer const & getExtrapolatedB();
    DataTypeInContainer const & getExtrapolatedHallEffect();
    DataTypeInContainer const & getExtrapolatedMagnetoResistance();
    
	~MagneticFieldDependence();

	//(получение, фильтрация, экстраполяция, увеличение/уменьшение, вырезка и т.п.,
	//    построение графиков, сохранение результатов)

	void getSplittedDataFromADC();

	void loadData(TStringList * tsl);

	void setDependence(DataTypeInContainer::iterator beginB, 
		DataTypeInContainer::iterator endB, DataTypeInContainer::iterator beginHall, 
		DataTypeInContainer::iterator beginResistance);
	// Сохранение данных.

	void SaveData(DataKind dataKind,SaveType saveType,AnsiString FileName);
	void SaveAllData(AnsiString FileName);

	//-------Построение графиков-------------------------------------- 
	void constructPlotFromTwoMassive(PlotType pt, DataKind dk,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color);

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

	// Экстраполяция результатов

	bool extrapolateData(const int polinomPowForMagnetoResistance, const int polinomPowForHallEffect);
	

	void setRoundNeeded(bool needRound);



private:	

	//-------Построение графиков-------------------------------------- 
	void plotData();

	// Сохранение результатов и т.д.----------------------------------
	unsigned int NumberOfDecimalPlaces;
	bool isRoundNeeded;

	
    template <class T>
    void RoundM(T *pos, T* endPos);

    AnsiString defaultExtension;
    void SaveDataHelper(DataTypeInContainer &saveB,
	DataTypeInContainer & saveHall,
	DataTypeInContainer & saveResistance,SaveType mode,
	AnsiString FileName);
	inline void ReplaceDotsToComma(std::string &in, std::string & out);
	inline void ReplaceCommaToDots(std::string &in, std::string & out);

	//------Фильтрация результатов-------------------------------------
	FilterParams *filterParamsHall;
	FilterParams *filterParamsResistance;

    void filterDataHelper(FilterParams &fP,
    PlotType dependenceType);

    //------Загрузка данных-------------------------------------------- 
	void loadDataHelper(DataTypeInContainer &temp, String AnsiS,const std::string delimiter);


	//---------Обработка данных----------------------------------------
	void featData(DataKind dataKind, long index, FeatType featType);
	void averagingData(); // усреднение зависимостей. не реализована
	void multiplyB(DataKind dataKind);
	void cutData();

	//---------------Много переменных----------------------------------
	MyDataType Temperature; // температура образца во время измерений.
	MyDataType Current; // ток на образце, в амперах.
	// Текущие магнитное поле и эффект Холла/магнитосопротивление,
	// после всяческих преобразований (вырезка, увеличение и т.п.).
	DataTypeInContainer B;
	DataTypeInContainer HallEffect;
    DataTypeInContainer MagnetoResistance;
	// Первоначальные значения, полученные от АЦП.
	DataTypeInContainer OriginalB;
    DataTypeInContainer OriginalHallEffect;
    DataTypeInContainer OriginalMagnetoResistance;
	// Фильтрованные значения.
	DataTypeInContainer FilteredB;
    DataTypeInContainer FilteredHallEffect;
    DataTypeInContainer FilteredMagnetoResistance;

	// Экстраполированные значения.
	DataTypeInContainer ExtrapolatedB;
    DataTypeInContainer ExtrapolatedHallEffect;
    DataTypeInContainer ExtrapolatedMagnetoResistance;

	MyDataType h; // шаг по магнитному полю, если он вообще понадобится:)

	unsigned int NumberOfPoints;

    
};

#endif
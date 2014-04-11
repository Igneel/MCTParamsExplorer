#ifndef MagneticFieldDependenceH
#define MagneticFieldDependenceH

#include <vector>
#include <algorithm>
#include "FilteringUnit.h"
#include "LCard.h"
#include <Series.hpp>
#include "FilterParams.h"
#include "UsedTypes.h"

/*
На данный момент:
1. Архитектура не была продумана до конца, так что придется кое-что править.

*/




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

	// Сохранение данных.

	void SaveData(DataKind dataKind,SaveType saveType,AnsiString FileName);
	void SaveAllData(AnsiString FileName);

	//-------Построение графиков-------------------------------------- 
	void constructPlotFromTwoMassive(PlotType pt, DataKind dk,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color);

	//------Фильтрация результатов-------------------------------------
	bool setFilterParams(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length);
	bool setFilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);

	FilterParams const * getFilterParams();
	// дискретизации, пропускания, затухания
	void filterData(FilterParams &fP);
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
    void MagneticFieldDependence::RoundM(T *pos, T* endPos);

    AnsiString defaultExtension;
    void MagneticFieldDependence::SaveDataHelper(DataTypeInContainer &saveB,
	DataTypeInContainer & saveHall,
	DataTypeInContainer & saveResistance,SaveType mode,
	AnsiString FileName);
	inline void ReplaceDotsToComma(std::string &in, std::string & out);
	inline void ReplaceCommaToDots(std::string &in, std::string & out);

	//------Фильтрация результатов-------------------------------------
	FilterParams *filterParams;

    void MagneticFieldDependence::filterDataHelper(FilterParams &fP,
    PlotType dependenceType);

    //------Загрузка данных-------------------------------------------- 
	bool loadData();

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
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
2. Нужно реализовать функции сохранения/генерации имен и т.д.
3. Явно надо разобраться с перечислимыми типами, т.к. кое-где есть дубляж. +


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

	 
	//DataTypeInContainer const &  getDataFromADC();

	void getSplittedDataFromADC();

	void SaveData(DataKind dataKind,SaveType saveType,AnsiString FileName);
	void SaveAllData(AnsiString FileName);

	void constructPlotFromTwoMassive(PlotType pt, DataKind dk,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color);


	bool setFilterParams(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length);
	bool setFilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);

	FilterParams const * getFilterParams();
	bool extrapolateData(const int polinomPowForMagnetoResistance, const int polinomPowForHallEffect);
	// дискретизации, пропускания, затухания
	void filterData(FilterParams &fP);
	void filterData();

	void setRoundNeeded(bool needRound);



private:

	bool isRoundNeeded;

	AnsiString defaultExtension;
    template <class T>
    void MagneticFieldDependence::RoundM(T *pos, T* endPos);

    void MagneticFieldDependence::SaveDataHelper(DataTypeInContainer &saveB,
	DataTypeInContainer & saveHall,
	DataTypeInContainer & saveResistance,SaveType mode,
	AnsiString FileName);


	FilterParams *filterParams;

	void multiplyB(DataKind dataKind);
	void cutData();
	void plotData();

	inline void ReplaceDotsToComma(std::string &in, std::string & out);
	inline void ReplaceCommaToDots(std::string &in, std::string & out);

    void MagneticFieldDependence::filterDataHelper(FilterParams &fP,
    PlotType dependenceType);

	bool loadData();
	void featData(DataKind dataKind, long index, FeatType featType);
	void averagingData(); // усреднение зависимостей. не реализована

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

    unsigned int NumberOfDecimalPlaces;
};

#endif
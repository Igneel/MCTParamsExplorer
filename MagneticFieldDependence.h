#ifndef MagneticFieldDependenceH
#define MagneticFieldDependenceH

#include <vector>
#include <algorithm>
#include "FilteringUnit.h"
#include "LCard.h"
#include <Series.hpp>

/*
На данный момент:
1. Архитектура не была продумана до конца, так что придется кое-что править.
2. Нужно реализовать функции сохранения/генерации имен и т.д.
3. Явно надо разобраться с перечислимыми типами, т.к. кое-где есть дубляж. +


*/


typedef long double MyDataType;

enum PlotType {MAGNETIC_FIELD, HALL_EFFECT, MAGNETORESISTANCE};
enum SaveType {ALL_POINTS,SOME_POINTS};
enum DataKind {CURRENT_DATA, FILTERED_DATA, EXTRAPOLATED_DATA, ORIGINAL_DATA};

struct FilterParams
{
	MyDataType SamplingFrequecy;
	MyDataType BandwidthFrequency;
	MyDataType AttenuationFrequency;
	int filterLength;
};

class MagneticFieldDependence
{
public:

	enum FeatType {ODD_FEAT, EVEN_FEAT};

    MagneticFieldDependence();
	MagneticFieldDependence::MagneticFieldDependence(MyDataType current);

    std::vector<MyDataType> const & getB();
    std::vector<MyDataType> const & getHallEffect();
    std::vector<MyDataType> const & getMagnetoResistance();
    std::vector<MyDataType> const & getOriginalB();
    std::vector<MyDataType> const & getOriginalHallEffect();
    std::vector<MyDataType> const & getOriginalMagnetoResistance();
    std::vector<MyDataType> const & getFilteredB();
    std::vector<MyDataType> const & getFilteredHallEffect();
    std::vector<MyDataType> const & getFilteredMagnetoResistance();
    std::vector<MyDataType> const & getExtrapolatedB();
    std::vector<MyDataType> const & getExtrapolatedHallEffect();
    std::vector<MyDataType> const & getExtrapolatedMagnetoResistance();
    
	~MagneticFieldDependence();

	//(получение, фильтрация, экстраполяция, увеличение/уменьшение, вырезка и т.п.,
	//    построение графиков, сохранение результатов)

	 
	//std::vector<MyDataType> const &  getDataFromADC();

	void SaveData(DataKind dataKind,SaveType saveType,std::string FileName);

	void getSplittedDataFromADC();

	bool saveData();


	void constructPlotFromTwoMassive(PlotType pt, DataKind dk,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color);


	bool setFilterParams(FilterParams & fp);
	FilterParams & getFilterParams();
	bool extrapolateData();
	// дискретизации, пропускания, затухания
	void filterData(FilterParams &fP);

private:

    template <class T>
    void MagneticFieldDependence::RoundM(T *pos, T* endPos);

    void MagneticFieldDependence::SaveDataHelper(std::vector<MyDataType> &saveB,
    std::vector<MyDataType> & saveHall,
    std::vector<MyDataType> & saveResistance,bool isRoundNeeded,SaveType mode);


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
	std::vector<MyDataType> B;
	std::vector<MyDataType> HallEffect;
    std::vector<MyDataType> MagnetoResistance;
	// Первоначальные значения, полученные от АЦП.
	std::vector<MyDataType> OriginalB;
    std::vector<MyDataType> OriginalHallEffect;
    std::vector<MyDataType> OriginalMagnetoResistance;
	// Фильтрованные значения.
	std::vector<MyDataType> FilteredB;
    std::vector<MyDataType> FilteredHallEffect;
    std::vector<MyDataType> FilteredMagnetoResistance;

	// Экстраполированные значения.
	std::vector<MyDataType> ExtrapolatedB;
    std::vector<MyDataType> ExtrapolatedHallEffect;
    std::vector<MyDataType> ExtrapolatedMagnetoResistance;

	MyDataType h; // шаг по магнитному полю, если он вообще понадобится:)

	unsigned int NumberOfPoints;

    unsigned int NumberOfDecimalPlaces;
};

#endif

#ifndef HALLEFFECT_H
#define HALLEFFECT_H

#include <vector>
#include <algorithm>
#include "FilteringUnit.h"
#include "LCard.h"
#include <Series.hpp>
#include "FilterParams.h"
#include "UsedTypes.h"

/*
Под эгидой упрощения монструозного класс Магнитополевых зависимостей.

*/

enum PlotType {MAGNETIC_FIELD, HALL_EFFECT, MAGNETORESISTANCE};
enum SaveType {ALL_POINTS,SOME_POINTS};
enum DataKind {CURRENT_DATA, FILTERED_DATA, EXTRAPOLATED_DATA, ORIGINAL_DATA};


class HallEffect
{
public:

	enum FeatType {ODD_FEAT, EVEN_FEAT};

	HallEffect::HallEffect(MyDataType current, MyDataType Temperature);

    DataTypeInContainer const & getB();
    DataTypeInContainer const & getHallEffect();
    DataTypeInContainer const & getOriginalB();
    DataTypeInContainer const & getOriginalHallEffect();
    DataTypeInContainer const & getFilteredB();
    DataTypeInContainer const & getFilteredHallEffect();
    DataTypeInContainer const & getExtrapolatedB();
    DataTypeInContainer const & getExtrapolatedHallEffect();
    
	~HallEffect();

	//(получение, фильтрация, экстраполяция, увеличение/уменьшение, вырезка и т.п.,
	//    построение графиков, сохранение результатов)

	 
	//DataTypeInContainer const &  getDataFromADC();

	void getHallEffect();

	void constructPlotFromTwoMassive(PlotType pt, DataKind dk,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color);

	// дискретизации, пропускания, затухания
	bool setFilterParams(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length);
	bool setFilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);

	FilterParams const * getFilterParams();

	void filterData(FilterParams &fP);
	void filterData();

	bool extrapolateData(const int polinomPowForMagnetoResistance, const int polinomPowForHallEffect);	

private:

	FilterParams *filterParams;

	void multiplyB(DataKind dataKind);
	void cutData();
	void plotData();

    void HallEffect::filterDataHelper(FilterParams &fP,
    PlotType dependenceType);

	void featData(DataKind dataKind, long index, FeatType featType);
	void averagingData(); // усреднение зависимостей. не реализована

	MyDataType Current; // ток на образце, в амперах.
	// Текущие магнитное поле и эффект Холла,
	// после всяческих преобразований (вырезка, увеличение и т.п.).
	DataTypeInContainer B;
	DataTypeInContainer HallEffect;
	// Первоначальные значения, полученные от АЦП.
	DataTypeInContainer OriginalB;
    DataTypeInContainer OriginalHallEffect;
	// Фильтрованные значения.
	DataTypeInContainer FilteredB;
    DataTypeInContainer FilteredHallEffect;

	// Экстраполированные значения.
	DataTypeInContainer ExtrapolatedB;
    DataTypeInContainer ExtrapolatedHallEffect;

	MyDataType h; // шаг по магнитному полю, если он вообще понадобится:)

	unsigned int NumberOfPoints; // не уверен что оно надо.

    
};


#endif
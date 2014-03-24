#ifndef MagneticFieldDependenceH
#define MagneticFieldDependenceH

#include <vector>
#include <algorithm>
#include "FilteringUnit.h"
#include "LCard.h"
#include <Series.hpp>

typedef long double MyDataType;

enum DependenceType {HALL_EFFECT, MAGNETORESISTANCE};
enum PlotType {MAGNETIC_FIELD, DEPENDENCE};
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

    std::vector<MyDataType> const & getB();
    std::vector<MyDataType> const & getDependence();
    std::vector<MyDataType> const & getOriginalB();
    std::vector<MyDataType> const & getOriginalDependence();
    std::vector<MyDataType> const & getFilteredB();
    std::vector<MyDataType> const & getFilteredDependence();
    std::vector<MyDataType> const & getExtrapolatedB();
    std::vector<MyDataType> const & getExtrapolatedDependence();

	MagneticFieldDependence(DependenceType d);
	MagneticFieldDependence(DependenceType dt, MyDataType current, MyDataType step);

	~MagneticFieldDependence();

	//(получение, фильтраци€, экстрапол€ци€, увеличение/уменьшение, вырезка и т.п.,
	//    построение графиков, сохранение результатов)

	 
	//std::vector<MyDataType> const &  getDataFromADC();

	void SaveData(DataKind dataKind,SaveType saveType,std::string FileName);

	void getSplittedDataFromADC();

	bool saveData();


	void constructPlotFromTwoMassive(DataKind p,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color);


	bool setFilterParams(FilterParams & fp);
	FilterParams & getFilterParams();
	bool extrapolateData();
	// дискретизации, пропускани€, затухани€
	void filterData(FilterParams &fP);

private:
	DependenceType dependenceType;

	FilterParams *filterParams;





	void multiplyB(DataKind dataKind);
	void cutData();
	void plotData();

	inline void ReplaceDotsToComma(std::string &in, std::string & out);
	inline void ReplaceCommaToDots(std::string &in, std::string & out);

	bool loadData();
	void featData(DataKind dataKind, long index, FeatType featType);
	void averagingData(); // усреднение зависимостей.

	MyDataType Current; // ток на образце, в амперах.
	// “екущие магнитное поле и эффект ’олла/магнитосопротивление,
	// после вс€ческих преобразований (вырезка, увеличение и т.п.).
	std::vector<MyDataType> B;
	std::vector<MyDataType> Dependence;
	// ѕервоначальные значени€, полученные от ј÷ѕ.
	std::vector<MyDataType> OriginalB;
	std::vector<MyDataType> OriginalDependence;
	// ‘ильтрованные значени€.
	std::vector<MyDataType> FilteredB;
	std::vector<MyDataType> FilteredDependence;
	// Ёкстраполированные значени€.
	std::vector<MyDataType> ExtrapolatedB;
	std::vector<MyDataType> ExtrapolatedDependence;

	MyDataType h; // шаг по магнитному полю, если он вообще понадобитс€:)

	unsigned int NumberOfPoints;

};

#endif
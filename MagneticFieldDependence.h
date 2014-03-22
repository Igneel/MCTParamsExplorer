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
enum DataKind {CURRENT_DATA, FILTERED_DATA, EXTRAPOLATED_DATA, ORIGINAL_DATA};

MagneticFieldDependence(DependenceType d);
MagneticFieldDependence::MagneticFieldDependence(DependenceType dt, MyDataType current, MyDataType step);

~MagneticFieldDependence();

//(получение, фильтраци€, экстрапол€ци€, увеличение/уменьшение, вырезка и т.п.,
//    построение графиков, сохранение результатов)

// 
std::vector<MyDataType> const &  getDataFromADC();

void getSplittedDataFromADC();

bool saveData();


 void MagneticFieldDependence::constructPlotFromTwoMassive(TLineSeries* s,TColor color);
void MagneticFieldDependence::constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color);


bool setFilterParams(FilterParams & fp);
FilterParams getFilterParams();

private:
DependenceType dependenceType;

FilterParams filterParams;



bool extrapolateData();
// дискретизации, пропускани€, затухани€
void filterData(DependenceType dependenceType,MyDataType SamplingFrequecy, MyDataType BandwidthFrequency,
     MyDataType AttenuationFrequency,int filterLength);
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
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

//(���������, ����������, �������������, ����������/����������, ������� � �.�.,
//    ���������� ��������, ���������� �����������)

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
// �������������, �����������, ���������
void filterData(DependenceType dependenceType,MyDataType SamplingFrequecy, MyDataType BandwidthFrequency,
     MyDataType AttenuationFrequency,int filterLength);
void multiplyB(DataKind dataKind);
void cutData();
void plotData();

inline void ReplaceDotsToComma(std::string &in, std::string & out);
inline void ReplaceCommaToDots(std::string &in, std::string & out);
bool loadData();
void featData(DataKind dataKind, long index, FeatType featType);
void averagingData(); // ���������� ������������.

MyDataType Current; // ��� �� �������, � �������.
// ������� ��������� ���� � ������ �����/��������������������,
// ����� ��������� �������������� (�������, ���������� � �.�.).
std::vector<MyDataType> B;
std::vector<MyDataType> Dependence;
// �������������� ��������, ���������� �� ���.
std::vector<MyDataType> OriginalB;
std::vector<MyDataType> OriginalDependence;
// ������������� ��������.
std::vector<MyDataType> FilteredB;
std::vector<MyDataType> FilteredDependence;
// ������������������ ��������.
std::vector<MyDataType> ExtrapolatedB;
std::vector<MyDataType> ExtrapolatedDependence;

MyDataType h; // ��� �� ���������� ����, ���� �� ������ �����������:)

unsigned int NumberOfPoints;

};

#endif
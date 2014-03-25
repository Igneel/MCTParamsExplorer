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

	//(���������, ����������, �������������, ����������/����������, ������� � �.�.,
	//    ���������� ��������, ���������� �����������)

	 
	//std::vector<MyDataType> const &  getDataFromADC();

	void SaveData(DataKind dataKind,SaveType saveType,std::string FileName);

	void getSplittedDataFromADC();

	bool saveData();


	void constructPlotFromTwoMassive(DataKind p,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color);


	bool setFilterParams(FilterParams & fp);
	FilterParams & getFilterParams();
	bool extrapolateData();
	// �������������, �����������, ���������
	void filterData(FilterParams &fP);

private:
	DependenceType dependenceType;

	FilterParams *filterParams;





	void multiplyB(DataKind dataKind);
	void cutData();
	void plotData();

	inline void ReplaceDotsToComma(std::string &in, std::string & out);
	inline void ReplaceCommaToDots(std::string &in, std::string & out);

    void MagneticFieldDependence::filterDataHelper(FilterParams &fP,
    DependenceType dependenceType);

	bool loadData();
	void featData(DataKind dataKind, long index, FeatType featType);
	void averagingData(); // ���������� ������������. �� �����������

	MyDataType Current; // ��� �� �������, � �������.
	// ������� ��������� ���� � ������ �����/��������������������,
	// ����� ��������� �������������� (�������, ���������� � �.�.).
	std::vector<MyDataType> B;
	std::vector<MyDataType> HallEffect;
    std::vector<MyDataType> MagnetoResistance;
	// �������������� ��������, ���������� �� ���.
	std::vector<MyDataType> OriginalB;
    std::vector<MyDataType> OriginalHallEffect;
    std::vector<MyDataType> OriginalMagnetoResistance;
	// ������������� ��������.
	std::vector<MyDataType> FilteredB;
    std::vector<MyDataType> FilteredHallEffect;
    std::vector<MyDataType> FilteredMagnetoResistance;

	// ������������������ ��������.
	std::vector<MyDataType> ExtrapolatedB;
    std::vector<MyDataType> ExtrapolatedHallEffect;
    std::vector<MyDataType> ExtrapolatedMagnetoResistance;

	MyDataType h; // ��� �� ���������� ����, ���� �� ������ �����������:)

	unsigned int NumberOfPoints;

};

#endif
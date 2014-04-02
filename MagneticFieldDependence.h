#ifndef MagneticFieldDependenceH
#define MagneticFieldDependenceH

#include <vector>
#include <algorithm>
#include "FilteringUnit.h"
#include "LCard.h"
#include <Series.hpp>

/*
�� ������ ������:
1. ����������� �� ���� ��������� �� �����, ��� ��� �������� ���-��� �������.
2. ����� ����������� ������� ����������/��������� ���� � �.�.
3. ���� ���� ����������� � ������������� ������, �.�. ���-��� ���� ������. +


*/


typedef long double MyDataType;

enum PlotType {MAGNETIC_FIELD, HALL_EFFECT, MAGNETORESISTANCE};
enum SaveType {ALL_POINTS,SOME_POINTS};
enum DataKind {CURRENT_DATA, FILTERED_DATA, EXTRAPOLATED_DATA, ORIGINAL_DATA};

class FilterParams
{
public:
	FilterParams();
	FilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);
	FilterParams(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length);
	void setFilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);

	MyDataType SamplingFrequecy;
	MyDataType BandwidthFrequency;
	MyDataType AttenuationFrequency;
	int filterLength;
};

class MagneticFieldDependence
{
public:

	enum FeatType {ODD_FEAT, EVEN_FEAT};

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

	void getSplittedDataFromADC();

	void SaveData(DataKind dataKind,SaveType saveType,AnsiString FileName);
	void SaveAllData(AnsiString FileName);

	void constructPlotFromTwoMassive(PlotType pt, DataKind dk,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color);


	bool setFilterParams(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length);
	bool setFilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);

	FilterParams const * getFilterParams();
	bool extrapolateData();
	// �������������, �����������, ���������
	void filterData(FilterParams &fP);
	void filterData();

	void setRoundNeeded(bool needRound);



private:

	bool isRoundNeeded;

	AnsiString defaultExtension;
    template <class T>
    void MagneticFieldDependence::RoundM(T *pos, T* endPos);

    void MagneticFieldDependence::SaveDataHelper(std::vector<MyDataType> &saveB,
	std::vector<MyDataType> & saveHall,
	std::vector<MyDataType> & saveResistance,SaveType mode,
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

    unsigned int NumberOfDecimalPlaces;
};

#endif
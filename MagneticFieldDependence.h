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
�� ������ ������:
1. ����������� �� ���� ��������� �� �����, ��� ��� �������� ���-��� �������.

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

	//(���������, ����������, �������������, ����������/����������, ������� � �.�.,
	//    ���������� ��������, ���������� �����������)

	void getSplittedDataFromADC();

	// ���������� ������.

	void SaveData(DataKind dataKind,SaveType saveType,AnsiString FileName);
	void SaveAllData(AnsiString FileName);

	//-------���������� ��������-------------------------------------- 
	void constructPlotFromTwoMassive(PlotType pt, DataKind dk,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color);

	//------���������� �����������-------------------------------------
	bool setFilterParams(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length);
	bool setFilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);

	FilterParams const * getFilterParams();
	// �������������, �����������, ���������
	void filterData(FilterParams &fP);
	void filterData();

	// ������������� �����������

	bool extrapolateData(const int polinomPowForMagnetoResistance, const int polinomPowForHallEffect);
	

	void setRoundNeeded(bool needRound);



private:	

	//-------���������� ��������-------------------------------------- 
	void plotData();

	// ���������� ����������� � �.�.----------------------------------
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

	//------���������� �����������-------------------------------------
	FilterParams *filterParams;

    void MagneticFieldDependence::filterDataHelper(FilterParams &fP,
    PlotType dependenceType);

    //------�������� ������-------------------------------------------- 
	bool loadData();

	//---------��������� ������----------------------------------------
	void featData(DataKind dataKind, long index, FeatType featType);
	void averagingData(); // ���������� ������������. �� �����������
	void multiplyB(DataKind dataKind);
	void cutData();

	//---------------����� ����������----------------------------------
	MyDataType Temperature; // ����������� ������� �� ����� ���������.
	MyDataType Current; // ��� �� �������, � �������.
	// ������� ��������� ���� � ������ �����/��������������������,
	// ����� ��������� �������������� (�������, ���������� � �.�.).
	DataTypeInContainer B;
	DataTypeInContainer HallEffect;
    DataTypeInContainer MagnetoResistance;
	// �������������� ��������, ���������� �� ���.
	DataTypeInContainer OriginalB;
    DataTypeInContainer OriginalHallEffect;
    DataTypeInContainer OriginalMagnetoResistance;
	// ������������� ��������.
	DataTypeInContainer FilteredB;
    DataTypeInContainer FilteredHallEffect;
    DataTypeInContainer FilteredMagnetoResistance;

	// ������������������ ��������.
	DataTypeInContainer ExtrapolatedB;
    DataTypeInContainer ExtrapolatedHallEffect;
    DataTypeInContainer ExtrapolatedMagnetoResistance;

	MyDataType h; // ��� �� ���������� ����, ���� �� ������ �����������:)

	unsigned int NumberOfPoints;

    
};

#endif
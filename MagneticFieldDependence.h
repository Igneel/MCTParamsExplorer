#ifndef MagneticFieldDependenceH
#define MagneticFieldDependenceH

#include <vector>
#include <algorithm>
#include <System.hpp>
#include "FilteringUnit.h"
#include "LCard.h"

#include <vcl.h>
#include "DataSaver.h"
#include "FilterParams.h"
#include "UsedTypes.h"
#include "ExtrapolateUnit.h"


/*
�� ������ ������:
1. ����������� �� ���� ��������� �� �����, ��� ��� �������� ���-��� �������.

�� ������ ����������� ������������.

����� ����� ����� Loader/Saver - ������ ���� ��� ������� ��������� � �����������, ����� ��� �� ��������� ���� �����.

����� ����� �������� ������ ����� � �������������������� �� ������ ������� � ������� �� ��� ��������� ����� ����� ������.

class MagneticFieldDependence
{
	HallEffect * hallEffect;
	MagnetoResistance * magnetoResistance;

	Saver * saver;
	Loader * loader;

} 

*/
/*
class Loader
{
public:
	Loader(AnsiString FileName);
	~Loader();

	DataTypeInContainer getLoadedData();

private:
	void loadData(TStringList * tsl);	
	inline void ReplaceDotsToComma(std::string &in, std::string & out);
};


*/



enum PlotType {MAGNETIC_FIELD, HALL_EFFECT, MAGNETORESISTANCE};


class MagneticFieldDependence
{
public:

	enum FeatType {ODD_FEAT, EVEN_FEAT};

	MagneticFieldDependence(AnsiString current, AnsiString temperature, AnsiString SampleInventoryNumber,
		AnsiString length, AnsiString width, AnsiString Thickness);

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

	void loadData(TStringList * tsl);

	void setDependence(DataTypeInContainer::iterator beginB, 
		DataTypeInContainer::iterator endB, DataTypeInContainer::iterator beginHall, 
		DataTypeInContainer::iterator beginResistance);
	// ���������� ������.
	void setSampleDescription(AnsiString Temperature, AnsiString Current, AnsiString SampleInventoryNumber, AnsiString length, AnsiString width, AnsiString Thickness);

	void MagneticFieldDependence::SaveAllData(AnsiString FileName,bool isCombinedParams=false);

	//-------���������� ��������-------------------------------------- 
	void constructPlotFromTwoMassive(PlotType pt, DataKind dk,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color);

	//------���������� �����������-------------------------------------
	bool setFilterParamsHall(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length);
	bool setFilterParamsHall(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);
	bool setFilterParamsResistance(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length);
	bool setFilterParamsResistance(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);

	FilterParams const * getFilterParamsHall();
	FilterParams const * getFilterParamsResistance();
	// �������������, �����������, ���������
	void filterData(FilterParams &fPHall, FilterParams &fPResistance);
	void filterData();

	// ������������� �����������

	bool extrapolateData(const int polinomPowForMagnetoResistance, const int polinomPowForHallEffect);
	

	void setRoundNeeded(bool needRound);

    void enableChangeChannels();
    void disableChangeChannels();

private:
    bool ChangeChannels;
	//-------���������� ��������-------------------------------------- 
	void plotData();

	// ���������� ����������� � �.�.----------------------------------

	DataSaver * saver;
	

	//------���������� �����������-------------------------------------
	FilterParams *filterParamsHall;
	FilterParams *filterParamsResistance;

    void filterDataHelper(FilterParams &fP,
    PlotType dependenceType);
    void filterDataHelper2(FilterParams &fP,
    PlotType dependenceType);

    //------�������� ������-------------------------------------------- 
	void loadDataHelper(DataTypeInContainer &temp, String AnsiS,const std::string delimiter);
	inline void MagneticFieldDependence::ReplaceDotsToComma(std::string &in, std::string & out);

	//---------��������� ������----------------------------------------
	void featData(DataKind dataKind, long index, FeatType featType);
	void averagingData(); // ���������� ������������. �� �����������
	void multiplyB(DataKind dataKind);
	void cutData();

	//---------------����� ����������----------------------------------
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
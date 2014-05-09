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
#include "commonFunctions.h"


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

	~MagneticFieldDependence();

    DataTypeInContainer const * getB();

	DataTypeInContainer const * getBHall();
	DataTypeInContainer const * getBMagnetoResistance();
    DataTypeInContainer const * getHallEffect();
    DataTypeInContainer const * getMagnetoResistance();
  
    DataTypeInContainer const * getFilteredB();

	DataTypeInContainer const * getFilteredBHall();
	DataTypeInContainer const * getFilteredBMagnetoResistance();
    DataTypeInContainer const * getFilteredHallEffect();
    DataTypeInContainer const * getFilteredMagnetoResistance();
    DataTypeInContainer const * getExtrapolatedB();

	DataTypeInContainer const * getExtrapolatedBHall();
	DataTypeInContainer const * getExtrapolatedBMagnetoResistance();
    DataTypeInContainer const * getExtrapolatedHallEffect();
    DataTypeInContainer const * getExtrapolatedMagnetoResistance();   

    DataTypeInContainer const * getSxx();
    DataTypeInContainer const * getSxy();
    DataTypeInContainer const * getAveragedB();
    DataTypeInContainer const * getRh_eff();
    DataTypeInContainer const * getS_eff();
	

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

	//-----������ ������� ������������---------------------------------
	
	void calcutaleTenzor(DataKind dataKind);

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
	
	// ������ �������� �� ������. �������� �� ���� ����� ���-������ ����������.
	void setRoundNeeded(bool needRound);

	void setParamsType(ParamsType pt);

	// ��� ���� ������ ���������
    void enableChangeChannels();
    void disableChangeChannels();

    // ���������� �����������------------------------------------------
	DataSaver * saver;

private:

	ParamsType paramsType;
	DataTypeInContainer leftBound;
	DataTypeInContainer rightBound;
	
    bool ChangeChannels;
	//-------���������� ��������--------------------------------------- 
	void plotData();

		

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
	void multiplyB(DataKind dataKind);

	void calculateEffectiveParamsFromSignals();
	void calculateTenzorFromEffectiveParams();
	void featData(DataKind dataKind); // ���������� ������������, �������� ���.
	void averageData(DataTypeInContainer & inY, DataTypeInContainer &outY, FeatType featType);
	
	void cutData(DataKind dataKind); // ��������� ������ ������������� �������� ���������� ����

	void MagneticFieldDependence::clearCurrentParams();

	DataTypeInContainer * getPointerB(DataKind dataKind);
	DataTypeInContainer * getPointerHall(DataKind dataKind);
	DataTypeInContainer * getPointerMagnetoResistance(DataKind dataKind);

	//---------------����� ����������----------------------------------
	// ������� ��������� ���� � ������ �����/��������������������,
	// ����� ��������� �������������� (�������, ���������� � �.�.).
	DataTypeInContainer B;
	DataTypeInContainer BHall;
	DataTypeInContainer BMagnetoResistance;
	DataTypeInContainer HallEffect;
    DataTypeInContainer MagnetoResistance;
	// �������������� ��������, ���������� �� ��� ����������� � �������.
	// ������������� ��������.
	DataTypeInContainer FilteredB;
	DataTypeInContainer FilteredBHall;
	DataTypeInContainer FilteredBMagnetoResistance;
    DataTypeInContainer FilteredHallEffect;
    DataTypeInContainer FilteredMagnetoResistance;

	// ������������������ ��������.
	DataTypeInContainer ExtrapolatedB;
	DataTypeInContainer ExtrapolatedBHall;
	DataTypeInContainer ExtrapolatedBMagnetoResistance;
    DataTypeInContainer ExtrapolatedHallEffect;
    DataTypeInContainer ExtrapolatedMagnetoResistance;

    // ����������� ��������.
    DataTypeInContainer AveragedB;
	DataTypeInContainer AveragedBHall;
	DataTypeInContainer AveragedBMagnetoResistance;
	DataTypeInContainer AveragedHallEffect;
	DataTypeInContainer AveragedMagnetoResistance;

	DataTypeInContainer s_eff;
	DataTypeInContainer Rh_eff;
	DataTypeInContainer sxx;
	DataTypeInContainer sxy;
    
};


#endif
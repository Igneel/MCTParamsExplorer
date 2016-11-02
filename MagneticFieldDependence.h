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
#include "Dependence.h"
#include "mobilityspectrum.h"

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

	TSignal getLoadedData();

private:
	void loadData(TStringList * tsl);	
	inline void ReplaceDotsToComma(std::string &in, std::string & out);
};


*/






class MagneticFieldDependence
{
public:

TSignal * MagneticFieldDependence::getPointerSxx(DataKind dataKind);
//-------------------------------------------------------------------------
TSignal * MagneticFieldDependence::getPointerSxy(DataKind dataKind);

	enum FeatType {ODD_FEAT, EVEN_FEAT};

	MagneticFieldDependence(AnsiString current, AnsiString temperature, AnsiString SampleInventoryNumber,
		AnsiString length, AnsiString width, AnsiString Thickness);

	~MagneticFieldDependence();

    TSignal const * getB();

	TSignal const * getBHall();
	TSignal const * getBMagnetoResistance();
    TSignal const * getHallEffect();
    TSignal const * getMagnetoResistance();
  
    TSignal const * getFilteredB();

	TSignal const * getFilteredBHall();
	TSignal const * getFilteredBMagnetoResistance();
    TSignal const * getFilteredHallEffect();
    TSignal const * getFilteredMagnetoResistance();
    TSignal const * getExtrapolatedB();

	TSignal const * getExtrapolatedBHall();
	TSignal const * getExtrapolatedBMagnetoResistance();
    TSignal const * getExtrapolatedHallEffect();
    TSignal const * getExtrapolatedMagnetoResistance();   

    TSignal const * getSxx();
    TSignal const * getSxy();
    TSignal const * getAveragedB();
    TSignal const * getRh_eff();
    TSignal const * getS_eff();

    TSignal const * getMobility();
	TSignal const * getHoleConductivity();
	TSignal const * getElectronConductivity();
	
    TSignal const * getHoleConcentration();
    TSignal const * getHoleMobility();
    TSignal const * getElectronConcentration();
    TSignal const * getElectronMobility();
	

	//(���������, ����������, �������������, ����������/����������, ������� � �.�.,
	//    ���������� ��������, ���������� �����������)

	void getSplittedDataFromADC();

	void loadData(TStringList * tsl);
	void loadSampleDescription(TStringList *Names,TStringList *Values,AnsiString FileName);

	void setDependence(TSignal::iterator beginB, 
		TSignal::iterator endB, TSignal::iterator beginHall, 
		TSignal::iterator beginResistance);
	// ���������� ������.
	void setSampleDescription(AnsiString Temperature, AnsiString Current, AnsiString SampleInventoryNumber, AnsiString length, AnsiString width, AnsiString Thickness);

	void SaveAllData(AnsiString FileName);

	//-------���������� ��������-------------------------------------- 
	bool constructPlotFromTwoMassive(SignalType pt, DataKind dk,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(SignalType p,TLineSeries* s,TColor color);

	void shiftCurve(DataKind dataKind,SignalType dependenceType,MyDataType shiftValue,MyDataType leftBound, MyDataType rightBound);

	//-----������ ������� ������������---------------------------------
	
	void calculateTenzor(DataKind dataKind);

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
        void blockfilterData();

	// ������������� �����������

    void setExtrapolateParams(int powPolinowHall,int powPolinomRes);
    void getExtrapolateParams(int & powPolinomHall,int & powPolinomRes);

	//bool extrapolateData(const int polinomPowForMagnetoResistance, const int polinomPowForHallEffect);
	bool extrapolateData( DataKind dataKind, const int polinomPowForMagnetoResistance, const int polinomPowForHallEffect);
	
	// ������ �������� �� ������. �������� �� ���� ����� ���-������ ����������.
	void setRoundNeeded(bool needRound);

	void setParamsType(ParamsType pt);

    void setChannelsInfo(channelsInfo & cI);

    void multiplySignal(SignalType s, MyDataType x);
    void rearrangeSignal();


    // ������ �����������
    bool calculateMobilitySpectrum();
    // ����������� ��������
    bool runMultiCarrierFit();



    // ���������� �����������------------------------------------------
	DataSaver * saver;




	mobilitySpectrum * MobilitySpectrumObj;



	

private:

	ParamsType paramsType;
	TSignal leftBound;
	TSignal rightBound;

	channelsInfo chanInfo;

	
	//-------���������� ��������--------------------------------------- 
	void plotData();
		

	//------���������� �����������-------------------------------------
	FilterParams *filterParamsHall;
	FilterParams *filterParamsResistance;

    int PowPolinomRes;
    int PowPolinomHall;

    void filterDataHelper(FilterParams &fP,
    SignalType dependenceType,DataKind dataKind);

    //------�������� ������-------------------------------------------- 
	void loadDataHelper(TSignal &temp, String AnsiS,const std::string delimiter);
	inline void ReplaceDotsToComma(std::string &in, std::string & out);

	

	//---------��������� ������----------------------------------------
	void multiplyB(DataKind dataKind);

    void GetEqualNumberOfPoints(TSignal & B,
TSignal & BHall,TSignal & BRes, TSignal & Hall,
TSignal & Res);

	void calculateEffectiveParamsFromSignals();
	void calculateTenzorFromEffectiveParams();
	void featData(DataKind dataKind); // ���������� ������������, �������� ���.
	void MagneticFieldDependence::averageData(TSignal & inY, TSignal &outY, FeatType featType, int centerOfSimmetry);
	
	void cutData(DataKind dataKind); // ��������� ������ ������������� �������� ���������� ����

	void clearCurrentParams();
	void clearFilteredParams();

	TSignal * getPointerB(DataKind dataKind);
	TSignal * getPointerBHall(DataKind dataKind);
	TSignal * getPointerBResistance(DataKind dataKind);
	TSignal * getPointerHall(DataKind dataKind);
	TSignal * getPointerMagnetoResistance(DataKind dataKind);

	//---------------����� ����������----------------------------------
	// ������� ��������� ���� � ������ �����/��������������������,
	// ����� ��������� �������������� (�������, ���������� � �.�.).

    DataSet currentData;

	TSignal B;
	TSignal BHall;
	TSignal BMagnetoResistance;
	TSignal HallEffect;
    TSignal MagnetoResistance;
	// �������������� ��������, ���������� �� ��� ����������� � �������.
	// ������������� ��������.
	TSignal FilteredB;
	TSignal FilteredBHall;
	TSignal FilteredBMagnetoResistance;
    TSignal FilteredHallEffect;
    TSignal FilteredMagnetoResistance;

	// ������������������ ��������.
	TSignal ExtrapolatedB;
	TSignal ExtrapolatedBHall;
	TSignal ExtrapolatedBMagnetoResistance;
    TSignal ExtrapolatedHallEffect;
    TSignal ExtrapolatedMagnetoResistance;

    // ����������� ��������.
    TSignal AveragedB;
	TSignal AveragedBHall;
	TSignal AveragedBMagnetoResistance;
	TSignal AveragedHallEffect;
	TSignal AveragedMagnetoResistance;

    // � ��� �� ������ ������ ��� ����������� ��������, ������� �����
    // ������� ������� �����������.
    /*
    TSignal AveragedB;
    TSignal sxx;
	TSignal sxy; */

	TSignal s_eff;
	TSignal Rh_eff;
	TSignal sxx;
	TSignal sxy;

	// ���������� ������ ������� �����������
	TSignal mobility;
	TSignal holeConductivity;
	TSignal electronConductivity;
	// � ���� �������� ��������, ��������������� ����� ��� ����� � ����������
    TSignal holeConcentration;
    TSignal holeMobility;
    TSignal electronConcentration;
    TSignal electronMobility;

    
};


#endif
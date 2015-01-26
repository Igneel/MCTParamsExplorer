#ifndef LCardH
#define LCardH
#include <string>
#include <Series.hpp>
#include <deque>
#include "include/lusbapi.h"
#pragma link "Lib/lusbapi.lib"
#pragma hdrstop

#include "UsedTypes.h"
#include "FilteringUnit.h"
#include "commonFunctions.h"





//������������� ��������, ���������, ������/������� ���������, ��������� ������
/*
�������� ������� �������������� �������, ����������� ��� �� ������ ���������� �������.
��������� ����� ������������� �������� �������� SettingADCParams

������ ������� ��������� �������������� ��������� 	StartMeasurement() StopMeasurement()

������ ���������� �� ������� ��� �������� getSplittedData();

����� ���� ������� ������
������� �������� ��������� (���� �� ��������� ������).

� ��������� �������������� ������.


*/
class LCardADC
{
public:

	LCardADC::LCardADC(double frenquency,int blockSize, TLabel * l1, TLabel * l2, TLabel * l3,
        channelsInfo cI);
	~LCardADC();

	std::string Error(std::string); // ��, ��� ���� �� ��������:)

	unsigned long __stdcall ServiceReadThreadReal(); // ��� ������� �������� ��������� ������� � �������� ������.

	bool LCardADC::SettingADCParams(double frenquency, int newBlockSize, channelsInfo & chanInfo);

	bool StartMeasurement();
	bool StopMeasurement();

	bool StartWriting();
	bool StopWriting();

    void EnableMedianFilter();
    void DisableMedianFilter();
    void setMedianFilterLength(size_t s);

    void EnableTestingMode();
    void DisableTestingMode();

	std::vector<TSignal > const &  LCardADC::getSplittedData();
    std::vector<TSignal >  *  LCardADC::getSplittedData(int a);

    void testSetReadBuffer(); // ��� �������.

	void clearBuffer(); // ������� ������.
	bool IsInitSuccessfull(); // ���������� True ���� ������������� ������ �������

    bool IsMeasurementRunning();
    bool isWritingEnabled();

	void setHallSeries(TLineSeries *s);
	void setMagnetoResistanceSeries(TLineSeries *s);
	void setBSeries (TLineSeries * s);

    void dataisntNeeded();

private:
    int counter;

    void LCardADC::realTimeFilter(TSignal & inData,
TSignal & outData);
    std::deque<TLabel *> ChannelLabels;

    channelsInfo chanInfo;

	bool successfullInit;
    bool isMedianFilterEnabled;
    size_t medianFilterLength;

    bool isDataNeeded;

    bool isMeasurementRunning;
    bool isWriting;

    bool TestingMode;
    
	bool DriverInit();

	TLineSeries *B;
	TLineSeries *HallSeries;
	TLineSeries *MagnetoResistanceSeries;


	void LCardADC::splitToChannels(TSignal &tempData,
	std::vector<TSignal > &splittedData); // ����� �� �������, ����������� � ����������� �������.

	void convertToVolt(); // �������������� � ������.
	MyDataType LCardADC::convertToVolt(MyDataType in,int channel);

	void LCardADC::writeDataToVector(TSignal & tempData); // ��������� ���������� ������.

	void LCardADC::DisplayOnForm(int i1, MyDataType v1);
	void InteractivePlottingData();
	void InteractivePlottingDataOne();

	// ������������� ������ ����� ������
	HANDLE hReadThread;
	unsigned long ReadTid;
	// ������ ���������� ������ ������ ����� ������
	bool IsReadThreadComplete;
	// ����� ������ ��� ���������� ����� ������
	WORD ReadThreadErrorNumber;
	bool needToStop;
	//-------------------------��������� ������ ���---------------------------------
	// ������ ����������
	unsigned long DllVersion;
	// ��������� �� ��������� ������
	ILE440 *pModule;
	// ���������� ����������
	HANDLE ModuleHandle;
	// �������� ������
	char ModuleName[7];
	// �������� ������ ���� USB
	unsigned char UsbSpeed;
	// ��������� � ������ ����������� � ������
	MODULE_DESCRIPTION_E440 ModuleDescription;
	// ��������� ���������� ������ ��� ������
	ADC_PARS_E440 ap;

    // ������������ �������� � ��������, ������� ����� ���� �������� � ���.
    long double ADCMaxValue;

    
	//------------------------����� ���������� ���----------------------------------

	//------------------------���������� ��� ��������� � �� �������-----------------

	// ���-�� ���������� �������� (������� 32) ��� �. ReadData()
	unsigned long DataStep;

	// ����� ������
	short *ReadBuffer;
	//TSignal ReadData;
	std::vector<TSignal > ReadData;
	std::vector<TSignal > splittedData;
	//------------------------------------------------------------------------------
	std::deque<std::deque<MyDataType> > DequeBuffer;

	//-----------------------------------------------------------------------------
	FilterLowBand * lowBandFilter;

};

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);

#endif
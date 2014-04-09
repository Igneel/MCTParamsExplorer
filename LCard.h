#ifndef LCardH
#define LCardH
#include <vector>
#include <string>
#include "UsedTypes.h"
#include <deque>

#include <Series.hpp>
#include "include/lusbapi.h"
#pragma link "Lib/lusbapi.lib"


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

	LCardADC::LCardADC(unsigned short channelsQuantity, double frenquency);
	~LCardADC();

	std::string Error(std::string); // ��, ��� ���� �� ��������:)

	unsigned long __stdcall ServiceReadThreadReal(); // ��� ������� �������� ��������� ������� � �������� ������.

	bool SettingADCParams(unsigned short channelsQuantity, double frenquency);

	bool StartMeasurement();
	void StopMeasurement();

    void EnableMedianFilter();
    void DisableMedianFilter();

    void EnableTestingMode();
    void DisableTestingMode();

	std::vector<DataTypeInContainer > const &  LCardADC::getSplittedData();

    void testSetReadBuffer(); // ��� �������.

	void clearBuffer(); // ������� ������.
	bool IsInitSuccessfull(); // ���������� True ���� ������������� ������ �������

    bool IsMeasurementRunning();

	void setHallSeries(TLineSeries *s);
	void setMagnetoResistanceSeries(TLineSeries *s);
	void setBSeries (TLineSeries * s);

private:
	bool successfullInit;
    bool isMedianFilterEnabled;
    bool isMeasurementRunning;

    bool TestingMode;
    
	bool DriverInit();

	TLineSeries *B;
	TLineSeries *HallSeries;
	TLineSeries *MagnetoResistanceSeries;


	void LCardADC::splitToChannels(DataTypeInContainer &tempData,
	std::vector<DataTypeInContainer > &splittedData); // ����� �� �������, ����������� � ����������� �������.

	void convertToVolt(); // �������������� � ������ (�������� ��� ��������� 10�����).
	MyDataType LCardADC::convertToVolt(MyDataType in);

	void LCardADC::writeDataToVector(DataTypeInContainer & tempData); // ��������� ���������� ������.

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
	//------------------------����� ���������� ���----------------------------------

	//------------------------���������� ��� ��������� � �� �������-----------------

	// ���-�� ���������� �������� (������� 32) ��� �. ReadData()
	unsigned long DataStep;

	// ����� ������
	short *ReadBuffer;
	//DataTypeInContainer ReadData;
	std::vector<DataTypeInContainer > ReadData;
	std::vector<DataTypeInContainer > splittedData;
	// ������� ������
	int Counter; // ������, ��� �� ������ �����, ������ ������ ������.
	//------------------------------------------------------------------------------
	std::deque<std::deque<MyDataType> > DequeBuffer;
};

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);

#endif
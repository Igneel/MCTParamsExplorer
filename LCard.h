#ifndef LCardH
#define LCardH
#include <vector>
#include <string>

#include <Series.hpp>
#include "include/lusbapi.h"
#pragma link "Lib/lusbapi.lib"

typedef long double MyDataType;

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

	std::vector<std::vector<MyDataType> > const &  LCardADC::getSplittedData();

    void testSetReadBuffer(); // ��� �������.

	void clearBuffer(); // ������� ������.
	bool IsInitSuccessfull(); // ���������� True ���� ������������� ������ �������

    bool IsMeasurementRunning();

	void setHallSeries(TLineSeries *s);
	void setMagnetoResistanceSeries(TLineSeries *s);

private:
	bool successfullInit;
    bool isMedianFilterEnabled;
    bool isMeasurementRunning;
    
	bool DriverInit();


	TLineSeries *HallSeries;
	TLineSeries *MagnetoResistanceSeries;


	void LCardADC::splitToChannels(std::vector<MyDataType> &tempData,
	std::vector<std::vector<MyDataType> > &splittedData); // ����� �� �������, ����������� � ����������� �������.

	void convertToVolt(); // �������������� � ������ (�������� ��� ��������� 10�����).
	MyDataType LCardADC::convertToVolt(MyDataType in);

	void LCardADC::writeDataToVector(std::vector<MyDataType> & tempData); // ��������� ���������� ������.

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
	//std::vector<MyDataType> ReadData;
	std::vector<std::vector<MyDataType> > ReadData;
	std::vector<std::vector<MyDataType> > splittedData;
	// ������� ������
	int Counter; // ������, ��� �� ������ �����, ������ ������ ������.
	//------------------------------------------------------------------------------

};

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);

#endif
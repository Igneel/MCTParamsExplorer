#ifndef LCardH
#define LCardH
#include <vector>
#include <string>

#include "include/lusbapi.h"
#pragma link "Lib/lusbapi.lib"

typedef long double MyDataType;

//������������� ��������, ���������, ������/������� ���������, ��������� ������
class LCardADC
{
public:
LCardADC();
~LCardADC();

std::string Error(std::string);
unsigned long __stdcall ServiceReadThreadReal();
bool SettingADCParams();

void StartMeasurement();
void StopMeasurement();

std::vector<MyDataType> const & getData();
void clearBuffer();

private:
bool DriverInit();

bool successfullInit;
bool needToStop;

void convertToVolt();



// ������������� �����
HANDLE hFile;

// ������������� ������ ����� ������
HANDLE hReadThread;
unsigned long ReadTid;

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

// ���-�� ���������� �������� (������� 32) ��� �. ReadData()
unsigned long DataStep;
// ����� �������� NDataBlock ������ �� DataStep �������� � ������
const WORD NDataBlock;
// ����� ������
short *ReadBuffer;

// ����� ������
short *AdcBuffer;

// ������ ���������� ������ ������ ����� ������
bool IsReadThreadComplete;
// ����� ������ ��� ���������� ����� ������
WORD ReadThreadErrorNumber;

// �������� �������-���������
DWORD Counter, OldCounter;

std::vector<MyDataType> ReadData;


};

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);

#endif
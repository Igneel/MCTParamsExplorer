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
DWORD WINAPI ServiceReadThreadReal();
bool SettingADCParams();

void StartMeasurement();
void StopMeasurement();

void getData();
void clearBuffer();

private:
bool DriverInit();


bool successfullInit;
bool needToStop;



// ������������� �����
HANDLE hFile;

// ������������� ������ ����� ������
HANDLE hReadThread;
DWORD ReadTid;

// ������ ����������
DWORD DllVersion;
// ��������� �� ��������� ������
ILE440 *pModule;
// ���������� ����������
HANDLE ModuleHandle;
// �������� ������
char ModuleName[7];
// �������� ������ ���� USB
BYTE UsbSpeed;
// ��������� � ������ ����������� � ������
MODULE_DESCRIPTION_E440 ModuleDescription;
// ��������� ���������� ������ ��� ������
ADC_PARS_E440 ap;

// ���-�� ���������� �������� (������� 32) ��� �. ReadData()
DWORD DataStep;
// ����� �������� NDataBlock ������ �� DataStep �������� � ������
const WORD NDataBlock;//80
// ����� ������
SHORT *ReadBuffer;

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
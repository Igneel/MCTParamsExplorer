#ifndef LCardH
#define LCardH
#include <vector>
#include <string>

#include "include/lusbapi.h"
#pragma link "Lib/lusbapi.lib"

typedef long double MyDataType;

//инициализация драйвера, настройка, запуск/останов измерений, получение данных
class LCardADC
{
public:
LCardADC();
~LCardADC();

std::string Error(std::string);
unsigned long __stdcall ServiceReadThreadReal();
bool SettingADCParams();
bool SettingADCParams(unsigned short channelsQuantity, double frenquency);

void StartMeasurement();
void StopMeasurement();

std::vector<MyDataType> const & getData();
void clearBuffer();
bool IsInitSuccessfull();

private:
bool DriverInit();

bool successfullInit;
bool needToStop;

void convertToVolt();

bool isMedianFilterEnabled;



// идентификатор потока сбора данных
HANDLE hReadThread;
unsigned long ReadTid;

// версия библиотеки
unsigned long DllVersion;
// указатель на интерфейс модуля
ILE440 *pModule;
// дескриптор устройства
HANDLE ModuleHandle;
// название модуля
char ModuleName[7];
// скорость работы шины USB
unsigned char UsbSpeed;
// структура с полной информацией о модуле
MODULE_DESCRIPTION_E440 ModuleDescription;
// структура параметров работы АЦП модуля
ADC_PARS_E440 ap;

// кол-во получаемых отсчетов (кратное 32) для Ф. ReadData()
unsigned long DataStep;

// буфер данных
short *ReadBuffer;


// флажок завершения работы потока сбора данных
bool IsReadThreadComplete;
// номер ошибки при выполнении сбора данных
WORD ReadThreadErrorNumber;

// счетчик кадров
int Counter;

std::vector<MyDataType> ReadData;


};

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);

#endif
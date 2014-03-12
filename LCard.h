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



// идентификатор файла
HANDLE hFile;

// идентификатор потока сбора данных
HANDLE hReadThread;
DWORD ReadTid;

// версия библиотеки
DWORD DllVersion;
// указатель на интерфейс модуля
ILE440 *pModule;
// дескриптор устройства
HANDLE ModuleHandle;
// название модуля
char ModuleName[7];
// скорость работы шины USB
BYTE UsbSpeed;
// структура с полной информацией о модуле
MODULE_DESCRIPTION_E440 ModuleDescription;
// структура параметров работы АЦП модуля
ADC_PARS_E440 ap;

// кол-во получаемых отсчетов (кратное 32) для Ф. ReadData()
DWORD DataStep;
// будем собирать NDataBlock блоков по DataStep отсчётов в каждом
const WORD NDataBlock;//80
// буфер данных
SHORT *ReadBuffer;

// флажок завершения работы потока сбора данных
bool IsReadThreadComplete;
// номер ошибки при выполнении сбора данных
WORD ReadThreadErrorNumber;

// экранный счетчик-индикатор
DWORD Counter, OldCounter;

std::vector<MyDataType> ReadData;


};

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);

#endif
#ifndef LCardH
#define LCardH
#include <vector>
#include <string>
#include "UsedTypes.h"

#include <Series.hpp>
#include "include/lusbapi.h"
#pragma link "Lib/lusbapi.lib"


//инициализация драйвера, настройка, запуск/останов измерений, получение данных
/*
Создание объекта инициализирует драйвер, настраивает АЦП на нужное количество каналов.
Настройки можно дополнительно изменить фукнцией SettingADCParams

Запуск останов измерений осуществляется функциями 	StartMeasurement() StopMeasurement()

Данные передаются во внешний мир функцией getSplittedData();

Также есть очистка буфера
Функция возврата состояния (идет ли измерение сейчас).

И настройка интерактивного вывода.


*/
class LCardADC
{
public:

	LCardADC::LCardADC(unsigned short channelsQuantity, double frenquency);
	~LCardADC();

	std::string Error(std::string); // ой, она пока не работает:)

	unsigned long __stdcall ServiceReadThreadReal(); // эта функция работает отдельным потоком и собирает данные.

	bool SettingADCParams(unsigned short channelsQuantity, double frenquency);

	bool StartMeasurement();
	void StopMeasurement();

    void EnableMedianFilter();
    void DisableMedianFilter();

	std::vector<DataTypeInContainer > const &  LCardADC::getSplittedData();

    void testSetReadBuffer(); // для отладки.

	void clearBuffer(); // очистка буфера.
	bool IsInitSuccessfull(); // возвращает True если инициализация прошла успешно

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


	void LCardADC::splitToChannels(DataTypeInContainer &tempData,
	std::vector<DataTypeInContainer > &splittedData); // делим по каналам, прописанным в контрольной таблице.

	void convertToVolt(); // преобразование в вольты (работает для диапазона 10Вольт).
	MyDataType LCardADC::convertToVolt(MyDataType in);

	void LCardADC::writeDataToVector(DataTypeInContainer & tempData); // сохраняет полученные данные.

	// идентификатор потока сбора данных
	HANDLE hReadThread;
	unsigned long ReadTid;
	// флажок завершения работы потока сбора данных
	bool IsReadThreadComplete;
	// номер ошибки при выполнении сбора данных
	WORD ReadThreadErrorNumber;
	bool needToStop;
	//-------------------------Параметры работы АЦП---------------------------------
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
	//------------------------Конец параметров АЦП----------------------------------

	//------------------------Контейнеры для измерений и их размеры-----------------

	// кол-во получаемых отсчетов (кратное 32) для Ф. ReadData()
	unsigned long DataStep;

	// буфер данных
	short *ReadBuffer;
	//DataTypeInContainer ReadData;
	std::vector<DataTypeInContainer > ReadData;
	std::vector<DataTypeInContainer > splittedData;
	// счетчик кадров
	int Counter; // однако, это не совсем кадры, скорее снимки буфера.
	//------------------------------------------------------------------------------

};

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);

#endif
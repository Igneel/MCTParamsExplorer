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

	LCardADC::LCardADC(double frenquency,int blockSize, TLabel * l1, TLabel * l2, TLabel * l3,
        channelsInfo cI);
	~LCardADC();

	std::string Error(std::string); // ой, она пока не работает:)

	unsigned long __stdcall ServiceReadThreadReal(); // эта функция работает отдельным потоком и собирает данные.

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

    void testSetReadBuffer(); // для отладки.

	void clearBuffer(); // очистка буфера.
	bool IsInitSuccessfull(); // возвращает True если инициализация прошла успешно

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
	std::vector<TSignal > &splittedData); // делим по каналам, прописанным в контрольной таблице.

	void convertToVolt(); // преобразование в вольты.
	MyDataType LCardADC::convertToVolt(MyDataType in,int channel);

	void LCardADC::writeDataToVector(TSignal & tempData); // сохраняет полученные данные.

	void LCardADC::DisplayOnForm(int i1, MyDataType v1);
	void InteractivePlottingData();
	void InteractivePlottingDataOne();

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

    // Максимальное значение в отсчетах, которое может быть получено с АЦП.
    long double ADCMaxValue;

    
	//------------------------Конец параметров АЦП----------------------------------

	//------------------------Контейнеры для измерений и их размеры-----------------

	// кол-во получаемых отсчетов (кратное 32) для Ф. ReadData()
	unsigned long DataStep;

	// буфер данных
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
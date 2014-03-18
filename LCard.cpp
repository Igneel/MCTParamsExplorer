
#include "LCard.h"
#include "Unit1.h"

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);


LCardADC::LCardADC()
{
isMedianFilterEnabled=true;// включение медианной фильтрации кадров.
needToStop=true;// флаг для остановки второго потока
successfullInit=false; // флаг успешной инициализации
ReadThreadErrorNumber=0;// переменная с кодом ошибки инициализации.
DataStep =256*64;  // кол-во отсчетов, кратное 32
// оно явно должно зависеть от количества измеряемых каналов и частоты.
Counter = 0;        // количество полученных кадров.

ReadBuffer=new SHORT[2*DataStep]; // в этот буфер считываются данные.

if(DriverInit()) // инициализиуем драйвер
{
if(SettingADCParams())// устанавливаем настройки
{
successfullInit=true; // всё прошло успешно.
}
}

}

bool LCardADC::DriverInit()
{
    if((DllVersion = GetDllVersion()) != CURRENT_VERSION_LUSBAPI)
	{
    // надо пугаться и бежать обновлять драйвера.
    return false;
    }

    // попробуем получить указатель на интерфейс
	pModule = static_cast<ILE440 *>(CreateLInstance("e440"));
	if(!pModule)
    {
    return false;
    }
    int i;
    // попробуем обнаружить модуль E14-440 в первых MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI виртуальных слотах
	for(i = 0x0; i < MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI; i++) if(pModule->OpenLDevice(i)) break;
	// что-нибудь обнаружили?
	if(i == MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI)
    {
    return false;
    // проверить подключен ли АЦП
    }

    // попробуем прочитать дескриптор устройства
	ModuleHandle = pModule->GetModuleHandle();
	if(ModuleHandle == INVALID_HANDLE_VALUE)
    {
    return false;
    // может АЦП занят кем-то другим?
    }

    // прочитаем название модуля в обнаруженном виртуальном слоте
	if(!pModule->GetModuleName(ModuleName))
    {
    return false;
    }

    // проверим, что это 'E14-440'
	if(strcmp(ModuleName, "E440"))
    {
    return false;
    // главное чтобы это был нужный АЦП:)
    }

    // попробуем получить скорость работы шины USB
	if(!pModule->GetUsbSpeed(&UsbSpeed))
    {
    Error(" GetUsbSpeed() --> Bad\n");
    return false;
    }

	// код LBIOS'а возьмём из соответствующего ресурса штатной DLL библиотеки
	if(!pModule->LOAD_MODULE())
    {
    Error(" LOAD_MODULE() --> Bad\n");
    return false;
    }


	// проверим загрузку модуля
 	if(!pModule->TEST_MODULE())
    {
    Error(" TEST_MODULE() --> Bad\n");
    return false;
    }

	// получим информацию из ППЗУ модуля
	if(!pModule->GET_MODULE_DESCRIPTION(&ModuleDescription))
    {
    Error(" GET_MODULE_DESCRIPTION() --> Bad\n");
    return false;
    }

	// получим текущие параметры работы АЦП
	if(!pModule->GET_ADC_PARS(&ap))
    {
    Error(" GET_ADC_PARS() --> Bad\n");
    return false;
    }

    return true;
}

bool LCardADC::IsInitSuccessfull()
{
    return successfullInit;
}

// настройки по умолчанию.
// 400 кГЦ, без межкадровой задержки, 4 канала, фирменные калибровочные коэффициенты.
bool LCardADC::SettingADCParams()
{
    return SettingADCParams(4, 400.0);
}

bool LCardADC::SettingADCParams(unsigned short channelsQuantity, double frenquency)
{
    // установим желаемые параметры работы АЦП
	ap.IsCorrectionEnabled = TRUE;			// разрешим корректировку данных на уровне драйвера DSP
	ap.InputMode = NO_SYNC_E440;			// обычный сбор данных безо всякой синхронизации ввода
	ap.ChannelsQuantity = channelsQuantity;	// количество активных каналов
	// формируем управляющую таблицу
	for(int i = 0x0; i < ap.ChannelsQuantity; i++)
        {
        unsigned short temp=
        ap.ControlTable[i] =  // макс/мин +- 10Вольт, дифференциальный режим.
        (WORD)(i | (ADC_INPUT_RANGE_10000mV_E440 << 0x6));
        temp=temp;
        }
	ap.AdcRate = frenquency;					// частота работы АЦП в кГц
	ap.InterKadrDelay = 0.0;					// межкадровая задержка в мс
	ap.AdcFifoBaseAddress = 0x0;			  	// базовый адрес FIFO буфера АЦП в DSP модуля
	ap.AdcFifoLength = MAX_ADC_FIFO_SIZE_E440;	// длина FIFO буфера АЦП в DSP модуля
	// будем использовать фирменные калибровочные коэффициенты, которые храняться в ППЗУ модуля
	for(int i = 0x0; i < ADC_CALIBR_COEFS_QUANTITY_E440; i++)
	{
		ap.AdcOffsetCoefs[i] =  ModuleDescription.Adc.OffsetCalibration[i];
            //double temp=ap.AdcOffsetCoefs[i];
		ap.AdcScaleCoefs[i] =  ModuleDescription.Adc.ScaleCalibration[i];
        //temp=ap.AdcScaleCoefs[i];
        //temp=temp;
	}
    ap.AdcScaleCoefs[0]=1.0073960381;
    ap.AdcOffsetCoefs[0]=0.5;

	// передадим требуемые параметры работы АЦП в модуль
	if(!pModule->SET_ADC_PARS(&ap))
    {
        Error(" SET_ADC_PARS() --> Bad\n");
        return false;
    }
    return true;
}

void LCardADC::StopMeasurement()
{
    needToStop=true;
    WaitForSingleObject(hReadThread, INFINITE);
    convertToVolt();
}

void LCardADC::StartMeasurement()
{
    needToStop=false;
    // Создаём и запускаем поток сбора данных
    hReadThread = CreateThread(0, 0x2000, ServiceReadThread, 0, 0, &ReadTid);
	if(!hReadThread)
    {
    Error(" ServiceReadThread() --> Bad\n");
    return;
    }

    // цикл записи получаемых данных и ожидания окончания работы приложения
    /*
	while(!IsReadThreadComplete)
	{
		if(OldCounter != Counter)
        {
            OldCounter = Counter;
        }
		else Sleep(20);
	} */

	// ждём окончания работы потока ввода данных
	//WaitForSingleObject(hReadThread, INFINITE);

	// проверим была ли ошибка выполнения потока сбора данных
}

std::string LCardADC::Error(std::string s)
{
    return s;
}

LCardADC::~LCardADC()
{
    delete[] ReadBuffer;
    // завершение работы.
    // подчищаем интерфейс модуля
	if(pModule)
	{
		// освободим интерфейс модуля
		if(!pModule->ReleaseLInstance())
        {
        //printf(" ReleaseLInstance() --> Bad\n");
        }
		else
        {
        //printf(" ReleaseLInstance() --> OK\n");
        }
		// обнулим указатель на интерфейс модуля
		pModule = NULL;
	}
}

//------------------------------------------------------------------------
// Поток, в котором осуществляется сбор данных
//------------------------------------------------------------------------

unsigned long __stdcall ServiceReadThread(PVOID /*Context*/)
{
    adc->ServiceReadThreadReal();
    return 0;
}

unsigned long __stdcall LCardADC::ServiceReadThreadReal()
{
    std::vector<long double> tempData;

	WORD i;
	WORD RequestNumber;
	DWORD FileBytesWritten;
	// массив OVERLAPPED структур из двух элементов
	OVERLAPPED ReadOv[2];
	// массив структур с параметрами запроса на ввод/вывод данных
	IO_REQUEST_LUSBAPI IoReq[2];

	// остановим работу АЦП и одновременно сбросим USB-канал чтения данных
	if(!pModule->STOP_ADC())
        {
            ReadThreadErrorNumber = 0x1;
            IsReadThreadComplete = true;
            return 0x0;
        }

	// формируем необходимые для сбора данных структуры
	for(i = 0x0; i < 0x2; i++)
	{
		// инициализация структуры типа OVERLAPPED
		ZeroMemory(&ReadOv[i], sizeof(OVERLAPPED));
		// создаём событие для асинхронного запроса
		ReadOv[i].hEvent = CreateEvent(NULL, FALSE , FALSE, NULL);
		// формируем структуру IoReq
		IoReq[i].Buffer = ReadBuffer + i*DataStep;
		IoReq[i].NumberOfWordsToPass = DataStep;
		IoReq[i].NumberOfWordsPassed = 0x0;
		IoReq[i].Overlapped = &ReadOv[i];
		IoReq[i].TimeOut = DataStep/ap.AdcRate + 1000;
	}

	// делаем предварительный запрос на ввод данных
	RequestNumber = 0x0;
	if(!pModule->ReadData(&IoReq[RequestNumber]))
    {
        CloseHandle(ReadOv[0].hEvent);
        CloseHandle(ReadOv[1].hEvent);
        ReadThreadErrorNumber = 0x2;
        IsReadThreadComplete = true;
        return 0x0;
    }

	// запустим АЦП
	if(pModule->START_ADC())
	{
		// цикл сбора данных
        while(!needToStop) // пока мы не попросим
		{
			// сделаем запрос на очередную порцию данных
			RequestNumber ^= 0x1;
			if(!pModule->ReadData(&IoReq[RequestNumber]))
            {
                ReadThreadErrorNumber = 0x2;
                break;
            }
			if(ReadThreadErrorNumber)
                break;

			// ждём завершения операции сбора предыдущей порции данных
			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT)
            {
                ReadThreadErrorNumber = 0x3;
                break;
            }
			if(ReadThreadErrorNumber)
                break;

			// запишем полученную порцию данных в вектор
            for(int i=0;i<DataStep;++i)
            {
                tempData.push_back(IoReq[RequestNumber^0x1].Buffer[i]);
            }
            if(isMedianFilterEnabled)
            {
                ReadData.push_back(medianFilter(tempData));
            }
            else
            {
                for(int i=0;i<DataStep;++i)
                {
                    ReadData.push_back(tempData[i]);
                }

            }
            tempData.clear();
            // надо выяснить в каком виде этот буфер хранит данные.
            // ну и как вставить весь буфер в вектор:)
            Sleep(20);
            ++Counter;


        	if(ReadThreadErrorNumber) break;
            Application->ProcessMessages();
        }
		// последняя порция данных
		if(!ReadThreadErrorNumber)
		{
			RequestNumber ^= 0x1;
			// ждём окончания операции сбора последней порции данных
			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT)
                ReadThreadErrorNumber = 0x3;

            // запишем полученную порцию данных в вектор
            for(int i=0;i<DataStep;++i)
            {
                tempData.push_back(IoReq[RequestNumber^0x1].Buffer[i]);
            }
			Counter++;
		}
	}
	

	// остановим работу АЦП
	if(!pModule->STOP_ADC()) ReadThreadErrorNumber = 0x1;
	// прервём возможно незавершённый асинхронный запрос на приём данных
	if(!CancelIo(ModuleHandle)) { ReadThreadErrorNumber = 0x7; }
	// освободим все идентификаторы событий
	for(i = 0x0; i < 0x2; i++) CloseHandle(ReadOv[i].hEvent);
	// небольшая задержка
	Sleep(100);
	// установим флажок завершения работы потока сбора данных
	IsReadThreadComplete = true;
	// теперь можно спокойно выходить из потока
	return 0x0;
}


std::vector<MyDataType> const &  LCardADC::getData()
{
    return ReadData;
}

void LCardADC::convertToVolt()
{
    // это работает только если пределы +- 10Вольт.
    std::vector<MyDataType>::iterator pos;
    for(pos=ReadData.begin();pos!=ReadData.end();++pos)
    *pos/=800.0;
}

void LCardADC::clearBuffer()
{
    ReadData.clear();

}






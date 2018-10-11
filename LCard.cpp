
#include "LCard.h"
extern LCardADC *adc;

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);

//------------------------------------------------------------------
LCardADC::LCardADC(double frenquency,int blockSize, TLabel * l1, TLabel * l2, TLabel * l3,
    channelsInfo cI, std::vector<std::pair<MyDataType, MyDataType> > &correcionCoeficients)
{
    ADCMaxValue=8000.0;
    
    ChannelLabels.push_back(l1);
    ChannelLabels.push_back(l2);
    ChannelLabels.push_back(l3);
    counter=0;
    chanInfo=cI;
    isWriting= false;
    isDataNeeded=false;
    TestingMode=false;
    medianFilterLength=32;
    isMeasurementRunning=false;
    B=0;
    HallSeries=0;  // указатель на график
    MagnetoResistanceSeries=0;
    isMedianFilterEnabled=true;// включение медианной фильтрации кадров.
    needToStop=true;// флаг для остановки второго потока
    successfullInit=false; // флаг успешной инициализации
    ReadThreadErrorNumber=0;// переменная с кодом ошибки инициализации.
    //DataStep =64*4*cI.size(); // 256*8 даёт 350 точек. достаточно быстро. Хотя точек возможно маловато.  // кол-во отсчетов, кратное 32
    // оно явно должно зависеть от количества измеряемых каналов и частоты.
    ReadBuffer=NULL;
    lowBandFilter=new FilterLowBand(256,400000,20,55);

    //ReadBuffer=new SHORT[2*DataStep]; // в этот буфер считываются данные.

    // нужна линейная коррекция данных вида U = k*Uацп+B для разных дифференциальных каналов.
    channelCorrectionKoefficients = correcionCoeficients;


    if(DriverInit()) // инициализиуем драйвер
    {
        if(SettingADCParams(frenquency,blockSize,cI,channelCorrectionKoefficients))// устанавливаем настройки
        {
            successfullInit=true; // всё прошло успешно.
        }
    }
}
//------------------------------------------------------------------
/*
--------------------------Работа с драйвером-------------------------
*/
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
//------------------------------------------------------------------
bool LCardADC::IsInitSuccessfull()
{
    return successfullInit;
}
//------------------------------------------------------------------
bool LCardADC::SettingADCParams(double frenquency,int newBlockSize, channelsInfo & chanInfo, std::vector<std::pair<MyDataType, MyDataType> > &correctionKoefficients)
{
    // установим желаемые параметры работы АЦП
	ap.IsCorrectionEnabled = TRUE;			// разрешим корректировку данных на уровне драйвера DSP
	ap.InputMode = NO_SYNC_E440;			// обычный сбор данных безо всякой синхронизации ввода
	ap.ChannelsQuantity = chanInfo.size();	// количество активных каналов
	// формируем управляющую таблицу

    this->chanInfo=chanInfo;
    channelsInfo::iterator pos;
    int i;
    for(i=0,pos=chanInfo.begin();pos!=chanInfo.end();++pos,++i)
    { 
        ap.ControlTable[i]=(WORD)(pos->first | (pos->second << 0x6));
    }

    // нужна линейная коррекция данных вида U = k*Uацп+B для разных дифференциальных каналов.
    channelCorrectionKoefficients = correctionKoefficients;

    DataStep=newBlockSize*chanInfo.size();
    if(ReadBuffer)
        delete[] ReadBuffer;

    ReadBuffer=new SHORT[2*DataStep]; // в этот буфер считываются данные.

        
	ap.AdcRate = frenquency;					// частота работы АЦП в кГц
	ap.InterKadrDelay = 0.0;					// межкадровая задержка в мс
	ap.AdcFifoBaseAddress = 0x0;			  	// базовый адрес FIFO буфера АЦП в DSP модуля
	ap.AdcFifoLength = MAX_ADC_FIFO_SIZE_E440;	// длина FIFO буфера АЦП в DSP модуля
	// будем использовать фирменные калибровочные коэффициенты, которые храняться в ППЗУ модуля
	for(int i = 0x0; i < ADC_CALIBR_COEFS_QUANTITY_E440; ++i)
	{
		ap.AdcOffsetCoefs[i] =  ModuleDescription.Adc.OffsetCalibration[i];
            //double temp=ap.AdcOffsetCoefs[i];
		ap.AdcScaleCoefs[i] =  ModuleDescription.Adc.ScaleCalibration[i];
        //temp=ap.AdcScaleCoefs[i];
        //temp=temp;
	}
    // наши калибровочные коэффициенты.
    // на всякий случай - их нужно будет проверить ещё раз, возможно их придется уточнить.
    // т.к. они определялись без медианной фильтрации.
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
//------------------------------------------------------------------
bool LCardADC::StopMeasurement()
{
    needToStop=true;
    WaitForSingleObject(hReadThread, INFINITE);
    isMeasurementRunning=false;
    return true;
}
//------------------------------------------------------------------
bool LCardADC::StartMeasurement()
{
    if(successfullInit)
    {
        needToStop=false;
        ReadData.resize(ap.ChannelsQuantity);

        clearBuffer();
        // Создаём и запускаем поток сбора данных
        hReadThread = CreateThread(0, 0x2000, ServiceReadThread, 0, 0, &ReadTid);
        if(!hReadThread)
        {
        Error(" ServiceReadThread() --> Bad\n");

        }
        isMeasurementRunning=true;
        if(HallSeries) HallSeries->Clear();
        if(MagnetoResistanceSeries) MagnetoResistanceSeries->Clear();
        return true;
    } 
    return false;   
}
//------------------------------------------------------------------
std::string LCardADC::Error(std::string s)
{
    return s;
}
//------------------------------------------------------------------
LCardADC::~LCardADC()
{
    if(ReadBuffer)
    delete[] ReadBuffer;
    if(lowBandFilter)
    delete lowBandFilter;

    
    if(!(splittedData.empty() && ReadData.empty()))
    {
    for(int i=0;i<ap.ChannelsQuantity;++i)
    {
        splittedData[i].clear();
        ReadData[i].clear();
    
    }

    ReadData.clear();
    splittedData.clear();

    }

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
/*

Не обошлось без костылей.
Вот например здесь несколько функций в режиме реального времени строят графики на форме.
С прямым доступом к самим графикам.

По хорошему надо будет:

Запрашивать данные по таймеру с формы.
Тут функции получения текущих значений встроить.
А это суровое нагромождение убрать.

*/
//------------------------------------------------------------------
void LCardADC::setBSeries(TLineSeries *s)
{
    B=s;
}
//------------------------------------------------------------------
void LCardADC::setHallSeries(TLineSeries *s)
{
    HallSeries=s;    
}
//------------------------------------------------------------------
void LCardADC::setMagnetoResistanceSeries(TLineSeries *s)
{
    MagnetoResistanceSeries=s;
}
//------------------------------------------------------------------
void LCardADC::InteractivePlottingDataOne()
{
    //long double bigNumber=1E20;
    // если определены графики - делаем вывод.

    if(B)
    {
        B->Clear();
        for (unsigned int i = 0; i < DequeBuffer[2].size(); ++i)
        {
            B->AddY(DequeBuffer[2].back(),"",clBlue);
        }
    }        
    if(HallSeries)
    {
        HallSeries->Clear();
        for (unsigned int i = 0; i < DequeBuffer[0].size(); ++i)
        {
            HallSeries->AddY(DequeBuffer[0].back(),"",clBlue);
        }   
    }
    if(MagnetoResistanceSeries)
    {
        MagnetoResistanceSeries->Clear();
        for (unsigned int i = 0; i < DequeBuffer[1].size(); ++i)
        {
            MagnetoResistanceSeries->AddY(DequeBuffer[1].back(),"",clBlue);
        }
    }
}
//-----------------------------------------------------------------
void LCardADC::InteractivePlottingData()
{
    long double bigNumber=1E20;
    // если определены графики - делаем вывод.
    if(HallSeries)
        if (ReadData[0].back()<bigNumber || ReadData[2].back()<bigNumber)
            HallSeries->AddXY(10*ReadData[2].back(),ReadData[0].back(),"",clBlue);
    if(MagnetoResistanceSeries)
        if (ReadData[1].back()<bigNumber || ReadData[2].back()<bigNumber)
            MagnetoResistanceSeries->AddXY(10*ReadData[2].back(),ReadData[1].back(),"",clBlue);
}
//------------------------------------------------------------------
void LCardADC::DisplayOnForm(int channelN, MyDataType value)
{
    if(counter%5==0)
    {
    ChannelLabels[channelN]->Caption=FloatToStrF(value,ffFixed,5,5);
    counter=0;
    }
    ++counter;
}
//------------------------------------------------------------------

/*
Вывод заканчивается. Начинается обработка полученных данных.
Ну и само получение данных.
*/

void LCardADC::realTimeFilter(TSignal & inData,
TSignal & outData)
{
    TSignal tempOutData;
    tempOutData.resize(inData.size());
    //outData.resize(inData.size());
    Filter (inData, tempOutData, 20, 1000, 30, 35);
    outData=tempOutData;

}
//------------------------------------------------------------------
// сохранение данных из буфера
// применяет медианный фильтр
void LCardADC::writeDataToVector(TSignal & tempData)
{

    splitToChannels(tempData,splittedData); // разделяем данные по каналам.

    if(isMedianFilterEnabled)
    { // применяем медианный фильтр, преобразование в вольты и добавляем к уже собранным данным.


        for(int i=0;i<ap.ChannelsQuantity;++i)
        {  // ахтунг!
           // realTimeFilter( splittedData[i], splittedData[i]);
           long double t =convertToVolt(medianFilter(splittedData[i]),i);
            ReadData[i].push_back(t);
            DisplayOnForm(i,ReadData[i].back());
        }  
        /*
        // Попытка внедрения медианного фильтра с длиной, не зависящей от величины блока.
        for(int i=0;i<ap.ChannelsQuantity;++i)
        {
            std::vector<long double> res;
            medianFilter(splittedData[i],res,medianFilterLength);
            for(unsigned int j=0;j<res.size();++j)
            {
                ReadData[i].push_back(convertToVolt(res[j],i));
            }

          // long double t =convertToVolt(medianFilter(splittedData[i]),i);
          //  ReadData[i].push_back(t);
            DisplayOnForm(i,ReadData[i].back());
        }   */
    }
    /*
    режим без медианной фильтрации.
    Ест много-много памяти.
    Для использования в реальности надо организовать запись данных в файл,
    а потом постепенно всё это обрабатывать, т.к. хранить несколько миллионов значений
    400 000 * 120 =  48 000 000 для каждого канала, т.е. всего 144 000 000
    на нашем не очень современном компе - затратно.
    Ибо памяти надо будет 1 440 000 000 байт = 1.5 Гбайта.
    А потом всякие фильтрации, экстраполяции и т.д. и понадобится уже ~8Гб
    */
    else
    {   for(int i=0;i<ap.ChannelsQuantity;++i)
            {
                for(unsigned int j=0;j<splittedData[i].size();++j)
                {
                    ReadData[i].push_back(convertToVolt(splittedData[i][j],i));
                    // снова грешим, отображая каждое тысячное значение.
                    if(j%1000==0)
                    DisplayOnForm(i,ReadData[i].back());
                }
            }
        
    }

    /*
    Теперь важный момент. Если у нас включена запись данных (и вывод на графики,
    соответственно.
    */
    if(isWriting)
        InteractivePlottingData();
    // чистим лишнее
    tempData.clear();
    for(int i=0;i<ap.ChannelsQuantity;++i)
    {
        splittedData[i].clear();
    }
    // если запись выключена и данные не нужны - удаляем и их.
    if(!isWriting && !isDataNeeded)
    {
    for(int i=0;i<ap.ChannelsQuantity;++i)
                {
        ReadData[i].clear();
                  }
    }
}

//------------------------------------------------------------------
unsigned long __stdcall LCardADC::ServiceReadThreadReal()
{
    TSignal tempData;

	WORD i;
	WORD RequestNumber;
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
            for(unsigned int i=0;i<DataStep;++i)
            {
                tempData.push_back(IoReq[RequestNumber^0x1].Buffer[i]);
            }
            writeDataToVector(tempData);
            // данные идут вот так: значение 1 канала, значение 2 канала, значение 3 канала, значение 1 канала...
            Sleep(1);
           
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
            for(unsigned int i=0;i<DataStep;++i)
            {
                tempData.push_back(IoReq[RequestNumber^0x1].Buffer[i]);
            }
            writeDataToVector(tempData);
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
//------------------------------------------------------------------
std::vector<TSignal > *  LCardADC::getSplittedData(int a)
{
    return &ReadData;
}
std::vector<TSignal > const &  LCardADC::getSplittedData()
{
    return ReadData;
}
//------------------------------------------------------------------
MyDataType LCardADC::convertToVolt(MyDataType in,int channel)
{
/*
Появился некоторый вопрос. Минимальный значащий разряд, а как его считать?

Вариант 1. Есть маскимальное число отсчетов 8000, есть максимальное значение напряжения
для какого-либо режима (пусть будет 10)
тогда МЗР = 8000 / 10 = 800

Вариант 2. Число разных значений отсчетов у нас 16001.
тогда диапазон в вольтах (20) раскладывается на 16001 значение и одно равно 800,05

Разница не велика, но думаю принципиально посчитать всё правильно:).

Судя по гуглению вернее будет именно второй вариант. Но это надо обсудить.

*/

/*
    MyDataType koef=1;
    switch(chanInfo[channel].second)
    {
        case 0: // +- 10V
            koef=(2*ADCMaxValue+1)/2/10.0;
            break;
        case 1: // +- 2.5V
            koef=(2*ADCMaxValue+1)/2/2.5;
            break;
        case 2: // 0.625V
            koef=(2*ADCMaxValue+1)/2/0.625;
            break;
        case 3: // 0.156V
            koef=(2*ADCMaxValue+1)/2/0.156;
            break;
        default:
            break;
    }
    return in/koef;
*/

    MyDataType koef=1;
    switch(chanInfo[channel].second)
    {
        case 0: // +- 10V
            koef=ADCMaxValue/10.0;
            break;
        case 1: // +- 2.5V
            koef=ADCMaxValue/2.5;
            break;
        case 2: // 0.625V
            koef=ADCMaxValue/0.625;
            break;
        case 3: // 0.156V
            koef=ADCMaxValue/0.156;
            break;
        default:
            break;
    }

    MyDataType res = in/koef;

    return channelCorrectionKoefficients[channel].first*res+channelCorrectionKoefficients[channel].second;
}
//------------------------------------------------------------------
void LCardADC::convertToVolt()
{
    if(ReadData.size()==0)
        return;
    
    TSignal::iterator pos;
    for(int i=0;i<ap.ChannelsQuantity;++i)
    for(pos=ReadData[i].begin();pos!=ReadData[i].end();++pos)
    *pos=convertToVolt(*pos,i);
}

//------------------------------------------------------------------
void LCardADC::clearBuffer()
{
    ReadData.clear();
    DequeBuffer.clear();
    splittedData.clear();
}


//------------------------------------------------------------------

void LCardADC::splitToChannels(TSignal &tempData,
std::vector<TSignal > &splittedData)
{
    // Выделяем места сколько надо.
    splittedData.resize(ap.ChannelsQuantity);
    // Количество точек может не поделиться на количетсво каналов нацело
    // Так как мы могли прервать сбор данных в самый неожиданный момент.
    unsigned int bound=(tempData.size()/(unsigned int)ap.ChannelsQuantity)*ap.ChannelsQuantity;
    for(unsigned int i=0;i<bound;)
    {
        for(int channel=0;channel<ap.ChannelsQuantity && i<bound;++channel,++i)
            splittedData[channel].push_back(tempData[i]);
    }
}

// ----------------------------------------------------------------------------
// Что-то тестовое, уже не помню даже зачем.
// Если в ближайшее время не пригодится - выпилить.
void LCardADC::testSetReadBuffer()
{
    int mk=300;
    short * B=new short [mk];

    B[0]=0;
    for(int i=1;i<mk;++i)
    {
        B[i]=B[i-1]+300.0/mk;
    }

    ap.ChannelsQuantity=3;
    ReadData.resize(ap.ChannelsQuantity);
    TSignal tempData;
    unsigned int tsize=5;

    short *tempBuffer=new short[tsize]; 

    for(int nK=0;nK<mk;++nK)
    {

    for(unsigned int i=1;i<tsize;i+=ap.ChannelsQuantity)
    tempBuffer[i]=8000;
    for(unsigned int i=0;i<tsize;i+=ap.ChannelsQuantity)
    tempBuffer[i]=B[nK]+1000;
    for(unsigned int i=2;i<tsize;i+=ap.ChannelsQuantity)
    tempBuffer[i]=B[nK];
    // запишем полученную порцию данных в вектор
    for(unsigned int i=0;i<tsize;++i)
    {
        tempData.push_back(tempBuffer[i]);
    }
    writeDataToVector(tempData);
    }
    delete[] tempBuffer;
    delete[] B;
}
//-----------------------------------------------------------------------------
bool LCardADC::IsMeasurementRunning()
{
    return isMeasurementRunning;    
}
//-----------------------------------------------------------------------------
void LCardADC::EnableMedianFilter()
{
    isMedianFilterEnabled=true;
}
void LCardADC::DisableMedianFilter()
{
    isMedianFilterEnabled=false;
}
//-----------------------------------------------------------------------------
void LCardADC::EnableTestingMode()
{
    TestingMode=true;
}
void LCardADC::DisableTestingMode()
{
    TestingMode=false;
}
//-----------------------------------------------------------------------------

bool LCardADC::StartWriting()
{
    if(isMeasurementRunning)
    {
        isWriting=true;
        isDataNeeded=true;
        return true;
    }
    else
    {
        return false;
    }
}
//-----------------------------------------------------------------------------
bool LCardADC::StopWriting()
{
    if(isMeasurementRunning)
    {
        isWriting=false;
        isDataNeeded=true;
        return true;
    }
    else
    {
        return false;
    }
}
//-----------------------------------------------------------------------------
bool LCardADC::isWritingEnabled()
{
    return isWriting;
}
//-----------------------------------------------------------------------------
void LCardADC::dataisntNeeded()
{
    isDataNeeded=false;
}
//-----------------------------------------------------------------------------
void LCardADC::setMedianFilterLength(size_t s)
{
    medianFilterLength=s;
}
//-----------------------------------------------------------------------------

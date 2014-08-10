
#include "LCard.h"
extern LCardADC *adc;

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);

//------------------------------------------------------------------
LCardADC::LCardADC(double frenquency,int blockSize, TLabel * l1, TLabel * l2, TLabel * l3,
    channelsInfo cI)
{
    ChannelLabels.push_back(l1);
    ChannelLabels.push_back(l2);
    ChannelLabels.push_back(l3);
    counter=0;
    chanInfo=cI;
    isWriting= false;
    isDataNeeded=false;
    TestingMode=false;
    isMeasurementRunning=false;
    B=0;
    HallSeries=0;  // ��������� �� ������
    MagnetoResistanceSeries=0;
    isMedianFilterEnabled=true;// ��������� ��������� ���������� ������.
    needToStop=true;// ���� ��� ��������� ������� ������
    successfullInit=false; // ���� �������� �������������
    ReadThreadErrorNumber=0;// ���������� � ����� ������ �������������.
    //DataStep =64*4*cI.size(); // 256*8 ��� 350 �����. ���������� ������. ���� ����� �������� ��������.  // ���-�� ��������, ������� 32
    // ��� ���� ������ �������� �� ���������� ���������� ������� � �������.
    ReadBuffer=NULL;
    lowBandFilter=new FilterLowBand(256,400000,20,55);

    //ReadBuffer=new SHORT[2*DataStep]; // � ���� ����� ����������� ������.

    if(DriverInit()) // ������������� �������
    {
        if(SettingADCParams(frenquency,blockSize,cI))// ������������� ���������
        {
            successfullInit=true; // �� ������ �������.
        }
    }

}
//------------------------------------------------------------------
bool LCardADC::DriverInit()
{
    if((DllVersion = GetDllVersion()) != CURRENT_VERSION_LUSBAPI)
	{
    // ���� �������� � ������ ��������� ��������.
        return false;
    }

    // ��������� �������� ��������� �� ���������
	pModule = static_cast<ILE440 *>(CreateLInstance("e440"));
	if(!pModule)
    {
        return false;
    }
    int i;
    // ��������� ���������� ������ E14-440 � ������ MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI ����������� ������
	for(i = 0x0; i < MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI; i++) if(pModule->OpenLDevice(i)) break;
	// ���-������ ����������?
	if(i == MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI)
    {
        return false;
    // ��������� ��������� �� ���
    }

    // ��������� ��������� ���������� ����������
	ModuleHandle = pModule->GetModuleHandle();
	if(ModuleHandle == INVALID_HANDLE_VALUE)
    {
        return false;
    // ����� ��� ����� ���-�� ������?
    }

    // ��������� �������� ������ � ������������ ����������� �����
	if(!pModule->GetModuleName(ModuleName))
    {
        return false;
    }

    // ��������, ��� ��� 'E14-440'
	if(strcmp(ModuleName, "E440"))
    {
        return false;
    // ������� ����� ��� ��� ������ ���:)
    }

    // ��������� �������� �������� ������ ���� USB
	if(!pModule->GetUsbSpeed(&UsbSpeed))
    {
        Error(" GetUsbSpeed() --> Bad\n");
        return false;
    }

	// ��� LBIOS'� ������ �� ���������������� ������� ������� DLL ����������
	if(!pModule->LOAD_MODULE())
    {
        Error(" LOAD_MODULE() --> Bad\n");
        return false;
    }


	// �������� �������� ������
 	if(!pModule->TEST_MODULE())
    {
        Error(" TEST_MODULE() --> Bad\n");
        return false;
    }

	// ������� ���������� �� ���� ������
	if(!pModule->GET_MODULE_DESCRIPTION(&ModuleDescription))
    {
        Error(" GET_MODULE_DESCRIPTION() --> Bad\n");
        return false;
    }

	// ������� ������� ��������� ������ ���
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
bool LCardADC::SettingADCParams(double frenquency,int newBlockSize, channelsInfo & chanInfo)
{
    // ��������� �������� ��������� ������ ���
	ap.IsCorrectionEnabled = TRUE;			// �������� ������������� ������ �� ������ �������� DSP
	ap.InputMode = NO_SYNC_E440;			// ������� ���� ������ ���� ������ ������������� �����
	ap.ChannelsQuantity = chanInfo.size();	// ���������� �������� �������
	// ��������� ����������� �������

    this->chanInfo=chanInfo;
    channelsInfo::iterator pos;
    int i;
    for(i=0,pos=chanInfo.begin();pos!=chanInfo.end();++pos,++i)
    { 
        ap.ControlTable[i]=(WORD)(i | (pos->second << 0x6));
    }

    DataStep=newBlockSize*chanInfo.size();
    if(ReadBuffer)
        delete[] ReadBuffer;

    ReadBuffer=new SHORT[2*DataStep]; // � ���� ����� ����������� ������.

        
	ap.AdcRate = frenquency;					// ������� ������ ��� � ���
	ap.InterKadrDelay = 0.0;					// ����������� �������� � ��
	ap.AdcFifoBaseAddress = 0x0;			  	// ������� ����� FIFO ������ ��� � DSP ������
	ap.AdcFifoLength = MAX_ADC_FIFO_SIZE_E440;	// ����� FIFO ������ ��� � DSP ������
	// ����� ������������ ��������� ������������� ������������, ������� ��������� � ���� ������
	for(int i = 0x0; i < ADC_CALIBR_COEFS_QUANTITY_E440; ++i)
	{
		ap.AdcOffsetCoefs[i] =  ModuleDescription.Adc.OffsetCalibration[i];
            //double temp=ap.AdcOffsetCoefs[i];
		ap.AdcScaleCoefs[i] =  ModuleDescription.Adc.ScaleCalibration[i];
        //temp=ap.AdcScaleCoefs[i];
        //temp=temp;
	}
    // ���� ������������� ������������.
    // �� ������ ������ - �� ����� ����� ��������� ��� ���, �������� �� �������� ��������.
    // �.�. ��� ������������ ��� ��������� ����������.
    ap.AdcScaleCoefs[0]=1.0073960381;
    ap.AdcOffsetCoefs[0]=0.5;

	// ��������� ��������� ��������� ������ ��� � ������
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
        // ������ � ��������� ����� ����� ������
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

    

    for(int i=0;i<ap.ChannelsQuantity;++i)
    {
        splittedData[i].clear();
        ReadData[i].clear();
    
    }

    ReadData.clear();
    splittedData.clear();

    // ���������� ������.
    // ��������� ��������� ������
	if(pModule)
	{
		// ��������� ��������� ������
		if(!pModule->ReleaseLInstance())
        {
        //printf(" ReleaseLInstance() --> Bad\n");
        }
		else
        {
        //printf(" ReleaseLInstance() --> OK\n");
        }
		// ������� ��������� �� ��������� ������
		pModule = NULL;
	}
}

//------------------------------------------------------------------------
// �����, � ������� �������������� ���� ������
//------------------------------------------------------------------------

unsigned long __stdcall ServiceReadThread(PVOID /*Context*/)
{
    adc->ServiceReadThreadReal();
    return 0;
}
//------------------------------------------------------------------
void LCardADC::InteractivePlottingDataOne()
{
    //long double bigNumber=1E20;
    // ���� ���������� ������� - ������ �����.

    if(B)
    {
        B->Clear();
        for (unsigned int i = 0; i < DequeBuffer[chanInfo[2].first].size(); ++i)
        {
            B->AddY(DequeBuffer[chanInfo[2].first].back(),"",clBlue);
        }
    }        
    if(HallSeries)
    {
        HallSeries->Clear();
        for (unsigned int i = 0; i < DequeBuffer[chanInfo[0].first].size(); ++i)
        {
            HallSeries->AddY(DequeBuffer[chanInfo[0].first].back(),"",clBlue);
        }   
    }
    if(MagnetoResistanceSeries)
    {
        MagnetoResistanceSeries->Clear();
        for (unsigned int i = 0; i < DequeBuffer[chanInfo[1].first].size(); ++i)
        {
            MagnetoResistanceSeries->AddY(DequeBuffer[chanInfo[1].first].back(),"",clBlue);
        }
    }
}
//-----------------------------------------------------------------
void LCardADC::InteractivePlottingData()
{
    long double bigNumber=1E20;
    // ���� ���������� ������� - ������ �����.
    if(HallSeries)
        if (ReadData[chanInfo[0].first].back()<bigNumber || ReadData[chanInfo[2].first].back()<bigNumber)
            HallSeries->AddXY(10*ReadData[chanInfo[2].first].back(),ReadData[chanInfo[0].first].back(),"",clBlue);
    if(MagnetoResistanceSeries)
        if (ReadData[chanInfo[1].first].back()<bigNumber || ReadData[chanInfo[2].first].back()<bigNumber)
            MagnetoResistanceSeries->AddXY(10*ReadData[chanInfo[2].first].back(),ReadData[chanInfo[1].first].back(),"",clBlue);
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
void LCardADC::realTimeFilter(DataTypeInContainer & inData,
DataTypeInContainer & outData)
{
    DataTypeInContainer tempOutData;
    tempOutData.resize(inData.size());
    //outData.resize(inData.size());
    Filter (inData, tempOutData, 20, 1000, 30, 35);
    outData=tempOutData;

}
//------------------------------------------------------------------
// ���������� ������ �� ������
// ��������� ��������� ������
void LCardADC::writeDataToVector(DataTypeInContainer & tempData)
{

    splitToChannels(tempData,splittedData); // ��������� ������ �� �������.

    if(isMedianFilterEnabled)
    { // ��������� ��������� ������, �������������� � ������ � ��������� � ��� ��������� ������.


        for(int i=0;i<ap.ChannelsQuantity;++i)
        {  // ������!
           // realTimeFilter( splittedData[i], splittedData[i]);
           long double t =convertToVolt(medianFilter(splittedData[i]),i);
            ReadData[i].push_back(t);
            DisplayOnForm(i,ReadData[i].back());
        }
    }
    else
    {   for(int i=0;i<ap.ChannelsQuantity;++i)
            {
                for(unsigned int j=0;j<splittedData[i].size();++j)
                {
                    ReadData[i].push_back(convertToVolt(splittedData[i][j],i));
                    if(j%1000==0)
                    DisplayOnForm(i,ReadData[i].back());
                }
            }
        
    }
    
    if(isWriting)
        InteractivePlottingData();
    tempData.clear();
    for(int i=0;i<ap.ChannelsQuantity;++i)
    {
        splittedData[i].clear();
    }

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
    DataTypeInContainer tempData;

	WORD i;
	WORD RequestNumber;
	// ������ OVERLAPPED �������� �� ���� ���������
	OVERLAPPED ReadOv[2];
	// ������ �������� � ����������� ������� �� ����/����� ������
	IO_REQUEST_LUSBAPI IoReq[2];

	// ��������� ������ ��� � ������������ ������� USB-����� ������ ������
	if(!pModule->STOP_ADC())
        {
            ReadThreadErrorNumber = 0x1;
            IsReadThreadComplete = true;
            return 0x0;
        }

	// ��������� ����������� ��� ����� ������ ���������
	for(i = 0x0; i < 0x2; i++)
	{
		// ������������� ��������� ���� OVERLAPPED
		ZeroMemory(&ReadOv[i], sizeof(OVERLAPPED));
		// ������ ������� ��� ������������ �������
		ReadOv[i].hEvent = CreateEvent(NULL, FALSE , FALSE, NULL);
		// ��������� ��������� IoReq
		IoReq[i].Buffer = ReadBuffer + i*DataStep;
		IoReq[i].NumberOfWordsToPass = DataStep;
		IoReq[i].NumberOfWordsPassed = 0x0;
		IoReq[i].Overlapped = &ReadOv[i];
		IoReq[i].TimeOut = DataStep/ap.AdcRate + 1000;
	}

	// ������ ��������������� ������ �� ���� ������
	RequestNumber = 0x0;
	if(!pModule->ReadData(&IoReq[RequestNumber]))
    {
        CloseHandle(ReadOv[0].hEvent);
        CloseHandle(ReadOv[1].hEvent);
        ReadThreadErrorNumber = 0x2;
        IsReadThreadComplete = true;
        return 0x0;
    }

	// �������� ���
	if(pModule->START_ADC())
	{
		// ���� ����� ������
        while(!needToStop) // ���� �� �� ��������
		{
			// ������� ������ �� ��������� ������ ������
			RequestNumber ^= 0x1;
			if(!pModule->ReadData(&IoReq[RequestNumber]))
            {
                ReadThreadErrorNumber = 0x2;
                break;
            }
			if(ReadThreadErrorNumber)
                break;

			// ��� ���������� �������� ����� ���������� ������ ������
			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT)
            {
                ReadThreadErrorNumber = 0x3;
                break;
            }
			if(ReadThreadErrorNumber)
                break;

			// ������� ���������� ������ ������ � ������
            for(unsigned int i=0;i<DataStep;++i)
            {
                tempData.push_back(IoReq[RequestNumber^0x1].Buffer[i]);
            }
            writeDataToVector(tempData);
            // ���� �������� � ����� ���� ���� ����� ������ ������.
            // ������ ���� ��� ���: �������� 1 ������, �������� 2 ������, �������� 3 ������, �������� 1 ������...
            Sleep(20);
           


        	if(ReadThreadErrorNumber) break;
            Application->ProcessMessages();
        }
		// ��������� ������ ������
		if(!ReadThreadErrorNumber)
		{
			RequestNumber ^= 0x1;
			// ��� ��������� �������� ����� ��������� ������ ������
			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT)
                ReadThreadErrorNumber = 0x3;

            // ������� ���������� ������ ������ � ������
            for(unsigned int i=0;i<DataStep;++i)
            {
                tempData.push_back(IoReq[RequestNumber^0x1].Buffer[i]);
            }
            writeDataToVector(tempData);
		}
	}
	

	// ��������� ������ ���
	if(!pModule->STOP_ADC()) ReadThreadErrorNumber = 0x1;
	// ������ �������� ������������� ����������� ������ �� ���� ������
	if(!CancelIo(ModuleHandle)) { ReadThreadErrorNumber = 0x7; }
	// ��������� ��� �������������� �������
	for(i = 0x0; i < 0x2; i++) CloseHandle(ReadOv[i].hEvent);
	// ��������� ��������
	Sleep(100);
	// ��������� ������ ���������� ������ ������ ����� ������
	IsReadThreadComplete = true;
	// ������ ����� �������� �������� �� ������
	return 0x0;
}
//------------------------------------------------------------------
std::vector<DataTypeInContainer > *  LCardADC::getSplittedData(int a)
{
    return &ReadData;
}
std::vector<DataTypeInContainer > const &  LCardADC::getSplittedData()
{
    return ReadData;
}
//------------------------------------------------------------------
MyDataType LCardADC::convertToVolt(MyDataType in,int channel)
{
    MyDataType koef=1;
    switch(chanInfo[channel].second)
    {
        case 0: // +- 10V
            koef=8000.0/10.0;
            break;
        case 1: // +- 2.5V
            koef=8000.0/2.5;
            break;
        case 2: // 0.625V
            koef=8000.0/0.625;
            break;
        case 3: // 0.156V
            koef=8000.0/0.156;
            break;
        default:
            break;
    }
    return in/koef;
}
//------------------------------------------------------------------
void LCardADC::convertToVolt()
{
    if(ReadData.size()==0)
        return;
    
    DataTypeInContainer::iterator pos;
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

void LCardADC::splitToChannels(DataTypeInContainer &tempData,
std::vector<DataTypeInContainer > &splittedData)
{
    splittedData.resize(ap.ChannelsQuantity);
    unsigned int bound=(tempData.size()/(unsigned int)ap.ChannelsQuantity)*ap.ChannelsQuantity;
    for(unsigned int i=0;i<bound;)
    {
        for(int channel=0;channel<ap.ChannelsQuantity && i<bound;++channel,++i)
            splittedData[channel].push_back(tempData[i]);
    }
}

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
    DataTypeInContainer tempData;
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
    // ������� ���������� ������ ������ � ������
    for(unsigned int i=0;i<tsize;++i)
    {
        tempData.push_back(tempBuffer[i]);
    }
    writeDataToVector(tempData);
    }
    delete[] tempBuffer;
    delete[] B;
}

bool LCardADC::IsMeasurementRunning()
{
    return isMeasurementRunning;    
}

void LCardADC::EnableMedianFilter()
{
    isMedianFilterEnabled=true;
}
void LCardADC::DisableMedianFilter()
{
    isMedianFilterEnabled=false;
}

void LCardADC::EnableTestingMode()
{
    TestingMode=true;
}
void LCardADC::DisableTestingMode()
{
    TestingMode=false;
}


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

bool LCardADC::isWritingEnabled()
{
    return isWriting;
}

void LCardADC::dataisntNeeded()
{
    isDataNeeded=false;
}

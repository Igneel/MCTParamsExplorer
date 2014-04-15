
#include "LCard.h"
#include "Unit1.h"

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);

//------------------------------------------------------------------
LCardADC::LCardADC(double frenquency, TLabel * l1, TLabel * l2, TLabel * l3,
    channelsInfo cI)
{
    ChannelLabels.push_back(l1);
    ChannelLabels.push_back(l2);
    ChannelLabels.push_back(l3);

    chanInfo=cI;

    TestingMode=false;
    isMeasurementRunning=false;
    B=0;
    HallSeries=0;  // ��������� �� ������
    MagnetoResistanceSeries=0;
    isMedianFilterEnabled=true;// ��������� ��������� ���������� ������.
    needToStop=true;// ���� ��� ��������� ������� ������
    successfullInit=false; // ���� �������� �������������
    ReadThreadErrorNumber=0;// ���������� � ����� ������ �������������.
    DataStep =256*32*cI.size(); // 256*64  // ���-�� ��������, ������� 32
    // ��� ���� ������ �������� �� ���������� ���������� ������� � �������.
    


    ReadBuffer=new SHORT[2*DataStep]; // � ���� ����� ����������� ������.

    if(DriverInit()) // ������������� �������
    {
    if(SettingADCParams(frenquency,cI))// ������������� ���������
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
bool LCardADC::SettingADCParams(double frenquency, channelsInfo & chanInfo)
{
    // ��������� �������� ��������� ������ ���
	ap.IsCorrectionEnabled = TRUE;			// �������� ������������� ������ �� ������ �������� DSP
	ap.InputMode = NO_SYNC_E440;			// ������� ���� ������ ���� ������ ������������� �����
	ap.ChannelsQuantity = chanInfo.size();	// ���������� �������� �������
	// ��������� ����������� �������
    channelsInfo::iterator pos;
    int i;
    for(i=0,pos=chanInfo.begin();pos!=chanInfo.end();++pos,++i)
    { 
        ap.ControlTable[i]=(WORD)(pos->first | (pos->second << 0x6));
    }

	/*for(int i = 0x0; i < ap.ChannelsQuantity; i++)
        {
        //unsigned short temp=
        ap.ControlTable[i] =  // ����/��� +- 10�����, ���������������� �����.
        (WORD)(i | (ADC_INPUT_RANGE_2500mV_E440 << 0x6));
        //temp=temp;
        }
        ap.ControlTable[2] =  // ����/��� +- 10�����, ���������������� �����.
        (WORD)(2 | (ADC_INPUT_RANGE_625mV_E440 << 0x6)); */

        
	ap.AdcRate = frenquency;					// ������� ������ ��� � ���
	ap.InterKadrDelay = 0.0;					// ����������� �������� � ��
	ap.AdcFifoBaseAddress = 0x0;			  	// ������� ����� FIFO ������ ��� � DSP ������
	ap.AdcFifoLength = MAX_ADC_FIFO_SIZE_E440;	// ����� FIFO ������ ��� � DSP ������
	// ����� ������������ ��������� ������������� ������������, ������� ��������� � ���� ������
	for(int i = 0x0; i < ADC_CALIBR_COEFS_QUANTITY_E440; i++)
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
void LCardADC::StopMeasurement()
{
    needToStop=true;
    WaitForSingleObject(hReadThread, INFINITE);
    isMeasurementRunning=false;
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
    delete[] ReadBuffer;
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
    long double bigNumber=1E20;
    // ���� ���������� ������� - ������ �����.

    if(B)
    {
        B->Clear();
        for (int i = 0; i < DequeBuffer[2].size(); ++i)
        {
            B->AddY(DequeBuffer[2].back(),"",clBlue);
        }
    }        
    if(HallSeries)
    {
        HallSeries->Clear();
        for (int i = 0; i < DequeBuffer[0].size(); ++i)
        {
            HallSeries->AddY(DequeBuffer[0].back(),"",clBlue);
        }   
    }
    if(MagnetoResistanceSeries)
    {
        MagnetoResistanceSeries->Clear();
        for (int i = 0; i < DequeBuffer[1].size(); ++i)
        {
            MagnetoResistanceSeries->AddY(DequeBuffer[1].back(),"",clBlue);
        }
    }
}
//------------------------------------------------------------------
void LCardADC::InteractivePlottingData()
{
    long double bigNumber=1E20;
    // ���� ���������� ������� - ������ �����.
    if(HallSeries)
        if (ReadData[0].back()<bigNumber || ReadData[2].back()<bigNumber)
            HallSeries->AddXY(ReadData[0].back(),ReadData[2].back(),"",clBlue);
    if(MagnetoResistanceSeries)
        if (ReadData[1].back()<bigNumber || ReadData[2].back()<bigNumber)
            MagnetoResistanceSeries->AddXY(ReadData[1].back(),ReadData[2].back(),"",clBlue);
}
//------------------------------------------------------------------
void LCardADC::DisplayOnForm(int i1, MyDataType v1)
{
    ChannelLabels[i1]->Caption=FloatToStrF(v1,ffFixed,5,5);
}
//------------------------------------------------------------------
// ���������� ������ �� ������
// ��������� ��������� ������
void LCardADC::writeDataToVector(DataTypeInContainer & tempData)
{

    splitToChannels(tempData,splittedData); // ��������� ������ �� �������.

    if(isMedianFilterEnabled)
    { // ��������� ��������� ������, �������������� � ������ � ��������� � ��� ��������� ������.

        for(int i=0;i<ap.ChannelsQuantity;i++)
        {
            ReadData[i].push_back(convertToVolt(medianFilter(splittedData[i]),i));
            DisplayOnForm(i,ReadData[i].back());
        }
    }
    else
    {
        if(TestingMode)
        {
        int TestBufferSize = 500;
        DequeBuffer.resize(ap.ChannelsQuantity);

            for(unsigned int i=0;i<ap.ChannelsQuantity;i++)
            {
                for(unsigned int j=0;j<splittedData[i].size();++j)
                {
                    if(DequeBuffer[i].size()>TestBufferSize && !DequeBuffer[i].empty())
                        DequeBuffer[i].pop_front();
                    DequeBuffer[i].push_back(convertToVolt(splittedData[i][j],i));
                }
            }  
            InteractivePlottingDataOne();  
        }
        else
        {
            for(unsigned int i=0;i<ap.ChannelsQuantity;i++)
            {
                for(unsigned int j=0;j<splittedData[i].size();++j)
                {
                    ReadData[i].push_back(convertToVolt(splittedData[i][j],i));
                    DisplayOnForm(i,ReadData[i].back());
                }
            }
        }
    }
    if(!TestingMode)
    InteractivePlottingData();
    tempData.clear();
    splittedData.clear();
}

//------------------------------------------------------------------
unsigned long __stdcall LCardADC::ServiceReadThreadReal()
{
    DataTypeInContainer tempData;

	WORD i;
	WORD RequestNumber;
	DWORD FileBytesWritten;
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
        for(int channel=0;channel<ap.ChannelsQuantity && i<bound;channel++,i++)
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

    for(int i=1;i<tsize;i+=ap.ChannelsQuantity)
    tempBuffer[i]=8000;
    for(int i=0;i<tsize;i+=ap.ChannelsQuantity)
    tempBuffer[i]=B[nK]+1000;
    for(int i=2;i<tsize;i+=ap.ChannelsQuantity)
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



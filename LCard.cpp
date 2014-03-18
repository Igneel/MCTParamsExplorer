
#include "LCard.h"
#include "Unit1.h"

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);


LCardADC::LCardADC():NDataBlock(10)
{

needToStop=true;// ���� ��� ��������� ������� ������
successfullInit=false; // ���� �������� �������������
ReadThreadErrorNumber=0;// ���������� � ����� ������ �������������.
DataStep = 256*1024;  // ���-�� ��������, ������� 32
Counter = 0x0;        // ���������� ���������� ������.
OldCounter = 0xFFFFFFFF;// � ��� ������� ������ �� ���-��� ���������� � ������� �� � ������ ���������.
ReadBuffer=new SHORT[2*DataStep]; // � ���� ����� ����������� ������.
AdcBuffer = new SHORT[2*DataStep];// ������� �� ���� ����� �� �����, ���� �����������.
if(DriverInit()) // ������������� �������
{
if(SettingADCParams())// ������������� ���������
{
successfullInit=true; // �� ������ �������.
}
}

}

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

	// ������ ��������� �������� ������ ���� USB
	//printf("   USB is in %s\n", UsbSpeed ? "High-Speed Mode (480 Mbit/s)" : "Full-Speed Mode (12 Mbit/s)");

	// ��� LBIOS'� ������ �� ���������������� ������� ������� DLL ����������
	if(!pModule->LOAD_MODULE())
    {
    Error(" LOAD_MODULE() --> Bad\n");
    return false;
    }
	//else printf(" LOAD_MODULE() --> OK\n");

	// �������� �������� ������
 	if(!pModule->TEST_MODULE())
    {
    Error(" TEST_MODULE() --> Bad\n");
    return false;
    }
	//else printf(" TEST_MODULE() --> OK\n");

	// ������� ���������� �� ���� ������
	if(!pModule->GET_MODULE_DESCRIPTION(&ModuleDescription))
    {
    Error(" GET_MODULE_DESCRIPTION() --> Bad\n");
    return false;
    }
	//else printf(" GET_MODULE_DESCRIPTION() --> OK\n");

	// ������� ������� ��������� ������ ���
	if(!pModule->GET_ADC_PARS(&ap))
    {
    Error(" GET_ADC_PARS() --> Bad\n");
    return false;
    }
    	//else printf(" GET_ADC_PARS() --> OK\n");
    return true;

}

bool LCardADC::IsInitSuccessfull()
{
    return successfullInit;
}

// ��������� �� ���������.
// 400 ���, ��� ����������� ��������, 4 ������, ��������� ������������� ������������.
bool LCardADC::SettingADCParams()
{
    return SettingADCParams(4, 400.0);
}

bool LCardADC::SettingADCParams(unsigned short channelsQuantity, double frenquency)
{
    // ��������� �������� ��������� ������ ���
	ap.IsCorrectionEnabled = TRUE;			// �������� ������������� ������ �� ������ �������� DSP
	ap.InputMode = NO_SYNC_E440;			// ������� ���� ������ ���� ������ ������������� �����
	ap.ChannelsQuantity = channelsQuantity;	// ������ �������� ������
	// ��������� ����������� �������
	for(int i = 0x0; i < ap.ChannelsQuantity; i++)
		ap.ControlTable[i] = (WORD)(i | (ADC_INPUT_RANGE_2500mV_E440 << 0x6));
	ap.AdcRate = frenquency;							// ������� ������ ��� � ���
	ap.InterKadrDelay = 0.0;					// ����������� �������� � ��
	ap.AdcFifoBaseAddress = 0x0;			  	// ������� ����� FIFO ������ ��� � DSP ������
	ap.AdcFifoLength = MAX_ADC_FIFO_SIZE_E440;	// ����� FIFO ������ ��� � DSP ������
	// ����� ������������ ��������� ������������� ������������, ������� ��������� � ���� ������
	for(int i = 0x0; i < ADC_CALIBR_COEFS_QUANTITY_E440; i++)
	{
		ap.AdcOffsetCoefs[i] =  ModuleDescription.Adc.OffsetCalibration[i];
		ap.AdcScaleCoefs[i] =  ModuleDescription.Adc.ScaleCalibration[i];
	}

	// ��������� ��������� ��������� ������ ��� � ������
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
}

void LCardADC::StartMeasurement()
{

    needToStop=false;
    // ������ � ��������� ����� ����� ������
    hReadThread = CreateThread(0, 0x2000, ServiceReadThread, 0, 0, &ReadTid);
	if(!hReadThread)
    {
    Error(" ServiceReadThread() --> Bad\n");
    return;
    }

    // ���� ������ ���������� ������ � �������� ��������� ������ ����������

	while(!IsReadThreadComplete)
	{
		if(OldCounter != Counter)
        {
            OldCounter = Counter;
        }
		else Sleep(20);
	}

	// ��� ��������� ������ ������ ����� ������
	WaitForSingleObject(hReadThread, INFINITE);

	// �������� ���� �� ������ ���������� ������ ����� ������
}

std::string LCardADC::Error(std::string s)
{
    return s;
}

LCardADC::~LCardADC()
{
    delete[] ReadBuffer;
    delete[] AdcBuffer;
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

unsigned long __stdcall LCardADC::ServiceReadThreadReal()
{
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
		for(i = 0x1; i < NDataBlock; i++)
		{
            if(needToStop)
            {
                i=NDataBlock-1;
                break;
            }
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
            for(int i=0;i<DataStep;++i)
            {
                ReadData.push_back(IoReq[RequestNumber^0x1].Buffer[i]);
            }
            // ���� �������� � ����� ���� ���� ����� ������ ������.
            // �� � ��� �������� ���� ����� � ������:)
            Sleep(20);
            ++Counter;


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
            for(int i=0;i<DataStep;++i)
            {
                //double temp=IoReq[RequestNumber^0x1].Buffer[i];
                ReadData.push_back(IoReq[RequestNumber^0x1].Buffer[i]);
            }
			Counter++;
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


std::vector<MyDataType> const &  LCardADC::getData()
{
    return ReadData;
}

void LCardADC::convertToVolt()
{
    // ��� �������� ������ ���� ������� +- 10�����.
    std::vector<MyDataType>::iterator pos;
    for(pos=ReadData.begin();pos!=ReadData.end();++pos)
    *pos/=800.0;
}






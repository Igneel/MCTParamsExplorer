
#include "LCard.h"
#include "Unit1.h"

DWORD WINAPI ServiceReadThread(PVOID /*Context*/);

LCardADC::LCardADC():NDataBlock(80)
{
needToStop=true;
successfullInit=false;
DataStep = 256*1024;
Counter = 0x0;
OldCounter = 0xFFFFFFFF;

if(DriverInit())
{
if(SettingADCParams())
{
successfullInit=true;
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
}

bool LCardADC::SettingADCParams()
{
// ��������� �������� ��������� ������ ���
	ap.IsCorrectionEnabled = TRUE;			// �������� ������������� ������ �� ������ �������� DSP
	ap.InputMode = NO_SYNC_E440;				// ������� ���� ������ ���� ������ ������������� �����
	ap.ChannelsQuantity = 0x4;					// ������ �������� ������
	// ��������� ����������� �������
	for(int i = 0x0; i < ap.ChannelsQuantity; i++)
		ap.ControlTable[i] = (WORD)(i | (ADC_INPUT_RANGE_2500mV_E440 << 0x6));
	ap.AdcRate = 400.0;							// ������� ������ ��� � ���
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

	//else printf(" SET_ADC_PARS() --> OK\n");
}

void LCardADC::StopMeasurement()
{
    needToStop=true;
}

void LCardADC::StartMeasurement()
{
    needToStop=false;
    // ������ � ��������� ����� ����� ������
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	hReadThread = CreateThread(0, 0x2000, ServiceReadThread, 0, 0, &ReadTid);
	if(!hReadThread)
    {
    Error(" ServiceReadThread() --> Bad\n");
    return;
    }
   	//else printf(" ServiceReadThread() --> OK\n");
    
	// ��������� ��������� ����� ������ ������ �� ������ ��������
	/*printf(" \n");
	printf(" Module E14-440 (S/N %s) is ready ... \n", ModuleDescription.Module.SerialNumber);
	printf("   Module Info:\n");
	printf("     Module  Revision   is '%c'\n", ModuleDescription.Module.Revision);
	printf("     MCU Driver Version is %s (%s)\n", ModuleDescription.Mcu.Version.Version, ModuleDescription.Mcu.Version.Date);
	printf("     LBIOS   Version    is %s (%s)\n", ModuleDescription.Dsp.Version.Version, ModuleDescription.Dsp.Version.Date);
	printf("   Adc parameters:\n");
	printf("     Data Correction is %s\n", ap.IsCorrectionEnabled ? "enabled" : "disabled");
	printf("     ChannelsQuantity = %2d\n", ap.ChannelsQuantity);
	printf("     AdcRate = %8.3f kHz\n", ap.AdcRate);
	printf("     InterKadrDelay = %2.4f ms\n", ap.InterKadrDelay);
	printf("     KadrRate = %8.3f kHz\n", ap.KadrRate);
    */
    // ���� ������ ���������� ������ � �������� ��������� ������ ����������
//	DacSample = 0x1000;
	//printf("\n Press any key if you want to terminate this program...\n\n");
	while(!IsReadThreadComplete)
	{
		if(OldCounter != Counter)
        {
            //printf(" Counter %3u from %3u\r", Counter, NDataBlock);
            OldCounter = Counter;
        }
		else Sleep(20);
//		if(!pModule->ENABLE_TTL_OUT(TRUE)) AbortProgram(" ������ ���������� �������� �������� �����");
//		if(!pModule->TTL_OUT(0xFFFF)) AbortProgram(" ������ ��������� �������� �������� �����");
//		if(!pModule->TTL_OUT(0x0000)) AbortProgram(" ������ ��������� �������� �������� �����");
//		if(!pModule->DAC_SAMPLE((SHORT *)&DacSample, 0x0)) AbortProgram(" ������ ��������� ������ ���");
	}

	// ��� ��������� ������ ������ ����� ������
	WaitForSingleObject(hReadThread, INFINITE);

	// �������� ���� �� ������ ���������� ������ ����� ������
}

std::string LCardADC::Error(std::string s)
{
//s.begin();
;
}

LCardADC::~LCardADC()
{
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

DWORD WINAPI ServiceReadThread(PVOID /*Context*/)
{
    adc->ServiceReadThreadReal();
}

DWORD WINAPI LCardADC::ServiceReadThreadReal()
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
			if(ReadThreadErrorNumber) break;

			// ��� ���������� �������� ����� ���������� ������ ������
			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT)
            {
            ReadThreadErrorNumber = 0x3;
            break;
            }
			if(ReadThreadErrorNumber) break;

			// ������� ���������� ������ ������ � ������
            for(int i=0;i<2*DataStep;++i)
            {
            ReadData.push_back(IoReq[RequestNumber^0x1].Buffer[i]);
            }
            // ���� �������� � ����� ���� ���� ����� ������ ������.
            // �� � ��� �������� ���� ����� � ������:)
            Sleep(20);
            ++Counter;
			/*if(!WriteFile(	hFile,													// handle to file to write to
		    					IoReq[RequestNumber^0x1].Buffer,					// pointer to data to write to file
								2*DataStep,	 											// number of bytes to write
	    						&FileBytesWritten,									// pointer to number of bytes written
						   	NULL			  											// pointer to structure needed for overlapped I/O
							   )) { ReadThreadErrorNumber = 0x4; break; }

			if(ReadThreadErrorNumber) break;   */
			//else

            /*if(kbhit())
            {
                ReadThreadErrorNumber = 0x5;
                break;
            }
			else
            {   */

            //}

		}

		// ��������� ������ ������
		if(!ReadThreadErrorNumber)
		{
			RequestNumber ^= 0x1;
			// ��� ��������� �������� ����� ��������� ������ ������
			if(WaitForSingleObject(ReadOv[RequestNumber^0x1].hEvent, IoReq[RequestNumber^0x1].TimeOut) == WAIT_TIMEOUT) ReadThreadErrorNumber = 0x3;

            // ������� ���������� ������ ������ � ������
            for(int i=0;i<2*DataStep;++i)
            {
            ReadData.push_back(IoReq[RequestNumber^0x1].Buffer[i]);
            }
			/*if(!WriteFile(	hFile,													// handle to file to write to
		    					IoReq[RequestNumber^0x1].Buffer,					// pointer to data to write to file
								2*DataStep,	 											// number of bytes to write
	    						&FileBytesWritten,									// pointer to number of bytes written
						   	NULL			  											// pointer to structure needed for overlapped I/O
							   )) ReadThreadErrorNumber = 0x4;    */
			Counter++;
		}
	}
	else { ReadThreadErrorNumber = 0x6; }

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






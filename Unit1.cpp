//---------------------------------------------------------------------------
#pragma hdrstop

#include "Unit1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

/*
TODO
����������� ������ "������" - �.�. ������� ���������� �������� � ������ ������ �����
������� �������� ������������� ��������� �����

���� �� ������������� ��������� ����� ��� ������.
� �������� ��� �� �������.

������� ������ � ������ ����� ����������.
�������������� ������������
������������� ������� (������������� ����, ������������� ����).

�������� �� ����� ������ ����������. �� ��� ����� ������� ������.
��� ��� ����� ���������� ���� �� �������������� ������ ��� ���������� ��.

��� ������ - ����� ������� �������� � �����������, ������� ���� �� �����������
�������������� ���������� ����������� ������� (������ ������ � �����).

���� �������� ������ ��� �����.

����� �������� ���������� �� ����.


*/

// ��������, ����������� �������� ���-������,
// �� ������ ������ ��� � ������������ ����:)


LCardADC *adc=0;
MagneticFieldDependence *params=0;
MagneticFieldDependence *paramsDirect=0;
MagneticFieldDependence *paramsReverse=0;
channelsInfo cI;

SettingsSaver * settings;

MagneticFieldDependence * TForm1::InitParams()
{
    MagneticFieldDependence ** p=ActiveParams();

    DeleteActiveParams();
    
    *p=new MagneticFieldDependence(uiCurrent->Text,uiTemperature->Text,uiInventoryNumber->Text,
        uiSampleLength->Text,uiSampleWidth->Text,uiSampleThickness->Text);
        /*
        �������������� Initializing Params Type with const int - �� ���������,
        �.�. �������� ��� ��� ��������� � �������������, �� ���������� � ���� �����.
        */
        (*p)->saver->setParamsType(ResCurveIndex->ItemIndex); // �������� �� ���������.
        (*ActiveParams())->setParamsType(ResCurveIndex->ItemIndex);
        (*ActiveParams())->setChannelsInfo(cI);
        (*p)->setFilterParamsResistance(eSamplingFRes->Text, eBandWidthFRes->Text,
     eAttenuationFRes->Text, eLengthFilterRes->Text);
    (*p)->setFilterParamsHall(eSamplingFHall->Text, eBandWidthFHall->Text,
     eAttenuationFHall->Text, eLengthFilterHall->Text);
    (*p)->setExtrapolateParams(PowPolinomHall->Text.ToInt(),PowPolinomRes->Text.ToInt());
    return *p;    
}

MagneticFieldDependence ** TForm1::ActiveParams()
{
    MagneticFieldDependence ** p=NULL;
    switch (ResCurveIndex->ItemIndex)
    {
    case 0:
        p=&paramsDirect;
        
    break;
    case 1:
        p=&paramsReverse;
    break;            
    case 2:
        p=&params;
    
    break;
    default:
    break;
    }
    return p;          
}

void TForm1::DeleteActiveParams()
{
    MagneticFieldDependence ** p=ActiveParams();
    if (*p)
    {
        delete *p;
        *p=NULL;
    }
}



void TForm1::UpdatePlots()
{
    StatusBar->Panels->Items[1]->Text="���������� ��������.";
    Form1->Update();
    MagneticFieldDependence ** par=ActiveParams();

    if(*par)
    {
    MagneticFieldDependence * p=*par;
    /*p->constructPlotFromOneMassive(HALL_EFFECT,SeriesHall1,clBlue);
    p->constructPlotFromOneMassive(MAGNETORESISTANCE,SeriesRes1,clBlue);
    p->constructPlotFromOneMassive(MAGNETIC_FIELD,Series1,clBlue);
    */
    p->constructPlotFromOneMassive(MAGNETIC_FIELD,Series1,clBlue);
    p->constructPlotFromOneMassive(MAGNETIC_FIELD_F,Series2,clRed);


    // ���������� ���� ������������ ��������.
    if(!p->constructPlotFromTwoMassive(HALL_EFFECT,CURRENT_DATA,SeriesHall1,clRed))
        ErrorLog->Lines->Add("����. ������� ������. �� ������� ��������� ������.");
    if(!p->constructPlotFromTwoMassive(HALL_EFFECT,FILTERED_DATA,SeriesHall2,clBlue))
        ErrorLog->Lines->Add("����. ������������� ������. �� ������� ��������� ������.");
    if(!p->constructPlotFromTwoMassive(HALL_EFFECT,EXTRAPOLATED_DATA,out2,clBlack))
        ErrorLog->Lines->Add("����. ������������������ ������. �� ������� ��������� ������.");

    if(!p->constructPlotFromTwoMassive(HALL_EFFECT,AVERAGED_DATA,SeriesFFTHall,clGreen))
        ErrorLog->Lines->Add("����. ����������� ������. �� ������� ��������� ������.");

    if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,AVERAGED_DATA,SeriesFFTRes,clGreen))
        ErrorLog->Lines->Add("�������������. ����������� ������. �� ������� ��������� ������.");

    if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,CURRENT_DATA,SeriesRes1,clRed))
        ErrorLog->Lines->Add("�������������. ������� ������. �� ������� ��������� ������.");
    if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,FILTERED_DATA,SeriesRes2,clBlue))
        ErrorLog->Lines->Add("�������������. ������������� ������. �� ������� ��������� ������.");
    if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,EXTRAPOLATED_DATA,out1,clBlack))
        ErrorLog->Lines->Add("�������������. ������������������ ������. �� ������� ��������� ������.");
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TForm1::saveSettings()
{
    // �������� ������� ����� ��������
    settings= new SettingsSaver();
    settings->Add("MeasurementFrencuency",uiFrenq->Text.c_str());
    settings->Add("ChannelB",IntToStr(ComboBox4->ItemIndex).c_str());
    settings->Add("ChannelResistance",IntToStr(ComboBox5->ItemIndex).c_str());
    settings->Add("ChannelHall",IntToStr(ComboBox6->ItemIndex).c_str());
    settings->Add("ChannelBRange",IntToStr(ComboBox1->ItemIndex).c_str());
    settings->Add("ChannelResistanceRange",IntToStr(ComboBox2->ItemIndex).c_str());
    settings->Add("ChannelHallRange",IntToStr(ComboBox3->ItemIndex).c_str());
    settings->Add("isMedianFilterEnabled",IntToStr(CheckBox1->Checked).c_str());
    settings->Add("uiCurrent",uiCurrent->Text.c_str());
    settings->Add("SampleTemperature",uiTemperature->Text.c_str());
    settings->Add("SampleInventoryNumber",uiInventoryNumber->Text.c_str());
    settings->Add("SampleLength",uiSampleLength->Text.c_str());
    settings->Add("SampleWidth",uiSampleWidth->Text.c_str());
    settings->Add("SampleThickness",uiSampleThickness->Text.c_str());
    settings->Add("eLengthFilterRes",eLengthFilterRes->Text.c_str());
    settings->Add("eSamplingFRes",eSamplingFRes->Text.c_str());
    settings->Add("eBandWidthFRes",eBandWidthFRes->Text.c_str());
    settings->Add("eAttenuationFRes",eAttenuationFRes->Text.c_str());
    settings->Add("PowPolinomRes",PowPolinomRes->Text.c_str());
    settings->Add("eSamplingFHall",eSamplingFHall->Text.c_str());
    settings->Add("eBandWidthFHall",eBandWidthFHall->Text.c_str());
    settings->Add("eAttenuationFHall",eAttenuationFHall->Text.c_str());
    settings->Add("PowPolinomHall",PowPolinomHall->Text.c_str());
    settings->Add("ActivePageIndex",IntToStr(PC->ActivePageIndex).c_str());
}

void TForm1::loadSettings()
{

}

void __fastcall TForm1::FormCreate(TObject *)
{
    // �������� ��������.

    // �������� ������� ����� ��������
    settings= new SettingsSaver();
    settings->Add("MeasurementFrencuency",uiFrenq->Text.c_str());
    settings->Add("ChannelB",IntToStr(ComboBox4->ItemIndex).c_str());
    settings->Add("ChannelResistance",IntToStr(ComboBox5->ItemIndex).c_str());
    settings->Add("ChannelHall",IntToStr(ComboBox6->ItemIndex).c_str());
    settings->Add("ChannelBRange",IntToStr(ComboBox1->ItemIndex).c_str());
    settings->Add("ChannelResistanceRange",IntToStr(ComboBox2->ItemIndex).c_str());
    settings->Add("ChannelHallRange",IntToStr(ComboBox3->ItemIndex).c_str());
    settings->Add("isMedianFilterEnabled",IntToStr(CheckBox1->Checked).c_str());
    settings->Add("uiCurrent",uiCurrent->Text.c_str());
    settings->Add("SampleTemperature",uiTemperature->Text.c_str());
    settings->Add("SampleInventoryNumber",uiInventoryNumber->Text.c_str());
    settings->Add("SampleLength",uiSampleLength->Text.c_str());
    settings->Add("SampleWidth",uiSampleWidth->Text.c_str());
    settings->Add("SampleThickness",uiSampleThickness->Text.c_str());
    settings->Add("eLengthFilterRes",eLengthFilterRes->Text.c_str());
    settings->Add("eSamplingFRes",eSamplingFRes->Text.c_str());
    settings->Add("eBandWidthFRes",eBandWidthFRes->Text.c_str());
    settings->Add("eAttenuationFRes",eAttenuationFRes->Text.c_str());
    settings->Add("PowPolinomRes",PowPolinomRes->Text.c_str());
    settings->Add("eSamplingFHall",eSamplingFHall->Text.c_str());
    settings->Add("eBandWidthFHall",eBandWidthFHall->Text.c_str());
    settings->Add("eAttenuationFHall",eAttenuationFHall->Text.c_str());
    settings->Add("PowPolinomHall",PowPolinomHall->Text.c_str());
    settings->Add("ActivePageIndex",IntToStr(PC->ActivePageIndex).c_str());
       
    

    cI.clear();
    cI.push_back(std::pair<int,int> (ComboBox4->ItemIndex,ComboBox1->ItemIndex));
    cI.push_back(std::pair<int,int> (ComboBox5->ItemIndex,ComboBox2->ItemIndex));
    cI.push_back(std::pair<int,int> (ComboBox6->ItemIndex,ComboBox3->ItemIndex));

    ErrorLog->Lines->Add(cI.size());
    // ��������� �������
    adc=new LCardADC(uiFrenq->Text.ToDouble(),uiBlockSize->Text.ToInt(),
    LabelChan1,LabelChan2,LabelChan3,cI);
    if(!adc->IsInitSuccessfull())
    {
    delete adc;
    adc=NULL;
    }
    uiFrenq->OnChange(NULL);
    bApplyADCSettings->Click();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3Click(TObject *Sender)// ����� �� ���������
{
    Form1->Close(); 
}
//----����� ����, ���������/�������� ����������� ��������, �� ����� ������ ���-
//---------------------------------------
void __fastcall TForm1::uiControlClick(TObject *Sender)
{
    if (!adc->isWritingEnabled())
    {
        MagneticFieldDependence * p=InitParams();

        if(p!=NULL)
        {
            p->setFilterParamsResistance(eSamplingFRes->Text, eBandWidthFRes->Text,
            eAttenuationFRes->Text, eLengthFilterRes->Text);
            p->setFilterParamsHall(eSamplingFHall->Text, eBandWidthFHall->Text,
            eAttenuationFHall->Text, eLengthFilterHall->Text);
        } 

        if(adc->StartWriting())
        {

            bClearPlotHall->Click();
            bClearPlotsRes->Click();

            uiFrenq->Enabled = false;
            uiBlockSize->Enabled=false;
            uiCurrent->Enabled=0;
            
            ResCurveIndex->Enabled=0;
            HallCurveIndex->Enabled=0;
            GainKoefFaradey->Enabled=0;

            //-- ������-----------------------------------------------
            
            bFilterRes->Enabled=0;

            uiFFTHall->Enabled=0;

            uiFFTFaradey->Enabled=0;
            
            uiFFTFoygt->Enabled=0;
            CurrentFaradey->Enabled=0;
            CurrentFoygt->Enabled=0;
            FaradeyCurveIndex->Enabled=0;
            FoygtCurveIndex->Enabled=0;
            GainKoefFoygt->Enabled=0;

            uiControl->Caption = AnsiString("���������� ������");
            uiResControl->Caption = AnsiString("���������� ������");
            uiHallControl->Caption = AnsiString("���������� ������");
            uiFaradeyControl->Caption = AnsiString("���������� ������");
            uiFoygtControl->Caption = AnsiString("���������� ������");
            StatusBar->Panels->Items[1]->Text="��� ������ ������";
        }
        else
        {
        DeleteActiveParams();
        }
    }
    else
    {
        GainKoefFaradey->Enabled=1;
        uiCurrent->Enabled=1;
        ResCurveIndex->Enabled=1;
        HallCurveIndex->Enabled=1;

        uiFrenq->Enabled =true;

        uiBlockSize->Enabled=true;

        
        bFilterRes->Enabled=1;

        uiFFTHall->Enabled=1;
        uiFFTFaradey->Enabled=1;
        uiFFTFoygt->Enabled=1;

        CurrentFaradey->Enabled=1;
        CurrentFoygt->Enabled=1;

        FaradeyCurveIndex->Enabled=1;
        FoygtCurveIndex->Enabled=1;
        GainKoefFoygt->Enabled=1;

        adc->StopWriting();
        adc->StopMeasurement();

        (*ActiveParams())->getSplittedDataFromADC();
        ErrorLog->Lines->Add( IntToStr((*ActiveParams())->getB()->size()));
        UpdatePlots();

        adc->StartMeasurement();
        uiControl->Caption = AnsiString("������ ������");
        uiResControl->Caption = AnsiString("������ ������");
        uiHallControl->Caption = AnsiString("������ ������");
        uiFaradeyControl->Caption = AnsiString("������ ������");
        uiFoygtControl->Caption = AnsiString("������ ������");
        
        StatusBar->Panels->Items[1]->Text="������ � ������.";        
      }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::bClearClick(TObject *Sender) // ������� ��:)
{  
    Series1->Clear();
    Series2->Clear();
    Series3->Clear();
    SeriesRes1->Clear();
    ErrorLog->Clear();
}
//---------------------------------------------------------------------------
//----------------------------------------------------------------------------
void __fastcall TForm1::bFilterResClick(TObject *Sender)
{

//eAttenuationFRes->Text=FloatToStr(StrToFloat(eBandWidthFRes->Text)+5.5/ StrToFloat( eLengthFilterRes->Text)* StrToFloat( eSamplingFRes->Text)*0.5);

MagneticFieldDependence ** par=ActiveParams();

if (*par==NULL)
{
    ShowMessage("�������� ������ �� ��� ������.");
}
else
{

    MagneticFieldDependence * p=*par;
    p->setFilterParamsResistance(eSamplingFRes->Text, eBandWidthFRes->Text,
     eAttenuationFRes->Text, eLengthFilterRes->Text);
    p->setFilterParamsHall(eSamplingFHall->Text, eBandWidthFHall->Text,
     eAttenuationFHall->Text, eLengthFilterHall->Text);
    StatusBar->Panels->Items[1]->Text="���������� ������.";
    Form1->Update();
    p->filterData();
    StatusBar->Panels->Items[1]->Text="������������� ������.";
    Form1->Update();
    p->extrapolateData(FILTERED_DATA,PowPolinomRes->Text.ToInt(),PowPolinomHall->Text.ToInt());
    UpdatePlots();
    StatusBar->Panels->Items[1]->Text="������ � ������.";
}
}
//---------------------------------------------------------------------------

void TForm1::UpdateSampleDescription(TStringList *Names,TStringList *Values)
{
    for(int i=0;i<Values->Count;++i)
    {
        uiCurrent->Text=Values->Strings[2];
        uiTemperature->Text=Values->Strings[1];
        uiInventoryNumber->Text=Values->Strings[0];
        uiSampleLength->Text=Values->Strings[3];
        uiSampleWidth->Text=Values->Strings[4];
        uiSampleThickness->Text=Values->Strings[5];    
    }
}


//-------------------�������� �����------------------------------------------

void __fastcall TForm1::N4Click(TObject *Sender)
{ 
    if(OpenDialog1->Execute())  // ���� �� ���-�� �������
    {
        TStringList *tts=new TStringList();  // ���� ����� ��������� �� �����
        tts->LoadFromFile(OpenDialog1->Files->Strings[0]);// ���������


        MagneticFieldDependence * p=InitParams();

        if (p)
        {
            TStringList *Names=new TStringList();
            TStringList *Values=new TStringList();

            p->loadSampleDescription(Names,Values,OpenDialog1->Files->Strings[0]);

            UpdateSampleDescription(Names,Values);
            
            p->loadData(tts);
            UpdatePlots();
        }
    }

}

 // ����� ��������� �������
 // ������������ ��� ���������� ����� � Unit2

TLineSeries * __fastcall TForm1::GetSelectedSeries(int index)
{

// ��������� ������ ����������� �� ��� �������.
    TLineSeries * SaveSeries;

    AllSeries[0]=PtrToInt(Series1);
    AllSeries[1]=PtrToInt(Series2);
    AllSeries[2]=PtrToInt(Series3);
    AllSeries[3]=PtrToInt(Series4);
    AllSeries[4]=PtrToInt(0);
    AllSeries[5]=PtrToInt(SeriesRes1);
    AllSeries[6]=PtrToInt(SeriesRes2);
    AllSeries[7]=PtrToInt(SeriesFFTRes);
    AllSeries[8]=PtrToInt(out1);
    AllSeries[9]=PtrToInt(SeriesHall1);
    AllSeries[10]=PtrToInt(SeriesHall2);
    AllSeries[11]=PtrToInt(SeriesFFTHall);
    AllSeries[12]=PtrToInt(out2);
    AllSeries[13]=PtrToInt(SeriesFaradey1);
    AllSeries[14]=PtrToInt(SeriesFaradey2);
    AllSeries[15]=PtrToInt(SeriesFFTFaradey);
    AllSeries[16]=PtrToInt(out3);
    AllSeries[17]=PtrToInt(SeriesFoygt1);
    AllSeries[18]=PtrToInt(SeriesFoygt2);
    AllSeries[19]=PtrToInt(SeriesFFTFoygt);
    AllSeries[20]=PtrToInt(out4);

    SaveSeries=static_cast<TLineSeries *>IntToPtr(AllSeries[index]);

    return SaveSeries;
}

void __fastcall TForm1::N5Click(TObject *Sender)  // ����������
{

    GetSelectedSeries(0);
    
    if(!SaveForm)
    {
    // �������� ����������� ����� ���������� ������.
    Application->CreateForm(__classid(TSaveForm), &SaveForm);
    SaveForm->Visible=true;
    }
    else
    SaveForm->BringToFront();

}


void __fastcall TForm1::bClearPlotsResClick(TObject *Sender)
{
    SeriesRes1->Clear();
    SeriesRes2->Clear();
    SeriesFFTRes->Clear();
    out1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bClearPlotHallClick(TObject *Sender)
{
    SeriesHall1->Clear();  
    SeriesHall2->Clear();
    SeriesFFTHall->Clear();
    out2->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button7Click(TObject *Sender)
{
    SeriesFaradey1->Clear();
    SeriesFaradey2->Clear();
    SeriesFFTFaradey->Clear();
    out3->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button8Click(TObject *Sender)
{
    SeriesFoygt1->Clear();
    SeriesFoygt2->Clear();
    SeriesFFTFoygt->Clear();
    out4->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1KeyPress(TObject *Sender, char &Key)
{
    if(Key=='.')
        Key=',';
    if(((int)Key<48 || (int)Key>57) && Key!=8 && Key!=',' && Key!='-')
        Key=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImpulsKillerClick(TObject *Sender)
{
    double x,y;
    x=Interval1->Text.ToDouble();
    y=Interval2->Text.ToDouble();
    KillImpulps(SeriesRes1,x,y);

}

//---------------------------------------------------------------------------
void fillspace(double *xm,double *ym, int l,int r)
{

    if(abs(r-l)==1)
    {
    xm[l+1]=(xm[l]+xm[r])/2;
    ym[l+1]=(ym[l]+ym[r])/2;
    return;
    }
    xm[(l+r)/2]=(xm[l]+xm[r])/2;
    ym[(l+r)/2]=(ym[l]+ym[r])/2;
    fillspace(xm,ym,l,(l+r)/2);
    fillspace(xm,ym,(l+r)/2,r);
}
//---------------------------------------------------------------------------
// ������� ����� ��������������� ���������
void KillImpulps(TLineSeries * a,double x, double y)
{

    int length=a->XValues->Count();
    if(length==0)
    {
    ShowMessage("������ ������!!!");
    return;
    }
    double *xm=new double [length];
    double *ym=new double [length];

    for(int i=0;i<length;i++)
     {
            xm[i]=a->XValues->Value[i];
            ym[i]=a->YValues->Value[i];
     }
    // ������� ���������
    //x=Interval1->Text.ToDouble();
    //y=Interval2->Text.ToDouble();
    double dx=fabs(xm[0]-x);// �������� ����� ��������� �����
    int i;
    double k=0;
    for(i=0;i<length;i++)
    {
            k=fabs(xm[i]-x);
            if(k>dx)  // ��� ������ ����� �� ���������� - ������� �� �����
            {
                 break;
            }
            dx=k;
    }
    int i1=i; // ���������� ������ ������
    dx=fabs(xm[i]-y);// ���� ������
    for(;i<length;i++)
    {
            k=fabs(xm[i]-y);
            if(k>dx)
            {
                    break;
            }
            dx=k;
    }
    int i2=i;
    //int left=i1;
    //int right=i2;

    fillspace(xm,ym,i1,i2);

     a->Clear();
     for(int i=0;i<length;i++)
     {
            a->AddXY(xm[i],ym[i],"",clRed);
                    }
    delete [] xm;
    delete [] ym;
}
// � ����������� �� �������� ������� ���������� ����� ��������� �������.
void __fastcall TForm1::N9Click(TObject *Sender)
{
    int a=PC->ActivePageIndex;
    switch (a)
    {
    case 1:
    bClearPlotsResClick(Sender);
    case 2:
    bClearPlotHallClick(Sender);
    case 3:
    Button7Click(Sender);
    case 4:
    Button8Click(Sender);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
    int len=SeriesRes1->XValues->Count();

    if(len==0) return;

    double *x= new double [len];
    for(int i=0;i<len;i++)
    x[i]=SeriesRes1->XValues->Value[i];

    double *y= new double [len];
    for(int i=0;i<len;i++)
    y[i]=SeriesRes1->YValues->Value[i];

    double *yf= new double [len];
    for(int i=0;i<len;i++)
    yf[i]=SeriesFFTRes->YValues->Value[i];

    for(int i=0;i<len;i++)
    {
    y[i]=fabs(fabs(y[i])-fabs(yf[i]));
    SeriesRes2->AddXY(x[i],y[i],"",clBlack);
    }

    double M=0,sko=0;

    for(int i=0;i<len;i++)
    {
    M+=y[i]/len;
    }
    for(int i=0;i<len;i++)
    {
    sko+=pow(y[i]-M,2);
    }
    sko/=len;
    sko=sqrt(sko);

    ErrorLog->Lines->Add("M=" + FloatToStr(M) + "\n");
    ErrorLog->Lines->Add("sko=" + FloatToStr(sko) + "\n");

    delete[] x;
    delete[] y;
    delete[] yf;

}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormDestroy(TObject *Sender)
{
if(adc)
    adc->StopMeasurement();

    AnsiString x= Application->ExeName;
    AnsiString nx=x.SubString(0,x.Length()-12);
    settings->Save(nx+"settings.txt");
    delete settings;
    if(adc)
        delete adc;
    if(params)
        delete params;   

    if (paramsDirect)
    {
        delete paramsDirect;
    }
    if (paramsReverse)
    {
        delete paramsReverse;
    }
}
//---------------------------------------------------------------------------


template <class T>
void Rounding(T *pos, T* endPos)
{
    int S=1;
    for(;pos!=endPos;++pos)
    {
        int n=(int)(*pos*S)%10;
        if(n<5)
            *pos=floorl(*pos*S)/S;
        else
            *pos=ceill(*pos*S)/S;
    }
}

//--------------------------------------------------------------------------
// ������ �������� ������������� ����������
//--------------------------------------------------------------------------
template <class T>
T Sko (std::vector<T> const &x0,std::vector<T> const &x)
{
    int l=x0.size();
    T z=0;
    for(int i=0;i<l;i++)
    z+= pow(fabs((fabs(x[i])-fabs(x0[i]))),2);
    z/=(T)l;
    return sqrt(z);
}

//---------------------------------------------------
// ������ ��������������� ��������
//-------------------------------------------------

template <class T>
 T Mo (std::vector<T> const &x)
 {
    int l=x.size();
     T M=0;
     for(int i=0;i<l;i++)
     M+=x[i];
     return M/l;
 }
// ���������� ����������� �����������
void Gist(std::vector<long double> & in)
{
    Form1->Series5->Clear();
    Rounding(in.begin(),in.end());
    int max = max_elem(in);
    int min = min_elem(in);

    int size=max-min+1;
    std::vector<long double>::iterator pos;
    std::vector<long double> gist(size);
    for(pos=gist.begin(); pos!=gist.end();++pos)
    {
        *pos=0;
    }

    for(pos=in.begin(); pos!=in.end();++pos)
    {
        gist[*pos]++;

    }
    for(int i=0; i<size;++i)
    {
        Form1->Series5->AddXY(i,gist[i],"",clWhite);
    }

    long double m=Mo(in);

    std::vector<long double> zeros(size);
    for(pos=zeros.begin(); pos!=zeros.end();++pos)
    {
        *pos=m;
    }
    long double sko=Sko(zeros,in);

    Form1->ErrorLog->Lines->Add("��� ��������: "+FloatToStr(m));
    Form1->ErrorLog->Lines->Add("���: "+FloatToStr(sko));

}
//------------------------------------------------------------------------------
void __fastcall TForm1::Button10Click(TObject *Sender)
{
std::vector<long double> y;

for(int i=0;i<400;i++)
{
y.push_back(rand()%100);
}

Gist(y);
}
//------------------------------------------------------------------------------
// ��������� ��.
void __fastcall TForm1::N11Click(TObject *Sender)
{
    CurrentResChange(NULL);
    if(params && paramsDirect && paramsReverse)
    {
        SaveDialog1->Title="���������� ���� ������:";
        if(SaveDialog1->Execute())
        {
            params->SaveAllData(SaveDialog1->FileName+"Com",true);
            paramsDirect->SaveAllData(SaveDialog1->FileName+"Dir");
            paramsReverse->SaveAllData(SaveDialog1->FileName+"Rev");
        }
    }

    else
    {
    if(params)
    {
        SaveDialog1->Title="���������� ������������ ������:";
        if(SaveDialog1->Execute())
        {
            params->SaveAllData(SaveDialog1->FileName+"Com",true);
        }
    }

    if(paramsDirect)
    {
        SaveDialog1->Title="���������� ������ ��� �������������� ���������� ����:";
        if(SaveDialog1->Execute())
        {
            paramsDirect->SaveAllData(SaveDialog1->FileName+"Dir");
        }
    }

    if(paramsReverse)
    {
        SaveDialog1->Title="���������� ������ ��� �������������� ���������� ����:";
        if(SaveDialog1->Execute())
        {
            paramsReverse->SaveAllData(SaveDialog1->FileName+"Rev");
        }
    }
    }


}
//---------------------------------------------------------------------------


// ���������� �������� ���
void __fastcall TForm1::bApplyADCSettingsClick(TObject *Sender)
{
if(adc)
{
    cI.clear();
    cI.push_back(std::pair<int,int> (ComboBox4->ItemIndex,ComboBox1->ItemIndex));
    cI.push_back(std::pair<int,int> (ComboBox5->ItemIndex,ComboBox2->ItemIndex));
    cI.push_back(std::pair<int,int> (ComboBox6->ItemIndex,ComboBox3->ItemIndex));

    adc->StopMeasurement();
    adc->SettingADCParams(uiFrenq->Text.ToDouble(),uiBlockSize->Text.ToInt(), cI);
    if(CheckBox1->Checked) adc->EnableMedianFilter();
    else adc->DisableMedianFilter();
    adc->DisableTestingMode();

    adc->setMedianFilterLength(eMedianFilterSize->Text.ToInt());

    adc->setMagnetoResistanceSeries(SeriesRes1);
    adc->setHallSeries(SeriesHall1);
    adc->setBSeries(Series1);
    adc->StartMeasurement();
}
}
//---------------------------------------------------------------------------
void TForm1::concatDependence()
{
    if(!(paramsDirect && paramsReverse))
    {
    ShowMessage("��� ����������� ����� �������� ��� �����������");
    return;
    }

    ResCurveIndex->ItemIndex=2;    
    MagneticFieldDependence * p=InitParams();
    if(p)

    {
        params->setFilterParamsResistance(eSamplingFRes->Text, eBandWidthFRes->Text,
         eAttenuationFRes->Text, eLengthFilterRes->Text);
        params->setFilterParamsHall(eSamplingFHall->Text, eBandWidthFHall->Text,
         eAttenuationFHall->Text, eLengthFilterHall->Text);
    }

    TSignal B; // ������� ����� ��� ����� ������������.
    TSignal Hall;
    TSignal Resistance;

    TSignal B2; // ������� ����� ��� ����� ������������.
    TSignal Hall2;
    TSignal Resistance2;

    StatusBar->Panels->Items[1]->Text="����������� ������������.";
    Form1->Update();
    // ���������� � �������� ������� ����������� ��� �������������� ���������� ����.
    for (TSignal::const_reverse_iterator i = paramsReverse->getB()->rbegin(); i != paramsReverse->getB()->rend(); ++i)
    {
        B.push_back(*i);    
    }

    for (TSignal::const_reverse_iterator i = paramsReverse->getHallEffect()->rbegin(); i != paramsReverse->getHallEffect()->rend(); ++i)
    {
        Hall.push_back(*i);    
    }

    for (TSignal::const_reverse_iterator i = paramsReverse->getMagnetoResistance()->rbegin(); i != paramsReverse->getMagnetoResistance()->rend(); ++i)
    {
        Resistance.push_back(*i);    
    }
    // ���������� � ������ ������� ����������� ��� �������������� ����.
    for (TSignal::const_iterator i = paramsDirect->getB()->begin(); i != paramsDirect->getB()->end(); ++i)
    {
        B.push_back(*i);   
        B2.push_back(*i);  
    }

    for (TSignal::const_iterator i = paramsDirect->getHallEffect()->begin(); i != paramsDirect->getHallEffect()->end(); ++i)
    {
        Hall.push_back(*i);  
        Hall2.push_back(*i);   
    }

    for (TSignal::const_iterator i = paramsDirect->getMagnetoResistance()->begin(); i != paramsDirect->getMagnetoResistance()->end(); ++i)
    {
        Resistance.push_back(*i);  
        Resistance2.push_back(*i);   
    }

    TSignal outB;
    TSignal outHall;
    TSignal outResistance;

    size_t minimalLength=paramsReverse->getB()->size()>paramsDirect->getB()->size()?
    paramsDirect->getB()->size() : paramsReverse->getB()->size();

    StatusBar->Panels->Items[1]->Text="������������ ������������.";
    Form1->Update();
    // ���-�� ������ ��� ��� ������� �������� ������� �� ��� ��� ����...
    //thiningSignal(B, Hall, outB, outHall, -2, 0, 2*minimalLength);
    //thiningSignal(B, Resistance, outB, outResistance, -2, 2, 2*minimalLength);
    // � � ���� ���� ��������� ���������� ������ ��� ���.
    // �������� ����� ���������� ����� ������� ���������� ������ � ���������� ���������� ����� (������ ��� ��������)
    // ����� ��������� ��� ������� �������� � ����� ����� ���������� ��.
    thiningSignal(B, Hall, outB, outHall, -2, 2, 2*minimalLength);
    thiningSignal(B, Resistance, outB, outResistance, -2, 2, 2*minimalLength);

    Form1->ErrorLog->Lines->Add(FloatToStr( B.size()));
    
    StatusBar->Panels->Items[1]->Text="��������� ����� ����������.";
    Form1->Update();
    params->setDependence(outB.begin(),outB.end(),outHall.begin(),outResistance.begin());

    UpdatePlots();
    StatusBar->Panels->Items[1]->Text="������ � ������.";
}


void __fastcall TForm1::bUniteDependenceClick(TObject *Sender)
{
    concatDependence();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CurrentResChange(TObject *Sender)
{
    if((*ActiveParams()))
    {
    (*ActiveParams())->setSampleDescription(uiTemperature->Text,uiCurrent->Text,
        uiInventoryNumber->Text,uiSampleLength->Text,uiSampleWidth->Text,uiSampleThickness->Text);
    (*ActiveParams())->setParamsType(ResCurveIndex->ItemIndex);
    }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::uiCalculateTenzorClick(TObject *Sender)
{
    MagneticFieldDependence ** par=ActiveParams();
    MagneticFieldDependence * p;
    if (*par==NULL)
    {
        ShowMessage("�������� ������ �� ��� ������.");   
        return; 
    }
    else
    {
        (*ActiveParams())->setSampleDescription(uiTemperature->Text,uiCurrent->Text,
        uiInventoryNumber->Text,uiSampleLength->Text,uiSampleWidth->Text,uiSampleThickness->Text);
        (*ActiveParams())->setParamsType(ResCurveIndex->ItemIndex);
        p=*par;
        p->calculateTenzor(uiDataKind->ItemIndex==0?CURRENT_DATA:FILTERED_DATA);

        p->constructPlotFromTwoMassive(SXX,AVERAGED_DATA,Series6,clRed);
        p->constructPlotFromTwoMassive(SXY,AVERAGED_DATA,LineSeries1,clRed);

        ErrorLog->Lines->Add(p->getSxx()->size());
        ErrorLog->Lines->Add(p->getSxy()->size());
        UpdatePlots();
        DataSaver * tenzorSaver=new DataSaver(uiTemperature->Text,
        uiCurrent->Text, uiInventoryNumber->Text,uiSampleLength->Text,uiSampleWidth->Text,uiSampleThickness->Text);
        if(SaveDialog1->Execute())
        {
        tenzorSaver->SaveData(CURRENT_DATA,p->getAveragedB(),
        p->getSxy(), p->getSxx(), ALL_POINTS,SaveDialog1->FileName);

        tenzorSaver->SaveData(CURRENT_DATA,p->getAveragedB(),
        p->getSxy(), p->getSxx(), POINTS_11,SaveDialog1->FileName);
        }
        delete tenzorSaver;
    }  

    


}
//---------------------------------------------------------------------------



void __fastcall TForm1::eLengthFilterResChange(TObject *Sender)
{
eLengthFilterHall->Text=eLengthFilterRes->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::eLengthFilterHallChange(TObject *Sender)
{
eLengthFilterRes->Text=eLengthFilterHall->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HallCurveIndexClick(TObject *Sender)
{
ResCurveIndex->ItemIndex=HallCurveIndex->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ResCurveIndexClick(TObject *Sender)
{
HallCurveIndex->ItemIndex=ResCurveIndex->ItemIndex;    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bResShiftCurveClick(TObject *Sender)
{
if(*ActiveParams())
{
/*
        �������������� Initializing Params Type with const int - �� ���������,
        �.�. �������� ��� ��� ��������� � �������������, �� ���������� � ���� �����.
        */
(*ActiveParams())->shiftCurve(uiDataKind->ItemIndex,MAGNETORESISTANCE,
StrToFloat(uiShiftValue->Text),StrToFloat(uiLeftBound->Text),StrToFloat(uiRightBound->Text));
UpdatePlots();
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bShiftHallCurveClick(TObject *Sender)
{
if(*ActiveParams())
{
(*ActiveParams())->shiftCurve(uiHallDataKind->ItemIndex,HALL_EFFECT,
StrToFloat(uiHallShiftValue->Text),StrToFloat(uiHallLeftBound->Text),StrToFloat(uiHallRightBound->Text));
UpdatePlots();
}
else
{
    ShowMessage("��������� ������ ����");
}
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ComboBox5Change(TObject *Sender)
{
switch(ComboBox5->ItemIndex)
{
case 0:
Label31->Caption="1� �����(�������������):"; // 1� �����
break;
case 1:
Label33->Caption="2� �����(�������������):";; // 2� �����
break;
case 2:
Label32->Caption="3� �����(�������������):";; // 3� �����
break;
default:
break;
}
switch(ComboBox6->ItemIndex)
{
case 0:
Label31->Caption="1� �����(����):"; // 1� �����
break;
case 1:
Label33->Caption="2� �����(����):";; // 2� �����
break;
case 2:
Label32->Caption="3� �����(����):";; // 3� �����
break;
default:
break;
}
switch(ComboBox4->ItemIndex)
{
case 0:
Label31->Caption="1� �����(����):"; // 1� �����
break;
case 1:
Label33->Caption="2� �����(����):";; // 2� �����
break;
case 2:
Label32->Caption="3� �����(����):";; // 3� �����
break;
default:
break;
}
}
//---------------------------------------------------------------------------





void __fastcall TForm1::Button2Click(TObject *Sender)
{
MagneticFieldDependence ** par=ActiveParams();

if (*par==NULL)
{
    ShowMessage("�������� ������ �� ��� ������.");
}
else
{
    MagneticFieldDependence * p=*par;
    p->setFilterParamsResistance(eSamplingFRes->Text, eBandWidthFRes->Text,
     eAttenuationFRes->Text, eLengthFilterRes->Text);
    p->setFilterParamsHall(eSamplingFHall->Text, eBandWidthFHall->Text,
     eAttenuationFHall->Text, eLengthFilterHall->Text);
    p->blockfilterData();
    p->extrapolateData(FILTERED_DATA,PowPolinomRes->Text.ToInt(),PowPolinomHall->Text.ToInt());

    UpdatePlots();
}
}
//---------------------------------------------------------------------------

int findMaximum(std::vector<long double> & diffY,int start)
{
    const long double min=10E-16;
    long double max;
    int i_max=diffY.size();
    max=min;
    for (int i = start; i < diffY.size(); ++i)
    {
        if(diffY[i]>max)
        {
            max=diffY[i];
            i_max=i;
        }
    }

    if (i_max==start && start>0 && diffY[i_max-1]>max)
    {
        i_max=-1;
    }

    //while (i<diffY.size()-2 && diffY[i+1]>=diffY[i]) ++i;
    //--i;
    //int i2=start;
   // while(i<diffY.size()-2 && diffY[])
    return i_max;
}

void calculateMobilitySpectrum(TSignal &B,TSignal &sxx,TSignal &sxy,int length)
{
      if(length==0)
      {
      Form1->ErrorLog->Lines->Add("����� � ������� ������� ����������� ����� ����. �� ���� �������.");
        return;
      }

      mobilitySpectrum c(B,sxx,sxy,B.size());

      int size=c.getResultSize();

      long double * ex=new long double [size];
      long double * eY=new long double [size];
      long double * hx=new long double [size];
      long double * hY=new long double [size];

      Form1->Series1->Clear();
      Form1->Series2->Clear();
      Form1->Series4->Clear();

      Form1->Chart1->LeftAxis->Logarithmic=true;
      Form1->Chart1->BottomAxis->Logarithmic=true;

      TStringList *tsl = new TStringList;


      for(int i =0;i<size;i++)
      {

      ex[i]=c.getResultEX(i);
      eY[i]=c.getResultEY(i);
      hx[i]=c.getResultHX(i);
      hY[i]=c.getResultHY(i);
      tsl->Add(FloatToStr(ex[i])+"\t"+FloatToStr(eY[i])+"\t"+FloatToStr(hY[i]));
      Form1->Series1->AddXY(ex[i],eY[i],"",clBlue);
      Form1->Series2->AddXY(hx[i],hY[i],"",clRed);
      }

      tsl->SaveToFile("mobilitySpectrum.txt");

      std::vector<long double> diffeY;
      for (int i = 1; i < size; ++i)
      {
          diffeY.push_back(fabs((eY[i]-eY[i-1])));
          Form1->Series4->AddXY(ex[i-1],diffeY[i-1],"",clGreen);
      }

    for(int i=0; i< diffeY.size();++i)
    {
    int p=findMaximum(diffeY,i);

    if (p!=diffeY.size()-1 && p>=0)
    {
        addPeak(Form1->Series1,p);
    }

    if(p>i) i = p;

    }
    
        
      

      


    delete [] ex;
    delete [] eY;
    delete [] hx;
    delete [] hY;

}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
/*
��� ����� ����� ����� ��������� � �������� �����.

*/

StringGrid1->Cells[0][1]="������� �����";
StringGrid1->Cells[0][2]="������ �����";
StringGrid1->Cells[0][3]="���������";

StringGrid1->Cells[1][0]="������������";
StringGrid1->Cells[2][0]="�����������";



//-------------------------------------------------------------


long double B[]={0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0};
long double sxx[]={42.2179,42.172,42.0579,41.8866,41.6706,41.4251,41.165,40.9024,40.646,40.4014,40.1721};
long double sxy[]={0.0,0.558,1.1061,1.6173,2.0797,2.4883,2.8441,3.1511,3.4162,3.6464,3.8487};

int testSize=11;
vector <long double> B2;
vector <long double> sxx2;
vector <long double> sxy2;
for(int i=0;i<testSize;++i)
{
B2.push_back(B[i]);
sxx2.push_back(sxx[i]);
sxy2.push_back(sxy[i]);
}
mobilitySpectrum c(B2,sxx2,sxy2,testSize);

    int size=c.getResultSize();
      long double * ex=new long double [size];
      long double * eY=new long double [size];
      long double * hx=new long double [size];
      long double * hY=new long double [size];
      
      Series1->Clear();
      Series2->Clear();
      Chart1->LeftAxis->Logarithmic=true;
      Chart1->BottomAxis->Logarithmic=true;

      
      for(int i =0;i<size;i++)
      {

      ex[i]=c.getResultEX(i);
      eY[i]=c.getResultEY(i);
      hx[i]=c.getResultHX(i);
      hY[i]=c.getResultHY(i);

      Series1->AddXY(ex[i],eY[i],"",clBlue);
      Series2->AddXY(hx[i],hY[i],"",clRed);
      }

      delete [] ex;
      delete [] eY;
      delete [] hx;
      delete [] hY;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bMobilitySpectrumClick(TObject *Sender)
{
    MagneticFieldDependence ** par=ActiveParams();
    MagneticFieldDependence * p;
    if (*par==NULL)
    {
        ShowMessage("�������� ������ �� ��� ������.");
        return;
    }
    else
    {
        p=*par;

        TSignal B(p->getAveragedB()->begin(),p->getAveragedB()->end());
        TSignal sxx(p->getSxx()->begin(),p->getSxx()->end());
        TSignal sxy(p->getSxy()->begin(),p->getSxy()->end());

        

        TSignal nB;
        TSignal nSxx;
        TSignal nSxy;

        thiningSignal(B, sxx, nB, nSxx,0, 2, 21);
        thiningSignal(B, sxy, nB, nSxy,0, 2, 21);

        calculateMobilitySpectrum(nB,nSxx,nSxy,nB.size());        
    }
}
//---------------------------------------------------------------------------

void addPeak(TChartSeries *Sender,int ValueIndex)
{
    long double electronCharge=1.602e-19;
    long double G_p=Sender->YValues->Value[ValueIndex];
    long double Mu= Sender->XValues->Value[ValueIndex];
    long double Concentration=G_p/(Mu*electronCharge);

    Form1->StringGrid1->Cells[2][Form1->StringGrid1->Selection.Top]= FloatToStr( Mu);
    Form1->StringGrid1->Cells[1][Form1->StringGrid1->Selection.Top]= FloatToStr(Concentration);

    TGridRect tgr=Form1->StringGrid1->Selection;
    tgr.Top++;
    tgr.Bottom++;
    if(tgr.Top>3)
    {
        tgr.Top=1;
        tgr.Bottom=1;
    }
    Form1->StringGrid1->Selection =tgr;
}

void __fastcall TForm1::Series1Click(TChartSeries *Sender, int ValueIndex,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{   
    addPeak(Sender, ValueIndex);    
}
//---------------------------------------------------------------------------





void __fastcall TForm1::N12Click(TObject *Sender)
{
    MagneticFieldDependence ** par=ActiveParams();
    MagneticFieldDependence * p;
    if (*par==NULL)
    {
        ShowMessage("�������� ������ �� ��� ������.");
        return;
    }
    else
    {
        p=*par;
        p->rearrangeSignal();
        UpdatePlots();
    }


}
//---------------------------------------------------------------------------

void __fastcall TForm1::N13Click(TObject *Sender)
{
MagneticFieldDependence ** par=ActiveParams();
    MagneticFieldDependence * p;
    if (*par==NULL)
    {
        //ShowMessage("�������� ������ �� ��� ������.");
        return;
    }
    else
    {
        p=*par;
    }
    
if(PC->ActivePageIndex==1) // �������������
{
    p->multiplySignal(MAGNETORESISTANCE,-1);
}

if(PC->ActivePageIndex==2) // ����
{
    p->multiplySignal(HALL_EFFECT,-1);
}
    UpdatePlots();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N17Click(TObject *Sender)
{

if(out1->XValues->Count()!=0)
{ out1->Clear();
  out2->Clear();
}
else
UpdatePlots();


     /*
p->constructPlotFromTwoMassive(HALL_EFFECT,CURRENT_DATA,SeriesHall1,clRed);
    p->constructPlotFromTwoMassive(HALL_EFFECT,FILTERED_DATA,SeriesHall2,clBlue);
    p->constructPlotFromTwoMassive(HALL_EFFECT,EXTRAPOLATED_DATA,out2,clBlack);

    p->constructPlotFromTwoMassive(MAGNETORESISTANCE,CURRENT_DATA,SeriesRes1,clRed);
    p->constructPlotFromTwoMassive(MAGNETORESISTANCE,FILTERED_DATA,SeriesRes2,clBlue);
    p->constructPlotFromTwoMassive(MAGNETORESISTANCE,EXTRAPOLATED_DATA,out1,clBlack);
          */
}
//---------------------------------------------------------------------------


void __fastcall TForm1::N16Click(TObject *Sender)
{
if(SeriesHall2->XValues->Count()!=0)
{ SeriesHall2->Clear();
  SeriesRes2->Clear();
}
else
UpdatePlots();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bTestClick(TObject *Sender)
{
size_t size=24000;
std::vector<long double> in;
std::vector<long double> out;
for(unsigned int i=0;i<size;++i)
{
in.push_back(rand());
}
medianFilter(in,out,11);
ErrorLog->Lines->Add("Number for in"+IntToStr(in.size()));
ErrorLog->Lines->Add("Number for out"+IntToStr(out.size()));
for(unsigned int i=0;i<in.size();++i)
{
Series1->AddY(in[i]);
}
for(unsigned int i=0;i<out.size();++i)
{
Series2->AddY(out[i]);
}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::uiFrenqChange(TObject *Sender)
{
uiSamplingFreq->Text=FloatToStr( StrToFloat(uiFrenq->Text)/StrToFloat(eMedianFilterSize->Text)); 
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
    typedef int __stdcall (*pointerToRunMultizoneFeat)
    (long double VesGxx,
    long double VesGxy,
    long double * LowBound,
    long double * UpBound,
    long double * MagSpectr,
    long double * GxxIn,
    long double * GxyIn,

    long double *outGxx,
    long double *outGxy,

    long double ** outValues,
    int GxxSize,
    int numberOfCarrierTypes);

    

    HANDLE hLibHandle=NULL;
    hLibHandle = LoadLibrary("lib\\MultizoneFeat.dll");
    
    if (hLibHandle)
    {
        long double B[11]={0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0};
        long double sxx[11]={42.2179,42.172,42.0579,41.8866,41.6706,41.4251,41.165,40.9024,40.646,40.4014,40.1721};
        long double sxy[11]={0.0,0.558,1.1061,1.6173,2.0797,2.4883,2.8441,3.1511,3.4162,3.6464,3.8487};

        long double lowBound[6]={-7.179, 0.239315, 0.011994, -3.4E16, 2.2539E19, 4.938E21};
        long double upBound[6]={-2.39315, 0.718, 0.036, -1.14E16, 6.7617E19, 1.481E22};

        int sizeBuf=11;
        int numberOfCarrierTypes=3;

        long double * outGxx= new long double [sizeBuf];
        long double * outGxy= new long double [sizeBuf];
        long double ** outValues= new long double*[2*numberOfCarrierTypes+2];

        for (int i = 0; i < 2*numberOfCarrierTypes+2; ++i)
        {
            outValues[i]=new long double [4];
        }
        
        for (int i = 0; i < sizeBuf; ++i)
        {
            outGxx[i]=0;
            outGxy[i]=0;
        }

        for (int i = 0; i < 2*numberOfCarrierTypes+2; ++i)
        {
            outValues[i][0]=0;
            outValues[i][1]=0;
            outValues[i][2]=0;
            outValues[i][3]=0;
        }

        pointerToRunMultizoneFeat RunMultizoneFeat = (pointerToRunMultizoneFeat)GetProcAddress(hLibHandle,"RunMultizoneFeat");
        if (!RunMultizoneFeat)
        {
            ErrorLog->Lines->Add("�� ���� ������������� �������.");
            return;
        }
        ErrorLog->Lines->Add(sizeof(long double));
        (*RunMultizoneFeat)(1,1,lowBound,upBound,B,sxx,sxy,
        outGxx,outGxy,outValues,sizeBuf,numberOfCarrierTypes);

        ErrorLog->Lines->Add("Preparing is finished");
        for(int i=0;i<11;++i)
        {
         //  Series6->AddXY(B[i],outGxx[i],"",clRed);
         //  LineSeries1->AddXY(B[i],outGxy[i],"",clRed);
        }
        ErrorLog->Lines->Add("minValues");
        for(int i=0;i<2*numberOfCarrierTypes;++i)
        {
          ErrorLog->Lines->Add(FloatToStr(outValues[i][0]));
        }
        ErrorLog->Lines->Add("middle");
        for(int i=0;i<2*numberOfCarrierTypes+1;++i)
        {
           ErrorLog->Lines->Add(FloatToStr(outValues[i][1]));
        }
        ErrorLog->Lines->Add("SKO");
        for(int i=0;i<2*numberOfCarrierTypes+1;++i)
        {
           ErrorLog->Lines->Add(FloatToStr(outValues[i][2]));
        }
        ErrorLog->Lines->Add("SKOPers");
        for(int i=0;i<2*numberOfCarrierTypes+1;++i)
        {
           ErrorLog->Lines->Add(FloatToStr(outValues[i][3]));
        }  

        
         

        delete[] outGxx;
        delete[] outGxy;
        for (int i = 0; i < 2*numberOfCarrierTypes+2; ++i)
        {
            delete[] outValues[i];
        }
        delete[] outValues;

         
        if ( hLibHandle )
        {
        FreeLibrary( hLibHandle );
        hLibHandle=NULL;
        }
    }
    else
    {
        ShowMessage("����� ����� ���� �_�");
    }


}
//---------------------------------------------------------------------------

void __fastcall TForm1::N19Click(TObject *Sender)
{
Form4->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
if(!testExtrapolateUnit()) ShowMessage("�� ������� ���� �� �������������");
TSignal inX;
TSignal inY;
inX.push_back(0);
inY.push_back(1.92);
	for (int i = 0; i < 3; ++i)
	{
		inX.push_back(inX.back()+0.02);
        inY.push_back(inX.back()*inX.back());
	}
TSignal newX;
newX.push_back(0);
	for (int i = 0; i < 151; ++i)
	{
		newX.push_back(newX.back()+0.02);
	}

TSignal outY;

LagrangeExtrapolation(inX,inY,newX,outY);

for (int i=0;i<inX.size();++i)
{
SeriesRes1->AddXY(inX[i],inY[i],"",clRed);
}

for (int i=0;i<newX.size();++i)
{
if(fabs(outY[i])<1000)
SeriesRes2->AddXY(newX[i],outY[i],"",clGreen);
}
}
//---------------------------------------------------------------------------


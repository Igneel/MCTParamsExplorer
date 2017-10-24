//---------------------------------------------------------------------------
#pragma hdrstop

#include "Unit1.h"
#include "multizoneFit.h"
#include "smartCalculation.h"
#include <Windows.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

/*
TODO
����������� ������ "������" - �.�. ������� ���������� �������� � ������ ������ �����
������� �������� ������������� ��������� �����
� ��� ����� ����� � ����, ����� ������� �������� ������ - � �������� ��� �� ����� ������.

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
    /*
    �������
    p->constructPlotFromOneMassive(HALL_EFFECT,SeriesHall1,clBlue);
    p->constructPlotFromOneMassive(MAGNETORESISTANCE,SeriesRes1,clBlue);
    p->constructPlotFromOneMassive(MAGNETIC_FIELD,Series1,clBlue);
    
    p->constructPlotFromOneMassive(MAGNETIC_FIELD,Series1,clBlue);
    p->constructPlotFromOneMassive(MAGNETIC_FIELD_F,Series2,clRed);
    */

    // ���������� ���� ������������ ��������.
    if(!p->constructPlotFromTwoMassive(HALL_EFFECT,CURRENT_DATA,SeriesHall1,clRed))
        ErrorLog->Lines->Add("����. ������� ������. �� ������� ��������� ������.");
    if(!p->constructPlotFromTwoMassive(HALL_EFFECT,FILTERED_DATA,SeriesHall2,clBlue))
        ErrorLog->Lines->Add("����. ������������� ������. �� ������� ��������� ������.");
    //if(!p->constructPlotFromTwoMassive(HALL_EFFECT,EXTRAPOLATED_DATA,out2,clBlack))
    //    ErrorLog->Lines->Add("����. ������������������ ������. �� ������� ��������� ������.");

    if(!p->constructPlotFromTwoMassive(HALL_EFFECT,AVERAGED_DATA,SeriesFFTHall,clGreen))
        ErrorLog->Lines->Add("����. ���������� ������. �� ������� ��������� ������.");

    if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,AVERAGED_DATA,SeriesFFTRes,clGreen))
        ErrorLog->Lines->Add("�������������. ���������� ������. �� ������� ��������� ������.");

    if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,CURRENT_DATA,SeriesRes1,clRed))
        ErrorLog->Lines->Add("�������������. ������� ������. �� ������� ��������� ������.");
    if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,FILTERED_DATA,SeriesRes2,clBlue))
        ErrorLog->Lines->Add("�������������. ������������� ������. �� ������� ��������� ������.");
    //if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,EXTRAPOLATED_DATA,out1,clBlack))
    //    ErrorLog->Lines->Add("�������������. ������������������ ������. �� ������� ��������� ������.");
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}


void __fastcall TForm1::FormCreate(TObject *)
{

    // �������� ��������.
    _di_IXMLNode node = XMLsettings->ChildNodes->FindNode("Settings");
    if(node)
    {
    _di_IXMLNode node2=node->ChildNodes->FindNode("Resistance");
        if(node2)
        {
            _di_IXMLNode node3=node2->ChildNodes->FindNode("FilterParams");
            if(node3)
            {
                _di_IXMLNode node4=node3->ChildNodes->FindNode("Length");
                if(node4)
                {
                    eLengthFilterRes->Text = node4->GetText();
                }

                node4=node3->ChildNodes->FindNode("eSamplingFRes");
                if(node4)
                {
                     eSamplingFRes->Text=node4->GetText();
                }
                node4=node3->ChildNodes->FindNode("eBandWidthFRes");
                if(node4)
                {
                     eBandWidthFRes->Text=node4->GetText();
                }
                node4=node3->ChildNodes->FindNode("eAttenuationFRes");
                if(node4)
                {
                     eAttenuationFRes->Text=node4->GetText();
                }
                node4=node3->ChildNodes->FindNode("PolinomPow");
                if(node4)
                {
                     PowPolinomRes->Text=node4->GetText();
                }
            }

            node3=node2->ChildNodes->FindNode("CurveNumber");
            if(node3)
            {
                ResCurveIndex->ItemIndex = StrToInt(node3->GetText());
            }

            node3=node2->ChildNodes->FindNode("uiDataKind");
            if(node3)
            {
                uiDataKind->ItemIndex=StrToInt(node3->GetText());
            }

            
        }
        node2=node->ChildNodes->FindNode("HallEffect");
        if(node2)
        {
        _di_IXMLNode node3=node2->ChildNodes->FindNode("FilterParams");
            if(node3)
            {
                _di_IXMLNode node4=node3->ChildNodes->FindNode("Length");
                if(node4)
                {
                     eLengthFilterHall->Text=node4->GetText();
                }

                node4=node3->ChildNodes->FindNode("eSamplingFHall");
                if(node4)
                {
                     eSamplingFHall->Text=node4->GetText();
                }
                node4=node3->ChildNodes->FindNode("eBandWidthFHall");
                if(node4)
                {
                     eBandWidthFHall->Text=node4->GetText();
                }
                node4=node3->ChildNodes->FindNode("eAttenuationFHall");
                if(node4)
                {
                     eAttenuationFHall->Text=node4->GetText();
                }
                node4=node3->ChildNodes->FindNode("PolinomPow");
                if(node4)
                {
                     PowPolinomHall->Text=node4->GetText();
                }
            }
            node3=node2->ChildNodes->FindNode("CurveNumber");
            if(node3)
            {
                HallCurveIndex->ItemIndex = StrToInt(node3->GetText());
            }
    }
}

    MobSpecResults->Cells[0][1]="������� �����";
    MobSpecResults->Cells[0][2]="������ �����";
    MobSpecResults->Cells[0][3]="���������";
    MobSpecResults->Cells[1][0]="������������";
    MobSpecResults->Cells[2][0]="�����������";

    FitResults->Cells[1][0]="Mu e";
    FitResults->Cells[4][0]="N e";
    FitResults->Cells[2][0]="Mu lh";
    FitResults->Cells[5][0]="P lh";
    FitResults->Cells[3][0]="Mu hh";
    FitResults->Cells[6][0]="P hh";
    FitResults->Cells[0][1]="����������� ��������";
    FitResults->Cells[0][2]="������� ��������";
    FitResults->Cells[0][3]="���";
    FitResults->Cells[0][4]="���, %%";
    

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
if(adc)
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

void __fastcall TForm1::openFileWithSignal(AnsiString filename)
{
        TStringList *tts=new TStringList();  // ���� ����� ��������� �� �����
        tts->LoadFromFile(filename);// ���������

        MagneticFieldDependence * p=InitParams();
        if (p)
        {
            StatusBar->Panels->Items[2]->Text=filename;
            TStringList *Names=new TStringList();
            TStringList *Values=new TStringList();

            p->loadSampleDescription(Names,Values,OpenDialog1->Files->Strings[0]);
            UpdateSampleDescription(Names,Values);
            p->loadData(tts);
            UpdatePlots();
            delete Names;
            delete Values;
        }
        delete tts;
}

void __fastcall TForm1::N4Click(TObject *Sender)
{ 
    if(OpenDialog1->Execute())  // ���� �� ���-�� �������
    {
        openFileWithSignal(OpenDialog1->Files->Strings[0]);
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

  _di_IXMLNode node = XMLsettings->ChildNodes->FindNode("Settings");
    if(node)
    {
    _di_IXMLNode node2=node->ChildNodes->FindNode("Resistance");
        if(node2)
        {
            _di_IXMLNode node3=node2->ChildNodes->FindNode("FilterParams");
            if(node3)
            {
                _di_IXMLNode node4=node3->ChildNodes->FindNode("Length");
                if(node4)
                {
                     node4->SetText(eLengthFilterRes->Text);
                }

                node4=node3->ChildNodes->FindNode("eSamplingFRes");
                if(node4)
                {
                     node4->SetText(eSamplingFRes->Text);
                }
                node4=node3->ChildNodes->FindNode("eBandWidthFRes");
                if(node4)
                {
                     node4->SetText(eBandWidthFRes->Text);
                }
                node4=node3->ChildNodes->FindNode("eAttenuationFRes");
                if(node4)
                {
                     node4->SetText(eAttenuationFRes->Text);
                }
                node4=node3->ChildNodes->FindNode("PolinomPow");
                if(node4)
                {
                     node4->SetText(PowPolinomRes->Text);
                }
            }
            node3=node2->ChildNodes->FindNode("CurveNumber");
            if(node3)
            {
                node3->SetText(IntToStr(ResCurveIndex->ItemIndex));
            }

            node3=node2->ChildNodes->FindNode("uiDataKind");
            if(node3)
            {
                node3->SetText(IntToStr(uiDataKind->ItemIndex));
            }
        }

        node2=node->ChildNodes->FindNode("HallEffect");
        if(node2)
        {
        _di_IXMLNode node3=node2->ChildNodes->FindNode("FilterParams");
            if(node3)
            {
                _di_IXMLNode node4=node3->ChildNodes->FindNode("Length");
                if(node4)
                {
                     node4->SetText(eLengthFilterHall->Text);
                }

                node4=node3->ChildNodes->FindNode("eSamplingFHall");
                if(node4)
                {
                     node4->SetText(eSamplingFHall->Text);
                }
                node4=node3->ChildNodes->FindNode("eBandWidthFHall");
                if(node4)
                {
                     node4->SetText(eBandWidthFHall->Text);
                }
                node4=node3->ChildNodes->FindNode("eAttenuationFHall");
                if(node4)
                {
                     node4->SetText(eAttenuationFHall->Text);
                }
                node4=node3->ChildNodes->FindNode("PolinomPow");
                if(node4)
                {
                     node4->SetText(PowPolinomHall->Text);
                }
            }

            node3=node2->ChildNodes->FindNode("CurveNumber");
            if(node3)
            {
                node3->SetText(IntToStr(HallCurveIndex->ItemIndex));
            }
        }
    }




if(adc)
    adc->StopMeasurement();

    
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
// ��������� ��.
void __fastcall TForm1::N11Click(TObject *Sender)
{
    CurrentResChange(NULL);
    if(params && paramsDirect && paramsReverse)
    {
        SaveDialog1->Title="���������� ���� ������:";
        if(SaveDialog1->Execute())
        {
            params->SaveAllData(SaveDialog1->FileName+"Com");
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
            params->SaveAllData(SaveDialog1->FileName+"Com");
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

void __fastcall TForm1::calculateTenzor()
{
    MagneticFieldDependence ** par=ActiveParams();
    MagneticFieldDependence * p;
    if (*par==NULL)
    {
        ErrorLog->Lines->Add("�������� ������ �� ��� ������.");   
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

        UpdatePlots();
    }
}

void __fastcall TForm1::uiCalculateTenzorClick(TObject *Sender)
{
    MagneticFieldDependence ** par=ActiveParams();
    MagneticFieldDependence * p;
    if (*par==NULL)
    {
        ErrorLog->Lines->Add("�������� ������ �� ��� ������.");   
        return; 
    }
    else
    {
        calculateTenzor();    
        
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

size_t searchSignalSlowdown(long double * y, size_t size, size_t startPosition, long double h)
{
    /*
    ������� ������ ����������� �� ���������� ��������� �������.
    �.�. �� ������������ ���.
    */
    if(startPosition>=size)
        return size;

      size_t dsize=size-2;
      size_t d2size=dsize-2;
      // ��������� ����������� ������� �������� ���������
      // ������� df/dx=1/h*(2*f(x+h)-f(x+2h)/2-3/2*f(x))

      // ������� df/dx=(f(x+h)-f(x-h))/2/h;
      std::vector<long double> dY(size);

      for(int i =0;i<dsize;i++)
      {
        dY[i]=(y[i+2]-y[i])/2.0/h;
      }

      std::vector<long double> d2Y(size);

      for(int i =0;i<d2size;i++)
      {
        //d2Y[i]=(dY[i+2]-dY[i])/2.0/h;
        d2Y[i]=(y[i]-2*y[i+1]+y[i+2])/h/h;
      }

      TStringList *tsl = new TStringList;


      for(int i =0;i<d2size;i++)
      {
      tsl->Add(FloatToStr(dY[i])+"\t"+FloatToStr(d2Y[i]));      
      }                          

      tsl->SaveToFile("mobilitySpectrumLogDerivative.txt");

      delete tsl;

      for (int i = startPosition; i < dsize-1; ++i)
        {
            /*
            ����� �����:
            1. ���� ������� �� ������� ������ ����������� �������������.
            */
            while (i<dsize-1 && (dY[i]>0)) ++i;

            while (i<dsize-1 && (dY[i]<0 && dY[i+1]-dY[i]<0)) ++i; // ������ ����������� ������������� � �����������

            while (i<dsize-1 && (dY[i]<0 && dY[i+1]-dY[i]>0)) ++i; // ������ ����������� ������������� � �������������

            --i;
            // ������ �������� ��������� ������� - ������ ����������� ������ �������� ����.
            // �������� ����� ��������� �������� ������ �� +-10 ��������
            if (i+10<dsize-1 && i-10>=0 && (d2Y[i-10]>0 && d2Y[i+10]<0) )
            {
                return i;
            }
            if( i<startPosition)
            {
            return size;
            }
        }
        return size;

}

size_t searchSignificantPeak(long double * y, size_t size, size_t startPosition, long double h)
{
    if(startPosition>=size)
        return size;

      size_t dsize=size-2;
      size_t d2size=dsize-2;
      // ��������� ����������� ������� �������� ���������
      // ������� df/dx=1/h*(2*f(x+h)-f(x+2h)/2-3/2*f(x))

      // ������� df/dx=(f(x+h)-f(x-h))/2/h;

      std::vector<long double> dY(size);

      for(int i =0;i<dsize;i++)
      {
        //dY[i]=1.0/(fabs(y[i+1]-y[i]))*(2.0*y[i+1]-y[i+2]/2.0-3.0/2.0*y[i]);
        dY[i]=(y[i+2]-y[i])/2.0/h;
      }

      std::vector<long double> d2Y(size);

      for(int i =0;i<d2size;i++)
      {
        //d2Y[i]=1.0/(fabs(dY[i+1]-dY[i]))*(2.0*dY[i+1]-dY[i+2]/2.0-3.0/2.0*dY[i]);
        // ������� f(x-h)-2f(x)+f(x+h)/h^2
        d2Y[i]=(y[i]-2*y[i+1]+y[i+2])/h/h;
        //d2Y[i]=(dY[i+2]-dY[i])/2.0/h;
      }

      TStringList *tsl = new TStringList;


      for(int i =0;i<d2size;i++)
      {
      tsl->Add(FloatToStr(dY[i])+"\t"+FloatToStr(d2Y[i]));      
      }                          

      tsl->SaveToFile("mobilitySpectrumDerivative.txt");

      delete tsl;

      /*
        ����� �����. ������� ����������� ������� � ������� ��������.
        ����� ���� ���������� ���� ������ ����� ����� ��������:
        1. ������ ����������� ������� ������ � ������������.
        2. ����� ���� - ������� � ������������ 

        3. ������ ����������� �� ���� - ������������.
        4. ����� ���� - ������������.
        5. ���-������ �����, ��� ��� ��������� ��� - ������ �����������, ��� �������, �.�. ��� ������ ���� ����� �������.
      */

        for (int i = startPosition; i < dsize-1; ++i)
        {
            /*
            ����� �����:
            1. ���� ������� �� ������� ������ ����������� ������������ � ������.
            */
            while (i<dsize-1 && (dY[i]<0)) ++i;
            // ���� �����, � ������� ���� ������ ����������� ������������
            while (i<dsize-1 && (dY[i]>0)) ++i;

            --i;
            // ������ �������� ��������� ������� - ������ ����������� ������������
            // �������� ����� ��������� �������� ������ �� +-10 ��������
            if (d2Y[i]<0 )
            {
                return i;
            }
        }
        return size;
}

long double calcConcentrationFromGp(long double G_p, long double Mu)
{
    long double electronCharge=1.602e-19;
    return G_p/(Mu*electronCharge);
}

void calculateMobilitySpectrum(TSignal &B,TSignal &sxx,TSignal &sxy,int length)
{
      if(length==0)
      {
      Form1->ErrorLog->Lines->Add("����� � ������� ������� ����������� ����� ����. �� ���� �������.");
        return;
      }

    Form1->MobSpecResults->Cells[1][3]="";
    Form1->MobSpecResults->Cells[2][3]="";
    Form1->MobSpecResults->Cells[1][1]="";
    Form1->MobSpecResults->Cells[2][1]="";
    Form1->MobSpecResults->Cells[1][2]="";
    Form1->MobSpecResults->Cells[2][2]="";

      mobilitySpectrum c(B,sxx,sxy,B.size());

      int size=c.getResultSize();

      long double * ex=new long double [size];
      long double * eY=new long double [size];
      long double * hx=new long double [size];
      long double * hY=new long double [size];

      Form1->ChspElecComponent->Clear();
      Form1->ChSpHoleComponent->Clear();
      Form1->Series4->Clear();

      //Form1->Chart1->LeftAxis->Logarithmic=true;
      //Form1->Chart1->BottomAxis->Logarithmic=true;

      TStringList *tsl = new TStringList;


      for(int i =0;i<size;i++)
      {

      ex[i]=c.getResultEX(i);
      eY[i]=c.getResultEY(i);
      hx[i]=c.getResultHX(i);
      hY[i]=c.getResultHY(i);
      tsl->Add(FloatToStr(ex[i])+"\t"+FloatToStr(eY[i])+"\t"+FloatToStr(hY[i]));
      Form1->ChspElecComponent->AddXY(ex[i],eY[i],"",clBlue);
      Form1->ChSpHoleComponent->AddXY(hx[i],hY[i],"",clRed);
      }

      tsl->SaveToFile("mobilitySpectrum.txt");

      /*std::vector<long double> diffeY;
      for (int i = 1; i < size; ++i)
      {
          diffeY.push_back(fabs((eY[i]-eY[i-1])));
          Form1->Series4->AddXY(ex[i-1],diffeY[i-1],"",clGreen);
      }*/

      size_t index = searchSignificantPeak(eY,size,0, ex[1]-ex[0]); // ���������
      if(index!=size)
      {
          Form1->MobSpecResults->Cells[1][3]= FloatToStr(calcConcentrationFromGp(eY[index],ex[index])); // ������������
          Form1->MobSpecResults->Cells[2][3]= FloatToStr(ex[index]); // �����������
      }

      index = searchSignificantPeak(hY,size,0, hx[1]-hx[0]); // ������� �����
      if(index!=size)
      {
          Form1->MobSpecResults->Cells[1][1]= FloatToStr(calcConcentrationFromGp(hY[index],hx[index])); // ������������
          Form1->MobSpecResults->Cells[2][1]= FloatToStr(hx[index]); // �����������
      }

      index = searchSignificantPeak(hY, size, index+4, hx[1]-hx[0]); // ������ �����
      if(index!=size)
      {
          Form1->MobSpecResults->Cells[1][2]= FloatToStr(calcConcentrationFromGp(hY[index],hx[index])); // ������������
          Form1->MobSpecResults->Cells[2][2]= FloatToStr(hx[index]); // �����������
      }
      else
      {
        index = searchSignificantPeak(hY,size,0, hx[1]-hx[0]); // ������� �����
        index = searchSignalSlowdown(hY, size, index+4, hx[1]-hx[0]); // ������ �����
        if(index!=size)
      {
          Form1->MobSpecResults->Cells[1][2]= FloatToStr(calcConcentrationFromGp(hY[index],hx[index])); // ������������
          Form1->MobSpecResults->Cells[2][2]= FloatToStr(hx[index]); // �����������
      }
      }

    delete [] ex;
    delete [] eY;
    delete [] hx;
    delete [] hY;
    delete tsl;

}


void __fastcall TForm1::Button1Click(TObject *Sender)
{
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
      
      //ChspElecComponent->Clear();
      Series1->Clear();
      Series2->Clear();
      Series3->Clear();
      Series4->Clear();
      Series5->Clear();
      Chart1->LeftAxis->Logarithmic=true;
      Chart1->BottomAxis->Logarithmic=true;

      
      for(int i =0;i<size;i++)
      {

      ex[i]=c.getResultEX(i);
      eY[i]=c.getResultEY(i);
      hx[i]=c.getResultHX(i);
      hY[i]=c.getResultHY(i);

      Series1->AddXY(ex[i],eY[i],"",clBlue);
      //ChspElecComponent->AddXY(ex[i],eY[i],"",clBlue);
      Series2->AddXY(hx[i],hY[i],"",clRed);
      }

     

      size_t index = searchSignificantPeak(eY,size,0, ex[1]-ex[0]); // ���������
      if(index!=size)
      {
          MobSpecResults->Cells[1][3]= FloatToStr(calcConcentrationFromGp(eY[index],ex[index])); // ������������
          MobSpecResults->Cells[2][3]= FloatToStr(ex[index]); // �����������
      }

      index = searchSignificantPeak(hY,size,0, hx[1]-hx[0]); // ������� �����
      if(index!=size)
      {
          MobSpecResults->Cells[1][1]= FloatToStr(calcConcentrationFromGp(hY[index],hx[index])); // ������������
          MobSpecResults->Cells[2][1]= FloatToStr(hx[index]); // �����������
      }

      index = searchSignificantPeak(hY, size, index+4, hx[1]-hx[0]); // ������ �����
      if(index!=size)
      {
          MobSpecResults->Cells[1][2]= FloatToStr(calcConcentrationFromGp(hY[index],hx[index])); // ������������
          MobSpecResults->Cells[2][2]= FloatToStr(hx[index]); // �����������
      }
      
      TStringList *tosaving=new TStringList;

      for (int i =0;i<size;i++)
      {
      tosaving->Add(FloatToStr(ex[i])+"\t"+FloatToStr(eY[i])+"\t"+FloatToStr(hY[i]));
      }
      
      tosaving->SaveToFile("MobilityTestSpectrum.txt");

      delete [] ex;
      delete [] eY;
      delete [] hx;
      delete [] hY;
      delete tosaving;
}
//---------------------------------------------------------------------------


void addPeak(TChartSeries *Sender,int ValueIndex)
{
    long double electronCharge=1.602e-19;
    long double G_p=Sender->YValues->Value[ValueIndex];
    long double Mu= Sender->XValues->Value[ValueIndex];
    long double Concentration=G_p/(Mu*electronCharge);

    Form1->MobSpecResults->Cells[2][Form1->MobSpecResults->Selection.Top]= FloatToStrF( Mu, ffFixed, 5, 5);
    Form1->MobSpecResults->Cells[1][Form1->MobSpecResults->Selection.Top]= FloatToStrF(Concentration, ffExponent, 5, 2);

    TGridRect tgr=Form1->MobSpecResults->Selection;
    tgr.Top++;
    tgr.Bottom++;
    if(tgr.Top>3)
    {
        tgr.Top=1;
        tgr.Bottom=1;
    }
    Form1->MobSpecResults->Selection =tgr;

    // ���� ����� �������� ���������� �������� ����� � ������ ��������������� ������������!!!
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

void __fastcall TForm1::uiFrenqChange(TObject *Sender)
{
uiSamplingFreq->Text=FloatToStr( StrToFloat(uiFrenq->Text)/StrToFloat(eMedianFilterSize->Text)); 
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
if(!testCommonFunctions()) ShowMessage("�� ������� ���� �� ����� ��������");
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

void TForm1::getAndDisplayMultiCarrierFitResults(MagneticFieldDependence * p)
{
    int numberOfCarrierTypes=3;
    InDataSpectr nMagSpectr;
    InDataSpectr nGxxIn;
    InDataSpectr nGxyIn;
    // ���� ����������� �������� ��������.
    MyData_spektr outGxx;
    MyData_spektr outGxy;
    TStatistic outValues;

    p->getMultiCarrierFitResults(nMagSpectr, nGxxIn, nGxyIn,
    outGxx,  outGxy, outValues);

    // ����� ����������� �� ������� � �����.
    ChSxx_theor->Clear();
    ChSxy_theor->Clear();
    ChSxxExper->Clear();
    ChSxyExper->Clear();


    long double koefSxx=1, koefSxy=1;

    if (CheckBox3->Checked) // ������������� ������?
    {
        koefSxx=max_elem(outGxx);
        koefSxy=max_elem(outGxy);

        if (max_elem(nGxxIn)>koefSxx)
        {
            koefSxx=max_elem(nGxxIn);
        }
        if (max_elem(nGxyIn)>koefSxy)
        {
            koefSxy=max_elem(nGxyIn);
        }
    }

    for (int i = 0; i < outGxx.size(); ++i)
    {
        ChSxx_theor->AddXY(nMagSpectr[i],outGxx[i]/koefSxx,"",clRed);
        ChSxy_theor->AddXY(nMagSpectr[i],outGxy[i]/koefSxy,"",clRed);
    }

    for (int i = 0; i < nMagSpectr.size(); ++i)
    {
        ChSxxExper->AddXY(nMagSpectr[i],nGxxIn[i]/koefSxx,"",clBlue);
        ChSxyExper->AddXY(nMagSpectr[i],nGxyIn[i]/koefSxy,"",clBlue);
    }

    // ���������� ���� � �������:
    // �� ������� �������:
    // ����������� ����������, ����������� ������ �����, ����������� ������� �����
    // ������������ ����������, ������������ ������ �����, ������������ ������� �����
    // �� ������� �������: �����������, �������, ���, ��� %

    TStringList * tsl = new TStringList();

    for (int j = 0; j < 4; ++j)
    {

        for(int i=0;i<2*numberOfCarrierTypes;++i)
        {
            tsl->Add(FloatToStr(outValues[i][j]));
        }
    }

    tsl->SaveToFile(SaveDialog1->FileName+eBandWidthFRes->Text+eBandWidthFHall->Text+"MZFitRes.txt");

    // min
    for(int i=0;i<2*numberOfCarrierTypes;++i)
    {

        if(i<numberOfCarrierTypes)
            FitResults->Cells[i+1][1]=FloatToStrF(outValues[i][0],ffFixed,6,6);
        else
      FitResults->Cells[i+1][1]=FloatToStrF(outValues[i][0],ffExponent,5,2);
    }
    //ErrorLog->Lines->Add("middle");
    for(int i=0;i<2*numberOfCarrierTypes+1;++i)
    {
        if(i<numberOfCarrierTypes)
            FitResults->Cells[i+1][2]=FloatToStrF(outValues[i][1],ffFixed,6,6);
        else
       FitResults->Cells[i+1][2]=FloatToStrF(outValues[i][1],ffExponent,5,2);
    }
    //ErrorLog->Lines->Add("SKO");
    for(int i=0;i<2*numberOfCarrierTypes+1;++i)
    {
        if(i<numberOfCarrierTypes)
            FitResults->Cells[i+1][3]=FloatToStrF(outValues[i][2],ffFixed,6,6);
        else
       FitResults->Cells[i+1][3]=FloatToStrF(outValues[i][2],ffExponent,5,2);
    }
    //ErrorLog->Lines->Add("SKOPers");
    for(int i=0;i<2*numberOfCarrierTypes+1;++i)
    {
        if(i<numberOfCarrierTypes)
            FitResults->Cells[i+1][4]=FloatToStrF(outValues[i][3],ffFixed,6,6);
        else
       FitResults->Cells[i+1][4]=FloatToStrF(outValues[i][3],ffExponent,5,2);
    }
    delete tsl;
}

void __fastcall TForm1::btnMultiCarrierFitClick(TObject *Sender)
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

        StatusBar->Panels->Items[1]->Text="��� �������� ������";

        long double VesGxx=StrToFloat(LabeledEdit1->Text);
        long double VesGxy=StrToFloat(LabeledEdit2->Text);

        if(p->runMultiCarrierFit(VesGxx,VesGxy))
        {
            getAndDisplayMultiCarrierFitResults(p);
        }
    }
     StatusBar->Panels->Items[1]->Text="������ � ������.";
     
}
//---------------------------------------------------------------------------


void __fastcall TForm1::btnMobilitySpectrumClick(TObject *Sender)
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
        p->calculateMobilitySpectrum();

        TSignal ex(p->getMobility()->begin(),p->getMobility()->end());
        TSignal eY(p->getElectronConductivity()->begin(),p->getElectronConductivity()->end());
        TSignal hx(p->getMobility()->begin(),p->getMobility()->end());
        TSignal hY(p->getHoleConductivity()->begin(),p->getHoleConductivity()->end());
        if(p->getElectronConcentration()->size()>=1)
        {
        Form1->MobSpecResults->Cells[1][3]=FloatToStrF(p->getElectronConcentration()->operator[](0),ffExponent,5,2);
        Form1->MobSpecResults->Cells[2][3]=FloatToStrF(p->getElectronMobility()->operator[](0),ffFixed,5,5);
        }
        if(p->getHoleConcentration()->size()>=1)
        {
        Form1->MobSpecResults->Cells[1][1]=FloatToStrF(p->getHoleConcentration()->operator[](0),ffExponent,5,2);
        Form1->MobSpecResults->Cells[2][1]=FloatToStrF(p->getHoleMobility()->operator[](0),ffFixed,5,5);
        }
        if(p->getHoleConcentration()->size()>=2)
        {
        Form1->MobSpecResults->Cells[1][2]=FloatToStrF(p->getHoleConcentration()->operator[](1),ffExponent,5,2);
        Form1->MobSpecResults->Cells[2][2]=FloatToStrF(p->getHoleMobility()->operator[](1),ffFixed,5,5);
        }
        Form1->ChspElecComponent->Clear();
        Form1->ChSpHoleComponent->Clear();
        Form1->Series4->Clear();

      for(int i =0;i<ex.size();++i)
      {
      Form1->ChspElecComponent->AddXY(ex[i],eY[i],"",clBlue);
      Form1->ChSpHoleComponent->AddXY(hx[i],hY[i],"",clRed);
      }
    } 
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N25Click(TObject *Sender)
{
bFilterRes->Click();
calculateTenzor();
btnMobilitySpectrum->Click();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N26Click(TObject *Sender)
{
N25->Click();
btnMultiCarrierFit->Click();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::eMedianFilterSizeKeyPress(TObject *Sender,
      char &Key)
{
    if(((int)Key<48 || (int)Key>57) && Key!=8)
        Key=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::XMLsettingsBeforeOpen(TObject *Sender)
{
 AnsiString FilePath=ExtractFilePath(Application->ExeName);
 XMLsettings->FileName=FilePath+"settings.xml";   
}
//---------------------------------------------------------------------------

std::vector<std::string> getAllFileNamesWithinFolder(std::string folder)
{
    std::vector<std::string> names;
    char search_path[200];
    if (folder.length() <200)
    {
        sprintf(search_path, "%s/*.*", folder.c_str());
        WIN32_FIND_DATA fd; 
        HANDLE hFind = ::FindFirstFile(search_path, &fd); 
        if(hFind != INVALID_HANDLE_VALUE) { 
            do { 
                // read all (real) files in current folder
                // , delete '!' read other 2 default folder . and ..
                if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                    names.push_back(fd.cFileName);
                }
            }while(::FindNextFile(hFind, &fd)); 
            ::FindClose(hFind); 
        } 
        return names;
    }
    return names;
}

void filterFileNames(std::vector<std::string> & files, std::string rem)
{
    for (int i = 0; i < files.size(); ++i)
    {
        size_t n = files[i].find(rem);
        if(n!= std::string::npos)
            files[i]="";
    }
}

void __fastcall TForm1::Button3Click(TObject *Sender)
{
/*
������� ������ ������������:
1. �������� �����.
2. ����������, ������ ��������, ������� � ��������.
3. ���������� ���� �����������.
*/

// ������� ����� � ���������� ��� ����� � ���
if (OpenDialog1->Execute())
{

    std::string folder = ExtractFilePath(OpenDialog1->Files->Strings[0]).c_str();

    std::vector<std::string> files=getAllFileNamesWithinFolder(folder);

    // ����� ������� �� ������ �����, ���������� � ����� "Description"

    filterFileNames(files, "Description");

    // ������������� ����������� ��������
    // ���� ������������ ��� ������ ��� ��������� - ���� �� ������ ���������� �����.

    int temp=files.size();

    for (int j = 0; j < files.size(); ++j) // �� ���� ������ ������
    {
        if (files[j]!="")
        {
        openFileWithSignal(files[j].c_str());


        char folderPath[500];
        if (files[j].length() <500)
        {
            sprintf(folderPath, "%s%s1", folder.c_str(), files[j].c_str());

            if(CreateDirectory(folderPath,NULL)) // ������ ��������� ����� ��� ���������� ������ �� ������� �����.
            {
                std::string name=folder+files[j]+"1\\1"; // ���� ��������� � ��������� �����.
                long double step=0.1;
                for (long double BandWidthFHall=0.01, AttenuationFHall=0.1; BandWidthFHall < 10; BandWidthFHall+=step, AttenuationFHall+=step)
                {
                    if(step==0.1 && BandWidthFHall>1.0)
                    {
                        step=1.0;
                    }
                    eBandWidthFHall->Text=FloatToStr(BandWidthFHall);
                    eAttenuationFHall->Text=FloatToStr(AttenuationFHall);

                    AnsiString t=name.c_str()+FloatToStr(BandWidthFHall);
                    //eLengthFilterRes->Text="300";
                    long double step=0.1;
                    for (long double BandWidthFRes=0.01, AttenuationFRes=0.1; BandWidthFRes < 10; BandWidthFRes+=step, AttenuationFRes+=step) // ��� ����� �������� ���� �� ������ ���������� ����������
                    { // �� ��� ������ ���� ������ ������ ������� �����
                        if(step==0.1 && BandWidthFRes>1.0)
                        {
                            step=1.0;
                        }
                        AnsiString t2=t+FloatToStr(BandWidthFRes);
                        SaveDialog1->FileName=t2;

                        eBandWidthFRes->Text=FloatToStr(BandWidthFRes);
                        eAttenuationFRes->Text=FloatToStr(AttenuationFRes);

                        Application->ProcessMessages();

                        if(CheckBox2->Checked==false)
                        {
                        N26->Click(); // ��������� ����������, ������ ��������, ������� � ��������.
                        }
                        else
                        {
                        N25->Click();
                        }

                        if(params)
                        {
                            params->SaveAllData(SaveDialog1->FileName+"Com");
                        }
                        //N11->Click(); // ��������� ��
                    }

                }

            }
            else
                ShowMessage(IntToStr(GetLastError()));
        }
        }
    }
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N21Click(TObject *Sender)
{
calculateTenzor();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnSaveCarrierParamsClick(TObject *Sender)
{
/*
��� ����� ������ �����������:
1. ��������� ��������� ������.
    �����������
    �������
    ���
    ��� %
2. ����������� ���������� ������� ������������.
*/
;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
    const TSignal * bv=(*ActiveParams())->getB();
    const TSignal * hallv=(*ActiveParams())->getHallEffect();
    const TSignal *mv=(*ActiveParams())->getMagnetoResistance();
    std::vector<long double> b=*bv;
    std::vector<long double> hall=*hallv;
    std::vector<long double> m=*mv;
    long double h = fabs(b[1]-b[0]);
    TSignal d1hall, d1m;
    TSignal d2hall, d2m;

    d1hall=calculateFirstDerivative(hall,h);
    d2m=calculateSecondDerivative(m,h);
    d1m=calculateFirstDerivative(m,h);
    d2hall=calculateSecondDerivative(hall,h);

    TStringList * tsl=new TStringList();

    for (int i = 0; i < d2hall.size(); ++i)
    {
        tsl->Add(FloatToStr(d1hall[i])+"\t"+FloatToStr(d2hall[i])+"\t"+FloatToStr(d1m[i])+"\t"+FloatToStr(d2m[i]));
    }

    tsl->SaveToFile("derivativeOfSignals.txt");   

    delete tsl;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SmartCalcClick(TObject *Sender)
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
        smartCalculation sC(p);
        sC.processData();
        SaveDialog1->Title="��������� ���������� ����������� �������:";
        if(SaveDialog1->Execute())
        {
            sC.saveResults(SaveDialog1->FileName.c_str());
        }
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button6Click(TObject *Sender)
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
        long double VesGxx=StrToFloat(LabeledEdit1->Text);
        long double VesGxy=StrToFloat(LabeledEdit2->Text);
        p=*par;
        p->runSmartCalcutation();
        p->runSmartMultiCarrierFit(VesGxx,VesGxy);    
        getAndDisplayMultiCarrierFitResults(p);
    }
}
//---------------------------------------------------------------------------


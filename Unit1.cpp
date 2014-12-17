//---------------------------------------------------------------------------
#pragma hdrstop

#include "Unit1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

/*
TODO
возможность записи "поверх" - т.е. удалять предыдущие значения и писать поверх новые
фукнция удаления определенного интервала точек

Надо бы предусмотреть отдельный поток для вывода.
И вызывать его по таймеру.

Выпилить ненужные параметры фильтрации. По сути мне достаточно иметь
частоту среза и длину фильтра с частотой дискретизации.
Впрочем частота среза определяется из длины фильтра (по идее).

Сделать работу с прогой более комфортной.
Автоматическое переключение
записываемого графика (положительное поле, отрицательное поле).

Возможно их стоит просто объединить. Но это очень спорный вопрос.
Ибо как тогда определять надо ли перезаписывать данные или дописывать их.

Ещё момент - прога изредка вылетает с ексепшенами, неплохо было бы реализовать
автоматическое сохранение измеряемого сигнала (скажем каждые Т точек).

Пора внедрять фильтр как класс.
Магнитное поле на данный момент уже фильтруется. Результаты неплохие.

Есть предложение фильтровать автоматически только уже объединенные зависимости.


Нужно добавить усреднение по току.


*/

// Внимание, понадобится добавить что-нибудь,
// не забудь внести это в заголовочный файл:)


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
        Предупреждение Initializing Params Type with const int - не актуально,
        т.к. значения как раз совпадают с перечислением, но аккуратней в этом месте.
        */
        (*p)->saver->setParamsType(ResCurveIndex->ItemIndex); // значения их совпадают.
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
    StatusBar->Panels->Items[1]->Text="Обновление графиков.";
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


    // Обновление всех используемых графиков.
    if(!p->constructPlotFromTwoMassive(HALL_EFFECT,CURRENT_DATA,SeriesHall1,clRed))
        ErrorLog->Lines->Add("Холл. Текущие данные. Не удалось построить график.");
    if(!p->constructPlotFromTwoMassive(HALL_EFFECT,FILTERED_DATA,SeriesHall2,clBlue))
        ErrorLog->Lines->Add("Холл. Фильтрованные данные. Не удалось построить график.");
    if(!p->constructPlotFromTwoMassive(HALL_EFFECT,EXTRAPOLATED_DATA,out2,clBlack))
        ErrorLog->Lines->Add("Холл. Экстраполированные данные. Не удалось построить график.");

    if(!p->constructPlotFromTwoMassive(HALL_EFFECT,AVERAGED_DATA,SeriesFFTHall,clGreen))
        ErrorLog->Lines->Add("Холл. Усреднённые данные. Не удалось построить график.");

    if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,AVERAGED_DATA,SeriesFFTRes,clGreen))
        ErrorLog->Lines->Add("Сопротивление. Усреднённые данные. Не удалось построить график.");

    if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,CURRENT_DATA,SeriesRes1,clRed))
        ErrorLog->Lines->Add("Сопротивление. Текущие данные. Не удалось построить график.");
    if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,FILTERED_DATA,SeriesRes2,clBlue))
        ErrorLog->Lines->Add("Сопротивление. Фильтрованные данные. Не удалось построить график.");
    if(!p->constructPlotFromTwoMassive(MAGNETORESISTANCE,EXTRAPOLATED_DATA,out1,clBlack))
        ErrorLog->Lines->Add("Сопротивление. Экстраполированные данные. Не удалось построить график.");
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
    // создание текущей копии настроек
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
    // загрузка настроек.

    // создание текущей копии настроек
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
    // загружаем драйвер
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

void __fastcall TForm1::N3Click(TObject *Sender)// выход из программы
{
    Form1->Close(); 
}
//----много кода, выключает/включает графические элементы, во время работы АЦП-
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

            //-- кнопки-----------------------------------------------
            
            bFilterRes->Enabled=0;
            uiHallFeat->Enabled=0;
            uiFFTHall->Enabled=0;
            uiFaradeyFeat->Enabled=0;
            uiFFTFaradey->Enabled=0;
            uiFoygtFeat->Enabled=0;
            uiFFTFoygt->Enabled=0;
            CurrentFaradey->Enabled=0;
            CurrentFoygt->Enabled=0;
            FaradeyCurveIndex->Enabled=0;
            FoygtCurveIndex->Enabled=0;
            GainKoefFoygt->Enabled=0;

            uiControl->Caption = AnsiString("Остановить запись");
            uiResControl->Caption = AnsiString("Остановить запись");
            uiHallControl->Caption = AnsiString("Остановить запись");
            uiFaradeyControl->Caption = AnsiString("Остановить запись");
            uiFoygtControl->Caption = AnsiString("Остановить запись");
            StatusBar->Panels->Items[1]->Text="Идёт запись данных";
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
        //CurrentHall->Enabled=1;
        ResCurveIndex->Enabled=1;
        HallCurveIndex->Enabled=1;

        uiFrenq->Enabled =true;

        uiBlockSize->Enabled=true;

        
        bFilterRes->Enabled=1;
        uiHallFeat->Enabled=1;
        uiFFTHall->Enabled=1;
        uiFaradeyFeat->Enabled=1;
        uiFFTFaradey->Enabled=1;
        uiFoygtFeat->Enabled=1;
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
        uiControl->Caption = AnsiString("Начать запись");
        uiResControl->Caption = AnsiString("Начать запись");
        uiHallControl->Caption = AnsiString("Начать запись");
        uiFaradeyControl->Caption = AnsiString("Начать запись");
        uiFoygtControl->Caption = AnsiString("Начать запись");
        /*
        if (paramsDirect && paramsReverse)
        {
            StatusBar->Panels->Items[1]->Text="Идёт объединение данных.";
            concatDependence();
        }        
           */
        StatusBar->Panels->Items[1]->Text="Готова к работе.";        
      }
}

//---------------------------------------------------------------------------


// Подгонка магнитосопротивления
void __fastcall TForm1::uiResFeatClick(TObject *Sender)
{
    EvenFeat(SeriesRes1,0);
    EvenFeat(SeriesRes2,0);
    MidCurve(SeriesRes1,SeriesRes2,0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::uiHallFeatClick(TObject *Sender)
{
    OddFeat(SeriesHall1,0);
    OddFeat(SeriesHall2,0);
    MidCurve(SeriesHall1,SeriesHall2,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFaradeyFeatClick(TObject *Sender)
{
    EvenFeat(SeriesFaradey1,0);
    EvenFeat(SeriesFaradey2,0);
    MidCurve(SeriesFaradey1,SeriesFaradey2,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFoygtFeatClick(TObject *Sender)
{
    FoygtFeat(SeriesFoygt1,SeriesFoygt2,0);
}
//----------------------------------------------------------------------------

void __fastcall TForm1::bClearClick(TObject *Sender) // очищаем всё:)
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
    ShowMessage("Вероятно выбран не тот график.");
}
else
{

    MagneticFieldDependence * p=*par;
    p->setFilterParamsResistance(eSamplingFRes->Text, eBandWidthFRes->Text,
     eAttenuationFRes->Text, eLengthFilterRes->Text);
    p->setFilterParamsHall(eSamplingFHall->Text, eBandWidthFHall->Text,
     eAttenuationFHall->Text, eLengthFilterHall->Text);
    StatusBar->Panels->Items[1]->Text="Фильтрация данных.";
    Form1->Update();
    p->filterData();
    StatusBar->Panels->Items[1]->Text="Экстраполяция данных.";
    Form1->Update();
    p->extrapolateData(PowPolinomRes->Text.ToInt(),PowPolinomHall->Text.ToInt());
    UpdatePlots();
    StatusBar->Panels->Items[1]->Text="Готова к работе.";
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


//-------------------Открытие файла------------------------------------------

void __fastcall TForm1::N4Click(TObject *Sender)
{ 
    if(OpenDialog1->Execute())  // если мы что-то выбрали
    {
        TStringList *tts=new TStringList();  // сюда будем загружать из файла
        tts->LoadFromFile(OpenDialog1->Files->Strings[0]);// загрузили


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

 // выбор активного графика
 // используется при сохранении файла в Unit2

TLineSeries * __fastcall TForm1::GetSelectedSeries(int index)
{

// заполняет массив указателями на все графики.
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

void __fastcall TForm1::N5Click(TObject *Sender)  // сохранение
{

    GetSelectedSeries(0);
    
    if(!SaveForm)
    {
    // включает отображение формы сохранения данных.
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
// убирает точки соответствующие всплескам
void KillImpulps(TLineSeries * a,double x, double y)
{

    int length=a->XValues->Count();
    if(length==0)
    {
    ShowMessage("Пустой график!!!");
    return;
    }
    double *xm=new double [length];
    double *ym=new double [length];

    for(int i=0;i<length;i++)
     {
            xm[i]=a->XValues->Value[i];
            ym[i]=a->YValues->Value[i];
     }
    // границы интервала
    //x=Interval1->Text.ToDouble();
    //y=Interval2->Text.ToDouble();
    double dx=fabs(xm[0]-x);// начинаем поиск ближайшей точки
    int i;
    double k=0;
    for(i=0;i<length;i++)
    {
            k=fabs(xm[i]-x);
            if(k>dx)  // как только пошли на увеличение - выходим из цикла
            {
                 break;
            }
            dx=k;
    }
    int i1=i; // записываем первый индекс
    dx=fabs(xm[i]-y);// ищем второй
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
// в зависимости от активной вкладки вызывается нужна процедура очистки.
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
// расчет среднего квадратичного отклонения
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
// расчет математического ожидания
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
// построение амплитудной гистограммы
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

    Form1->ErrorLog->Lines->Add("Мат ожидание: "+FloatToStr(m));
    Form1->ErrorLog->Lines->Add("СКО: "+FloatToStr(sko));

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
// Сохранить всё.
void __fastcall TForm1::N11Click(TObject *Sender)
{
    CurrentResChange(NULL);
    if(params && paramsDirect && paramsReverse)
    {
        SaveDialog1->Title="Сохранение всех данных:";
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
        SaveDialog1->Title="Сохранение объединенных данных:";
        if(SaveDialog1->Execute())
        {
            params->SaveAllData(SaveDialog1->FileName+"Com",true);
        }
    }

    if(paramsDirect)
    {
        SaveDialog1->Title="Сохранение данных для положительного магнитного поля:";
        if(SaveDialog1->Execute())
        {
            paramsDirect->SaveAllData(SaveDialog1->FileName+"Dir");
        }
    }

    if(paramsReverse)
    {
        SaveDialog1->Title="Сохранение данных для отрицательного магнитного поля:";
        if(SaveDialog1->Execute())
        {
            paramsReverse->SaveAllData(SaveDialog1->FileName+"Rev");
        }
    }
    }


}
//---------------------------------------------------------------------------


// применение настроек АЦП
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
    ShowMessage("Для объединения нужно измерить две зависимости");
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

    DataTypeInContainer B; // создаем буфер для новых зависимостей.
    DataTypeInContainer Hall;
    DataTypeInContainer Resistance;

    DataTypeInContainer B2; // создаем буфер для новых зависимостей.
    DataTypeInContainer Hall2;
    DataTypeInContainer Resistance2;

    StatusBar->Panels->Items[1]->Text="Объединение зависимостей.";
    Form1->Update();
    // вбрасываем в обратном порядке зависимости для отрицательного магнитного поля.
    for (DataTypeInContainer::const_reverse_iterator i = paramsReverse->getB()->rbegin(); i != paramsReverse->getB()->rend(); ++i)
    {
        B.push_back(*i);    
    }

    for (DataTypeInContainer::const_reverse_iterator i = paramsReverse->getHallEffect()->rbegin(); i != paramsReverse->getHallEffect()->rend(); ++i)
    {
        Hall.push_back(*i);    
    }

    for (DataTypeInContainer::const_reverse_iterator i = paramsReverse->getMagnetoResistance()->rbegin(); i != paramsReverse->getMagnetoResistance()->rend(); ++i)
    {
        Resistance.push_back(*i);    
    }
    // вбрасываем в прямом порядке зависимости для положительного поля.
    for (DataTypeInContainer::const_iterator i = paramsDirect->getB()->begin(); i != paramsDirect->getB()->end(); ++i)
    {
        B.push_back(*i);   
        B2.push_back(*i);  
    }

    for (DataTypeInContainer::const_iterator i = paramsDirect->getHallEffect()->begin(); i != paramsDirect->getHallEffect()->end(); ++i)
    {
        Hall.push_back(*i);  
        Hall2.push_back(*i);   
    }

    for (DataTypeInContainer::const_iterator i = paramsDirect->getMagnetoResistance()->begin(); i != paramsDirect->getMagnetoResistance()->end(); ++i)
    {
        Resistance.push_back(*i);  
        Resistance2.push_back(*i);   
    }

    DataTypeInContainer outB;
    DataTypeInContainer outHall;
    DataTypeInContainer outResistance;

    size_t minimalLength=paramsReverse->getB()->size()>paramsDirect->getB()->size()?
    paramsDirect->getB()->size() : paramsReverse->getB()->size();

    StatusBar->Panels->Items[1]->Text="Прореживание зависимостей.";
    Form1->Update();
    // что-то похоже что эта функция работает немного не так как надо...
    //thiningSignal(B, Hall, outB, outHall, -2, 0, 2*minimalLength);
    //thiningSignal(B, Resistance, outB, outResistance, -2, 2, 2*minimalLength);
    // и у меня есть некоторое подозрение почему оно так.
    // возможно более правильным будет сначала определить сигнал с наименьшим количество точек (прямой или обратный)
    // потом проредить оба сигнала отдельно и после этого объединить их.
    thiningSignal(B, Hall, outB, outHall, -2, 2, 2*minimalLength);
    thiningSignal(B, Resistance, outB, outResistance, -2, 2, 2*minimalLength);

    Form1->ErrorLog->Lines->Add(FloatToStr( B.size()));
    
    StatusBar->Panels->Items[1]->Text="Установка новых параметров.";
    Form1->Update();
    params->setDependence(outB.begin(),outB.end(),outHall.begin(),outResistance.begin());

    UpdatePlots();
    StatusBar->Panels->Items[1]->Text="Готова к работе.";
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



void __fastcall TForm1::Button3Click(TObject *Sender)
{
    MagneticFieldDependence ** par=ActiveParams();
    MagneticFieldDependence * p;
    if (*par==NULL)
    {
        ShowMessage("Вероятно выбран не тот график.");   
        return; 
    }
    else
    {
        (*ActiveParams())->setSampleDescription(uiTemperature->Text,uiCurrent->Text,
        uiInventoryNumber->Text,uiSampleLength->Text,uiSampleWidth->Text,uiSampleThickness->Text);
        (*ActiveParams())->setParamsType(ResCurveIndex->ItemIndex);
        p=*par;
        p->calcutaleTenzor(uiDataKind->ItemIndex==0?CURRENT_DATA:FILTERED_DATA);

        p->constructPlotFromTwoMassive(SXX,CURRENT_DATA,Series6,clRed);
        p->constructPlotFromTwoMassive(SXY,CURRENT_DATA,LineSeries1,clRed);

        ErrorLog->Lines->Add(p->getSxx()->size());
        ErrorLog->Lines->Add(p->getSxy()->size());

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
        Предупреждение Initializing Params Type with const int - не актуально,
        т.к. значения как раз совпадают с перечислением, но аккуратней в этом месте.
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
    ShowMessage("Выбранный график пуст");
}
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ComboBox5Change(TObject *Sender)
{
switch(ComboBox5->ItemIndex)
{
case 0:
Label31->Caption="1й канал(Сопротивление):"; // 1й канал
break;
case 1:
Label33->Caption="2й канал(Сопротивление):";; // 2й канал
break;
case 2:
Label32->Caption="3й канал(Сопротивление):";; // 3й канал
break;
default:
break;
}
switch(ComboBox6->ItemIndex)
{
case 0:
Label31->Caption="1й канал(Поле):"; // 1й канал
break;
case 1:
Label33->Caption="2й канал(Поле):";; // 2й канал
break;
case 2:
Label32->Caption="3й канал(Поле):";; // 3й канал
break;
default:
break;
}
switch(ComboBox4->ItemIndex)
{
case 0:
Label31->Caption="1й канал(Холл):"; // 1й канал
break;
case 1:
Label33->Caption="2й канал(Холл):";; // 2й канал
break;
case 2:
Label32->Caption="3й канал(Холл):";; // 3й канал
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
    ShowMessage("Вероятно выбран не тот график.");
}
else
{
    MagneticFieldDependence * p=*par;
    p->setFilterParamsResistance(eSamplingFRes->Text, eBandWidthFRes->Text,
     eAttenuationFRes->Text, eLengthFilterRes->Text);
    p->setFilterParamsHall(eSamplingFHall->Text, eBandWidthFHall->Text,
     eAttenuationFHall->Text, eLengthFilterHall->Text);
    p->blockfilterData();
    p->extrapolateData(PowPolinomRes->Text.ToInt(),PowPolinomHall->Text.ToInt());

    UpdatePlots();
}
}
//---------------------------------------------------------------------------
    typedef int __stdcall (*pointerToFunc)(long double &, long double &,long double &, DWORD);

    typedef long double * __stdcall (*resPointFunc) (long double *,long double *,long double *,long double *);

    typedef long double  __stdcall (*resFunc) (int);



void calculateMobilitySpectrum(long double *B,long double *sxx,long double *sxy,int length)
{

    HANDLE hLibHandle;
    hLibHandle = LoadLibrary("lib\\MobilitySpectrum.dll");

    pointerToFunc pFunc = (pointerToFunc)GetProcAddress(hLibHandle,"RunMobilitySpectrum");

    //resPointFunc getResult = (resPointFunc)GetProcAddress(hLibHandle,"getResults");

      resFunc getEY=(resFunc) GetProcAddress(hLibHandle,"getResultEY");
      resFunc getEX=(resFunc) GetProcAddress(hLibHandle,"getResultEX");
      resFunc getHY=(resFunc) GetProcAddress(hLibHandle,"getResultHY");
      resFunc getHX=(resFunc) GetProcAddress(hLibHandle,"getResultHX");

      if(length==0)
      {
      Form1->ErrorLog->Lines->Add("Длина в расчете спектра подвижности равна нулю. Не могу считать.");
        return;
      }
      int size=(*pFunc)(*B,*sxx,*sxy, length);

      long double * ex=new long double [size];
      long double * eY=new long double [size];
      long double * hx=new long double [size];
      long double * hY=new long double [size];

      Form1->Series1->Clear();
      Form1->Series2->Clear();

      Form1->Chart1->LeftAxis->Logarithmic=true;
      Form1->Chart1->BottomAxis->Logarithmic=true;

      for(int i =0;i<size;i++)
      {
      ex[i]=getEX(i);
      eY[i]=getEY(i);
      hx[i]=getHX(i);
      hY[i]=getHY(i);

      Form1->Series1->AddXY(ex[i],eY[i],"",clBlue);
      Form1->Series2->AddXY(hx[i],hY[i],"",clRed);
      }

      if ( hLibHandle )
      FreeLibrary( hLibHandle );

    delete [] ex;
    delete [] eY;
    delete [] hx;
    delete [] hY;

}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
/*
Это потом нужно будет перенести в создание формы.

*/

StringGrid1->Cells[0][1]="Тяжелые дырки";
StringGrid1->Cells[0][2]="Легкие дырки";
StringGrid1->Cells[0][3]="Электроны";

StringGrid1->Cells[1][0]="Концентрация";
StringGrid1->Cells[2][0]="Подвижность";



//-------------------------------------------------------------


long double B[11]={0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0};
long double sxx[11]={42.2179,42.172,42.0579,41.8866,41.6706,41.4251,41.165,40.9024,40.646,40.4014,40.1721};
long double sxy[11]={0.0,0.558,1.1061,1.6173,2.0797,2.4883,2.8441,3.1511,3.4162,3.6464,3.8487};

HANDLE hLibHandle;
hLibHandle = LoadLibrary("lib\\MobilitySpectrum.dll");

      pointerToFunc pFunc = (pointerToFunc)GetProcAddress(hLibHandle,"RunMobilitySpectrum");

      resPointFunc getResult = (resPointFunc)GetProcAddress(hLibHandle,"getResults");

      resFunc getEY=(resFunc) GetProcAddress(hLibHandle,"getResultEY");
      resFunc getEX=(resFunc) GetProcAddress(hLibHandle,"getResultEX");
      resFunc getHY=(resFunc) GetProcAddress(hLibHandle,"getResultHY");
      resFunc getHX=(resFunc) GetProcAddress(hLibHandle,"getResultHX");

      int size=(*pFunc)(*B,*sxx,*sxy, 11);

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
      ex[i]=getEX(i);
      eY[i]=getEY(i);
      hx[i]=getHX(i);
      hY[i]=getHY(i);

      Series1->AddXY(getEX(i),getEY(i),"",clBlue);
      Series2->AddXY(getHX(i),getHY(i),"",clRed);
      }

      if ( hLibHandle )
      FreeLibrary( hLibHandle );

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
        ShowMessage("Вероятно выбран не тот график.");
        return;
    }
    else
    {
        p=*par;

        long double * B=new long double [(p->getAveragedB())->size()];
        long double * sxx=new long double [(p->getAveragedB())->size()];
        long double * sxy=new long double [(p->getAveragedB())->size()];

        for(unsigned int i=0;i<(p->getAveragedB())->size();i++)
        {
        B[i]=(*p->getAveragedB())[i];
        sxx[i]=(*p->getSxx())[i];
        sxy[i]=(*p->getSxy())[i];
        }

        calculateMobilitySpectrum(B,sxx,sxy,(p->getAveragedB())->size());

        delete [] B;
        delete [] sxx;
        delete [] sxy;
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Series1Click(TChartSeries *Sender, int ValueIndex,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{   
    long double electronCharge=1.602e-19;
    long double G_p=Sender->YValues->Value[ValueIndex];
    long double Mu= Sender->XValues->Value[ValueIndex];
    long double Concentration=G_p/(Mu*electronCharge);

    StringGrid1->Cells[2][StringGrid1->Selection.Top]= FloatToStr( Mu);
    StringGrid1->Cells[1][StringGrid1->Selection.Top]= FloatToStr(Concentration);

    TGridRect tgr=StringGrid1->Selection;
    tgr.Top++;
    tgr.Bottom++;
    if(tgr.Top>3)
    {
        tgr.Top=1;
        tgr.Bottom=1;
    }
    StringGrid1->Selection =tgr;
}
//---------------------------------------------------------------------------





void __fastcall TForm1::N12Click(TObject *Sender)
{
    MagneticFieldDependence ** par=ActiveParams();
    MagneticFieldDependence * p;
    if (*par==NULL)
    {
        ShowMessage("Вероятно выбран не тот график.");
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
        //ShowMessage("Вероятно выбран не тот график.");
        return;
    }
    else
    {
        p=*par;
    }
    
if(PC->ActivePageIndex==1) // Сопротивление
{
    p->multiplySignal(MAGNETORESISTANCE,-1);
}

if(PC->ActivePageIndex==2) // Холл
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
for(int i=0;i<size;++i)
{
in.push_back(rand());
}
medianFilter(in,out,11);
ErrorLog->Lines->Add("Number for in"+IntToStr(in.size()));
ErrorLog->Lines->Add("Number for out"+IntToStr(out.size()));
for(int i=0;i<in.size();++i)
{
Series1->AddY(in[i]);
}
for(int i=0;i<out.size();++i)
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


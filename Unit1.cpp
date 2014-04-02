//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>


#include "Unit1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

/*
TODO
ручная регулировка шага
предусмотреть "единичное измерение"
возможность записи "поверх" - т.е. удалять предыдущие значения и писать поверх новые
фукнция удаления определенного интервала точек

Полномасштабный рефакторинг

Так-с, сделаем классы:
1. LCardADC АЦП (инициализация драйвера, настройка, запуск/останов измерений, получение данных).
2. MagneticFieldDependence Измеренные зависимости (получение, фильтрация, экстраполяция, увеличение/уменьшение, вырезка и т.п.,
    построение графиков, сохранение результатов).
3.

Покромсать на модули:
1. Графический интерфейс +
2. Фильтрация            +
3. Настройка драйвера    +
4. Настройка измерений
5. Экстраполяция         +

Избавиться от глобальных переменных. + Ачивка получена.
Попытаться соорудить классы: драйвера и измерений. +

Нормально назвать все кнопки и прочее.

*/

// Внимание, понадобится добавить что-нибудь,
// не забудь внести это в заголовочный файл:)


LCardADC *adc=0;
MagneticFieldDependence *params=0;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *)
{
   
// Просматриваем все доступные драйверы и помещаем в список только те,
    // что поддерживают передачу данных по DMA

    // загружаем драйвер
    adc=new LCardADC(3,400);

    float       MaxFreq=400;   //Максимальная частота
    // Допустимые частоты квантования и размеры блоков
    //Узнаем максимальную частоту
    //Устанавливаем слайдеры 

    uiBlockSize2->Text=1024;

    //------------------------------------------------------------------------
    Chan1->ItemIndex=0;
    Chan2->ItemIndex=0;
    GainKoefFaradey->ItemIndex=0;
    //------------------------------------------------------------------------
    //-----------------------------------------------------------------
    

    // Тут заполняем необходимые данные интерфейса

    GainKoefFoygt->ItemIndex=0;
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
    if (!adc->IsMeasurementRunning())
    {
        if(params)
        {
        delete params;
        params=0;
        }

        params=new MagneticFieldDependence(StrToFloat(CurrentRes->Text));

        params->setFilterParams(Fd1->Text, Fp1->Text, Fz1->Text, Lfilter1->Text);

        adc->clearBuffer();
        adc->setMagnetoResistanceSeries(SeriesRes1);
        adc->setHallSeries(SeriesHall1);

        if(adc->StartMeasurement())
        {
        
        uiFrenq->Enabled = false;
        uiBlockSize2->Enabled=false;
        Chan1->Enabled=false;
        Chan2->Enabled=false;
        CurrentRes->Enabled=0;
        CurrentHall->Enabled=0;
        ResCurveIndex->Enabled=0;
        HallCurveIndex->Enabled=0;
        GainKoefFaradey->Enabled=0;

        //-- кнопки-----------------------------------------------
        uiResFeat->Enabled=0;
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

        uiControl->Caption = AnsiString("Stop");
        uiResControl->Caption = AnsiString("Stop");
        uiHallControl->Caption = AnsiString("Stop");
        uiFaradeyControl->Caption = AnsiString("Stop");
        uiFoygtControl->Caption = AnsiString("Stop");
        StatusBar->Panels->Items[1]->Text="Проводится измерение";
        }
    }
    else
    {
        GainKoefFaradey->Enabled=1;
        CurrentRes->Enabled=1;
        CurrentHall->Enabled=1;
        ResCurveIndex->Enabled=1;
        HallCurveIndex->Enabled=1;

        uiFrenq->Enabled =true;

        uiBlockSize2->Enabled=true;
        Chan1->Enabled=true;
        Chan2->Enabled=true;

        uiResFeat->Enabled=1;
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

        adc->StopMeasurement();
        params->getSplittedDataFromADC();


        params->constructPlotFromTwoMassive(HALL_EFFECT,CURRENT_DATA,SeriesHall1,clBlue);
        params->constructPlotFromTwoMassive(HALL_EFFECT,FILTERED_DATA,SeriesHall2,clRed);
        params->constructPlotFromTwoMassive(HALL_EFFECT,EXTRAPOLATED_DATA,out1,clBlack);

        params->constructPlotFromTwoMassive(MAGNETORESISTANCE,CURRENT_DATA,SeriesRes1,clBlue);
        params->constructPlotFromTwoMassive(MAGNETORESISTANCE,FILTERED_DATA,SeriesRes2,clRed);
        params->constructPlotFromTwoMassive(MAGNETORESISTANCE,EXTRAPOLATED_DATA,out1,clBlack);


        //params->constructPlotFromOneMassive(MAGNETIC_FIELD,SeriesRes2,clBlue);
        //params->constructPlotFromOneMassive(DEPENDENCE,SeriesRes1,clRed);
        //std::vector<MyDataType> temp(params->getFilteredDependence());
        //int NumberOfPoints=temp.size();
        //out1->Clear();
	//for (unsigned int i = 0; i < NumberOfPoints; i++)
	{
		//out1->AddY(temp[i],"",clBlack);
	}

        /*long double temp=0;
        for(int i=0;i<SeriesRes1->YValues->Count();i++)
        {
        temp+=SeriesRes1->YValues->Value[i];
        }
        temp/=SeriesRes1->YValues->Count();
        Memo2->Lines->Add(FloatToStr(temp));  */
        uiControl->Caption = AnsiString("Start");
        uiResControl->Caption = AnsiString("Start");
        uiHallControl->Caption = AnsiString("Start");
        uiFaradeyControl->Caption = AnsiString("Start");
        uiFoygtControl->Caption = AnsiString("Start");
        StatusBar->Panels->Items[1]->Text="Готова к работе. Тестовая версия";
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
    Memo1->Clear();
}
//---------------------------------------------------------------------------
//----------------------------------------------------------------------------
void __fastcall TForm1::bFilterResClick(TObject *Sender)
{
    params->setFilterParams(Fd1->Text, Fp1->Text, Fz1->Text, Lfilter1->Text);
    params->filterData();

    /*Tr_Filter((ResCurveIndex->ItemIndex==0?SeriesRes1:SeriesRes2),SeriesFFTRes,
    Lfilter1->Text.ToInt(),Fd1->Text.ToInt(),
    Fp1->Text.ToInt(),Fz1->Text.ToInt());*/

}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFFTHallClick(TObject *Sender)
{
    Tr_Filter((HallCurveIndex->ItemIndex==0?SeriesHall1:SeriesHall2),SeriesFFTHall,Lfilter2->Text.ToInt(),Fd2->Text.ToInt(),
    Fp2->Text.ToInt(),Fz2->Text.ToInt());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFFTFaradeyClick(TObject *Sender)
{
    Tr_Filter((FaradeyCurveIndex->ItemIndex==0?SeriesFaradey1:SeriesFaradey2),SeriesFFTFaradey,Lfilter3->Text.ToInt(),Fd3->Text.ToInt(),
    Fp3->Text.ToInt(),Fz3->Text.ToInt());

}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFFTFoygtClick(TObject *Sender)
{
    Tr_Filter((FoygtCurveIndex->ItemIndex==0?SeriesFoygt1:SeriesFoygt2),SeriesFFTFoygt,Lfilter4->Text.ToInt(),Fd4->Text.ToInt(),
    Fp4->Text.ToInt(),Fz4->Text.ToInt());
}
//---------------------------------------------------------------------------

//-------------------Открытие файла------------------------------------------

void __fastcall TForm1::N4Click(TObject *Sender)
{
    if(OpenDialog1->Execute())  // если мы что-то выбрали
    {
    TLineSeries * S=GetCurrentSeries2(1);  // получаем активный график

    S->Clear(); // очищаем сначала график, куда пишем:)

    TStringList *tts=new TStringList();  // сюда будем загружать из файла
    int b;
    tts->LoadFromFile(OpenDialog1->Files->Strings[0]);// загрузили
    for(int i=0;i<tts->Count;i++) // по количеству строк
    {
        if(tts->Strings[i].IsEmpty()) // пустые строки пропускаем
            continue;
        
        for(b=1;b<tts->Strings[i].Length();b++) // в непустых строках ищем пробел
            if(tts->Strings[i].operator [](b)==' ' || tts->Strings[i].operator [](b)=='\t')
                break;
            S->AddXY(tts->Strings[i].SubString(1,b-1).ToDouble(), // первая часть до пробела это х, вторая после у
            tts->Strings[i].SubString(b+1,tts->Strings[i].Length()).ToDouble(),
            "",clRed);
    }    
    }
}
//---------------------------------------------------------------------------

//---------Двигаем полосу "обрезания" для БФП-------------------------------// выбор активного графика
TLineSeries * __fastcall TForm1::GetCurrentSeries2(int curve)
{
// вызывается при открытии файла.
    TLineSeries * SaveSeries;
    switch(Form1->PC->ActivePageIndex)
    {           // выбираем нужный массив данных для сохранения
        case 0:
                // случай отладки, когда активна вкладка "FirstTab"
                SaveSeries=Form1->Series3;
            break;
        case 1:
            if(curve) // если мы открываем/сохраняем файл, то в зависимости от выборанного графика сохраняем нужный
                SaveSeries=(ResCurveIndex->ItemIndex==0?Form1->SeriesRes1:Form1->SeriesRes2);
            else     
                SaveSeries=Form1->SeriesRes1;
            break;
        case 2:
            if(curve)
                SaveSeries=(HallCurveIndex->ItemIndex==0?Form1->SeriesHall1:Form1->SeriesHall2);
                // Эффект Холла
            else
                SaveSeries=Form1->SeriesHall1;
            break;
        case 3:
                // Фотопроводимость в геометрии Фарадея
            if(curve)
                SaveSeries=(FaradeyCurveIndex->ItemIndex==0?Form1->SeriesFaradey1:Form1->SeriesFaradey2);
            else
                SaveSeries=Form1->SeriesFaradey1;
            break;
                
        case 4:
                // Фотопроводимость в геометрии Фойгта
            if(curve)
                SaveSeries=(FoygtCurveIndex->ItemIndex==0?Form1->SeriesFoygt1:Form1->SeriesFoygt2);
            else
                SaveSeries=Form1->SeriesFoygt1;
            break;
            // в случае ошибки - предупреждаем пользователя
        default:
            ShowMessage("Неверный индекс! Возможно программа была некорректно обновлена. Обратитесь к разработчику!");
            return NULL;
    }
    return SaveSeries;  // возвращаем указатель на активный график
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
void __fastcall TForm1::Button6Click(TObject *Sender)
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
    if(((int)Key<48 || (int)Key>57) && Key!=8 && Key!=',')
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

void __fastcall TForm1::bMultuplyBClick(TObject *Sender)
{
    for(int i=0;i<SeriesRes1->XValues->Count();i++)
        SeriesRes1->XValues->Value[i]*=10;
}
//---------------------------------------------------------------------------

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
    Button6Click(Sender);
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

    Memo2->Lines->Add("M=" + FloatToStr(M) + "\n");
    Memo2->Lines->Add("sko=" + FloatToStr(sko) + "\n");

    delete[] x;
    delete[] y;
    delete[] yf;

}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormDestroy(TObject *Sender)
{
if(adc)
delete adc;
delete params;    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bTestClick(TObject *Sender)
{

if(params)
{
delete params;
params=0;
}
params=new MagneticFieldDependence(CurrentRes->Text.ToDouble());
params->setFilterParams(Fd1->Text, Fp1->Text, Fz1->Text, Lfilter1->Text);
 adc->clearBuffer();
        //adc->setInteractiveSeries(Series1);  !
adc->testSetReadBuffer();

adc->StopMeasurement();

        params->getSplittedDataFromADC();
        SeriesRes1->Clear();
        //params->constructPlotFromOneMassive(DEPENDENCE,SeriesRes1,clBlue);
        params->constructPlotFromTwoMassive(HALL_EFFECT,CURRENT_DATA,SeriesRes2,clBlue);
        params->constructPlotFromTwoMassive(HALL_EFFECT,FILTERED_DATA,SeriesRes1,clRed);
        //params->constructPlotFromTwoMassive(HALL_EFFECT,EXTRAPOLATED_DATA,out1,clBlack);

        /*std::vector<MyDataType> temp(params->getExtrapolatedHallEffect());
        for(int i=0;i<temp.size();i++)
        Memo1->Lines->Add(FloatToStr(temp[i]));

        Memo1->Lines->Add(temp.size());
        Memo1->Lines->Add(IntToStr((int)temp[0])); */
        //params->constructPlotFromOneMassive(MAGNETIC_FIELD,SeriesRes1,clRed);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
;
}
void __fastcall TForm1::N11Click(TObject *Sender)
{
if(params)
if(SaveDialog1->Execute())
{
    params->SaveAllData(SaveDialog1->FileName);
}
}
//---------------------------------------------------------------------------


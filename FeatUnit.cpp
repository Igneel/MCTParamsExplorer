#include "FeatUnit.h"

void FeatCurve(TLineSeries* a, long index, FeatType featType)
{
    int size=a->XValues->Count();
    if(size==0)
    {
        ShowMessage("Получен пустой массив данных!!!");
        return;
    }
    TChartValueList * b,*c;

    // параметры массив и переменная относительно которой функция нечетна
    // 0 для х, 1 для y

    if(!index)
    {
        c=a->YValues;
        b=a->XValues;
    }
    else
    {
        b=a->YValues;
        c=a->XValues;
    }
    for(int i=0;i<size/2;i++)
    {
        b->Value[i]=(b->Value[i]-b->Value[size-1-i])/2.0;
        b->Value[size-1-i]=-b->Value[i];
        switch(featType)
        {
        case ODD_FEAT: // нечетная подгонка
            c->Value[i]=(c->Value[i]-c->Value[size-1-i])/2.0;
            c->Value[size-1-i]=-c->Value[i];
            break;
        case EVEN_FEAT: // четная подгонка
            c->Value[i]=(c->Value[i]+c->Value[size-1-i])/2.0;
            c->Value[size-1-i]=c->Value[i];
            break;
        }

    }
}

//---------------------------------------------------------------------------
// нечетная подгонка
// параметры массив и переменная относительно которой функция нечетна
// 0 для х, 1 для y
void  OddFeat(TLineSeries* a, long index)
{
    FeatCurve(a,index,ODD_FEAT);
}
//-----------------------------------------------------------------------------
//четная подгонка
// параметры массив и переменная относительно которой функция нечетна
// 0 для х, 1 для y
void  EvenFeat(TLineSeries* a, long index)
{
    FeatCurve(a,index,EVEN_FEAT);
}
//-----------------------------------------------------------------------------
//----------Подгонка Фотопроводимости в геометрии Фойгта-----------------------
void  FoygtFeat(TLineSeries* a,TLineSeries* b, long index)
{
    int size=a->XValues->Count();
    if(size==0 || b->XValues->Count()==0)
    {
        ShowMessage("Получен пустой массив данных!!!");
        return;
    }

    for(int i=0;i<size;i++)
    {
        a->XValues->Value[i]=(a->XValues->Value[i]-b->XValues->Value[size-1-i])/2;
        a->YValues->Value[i]=(a->YValues->Value[i]-b->YValues->Value[size-1-i])/2;
    }
}
//-----------------------------------------------------------------------------
// усреднение двух кривых
// индекс - зарезервирован, пока не используется
// функция сохраняет результат в ПЕРВЫЙ передаваемый массив!
void  MidCurve(TLineSeries* a, TLineSeries* b, long index)
{

    int size=a->XValues->Count();

    if(size==0 || b->XValues->Count()==0)
    {
        ShowMessage("Получен пустой массив данных!!!");
        return;
    }
    if(a->XValues->Count()!=b->XValues->Count())
    {
        ShowMessage("Разное количество точек на графиках!!!");
        return;
    }

    for(int i=0;i<size;i++)
    {
    a->XValues->Value[i]=(a->XValues->Value[i]+b->XValues->Value[i])/2;
    a->YValues->Value[i]=(a->YValues->Value[i]-b->YValues->Value[i])/2;
    }
}
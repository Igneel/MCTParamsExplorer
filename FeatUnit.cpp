#include "FeatUnit.h"

void FeatCurve(TLineSeries* a, long index, FeatType featType)
{
    int size=a->XValues->Count();
    if(size==0)
    {
        ShowMessage("������� ������ ������ ������!!!");
        return;
    }
    TChartValueList * b,*c;

    // ��������� ������ � ���������� ������������ ������� ������� �������
    // 0 ��� �, 1 ��� y

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
        case ODD_FEAT: // �������� ��������
            c->Value[i]=(c->Value[i]-c->Value[size-1-i])/2.0;
            c->Value[size-1-i]=-c->Value[i];
            break;
        case EVEN_FEAT: // ������ ��������
            c->Value[i]=(c->Value[i]+c->Value[size-1-i])/2.0;
            c->Value[size-1-i]=c->Value[i];
            break;
        }

    }
}

//---------------------------------------------------------------------------
// �������� ��������
// ��������� ������ � ���������� ������������ ������� ������� �������
// 0 ��� �, 1 ��� y
void  OddFeat(TLineSeries* a, long index)
{
    FeatCurve(a,index,ODD_FEAT);
}
//-----------------------------------------------------------------------------
//������ ��������
// ��������� ������ � ���������� ������������ ������� ������� �������
// 0 ��� �, 1 ��� y
void  EvenFeat(TLineSeries* a, long index)
{
    FeatCurve(a,index,EVEN_FEAT);
}
//-----------------------------------------------------------------------------
//----------�������� ���������������� � ��������� ������-----------------------
void  FoygtFeat(TLineSeries* a,TLineSeries* b, long index)
{
    int size=a->XValues->Count();
    if(size==0 || b->XValues->Count()==0)
    {
        ShowMessage("������� ������ ������ ������!!!");
        return;
    }

    for(int i=0;i<size;i++)
    {
        a->XValues->Value[i]=(a->XValues->Value[i]-b->XValues->Value[size-1-i])/2;
        a->YValues->Value[i]=(a->YValues->Value[i]-b->YValues->Value[size-1-i])/2;
    }
}
//-----------------------------------------------------------------------------
// ���������� ���� ������
// ������ - ��������������, ���� �� ������������
// ������� ��������� ��������� � ������ ������������ ������!
void  MidCurve(TLineSeries* a, TLineSeries* b, long index)
{

    int size=a->XValues->Count();

    if(size==0 || b->XValues->Count()==0)
    {
        ShowMessage("������� ������ ������ ������!!!");
        return;
    }
    if(a->XValues->Count()!=b->XValues->Count())
    {
        ShowMessage("������ ���������� ����� �� ��������!!!");
        return;
    }

    for(int i=0;i<size;i++)
    {
    a->XValues->Value[i]=(a->XValues->Value[i]+b->XValues->Value[i])/2;
    a->YValues->Value[i]=(a->YValues->Value[i]-b->YValues->Value[i])/2;
    }
}